/*
 * Random graph generator for Halmstad course DT8014 Algorithms
 *
 * Copyright (C) 2014 Roland Philippsen. All rights reserved.
 *
 * BSD license:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of
 *    contributors to this software may be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR THE CONTRIBUTORS TO THIS SOFTWARE BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

package main

import (
	"fmt"
	"math"
	"math/rand"
	"time"
	"encoding/json"
	"os"
	"io"
	"io/ioutil"
	"log"
	"container/heap"
)


const (
	// threshold value for things considered "zero" (could be made configurable)
	epsilon = 1.0e-3
)


type Setup struct {
	Dimx float64		// dimension (size) along X
	Dimy float64		// dimension along Y
	Obstacles [][]float64	// obstacle (lines specified using 4 numbers [x0 y0 x1 y1])
	RandomSeed int64        // random seed, in case you need repeatable runs
	Dsample float64	        // distance to keep clear from already sampled points
	Ntries int		// number of tries before giving up during sampling
	Nvertices int		// graph size (nominal -- you may get fewer vertices)
	Dconnect float64	// distance at which to stop, if Nconnect has been reached
	Nconnect int		// number of neighbors per vertex (nominal)
	Gnuplotfname string     // "" means do not generate, "-" is stdout, otherwise file fname
	Adjfname string		// similar but for adjacency list output
}


type Node struct {
	px float64
	py float64
	nbor map[*Node]float64
	ccparent *Node
	ccrank int
}


//////////////////////////////////////////////////
// Disjoint-Set stuff copy-paste-adapted from Wikipedia

func CCMakeSet(nn *Node) {
	nn.ccparent = nn
	nn.ccrank = 0
}

func CCUnion(nn, mm *Node) {
	nroot := CCFind(nn)
	mroot := CCFind(mm)
	if (nroot == mroot) {
		return
	}

	if nroot.ccrank < mroot.ccrank {
		nroot.ccparent = mroot
	} else if nroot.ccrank > mroot.ccrank {
		mroot.ccparent = nroot
	} else {
		mroot.ccparent = nroot
		nroot.ccrank += 1
	}
}

func CCFind(nn *Node) *Node {
	if nn.ccparent != nn {
		nn.ccparent = CCFind(nn.ccparent)
	}
	return nn.ccparent
}

//////////////////////////////////////////////////
// Priority queue copied form example on http://golang.org/pkg/container/heap/

type PQItem struct {
	node *Node
	distance float64
	// The index is needed by update and is maintained by the heap.Interface methods.
	index int // The index of the item in the heap.
}

type PriorityQueue []*PQItem

func (pq PriorityQueue) Len() int { return len(pq) }

func (pq PriorityQueue) Less(i, j int) bool {
	return pq[i].distance < pq[j].distance
}

func (pq PriorityQueue) Swap(i, j int) {
	pq[i], pq[j] = pq[j], pq[i]
	pq[i].index = i
	pq[j].index = j
}

func (pq *PriorityQueue) Push(x interface{}) {
	n := len(*pq)
	item := x.(*PQItem)
	item.index = n
	*pq = append(*pq, item)
}

func (pq *PriorityQueue) Pop() interface{} {
	old := *pq
	n := len(old)
	item := old[n-1]
	item.index = -1 // for safety
	*pq = old[0 : n-1]
	return item
}

// end copy-paste-adapt from golang.org
//////////////////////////////////////////////////


func DefaultSetup() Setup {
	var setup Setup
	setup.Dimx = 100
	setup.Dimy = 100
	setup.Obstacles = [][]float64 {
		{  20.0, 40.0, 40.0, 40.0 },
		{  30.0, 30.0, 30.0, 50.0 },
		{  60.0, 60.0, 80.0, 80.0 },
		{  60.0, 80.0, 80.0, 60.0 },
	}
	setup.RandomSeed = time.Now().UnixNano()
	setup.Dsample = 5.0
	setup.Ntries = 100
	setup.Nvertices = 100
	setup.Dconnect = 10.0
	setup.Nconnect = 3
	setup.Gnuplotfname = "-"
	setup.Adjfname = ""
	return setup
}


func CreateSetup() Setup {
	setup := DefaultSetup()
	args := os.Args[1:]
	for ia := 0; ia < len(args); ia += 1 {
		if len(args[ia]) < 1 {
			continue
		}
		if args[ia][0] != '-' {
			msg, err := ioutil.ReadFile(args[ia])
			if nil != err {
				log.Fatal("reading ", args[ia], " failed: ", err)
			}
			err = json.Unmarshal(msg, &setup)
			if nil != err {
				log.Fatal("parsing ", args[ia], " failed: ", err)
			}
			continue
		}
		if args[ia] == "-g" {
			if ia >= len(args) - 1 {
				log.Fatal("missing gnuplot filename argument")
			}
			setup.Gnuplotfname = args[ia + 1]
			ia += 1
		} else if args[ia] == "-a" {
			if ia >= len(args) - 1 {
				log.Fatal("missing adjacency filename argument")
			}
			setup.Adjfname = args[ia + 1]
			ia += 1
		} else {
			log.Fatal("unknown option", args[ia])
		}
		
	}
	return setup
}


// Returns -1 when there is no intersection, and otherwise the ratio
// between d(q1,q2) and d(q1,r) where r denotes the intersection point
// and d(.) the Euclidean distance.
//
func LineIntersect(p0x, p0y, p1x, p1y, q0x, q0y, q1x, q1y float64) float64 {
	vx := p1x - p0x
	vy := p1y - p0y
	wx := q1x - q0x
	wy := q1y - q0y
	
	det := wx * vy - vx * wy
	if math.Abs(det) <= epsilon {
		return -1     // parallel, or zero length or direction
	}
	mu0 := (wx * (q0y - p0y) - wy * (q0x - p0x)) / det
	if (mu0 < 0) || (mu0 > 1) {
		return -1	// outside of segment
	}
	
	// calculate distance from q0 to intersection
	mu1 := (vx * (q0y - p0y) - vy * (q0x - p0x)) / det
	if mu1 < 0 {
		return -1	// reverse intersection
	}
	if mu1 > 1 {
		return -1	// intersects beyond segment q
	}
	
	return mu1
}


func AreConnected(from, to *Node) float64 {
	dist, ok := from.nbor[to]
	if ok {
		return dist
	}
	dist, ok = to.nbor[from]
	if ok {
		return dist
	}
	return -1
}


func CanConnect(from, to *Node, obstacles [][]float64) bool {
	for _, oo := range(obstacles) {
		dd := LineIntersect(oo[0], oo[1], oo[2], oo[3],
			from.px, from.py, to.px, to.py)
		if 0 <= dd && 1 >= dd {
			return false
		}
	}
	return true
}


func Sample(dimx, dimy float64, nsamples int, dsample float64, ntries int) []*Node {
	nodes := make([]*Node, 0)
	for ii := 0; ii < nsamples; ii += 1 {
		for jj := 0; jj < ntries; jj += 1 {
			px := rand.Float64() * dimx
			py := rand.Float64() * dimy
			found := true
			for kk := 0; kk < len(nodes); kk += 1 {
				if math.Hypot(nodes[kk].px - px, nodes[kk].py - py) < dsample {
					found = false
					break
				}
			}
			if found {
				nodes = append(nodes, &Node {
					px: px, py: py, nbor: make(map[*Node]float64)})
				break
			}
		}
	}
	return nodes
}


func Connectify(graph []*Node, obstacles [][]float64, dconnect float64, nconnect int) {
	for ii := 0; ii < len(graph); ii += 1 {
		pq := make(PriorityQueue, len(graph) - 1)
		kk := 0
		for jj := 0; jj < len(graph); jj += 1 {
			if ii == jj {
				continue
			}
			pq[kk] = &PQItem {
				node: graph[jj],
				distance: math.Hypot(graph[jj].px - graph[ii].px,
					graph[jj].py - graph[ii].py),
				index: kk,
			}
			kk += 1
		}
		heap.Init(&pq)
		
		kk = 0
		for pq.Len() > 0 {
			item := heap.Pop(&pq).(*PQItem)
			if item.distance > dconnect && kk >= nconnect {
				break
			}
			if 0 <= AreConnected(graph[ii], item.node) {
				kk += 1
			} else if CanConnect(graph[ii], item.node, obstacles) {
				graph[ii].nbor[item.node] = item.distance
				kk += 1
			}
			// if kk >= nconnect {
			// 	break
			// }
		}
	}
}


func OutputGnuplot(ww io.Writer, setup Setup, graph []*Node) {
	fmt.Fprintln(ww, "set view equal xy")
	fmt.Fprintln(ww, "set size ratio -1")
	fmt.Fprintf(ww, "set xrange [0:%f]\n", setup.Dimx)
	fmt.Fprintf(ww, "set yrange [0:%f]\n", setup.Dimy)
	
	fmt.Fprintln(ww, "plot '-' u 1:2 w l t '', '' u 1:2 w p t ''")
	// fmt.Fprintln(ww, "plot '-' u 1:2 w p t '', '' u 1:2 w l t '', '' u 1:2 w l t ''")
	
	// fmt.Fprintln(ww, "# obstacles");
	// for _, oo := range(setup.Obstacles) {
	// 	fmt.Fprintf(ww, "% 5f  % 5f\n% 5f  % 5f\n\n\n", oo[0], oo[1], oo[2], oo[3])
	// }
	// fmt.Fprintln(ww, "e")
	
	fmt.Fprintln(ww, "# edges");
	for _, nn := range(graph) {
		for mm, _ := range nn.nbor {
			fmt.Fprintf(ww, "% 5f  % 5f\n% 5f  % 5f\n\n\n", nn.px, nn.py, mm.px, mm.py)
		}
	}
	fmt.Fprintln(ww, "e")

	fmt.Fprintln(ww, "# nodes");
	for _, nn := range(graph) {
		fmt.Fprintf(ww, "% 5f  % 5f\n", nn.px, nn.py)
	}
	fmt.Fprintln(ww, "e")
}


func OutputAdj(ww io.Writer, setup Setup, graph []*Node) {
	for _, nn := range(graph) {
		for mm, dd := range(nn.nbor) {
			fmt.Fprintf(ww, "(%f,%f)    (%f,%f)    %f\n", nn.px, nn.py, mm.px, mm.py, dd)
		}
	}
}


func Cluster(graph []*Node) map[*Node][]*Node {
	for _, nn := range(graph) {
		CCMakeSet(nn)
	}
	
	for _, nn := range(graph) {
		for mm, _ := range(nn.nbor) {
			if CCFind(nn) != CCFind(mm) {
				CCUnion(nn,mm)
			}
		}
	}
	
 	cc := make(map[*Node][]*Node, 0)
	for _, nn := range(graph) {
		cc[CCFind(nn)] = append(cc[CCFind(nn)], nn)
	}
	
	return cc
}


func main() {
	
	setup := CreateSetup()
	rand.Seed(setup.RandomSeed)
	
	graph := Sample(setup.Dimx, setup.Dimy, setup.Nvertices, setup.Dsample, setup.Ntries)
	Connectify(graph, setup.Obstacles, setup.Dconnect, setup.Nconnect)
	
	connected_components := Cluster(graph)
	biggest := make([]*Node, 0)
	for _, cc := range(connected_components) {
		if len(cc) > len(biggest) {
			biggest = cc
		}
	}
	
	if setup.Gnuplotfname == "-" {
		OutputGnuplot(os.Stdout, setup, biggest)
	} else if len(setup.Gnuplotfname) > 0 {
		ff, err := os.OpenFile(setup.Gnuplotfname,
			os.O_WRONLY | os.O_TRUNC | os.O_CREATE, 0660)
		if err != nil {
			log.Fatal(err)
		}
		defer ff.Close()
		OutputGnuplot(ff, setup, biggest)
	}
	
	if setup.Adjfname == "-" {
		OutputAdj(os.Stdout, setup, biggest)
	} else if len(setup.Adjfname) > 0 {
		ff, err := os.OpenFile(setup.Adjfname,
			os.O_WRONLY | os.O_TRUNC | os.O_CREATE, 0660)
		if err != nil {
			log.Fatal(err)
		}
		defer ff.Close()
		OutputAdj(ff, setup, biggest)
	}
}
