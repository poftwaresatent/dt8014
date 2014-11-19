#include "gfx.hpp"
#include <cmath>
#include <iostream>
#include <set>
#include <list>
#include <vector>


using namespace gfx;
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
    : xx (x), yy (y), rr (r), dx (0.0), dy (0.0), cc (c)
  {
  }
  
  bool grab (double mx, double my)
  {
    if (sqrt (pow (mx - xx, 2.0) + pow (my - yy, 2.0)) <= rr) {
      dx = xx - mx;
      dy = yy - my;
      return true;
    }
    return false;
  }
  
  void move (double mx, double my)
  {
    xx = mx + dx;
    yy = my + dy;
  }
  
  void draw () const
  {
    set_pen (1.0, cc.rr, cc.gg, cc.bb, cc.aa);
    fill_arc (xx, yy, rr, 0.0, 2 * M_PI);
  }
  
  double xx, yy, rr, dx, dy;
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
    pp.cc.aa *= 0.25;
    qq.cc.aa *= 0.25;
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
    set_pen (1.0, cc.rr, cc.gg, cc.bb, cc.aa);
    draw_line (pp.xx, pp.yy, qq.xx, qq.yy);
  }
  
  Handle pp, qq;
  Color cc;
};


static list<Line*> lines;

  
void cb_draw ()
{
  set_view (-1.0, -1.0, 1.0, 1.0);
  for (set<Handle*>::const_iterator ih (handles.begin()); ih != handles.end(); ++ih) {
    (*ih)->draw();
  }
  for (list<Line*>::const_iterator il (lines.begin()); il != lines.end(); ++il) {
    (*il)->draw();
    list<Line*>::const_iterator jl (il);
    for (++jl; jl != lines.end(); ++jl) {
      double px, py;
      if ((*il)->intersect (**jl, px, py)) {
	set_pen (1.0, 1.0, 0.0, 0.0, 1.0);
	draw_arc (px, py, 0.03, 0.0, 2 * M_PI);
      }
    }
  }
}


static vector<Color> colors;


void cb_mouse (double mx, double my, int flags)
{
  if (flags & MOUSE_PRESS) {
    
    for (set<Handle*>::iterator ih (handles.begin()); ih != handles.end(); ++ih) {
      if ((*ih)->grab (mx, my)) {
	grabbed = (*ih);
	break;
      }
    }
    
    if (0 == grabbed) {
      lines.push_back (new Line (mx, my, mx, my, colors[lines.size() % colors.size()]));
      grabbed = &lines.back()->qq;
    }
    
  }
  
  else if (flags & MOUSE_DRAG) {
    grabbed->move (mx, my);
  }
  
  else if (flags & MOUSE_RELEASE) {
    grabbed = 0;
  }
}


int main (int argc, char ** argv)
{
  colors.push_back (Color (0.0, 0.0, 0.5));
  colors.push_back (Color (0.0, 0.5, 0.0));
  colors.push_back (Color (0.0, 0.5, 0.5));
  colors.push_back (Color (0.5, 0.0, 0.0));
  colors.push_back (Color (0.5, 0.0, 0.5));
  colors.push_back (Color (0.5, 0.5, 0.0));
  
  lines.push_back (new Line (-0.5, -0.5,  -0.5, 0.5, colors[lines.size() % colors.size()]));
  lines.push_back (new Line ( 0.5, -0.5,   0.5, 0.5, colors[lines.size() % colors.size()]));
  
  main (argv[0], cb_draw, cb_mouse);
  for (list<Line*>::iterator il (lines.begin()); il != lines.end(); ++il) {
    delete *il;
  }
}
