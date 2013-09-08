#include "gfx.hpp"
#include <iostream>

using namespace std;


int main (int argc, char ** argv)
{
  cerr << "main: before gfx_main\n";
  gfx_main (argc, argv);
  cerr << "main: after gfx_main\n";
}
