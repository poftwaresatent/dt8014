#include <iostream>


struct circle {
  double cx, cy, rad;
};


struct rectangle {
  double x0, y0, x1, y1;
};


int main (int argc, char ** argv)
{
  circle aa;
  aa.cx = 0.6;
  aa.cy = 12.7;
  aa.rad = 21.3;
  
  rectangle bb;
  bb.x0 = 5.5;
  bb.y0 = -2.1;
  bb.x1 = 10.6;
  bb.y1 = 22.2;
  
  return 0;
}
