#include "gfx.hpp"
#include <iostream>
#include <cmath>
#include <list>
#include <algorithm>

using namespace std;
using namespace gfx;


struct Point {
  Point (double xx, double yy) : x (xx), y (yy) {}
  double x, y;
};


struct Box {
  Box (double xx0, double yy0, double xx1, double yy1)
    : x0 (xx0), y0 (yy0), x1 (xx1), y1 (yy1) {}
  double x0, y0, x1, y1;
};


static double red, green, blue;
static list<Point> points;
static Box zoom (0.0, 0.0, 0.0, 0.0), view (-1.0, -1.0, 1.0, 1.0);
static bool zooming;


void cb_draw ()
{
  set_view (view.x0, view.y0, view.x1, view.y1);
  
  switch (points.size()) {

  case 0:
    break;

  case 1:
    set_pen (4.0, red, green, blue, 1.0);
    draw_point (points.front().x, points.front().y);
    break;

  case 2:
    set_pen (2.0, red, green, blue, 1.0);
    draw_line (points.front().x, points.front().y, points.back().x, points.back().y);
    break;

  default:
    set_pen (1.0, red, green, blue, 1.0);
    auto ip (points.begin());
    begin_poly (ip->x, ip->y);
    for (++ip; ip != points.end(); ++ip) {
      add_poly (ip->x, ip->y);
    }
    fill_poly ();
    set_pen (2.0, red / 2, green / 2, blue / 2, 1.0);
    ip = points.begin();
    begin_poly (ip->x, ip->y);
    for (++ip; ip != points.end(); ++ip) {
      add_poly (ip->x, ip->y);
    }
    draw_poly ();
  }
  
  if (zooming) {
    set_pen (1.0, 0.5, 0.5, 0.5, 1.0);
    draw_line (zoom.x0, zoom.y0, zoom.x1, zoom.y0);
    draw_line (zoom.x1, zoom.y0, zoom.x1, zoom.y1);
    draw_line (zoom.x1, zoom.y1, zoom.x0, zoom.y1);
    draw_line (zoom.x0, zoom.y1, zoom.x0, zoom.y0);
  }
}


void cb_mouse (double px, double py, int flags)
{
  if (flags & MOUSE_PRESS) {
    if (flags & MOUSE_B1) {
      points.push_back (Point (px, py));
    }
    else if (flags & MOUSE_B2) {
      zooming = true;
      zoom.x0 = px;
      zoom.y0 = py;
      zoom.x1 = px;
      zoom.y1 = py;
    }
  }
  else if (flags & MOUSE_DRAG) {
    if (flags & MOUSE_B1) {
      points.back().x = px;
      points.back().y = py;
    }
    else if (flags & MOUSE_B2) {
      zoom.x1 = px;
      zoom.y1 = py;
    }
  }
  else {
    if (flags & MOUSE_B2) {
      view.x0 = min (zoom.x0, zoom.x1);
      view.y0 = min (zoom.y0, zoom.y1);
      view.x1 = max (zoom.x0, zoom.x1);
      view.y1 = max (zoom.y0, zoom.y1);
    }
    zooming = false;
  }
}


void cb_zoom_out ()
{
  double const dx (fabs (view.x1 - view.x0));
  double const dy (fabs (view.y1 - view.y0));
  double const cx ((view.x0 + view.x1) / 2.0);
  double const cy ((view.y0 + view.y1) / 2.0);
  view.x0 = cx - dx;
  view.y0 = cy - dy;
  view.x1 = cx + dx;
  view.y1 = cy + dy;
}


void cb_zoom_fit ()
{
  if (points.empty()) {
    view.x0 = -1.0;
    view.y0 = -1.0;
    view.x1 =  1.0;
    view.y1 =  1.0;
  }
  else {
    auto ip (points.begin());
    view.x0 = ip->x;
    view.y0 = ip->y;
    view.x1 = ip->x;
    view.y1 = ip->y;
    for (++ip; ip != points.end(); ++ip) {
      view.x0 = min (view.x0, ip->x);
      view.y0 = min (view.y0, ip->y);
      view.x1 = max (view.x1, ip->x);
      view.y1 = max (view.y1, ip->y);
    }
    if (fabs (view.x1 - view.x0) < 1e-3) {
      view.x0 -= 0.1;
      view.x1 += 0.1;
    }
    if (fabs (view.y1 - view.y0) < 1e-3) {
      view.y0 -= 0.1;
      view.y1 += 0.1;
    }
  }
}


int main (int argc, char ** argv)
{
  red = 0.0;
  green = 1.0;
  blue = 0.5;
  debug (&cout);
  add_button ("zoom out", cb_zoom_out);
  add_button ("zoom fit", cb_zoom_fit);
  main (argv[0], cb_draw, cb_mouse);
}
