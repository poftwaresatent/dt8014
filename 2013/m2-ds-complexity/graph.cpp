/* Halmstad University Algorithms Course Code Examples.
 *
 * Copyright (C) 2013 Roland Philippsen. All rights reserved.
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

#include "graph.hpp"
#include <iostream>
#include <fstream>


namespace dt8014 {

  
  Vertex::
  Vertex (string const & name)
    : name_ (name),
      value_ (-1),
      back_ (0)
  {
  }


  Vertex::
  ~Vertex ()
  {
    for (auto ie (out_.begin()); ie != out_.end(); ++ie) {
      delete ie->second;
    }
  }
  
  
  Edge & Vertex::
  edge (Vertex & dst, string const & via)
  {
    auto ie (out_.find (dst.name_));
    if (ie == out_.end()) {
      Edge * ee (new Edge (*this, dst, via));
      out_.insert (make_pair (dst.name_, ee));
      return *ee;
    }
    ie->second->via_.insert (via);
    return *(ie->second);
  }
  
  
  Edge::
  Edge (Vertex & src, Vertex & dst, string const & via)
    : src_ (src),
      dst_ (dst)
  {
    via_.insert (via);
  }
  
  
  Graph::
  ~Graph ()
  {
    for (auto iv (vertex_.begin()); iv != vertex_.end(); ++iv) {
      delete iv->second;
    }
  }
  
  
  Vertex & Graph::
  vertex (string const & name)
  {
    auto iv (vertex_.find (name));
    if (iv == vertex_.end()) {
      Vertex * vv (new Vertex (name));
      vertex_.insert (make_pair (name, vv));
      return *vv;
    }
    return *(iv->second);
  }
  
  
  Edge & Graph::
  edge (string const & src, string const & dst, string const & via)
  {
    Vertex & vsrc (vertex (src));
    Vertex & vdst (vertex (dst));
    return vsrc.edge (vdst, via);
  }
  
  
  void Graph::
  print (ostream & os) const
  {
    os << "digraph \"dt8014\" {\n  graph [overlap=scale];\n";
    
    for (auto iv (vertex_.begin()); iv != vertex_.end(); ++iv) {
      os << "  \"" << iv->first
	 << "\" [label=\"" << iv->first
	 << "\\n" << iv->second->value_
	 << "\"" << (iv->second->value_ < 0 ? ",style=dotted];\n" :
		     (iv->second->value_ == 0 ? ",style=bold];\n" :
		      "];\n"));
    }
    
    for (auto iv (vertex_.begin()); iv != vertex_.end(); ++iv) {
      for (auto ie (iv->second->out_.begin()); ie != iv->second->out_.end(); ++ie) {
	os << "  \"" << iv->first << "\" -> \"" << ie->first << "\" [label=\"";
	auto ii (ie->second->via_.begin());
	os << *ii;
	for (++ii; ii != ie->second->via_.end(); ++ii) {
	  os << ", " << *ii;
	}
	os << "\",len=5"
	   << (iv->second->value_ < 0 ? ",style=dotted];\n" :
	       (ie->second->dst_.back_ == ie->second ? ",style=bold];\n" :
		"];\n"));
      }
    }
    
    os << "}\n";
  }
  
  
  void Graph::
  parse (istream & is)
  {
    unordered_map <string, unordered_set <string> > data;
    while (is) {
      string src;
      is >> src;
      if ( !is) {
	break;
      }
      string via;
      is >> via;
      data[via].insert (src);
    }
    
    for (auto ii (data.begin()); ii != data.end(); ++ii) {
      for (auto jj (ii->second.begin()); jj != ii->second.end(); ++jj) {
	for (auto kk (ii->second.begin()); kk != ii->second.end(); ++kk) {
	  if (jj != kk) {
	    edge (*jj, *kk, ii->first);
	    // cout << *jj << " -> " << ii->first << " -> " << *kk << "\n";
	  }
	}
      }
    }
  }
  
  
  void Graph::
  load (string const & fname)
  {
    ifstream is (fname);
    parse (is);
  }
  
}
