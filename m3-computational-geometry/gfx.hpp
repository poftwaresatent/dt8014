#ifndef DT8014_GFX_HPP
#define DT8014_GFX_HPP

#include <string>

namespace dt8014 {
  namespace gfx {
    
    typedef enum {
      MOUSE_PRESS   = 1 <<  1,
      MOUSE_RELEASE = 1 <<  2,
      MOUSE_DRAG    = 1 <<  3,
      MOUSE_B1      = 1 <<  4,	// usually the left button
      MOUSE_B2      = 1 <<  5,	// usually the right button
      MOUSE_B3      = 1 <<  6,
      MOUSE_B4      = 1 <<  7,
      MOUSE_B5      = 1 <<  8,
      MOUSE_SHIFT   = 1 <<  9,
      MOUSE_CTRL    = 1 << 10,
      MOUSE_MOD1    = 1 << 11,	// usually the Alt key
      MOUSE_MOD2    = 1 << 12,
      MOUSE_MOD3    = 1 << 13,
      MOUSE_MOD4    = 1 << 14,
      MOUSE_MOD5    = 1 << 15
    } mouse_flags_t;
    
    
    /*
      Only has an effect before main()
    */
    void add_button (std::string const & label, void (*click_callback)());
    
    /*
      Intended to be used /outside/ a draw_callback, but might also work inside.
    */
    void set_view (double x0, double y0, double x1, double y1);
    
    /*
      The following functions only have an effect inside a draw_callback.
    */
    void set_pen (double width, double red, double green, double blue);
    
    void draw_point (double x, double y);
    
    void draw_line (double x0, double y0, double x1, double y1);
    
    void draw_arc (double cx, double cy, double rr, double a0, double a1);
    
    void fill_arc (double cx, double cy, double rr, double a0, double a1);
    
    void begin_poly (double x, double y);
    
    void add_poly (double x, double y);
    
    void draw_poly ();
    
    void fill_poly ();
    
    /*
      Any extra buttons need to be set up before calling main() by
      using add_button(). This function only returns when the user has
      requested to quit the application. The given draw_callback()
      gets called each time the scene should be drawn. Inside the
      draw_callback, use the various other gfx:: functions to actually
      draw things.
    */
    void main (std::string const & window_title,
	       void (*draw_callback)(),
	       void (*mouse_callback)(double px, double py, int flags));
    
    std::ostream * debug (std::ostream * debug_os);
    
  }
}

#endif // DT8014_GFX_HPP
