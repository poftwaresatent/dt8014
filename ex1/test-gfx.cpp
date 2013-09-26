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


void cb_click ()
{
  static int count = 0;
  if (++count % 2) {
    gfx_set_view (-2.0, -2.0, 2.0, 2.0);
  }
  else {
    gfx_set_view (-3.0, -3.0, 3.0, 3.0);
  }
}


int main (int argc, char ** argv)
{
  gfx_add_button ("click", cb_click);
  gfx_main (cb_draw);
}
