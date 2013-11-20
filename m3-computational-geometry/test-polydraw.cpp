#include "gfx.hpp"
#include <iostream>
#include <cmath>
#include <list>

using namespace std;
using namespace dt8014::gfx;


struct Point {
  Point (double xx, double yy) : x (xx), y (yy) {}
  double x, y;
};


static double red, green, blue;
static list<Point> points;


void cb_draw ()
{
  switch (points.size()) {
  case 0:
    break;
  case 1:
    set_pen (4.0, red, green, blue);
    draw_point (points.front().x, points.front().y);
    break;
  case 2:
    set_pen (1.0, red, green, blue);
    draw_line (points.front().x, points.front().y, points.back().x, points.back().y);
    break;
  default:
    set_pen (1.0, red, green, blue);
    auto ip (points.begin());
    begin_poly (ip->x, ip->y);
    for (++ip; ip != points.end(); ++ip) {
      add_poly (ip->x, ip->y);
    }
    end_poly ();
  }
}


void cb_mouse (double px, double py, mouse_event_t ee)
{
  switch (ee) {
  case MOUSE_PRESS:
    points.push_back (Point (px, py));
    break;
  case MOUSE_DRAG:
    points.back().x = px;
    points.back().y = py;
  }
}


void cb_red ()
{
  red = 1.0;
  green = 0.0;
  blue = 0.0;
}


void cb_green ()
{
  red = 0.0;
  green = 1.0;
  blue = 0.0;
}


void cb_blue ()
{
  red = 0.0;
  green = 0.0;
  blue = 1.0;
}


int main (int argc, char ** argv)
{
  debug (&cout);
  add_button ("red", cb_red);
  add_button ("green", cb_green);
  add_button ("blue", cb_blue);
  set_view (-1.0, -1.0, 1.0, 1.0);
  main (argv[0], cb_draw, cb_mouse);
}
