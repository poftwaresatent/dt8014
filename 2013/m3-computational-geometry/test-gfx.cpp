/* Halmstad University Algorithms Course Code Examples.
 *
 * Copyright (C) 2013 Roland Philippsen. All rights reserved.
 *
 * BSD license:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of
 *    contributors to this software may be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR THE CONTRIBUTORS TO THIS SOFTWARE BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
   \file test-gfx.cpp
   
   Small example program that uses gfx.hpp.
*/


#include "gfx.hpp"
#include <iostream>
#include <cmath>

using namespace std;


static double mx0, my0, mx1, my1;


/*
  This function is a callback which gets executed each time the gfx
  wrapper wants you to draw something on the canvas.  You pass the
  pointer to this function to gfx::main.
*/
static void cb_draw ()
{
  gfx::set_pen (1.0, 0.0, 0.0, 0.0, 1.0); // width, red, green, blue, alpha
  gfx::draw_arc (1.0, 0.0, 1.0, 0.0, 2 * M_PI);
  
  gfx::set_pen (1.0, 0.5, 0.5, 0.5, 1.0);
  gfx::fill_arc (-1.0, 0.0, 1.0, 0.0, 2 * M_PI);
  
  gfx::set_pen (1.0, 1.0, 0.0, 0.0, 1.0);
  gfx::draw_line (0.0, 0.0, 1.0, 0.0);
  
  gfx::set_pen (1.0, 0.0, 1.0, 0.0, 1.0);
  gfx::draw_line (0.0, 0.0, 0.0, 1.0);
  
  gfx::set_pen (1.0, 0.0, 0.0, 1.0, 1.0);
  gfx::draw_line (0.0, 0.0, 1.0, 1.0);
  gfx::draw_line (1.0, 0.0, 0.0, 1.0);
  
  gfx::set_pen (3.0, 0.5, 0.0, 0.0, 1.0);
  gfx::draw_line (mx0, my0, mx1, my1);
}


/*
  This is a callback which notifies you of mouse events.  Each time
  the mouse button is pressed, dragged, or released, this function
  gets called.  The parameters (mx,my) are the x- and y- cooridnates
  of the mouse at the time of the event, and the flags parameter is a
  bitmask encoding what happened.  For example, if the right mouse
  button got dragged while the Shift key was pressed, then flags will
  be gfx::MOUSE_DRAG | gfx::MOUSE_B2 | gfx::MOUSE_SHIFT.  Similarly to
  cb_draw, the pointer to this function gets passed to gfx::main.
*/
static void cb_mouse (double mx, double my, int flags)
{
  if (flags & gfx::MOUSE_PRESS) {
    mx0 = mx;
    my0 = my;
  }
  else if (flags & gfx::MOUSE_DRAG) {
    mx1 = mx;
    my1 = my;
  }
}


/*
  This callback will get registered for a custom button simply labeled
  "click".  The idea is that each custom button will get a separate
  such callback function, so that you can easily distinguish which one
  has been clicked.  (The test-polydraw example has more than one
  custom button).
*/
static void cb_click ()
{
  static int count = 0;
  switch (++count % 4) {
  case 0:
    gfx::set_view (-1.0, -1.0, 2.0, 2.0);
    break;
  case 1:
    gfx::set_view (-2.0, -1.0, 1.0, 2.0);
    break;
  case 2:
    gfx::set_view (-2.0, -2.0, 1.0, 1.0);
    break;
  default:
    gfx::set_view (-1.0, -2.0, 2.0, 1.0);
  }
}


int main (int argc, char ** argv)
{
  mx0 =  0.0;
  my0 =  0.0;
  mx1 =  0.5;
  my1 = -0.5;
  
  // This just enables (rather verbose) debug messages from the gfx
  // wrapper.  It is optional but can be rather useful.
  gfx::debug (&cout);
  
  // Adding a custom button needs to happen before gfx::main is called.
  gfx::add_button ("click", cb_click);
  
  // The gfx::main function enters the GUI processing loop.  It
  // returns when the user has clicked the "quit" button (which gets
  // added automatically) or otherwise exited the GUI application.
  gfx::main ("test-gfx", cb_draw, cb_mouse);
}
