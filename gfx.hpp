#ifndef EX1_GFX_HPP
#define EX1_GFX_HPP

#include <string>


/* Only has an effect before gfx_main() */
void gfx_add_button (std::string const & label, void (*click_callback)());

/* Only has an effect inside a draw_callback. */
void gfx_set_view (double x0, double y0, double x1, double y1);

/* Only has an effect inside a draw_callback. */
void gfx_set_pen (double width, double red, double green, double blue);

/* Only has an effect inside a draw_callback. */
void gfx_draw_line (double x0, double y0, double x1, double y1);

/* Only has an effect inside a draw_callback. */
void gfx_draw_arc (double cx, double cy, double rr, double a0, double a1);

/* Only has an effect inside a draw_callback. */
void gfx_fill_arc (double cx, double cy, double rr, double a0, double a1);

/* Any extra buttons need to be set up before calling gfx_main() by
   using gfx_add_button(). This function only returns when the user
   has requested to quit the application. The given draw_callback()
   gets called each time the scene should be drawn. Inside the
   draw_callback, use the various other gfx_ functions to actually
   draw things. */
void gfx_main (void (*draw_callback)());

std::ostream * gfx_debug (std::ostream * debug_os);


#endif // EX1_GFX_HPP
