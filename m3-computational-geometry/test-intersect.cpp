#include "gfx.hpp"
#include <cmath>


using namespace dt8014::gfx;


class Point
{
public:
  Point (double xx, double yy)
    : x_(xx), y_(yy)
  {
  }
  
  static double distance (double px, double py, double qx, double qy)
  {
    return sqrt (pow (px - qx, 2.0) + pow (py - qy, 2.0));
  }
  
  double x_, y_;
};


class Line
{
public:
  Line (double x0, double y0, double x1, double y1)
    : x0_(x0), y0_(y0), x1_(x1), y1_(y1)
  {
  }
  
  static Point const intersect (Line const & l1, Line const & l2)
  {
    Point pp (0.0, 0.0);
    return pp;
  }
  
  double x0_, y0_, x1_, y1_;
};


static Line ll (-0.5, -0.5, -0.5, 0.5);
static Line mm ( 0.5, -0.5,  0.5, 0.5);

static enum { NONE, L0, L1, M0, M1 } grabbed;

  
void cb_draw ()
{
  set_view (-1.0, -1.0, 1.0, 1.0);
  
  set_pen (1, 1.0, 0.5, 0.5);
  draw_line (ll.x0_, ll.y0_, ll.x1_, ll.y1_);
  set_pen (3, 1.0, 0.5, 0.5);
  draw_point (ll.x0_, ll.y0_);
  draw_point (ll.x1_, ll.y1_);
  
  set_pen (1, 0.5, 1.0, 0.5);
  draw_line (mm.x0_, mm.y0_, mm.x1_, mm.y1_);
  set_pen (3, 0.5, 1.0, 0.5);
  draw_point (mm.x0_, mm.y0_);
  draw_point (mm.x1_, mm.y1_);
  
  Point pp (Line::intersect (ll, mm));
  set_pen (1.0, 0.5, 0.5, 1.0);
  draw_arc (pp.x_, pp.y_, 0.1, 0.0, 2 * M_PI);
}


void cb_mouse (double px, double py, int flags)
{
  if (flags & MOUSE_PRESS) {
    if (Point::distance (px, ll.x0_, py, ll.y0_) < 0.2) {
      grabbed = L0;
      ll.x0_ = px;
      ll.y0_ = py;
    }
    else if (Point::distance (px, ll.x1_, py, ll.y1_) < 0.2) {
      grabbed = L1;
      ll.x1_ = px;
      ll.y1_ = py;
    }
    else if (Point::distance (px, mm.x0_, py, mm.y0_) < 0.2) {
      grabbed = M0;
      mm.x0_ = px;
      mm.y0_ = py;
    }
    else if (Point::distance (px, mm.x1_, py, mm.y1_) < 0.2) {
      grabbed = M1;
      mm.x1_ = px;
      mm.y1_ = py;
    }
  }
  else if (flags & MOUSE_DRAG) {
    if (L0 == grabbed) {
      ll.x0_ = px;
      ll.y0_ = py;
    }
    else if (L1 == grabbed) {
      ll.x1_ = px;
      ll.y1_ = py;
    }
    else if (M0 == grabbed) {
      mm.x0_ = px;
      mm.y0_ = py;
    }
    else if (M1 == grabbed) {
      grabbed = M1;
      mm.x1_ = px;
      mm.y1_ = py;
    }
  }
}


int main (int argc, char ** argv)
{
  main (argv[0], cb_draw, cb_mouse);
}
