#include "gfx.hpp"
#include <iostream>

using namespace std;


void cb_draw ()
{
  gfx_set_pen (1.0, 1.0, 0.0, 0.0); // width, red, green, blue
  gfx_draw_line (0.0, 0.0, 1.0, 0.0); // x0, y0, x1, y1
  
  gfx_set_pen (1.0, 0.0, 1.0, 0.0);
  gfx_draw_line (0.0, 0.0, 0.0, 1.0);
  
  gfx_set_pen (1.0, 0.0, 0.0, 1.0);
  gfx_draw_line (0.0, 0.0, 1.0, 1.0);
  gfx_draw_line (1.0, 0.0, 0.0, 1.0);
}


int main (int argc, char ** argv)
{
  cerr << "main: before gfx_main\n";
  gfx_main (cb_draw);
  cerr << "main: after gfx_main\n";
}
