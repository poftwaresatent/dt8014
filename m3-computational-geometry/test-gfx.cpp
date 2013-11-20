#include "gfx.hpp"
#include <iostream>
#include <cmath>

using namespace std;


void cb_draw ()
{
  gfx_set_pen (1.0, 0.0, 0.0, 0.0);
  gfx_draw_arc (1.0, 0.0, 1.0, 0.0, 2 * M_PI);
  
  gfx_set_pen (1.0, 0.5, 0.5, 0.5);
  gfx_fill_arc (-1.0, 0.0, 1.0, 0.0, 2 * M_PI);
  
  gfx_set_pen (1.0, 1.0, 0.0, 0.0); // width, red, green, blue
  gfx_draw_line (0.0, 0.0, 1.0, 0.0); // x0, y0, x1, y1
  
  gfx_set_pen (1.0, 0.0, 1.0, 0.0);
  gfx_draw_line (0.0, 0.0, 0.0, 1.0);
  
  gfx_set_pen (1.0, 0.0, 0.0, 1.0);
  gfx_draw_line (0.0, 0.0, 1.0, 1.0);
  gfx_draw_line (1.0, 0.0, 0.0, 1.0);
}


void cb_click ()
{
  static int count = 0;
  switch (++count % 4) {
  case 0:
    gfx_set_view (-1.0, -1.0, 2.0, 2.0);
    break;
  case 1:
    gfx_set_view (-2.0, -1.0, 1.0, 2.0);
    break;
  case 2:
    gfx_set_view (-2.0, -2.0, 1.0, 1.0);
    break;
  default:
    gfx_set_view (-1.0, -2.0, 2.0, 1.0);
  }
}


int main (int argc, char ** argv)
{
  gfx_debug (&cout);
  gfx_add_button ("click", cb_click);
  gfx_main (cb_draw, "test-gfx");
}
