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

#ifndef DT8014_M2_GRAPH_HPP
#define DT8014_M2_GRAPH_HPP

#include <unordered_set>
#include <unordered_map>
#include <iosfwd>
#include <string>


namespace dt8014 {
  
  using namespace std;
  
  
  class Edge;
  
  
  class Vertex
  {
  public:
    explicit Vertex (string const & name);
    ~Vertex ();
    
    Edge & edge (Vertex & dst, string const & via);
    
    string const name_;
    int value_;
    Edge * back_;
    
    unordered_map <string, Edge *> out_;
  };
  
  
  class Edge
  {
  public:
    Edge (Vertex & src, Vertex & dst, string const & via);
    
    Vertex & src_;
    Vertex & dst_;
    unordered_set <string> via_;
  };
  
  
  class Graph
  {
  public:
    ~Graph ();
    
    Vertex & vertex (string const & name);
    Edge & edge (string const & src, string const & dst, string const & via);
    void print (ostream & os) const;
    void parse (istream & is);
    void load (string const & fname);
    
    unordered_map <string, Vertex *> vertex_;
  };

}

#endif /* DT8014_M2_GRAPH_HPP */
