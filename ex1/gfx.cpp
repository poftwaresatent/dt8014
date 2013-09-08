#include <gtk/gtk.h>
#include <iostream>


static GtkWidget * canvas;
static gint canvas_width, canvas_height;
static gint canvas_sx, canvas_sy, canvas_x0, canvas_y0;
static double view_x0, view_y0, view_x1, view_y1;
static cairo_t * cairo;

static void (*draw)();


static double v2cx (double vx)
{
  return canvas_x0 + (vx - view_x0) * canvas_sx;
}


static double v2cy (double vy)
{
  return canvas_y0 + (vy - view_y0) * canvas_sy;
}


static gint cb_idle (gpointer data)
{
  gtk_widget_queue_draw (canvas);
  return TRUE;
}


static gint cb_expose (GtkWidget * ww,
		       GdkEventExpose * ee,
		       gpointer data)
{
  cairo = gdk_cairo_create (ee->window);
  
  cairo_set_source_rgb (cairo, 1.0, 1.0, 1.0);
  cairo_rectangle (cairo, 0, 0, canvas_width, canvas_height);
  cairo_fill (cairo);
  
  draw ();
  
  cairo_destroy (cairo);
  cairo = 0;
  
  return TRUE;
}


static gint cb_size_allocate (GtkWidget * ww,
			      GtkAllocation * aa,
			      gpointer data)
{
  canvas_width = aa->width;
  canvas_height = aa->height;
  
  canvas_sx = canvas_width / (view_x1 - view_x0);
  if (canvas_sx < 1) {
    canvas_sx = 1;
  }
  canvas_sy = - canvas_height / (view_y1 - view_y0);
  if ( - canvas_sy < 1) {
    canvas_sy = -1;
  }
  if (canvas_sx > - canvas_sy) {
    canvas_sx = - canvas_sy;
  }
  else {
    canvas_sy = - canvas_sx;
  }
  canvas_x0 = (canvas_width - (view_x1 - view_x0) * canvas_sx) / 2;
  canvas_y0 = canvas_height - (canvas_height + (view_y1 - view_y0) * canvas_sy) / 2;
  
  return TRUE;
}


static void cb_quit (GtkWidget * ww, gpointer data)
{
  gtk_main_quit();
}


void gfx_set_pen (double width, double red, double green, double blue)
{
  if (!cairo) {
    return;
  }
  cairo_set_source_rgb (cairo, red, green, blue);
  ////  cairo_set_line_width (cairo, width / canvas_sx);
  cairo_set_line_width (cairo, 1.0);
}


void gfx_draw_line (double x0, double y0, double x1, double y1)
{
  if (!cairo) {
    return;
  }
  cairo_move_to (cairo, v2cx(x0), v2cy(y0));
  cairo_line_to (cairo, v2cx(x1), v2cy(y1));
  cairo_stroke (cairo);
}


void gfx_main (void (*draw_callback)())
{
  GtkWidget *window, *vbox, *hbox, *btn;
  
  gtk_init (0, 0);
  view_x0 = -1.0;
  view_y0 = -1.0;
  view_x1 =  1.0;
  view_y1 =  1.0;
  draw = draw_callback;
  cairo = 0;
  
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  
  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), vbox);
  gtk_widget_show (vbox);
  
  canvas = gtk_drawing_area_new ();
  g_signal_connect (canvas, "expose_event", G_CALLBACK (cb_expose), NULL);
  g_signal_connect (canvas, "size_allocate", G_CALLBACK (cb_size_allocate), NULL);
  // g_signal_connect (canvas, "button_press_event", G_CALLBACK (cb_click), NULL);
  // g_signal_connect (canvas, "button_release_event", G_CALLBACK (cb_click), NULL);
  // g_signal_connect (canvas, "motion_notify_event", G_CALLBACK (cb_motion), NULL);
  // gtk_widget_set_events (canvas,
  // 			 GDK_BUTTON_PRESS_MASK |
  // 			 GDK_BUTTON_RELEASE_MASK |
  // 			 GDK_BUTTON_MOTION_MASK);
  
  gtk_widget_show (canvas);
  
  gtk_widget_set_size_request (canvas, 400, 500);
  gtk_box_pack_start (GTK_BOX (vbox), canvas, TRUE, TRUE, 0);
  
  hbox = gtk_hbox_new (TRUE, 3);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 0);
  gtk_widget_show (hbox);
  
  btn = gtk_button_new_with_label ("quit");
  g_signal_connect (btn, "clicked", G_CALLBACK (cb_quit), NULL);
  gtk_box_pack_start (GTK_BOX (hbox), btn, TRUE, TRUE, 0);
  gtk_widget_show (btn);
  
  gtk_idle_add (cb_idle, 0);
  
  gtk_widget_show (window);
  
  std::cerr << "gfx_main: before gtk_main\n";
  gtk_main ();
  std::cerr << "gfx_main: after gtk_main\n";
}
