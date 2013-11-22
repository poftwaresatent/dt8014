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
