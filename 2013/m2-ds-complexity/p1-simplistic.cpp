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

/**
   \file p1-simplistic.cpp
   
   Simplistic solution example for project 1.  Characters and books
   are both encoded as vertices.  Everything is stored in hashmaps, no
   attempt is made to implement a "proper" graph structure.
*/

#include <iostream>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <list>

using namespace std;


static string chop (string const & in)
{
  size_t beg (0);
  size_t len (in.size());
  while (in[beg] == '"' || in[beg] == ' ') {
    ++beg;
    --len;
  }
  while (in[beg+len-1] == '"' || in[beg+len-1] == ' ') {
    --len;
  }
  return in.substr (beg, len);
}


int main (int argc, char ** argv)
{
  if (argc < 4) {
    cerr << "three arguments required: file name, source name, destination name\n";
    return 42;
  }
  
  ifstream ds (argv[1]);
  string character, book;
  unordered_map <string, unordered_set <string>> graph;
  
  while (getline (ds, character, '\t')) {
    if ( !getline (ds, book, '\n')) {
      break;
    }
    character = chop (character);
    book = chop (book);
    if (0 == character.size() || 0 == book.size()) {
      continue;
    }
    graph[character].insert (book);
    graph[book].insert (character);
  }
  
  cout << "parsed graph:\n";
  for (auto iv (graph.begin()); iv != graph.end(); ++iv) {
    for (auto ie (iv->second.begin()); ie != iv->second.end(); ++ie) {
      cout << "  {" << iv->first << "} -> {" << *ie << "}\n";
    }
  }
  
  string const src (argv[2]);
  string const dst (argv[3]);
  unordered_map <string, size_t> dist;
  unordered_map <string, string> back;
  list <string> queue;
  
  queue.push_back (src);
  dist.insert (make_pair (src, 0));
  bool found (false);
  while ( !queue.empty()) {
    
    auto ifrom (graph.find (queue.front()));
    if (ifrom == graph.end()) {
      cerr << "ERROR queue item {" << queue.front() << "} not found in graph\n";
      return 42;
    }
    
    size_t const dnext (dist[queue.front()] + 1);
    for (auto ito (ifrom->second.begin()); ito != ifrom->second.end(); ++ito) {
      if (dist.find (*ito) == dist.end()) {
	dist.insert (make_pair (*ito, dnext));
	back.insert (make_pair (*ito, queue.front()));
	if (*ito == dst) {
	  found = true;
	  break;
	}
	queue.push_back (*ito);
      }
    }
    if (found) {
      break;
    }
    
    queue.pop_front();
  }
  
  if ( !found) {
    cout << "no path from {" << src << "} to {" << dst << "}\n";
    return 0;
  }
  
  queue.clear ();
  queue.push_front (dst);
  for (auto iback (back.find (dst)); iback != back.end(); iback = back.find (iback->second)) {
    queue.push_front (iback->second);
  }
  cout << "the path from {" << src << "} to {" << dst << "} takes " << queue.size() << " steps\n";
  for (auto ipath (queue.begin()); ipath != queue.end(); ++ipath) {
    cout << "  {" << *ipath << "}\n";
  }
}
