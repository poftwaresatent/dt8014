#ifndef EX1_GFX_HPP
#define EX1_GFX_HPP


void gfx_set_pen (double width, double red, double green, double blue);
void gfx_draw_line (double x0, double y0, double x1, double y1);

void gfx_main (void (*draw_callback)());

#endif // EX1_GFX_HPP
