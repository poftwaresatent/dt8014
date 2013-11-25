#include "gfx.hpp"
#include <cmath>
#include <iostream>
#include <set>
#include <list>


using namespace dt8014::gfx;
using namespace std;


class Handle
{
public:
  Handle (double px, double py, double radius,
	  double red, double green, double blue)
    : px_(px), py_(py), radius_(radius),
      red_(red), green_(green), blue_(blue)
  {
  }
  
  double pdist (double px, double py) const
  {
    return sqrt (pow (px - px_, 2.0) + pow (py - py_, 2.0));
  }
  
  void draw () const
  {
    cout << "draw handle " << px_ << "  " << py_ << "  " << radius_ << "\n";
    set_pen (1.0, red_, green_, blue_, 0.5);
    fill_arc (px_, py_, radius_, 0.0, 2 * M_PI);
  }
  
  double px_, py_, radius_;
  double red_, green_, blue_;
};


static set<Handle*> handles;
static Handle * grabbed (0);


static size_t solve (double a1, double a2,
		     double b1, double b2,
		     double k1, double k2,
		     double &x, double &y)
{
  static double const epsilon (1.0e-4);
  double const dd (a1 * b2 - a2 * b1);
  double const db (k1 * b2 - k2 * b1);
  double const da (a1 * k2 - a2 * k1);
  if (fabs (dd) < epsilon) {
    return fabs (db) < epsilon && fabs (da) < epsilon ? 2 : 0;
  }
  x = db / dd;
  y = da / dd;
  return 1;
}


class Line
{
public:
  Line (double px, double py, double qx, double qy,
	double red, double green, double blue)
    : hp_(px, py, 0.1, red, green, blue),
      hq_(qx, qy, 0.1, red, green, blue),
      red_(red), green_(green), blue_(blue)
  {
    handles.insert (&hp_);
    handles.insert (&hq_);
  }
  
  ~Line ()
  {
    if (grabbed == &hp_ || grabbed == &hq_) {
      grabbed = 0;
    }
    handles.erase (&hp_);
    handles.erase (&hq_);
  }
  
  bool intersect (Line const & other, double & px, double & py) const
  {
    double alpha, beta;
    size_t const result (solve (hp_.px_ - hq_.px_,
				hp_.py_ - hq_.py_,
				other.hq_.px_ - other.hp_.px_,
				other.hq_.py_ - other.hp_.py_,
				other.hq_.px_ - hq_.px_,
				other.hq_.py_ - hq_.py_,
				alpha, beta));
    if (result != 1) {
      return false;
    }
    if (alpha < 0.0 || alpha > 1.0 || beta < 0.0 || beta > 1.0) {
      return false;
    }
    px = alpha * hp_.px_ + (1.0 - alpha) * hq_.px_;
    py = alpha * hp_.py_ + (1.0 - alpha) * hq_.py_;
    return true;
  }
  
  void draw () const
  {
    set_pen (3.0, red_, green_, blue_, 1.0);
    draw_line (hp_.px_, hp_.py_, hq_.px_, hq_.py_);
  }
  
  Handle hp_, hq_;
  double red_, green_, blue_;
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
  cout << "cb_mouse " << px << "  " << py << ": ";
  
  if (flags & MOUSE_PRESS) {
    
    for (auto ih (handles.begin()); ih != handles.end(); ++ih) {
      if ((*ih)->pdist (px, py) < (*ih)->radius_) {
	cout << "grabbed\n";
	(*ih)->px_ = px;
	(*ih)->py_ = py;
	grabbed = (*ih);
	break;
      }
    }
    if (0 == grabbed) {
      cout << "nothing to grab\n";
    }
    
  }
  
  else if (flags & MOUSE_DRAG) {
    
    if (0 != grabbed) {
      cout << "dragged\n";
      grabbed->px_ = px;
      grabbed->py_ = py;
    }
    
    else {
      cout << "nothing to drag\n";
    }
    
  }
  
  else if (flags & MOUSE_RELEASE) {
    
    if (0 != grabbed) {
      cout << "released\n";
      grabbed = 0;
    }
    
    else {
      cout << "nothing to release\n";
    }
    
  }
}


int main (int argc, char ** argv)
{
  lines.push_back (new Line (-0.5, -0.5,  -0.5, 0.5,  0.5, 0.0, 0.0));
  lines.push_back (new Line ( 0.5, -0.5,   0.5, 0.5,  0.0, 0.5, 0.0));
  main (argv[0], cb_draw, cb_mouse);
  for (auto il (lines.begin()); il != lines.end(); ++il) {
    delete *il;
  }
}
