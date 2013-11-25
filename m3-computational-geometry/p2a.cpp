#include "gfx.hpp"
#include <cmath>
#include <iostream>
#include <set>
#include <list>


using namespace dt8014::gfx;
using namespace std;


class Color
{
public:
  Color (double r, double g, double b, double a = 1.0)
    : rr (r), gg (g), bb (b), aa (a)
  {
  }
  
  double rr, gg, bb, aa;
};


class Handle
{
public:
  Handle (double x, double y, double r, Color const & c)
    : xx (x), yy (y), rr (r), cc (c)
  {
  }
  
  double pdist (double px, double py) const
  {
    return sqrt (pow (px - xx, 2.0) + pow (py - yy, 2.0));
  }
  
  void draw () const
  {
    set_pen (1.0, cc.rr, cc.gg, cc.bb, cc.aa);
    fill_arc (xx, yy, rr, 0.0, 2 * M_PI);
  }
  
  double xx, yy, rr;
  Color cc;
};


static set<Handle*> handles;
static Handle * grabbed (0);


static size_t solve (double a1, double a2,
		     double b1, double b2,
		     double k1, double k2,
		     double &x, double &y)
{
  static double const epsilon (1.0e-4);
  double const det0 (a1 * b2 - a2 * b1);
  double const det1 (k1 * b2 - k2 * b1);
  double const det2 (a1 * k2 - a2 * k1);
  if (fabs (det0) < epsilon) {
    return fabs (det1) < epsilon && fabs (det2) < epsilon ? 2 : 0;
  }
  x = det1 / det0;
  y = det2 / det0;
  return 1;
}


class Line
{
public:
  Line (double px, double py, double qx, double qy, Color const & c)
    : pp (px, py, 0.1, c),
      qq (qx, qy, 0.1, c),
      cc (c)
  {
    pp.cc.aa *= 0.5;
    qq.cc.aa *= 0.5;
    handles.insert (&pp);
    handles.insert (&qq);
  }
  
  ~Line ()
  {
    if (grabbed == &pp || grabbed == &qq) {
      grabbed = 0;
    }
    handles.erase (&pp);
    handles.erase (&qq);
  }
  
  bool intersect (Line const & other, double & px, double & py) const
  {
    double alpha, beta;
    size_t const result (solve (pp.xx - qq.xx,
				pp.yy - qq.yy,
				other.qq.xx - other.pp.xx,
				other.qq.yy - other.pp.yy,
				other.qq.xx - qq.xx,
				other.qq.yy - qq.yy,
				alpha, beta));
    if (result != 1) {
      return false;
    }
    if (alpha < 0.0 || alpha > 1.0 || beta < 0.0 || beta > 1.0) {
      return false;
    }
    px = alpha * pp.xx + (1.0 - alpha) * qq.xx;
    py = alpha * pp.yy + (1.0 - alpha) * qq.yy;
    return true;
  }
  
  void draw () const
  {
    set_pen (3.0, cc.rr, cc.gg, cc.bb, cc.aa);
    draw_line (pp.xx, pp.yy, qq.xx, qq.yy);
  }
  
  Handle pp, qq;
  Color cc;
};


static list<Line*> lines;

  
void cb_draw ()
{
  set_view (-1.0, -1.0, 1.0, 1.0);
  for (auto ih (handles.begin()); ih != handles.end(); ++ih) {
    (*ih)->draw();
  }
  for (auto il (lines.begin()); il != lines.end(); ++il) {
    (*il)->draw();
    auto jl (il);
    for (++jl; jl != lines.end(); ++jl) {
      double px, py;
      if ((*il)->intersect (**jl, px, py)) {
	set_pen (1.0, 0.5, 0.5, 1.0, 1.0);
	draw_arc (px, py, 0.05, 0.0, 2 * M_PI);
      }
    }
  }
}


void cb_mouse (double px, double py, int flags)
{
  if (flags & MOUSE_PRESS) {
    
    for (auto ih (handles.begin()); ih != handles.end(); ++ih) {
      if ((*ih)->pdist (px, py) < (*ih)->rr) {
	(*ih)->xx = px;
	(*ih)->yy = py;
	grabbed = (*ih);
	break;
      }
    }
    
  }
  
  else if (flags & MOUSE_DRAG) {
    
    if (0 != grabbed) {
      grabbed->xx = px;
      grabbed->yy = py;
    }
    
  }
  
  else if (flags & MOUSE_RELEASE) {
    
    if (0 != grabbed) {
      grabbed = 0;
    }
    
  }
}


int main (int argc, char ** argv)
{
  lines.push_back (new Line (-0.5, -0.5,  -0.5, 0.5, Color (0.5, 0.0, 0.0)));
  lines.push_back (new Line ( 0.5, -0.5,   0.5, 0.5, Color (0.0, 0.5, 0.0)));
  main (argv[0], cb_draw, cb_mouse);
  for (auto il (lines.begin()); il != lines.end(); ++il) {
    delete *il;
  }
}
