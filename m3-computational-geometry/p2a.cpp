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
    px = 0.0;
    py = 0.0;
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
  cout << "cb_draw handles";
  for (auto ih (handles.begin()); ih != handles.end(); ++ih) {
    cout << ".";
    (*ih)->draw();
  }
  cout << " lines";
  for (auto il (lines.begin()); il != lines.end(); ++il) {
    cout << ".";
    (*il)->draw();
  }
  cout << "\n";
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
