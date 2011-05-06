/*
 * Copyright (C) 2009, 2010 Hermann Meyer, James Warden, Andreas Degert
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


#include "GxPaintBox.h"
#include "GxGradient.h"
#include <gtk/gtkprivate.h>
#include <gtk/gtk.h>
#include <cmath>
#include <cstring>

#define P_(s) (s)   // FIXME -> gettext




enum {
	PROP_PAINT_FUNC = 1,
	PROP_BORDER_WIDTH = 2,
	PROP_SPACING = 3,
	PROP_HOMOGENEOUS = 4,
	PROP_ICON_SET = 5,
	PROP_WIDTH = 6,
	PROP_HEIGHT = 7,
};

static void gx_paint_box_destroy(GtkObject *object);
static void gx_paint_box_set_property(
	GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static void gx_paint_box_get_property(
	GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);
static gboolean gx_paint_box_expose(GtkWidget *widget, GdkEventExpose *event);
static void gx_paint_box_style_set (GtkWidget *widget, GtkStyle  *previous_style);

G_DEFINE_TYPE(GxPaintBox, gx_paint_box, GTK_TYPE_HBOX)

#define get_stock_id(widget) (GX_PAINT_BOX_CLASS(GTK_OBJECT_GET_CLASS(widget))->stock_id)
#define get_main_image_id(widget) (GX_PAINT_BOX_CLASS(GTK_OBJECT_GET_CLASS(widget))->main_image_id)

static void gx_paint_box_class_init (GxPaintBoxClass *klass)
{
	GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
	GtkObjectClass *object_class = (GtkObjectClass*) klass;
	GtkWidgetClass *widget_class = (GtkWidgetClass*)klass;
	gobject_class->set_property = gx_paint_box_set_property;
	gobject_class->get_property = gx_paint_box_get_property;
	object_class->destroy = gx_paint_box_destroy;
	widget_class->style_set = gx_paint_box_style_set;
	widget_class->expose_event = gx_paint_box_expose;
	klass->stock_id = "gxhead";
	klass->main_image_id = "main_image";
	g_object_class_install_property(
		gobject_class, PROP_PAINT_FUNC,
		g_param_spec_string("paint-func",
		                    P_("Paint Type"),
		                    P_("Type of paint function for background"),
		                    "",
		                    GParamFlags(GTK_PARAM_READWRITE)));
	gtk_widget_class_install_style_property_parser(
		GTK_WIDGET_CLASS(klass),
		g_param_spec_boxed("skin-gradient",
		                   P_("Skin color"),
		                   P_("Color gradient defined as part of skin"),
		                   GX_TYPE_GRADIENT,
		                   GParamFlags(GTK_PARAM_READABLE)),
		gx_parse_gradient);
	gtk_widget_class_install_style_property(
		GTK_WIDGET_CLASS(klass),
		g_param_spec_string("paint-func",
		                    P_("Paint Type"),
		                    P_("Type of paint function for background"),
		                    NULL,
		                    GParamFlags(GTK_PARAM_READABLE)));
	g_object_class_install_property(
		gobject_class,PROP_ICON_SET,
	    g_param_spec_int ("icon-set",
						 P_("Icon Set"),
						 P_("Type of Icon function for background"),
						 0,
						 G_MAXINT,
						 0,
						 GParamFlags(GTK_PARAM_READWRITE)));
	gtk_widget_class_install_style_property(
		GTK_WIDGET_CLASS(klass),
		g_param_spec_int("icon-set",
		                 P_("Icon Set"),
		                 P_("Type of Icon function for background"),
		                 0,
						 G_MAXINT,
						 0,
		                 GParamFlags(GTK_PARAM_READABLE)));
	g_object_class_install_property(
		gobject_class,PROP_WIDTH,
	    g_param_spec_int ("width",
						 P_("Width"),
						 P_("size.width request for paintbox"),
						 0,
						 G_MAXINT,
						 0,
						 GParamFlags(GTK_PARAM_READWRITE)));
	gtk_widget_class_install_style_property(
		GTK_WIDGET_CLASS(klass),
		g_param_spec_int("width",
		                 P_("Width"),
		                 P_("size.width request for paintbox"),
		                 0,
						 G_MAXINT,
						 0,
		                 GParamFlags(GTK_PARAM_READABLE)));
	g_object_class_install_property(
		gobject_class,PROP_HEIGHT,
	    g_param_spec_int ("height",
						 P_("Height"),
						 P_("size.height request for paintbox"),
						 0,
						 G_MAXINT,
						 0,
						 GParamFlags(GTK_PARAM_READWRITE)));
	gtk_widget_class_install_style_property(
		GTK_WIDGET_CLASS(klass),
		g_param_spec_int("height",
		                 P_("Height"),
		                 P_("size.height request for paintbox"),
		                 0,
						 G_MAXINT,
						 0,
		                 GParamFlags(GTK_PARAM_READABLE)));

}

static void set_expose_func(GxPaintBox *paint_box, const gchar *paint_func);

static void set_paint_func(GxPaintBox *paint_box, const gchar *paint_func)
{
	gchar *spf;
	gtk_widget_style_get(GTK_WIDGET(paint_box), "paint-func", &spf, NULL);
	if (spf) {
		if (paint_box->paint_func && strcmp(paint_box->paint_func, spf) == 0) {
			return;
		}
	} else {
		if (!paint_func) {
			paint_func = "";
		}
		if (paint_box->paint_func && strcmp(paint_box->paint_func, paint_func) == 0) {
			return;
		}
		spf = g_strdup(paint_func);
	}
	g_free(paint_box->paint_func);
	paint_box->paint_func = spf;
	set_expose_func(paint_box, spf);
	g_object_notify(G_OBJECT(paint_box), "paint-func");
}

static void gx_paint_box_style_set(GtkWidget *widget, GtkStyle  *previous_style)
{
	GxPaintBox *paint_box = GX_PAINT_BOX(widget);
	set_paint_func(paint_box, paint_box->paint_func);
}

static void gx_paint_box_init (GxPaintBox *paint_box)
{
	paint_box->paint_func = g_strdup("");
	gtk_widget_set_redraw_on_allocate(GTK_WIDGET(paint_box), TRUE);
	set_paint_func(paint_box, NULL);
}

static void gx_paint_box_destroy(GtkObject *object)
{
	GxPaintBox *paint_box = GX_PAINT_BOX(object);
	if (paint_box->paint_func) {
		g_free(paint_box->paint_func);
		paint_box->paint_func = NULL;
	}
	GtkWidget *wi = GTK_WIDGET( g_object_new (GX_TYPE_PAINT_BOX, NULL ));
	if (G_IS_OBJECT(GX_PAINT_BOX_CLASS(GTK_OBJECT_GET_CLASS(wi))-> gxh_image)) {
		g_object_unref(GX_PAINT_BOX_CLASS(GTK_OBJECT_GET_CLASS(wi))->gxh_image);
	}
	if (G_IS_OBJECT(GX_PAINT_BOX_CLASS(GTK_OBJECT_GET_CLASS(wi))-> gxr_image)) {
		g_object_unref(GX_PAINT_BOX_CLASS(GTK_OBJECT_GET_CLASS(wi))->gxr_image);
	}
}

static gboolean gx_paint_box_expose(GtkWidget *widget, GdkEventExpose *event)
{
	GxPaintBox *paint_box = GX_PAINT_BOX(widget);
	if (paint_box->expose_func) {
		paint_box->expose_func(widget, event);
	}
	GTK_WIDGET_CLASS(GTK_OBJECT_CLASS(gx_paint_box_parent_class))->expose_event(widget, event);
	return FALSE;
}
static void set_icon(GxPaintBox *paint_box, int value)
{
	int spf;
	gtk_widget_style_get(GTK_WIDGET(paint_box), "icon-set", &spf, NULL);
	 paint_box->icon_set = spf;
}
static void set_width(GxPaintBox *paint_box, int value)
{
	int spf;
	gtk_widget_style_get(GTK_WIDGET(paint_box), "width", &spf, NULL);
	 paint_box->width = spf;
}
static void set_height(GxPaintBox *paint_box, int value)
{
	int spf;
	gtk_widget_style_get(GTK_WIDGET(paint_box), "height", &spf, NULL);
	 paint_box->height = spf;
}
static void gx_paint_box_set_property(
	GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	GxPaintBox *paint_box = GX_PAINT_BOX(object);
	switch (prop_id) {
	case PROP_PAINT_FUNC:
		set_paint_func(paint_box, g_value_get_string(value));
		break;
	case PROP_BORDER_WIDTH:
		gtk_container_set_border_width (GTK_CONTAINER(paint_box), g_value_get_uint (value));
		break;
	case PROP_SPACING:
		gtk_box_set_spacing (GTK_BOX(paint_box), g_value_get_int (value));
		break;
	case PROP_HOMOGENEOUS:
		gtk_box_set_homogeneous (GTK_BOX(paint_box), g_value_get_boolean (value));
		break;
	case PROP_ICON_SET:
		set_icon(paint_box, g_value_get_int(value));
		break;
	case PROP_WIDTH:
		set_width(paint_box, g_value_get_int(value));
		break;
	case PROP_HEIGHT:
		set_height(paint_box, g_value_get_int(value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
		break;
	}
}

static void gx_paint_box_get_property(
	GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	switch (prop_id) {
	case PROP_PAINT_FUNC:
		g_value_set_string(value, GX_PAINT_BOX(object)->paint_func);
		break;
	case PROP_BORDER_WIDTH:
		g_value_set_uint (value, GX_PAINT_BOX(object)->border_width);
		break;
	case PROP_SPACING:
		g_value_set_int (value, GX_PAINT_BOX(object)->spacing);
		break;
	case PROP_HOMOGENEOUS:
		g_value_set_boolean (value, GX_PAINT_BOX(object)->homogeneous);
		break;
	case PROP_ICON_SET:
		g_value_set_int (value, GX_PAINT_BOX(object)->icon_set);
		break;
	case PROP_WIDTH:
		g_value_set_int (value, GX_PAINT_BOX(object)->width);
		break;
	case PROP_HEIGHT:
		g_value_set_int (value, GX_PAINT_BOX(object)->height);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
		break;
	}
}

void gx_box_pack_start (GxPaintBox *box, GtkWidget *child, 
	gboolean expand, gboolean fill, guint padding)
{
	gtk_box_pack_start (GTK_BOX(box), child, expand, fill, padding);
}

void gx_box_pack_end (GxPaintBox *box, GtkWidget *child,
	gboolean expand, gboolean fill, guint padding)
{
	gtk_box_pack_end (GTK_BOX(box), child, expand, fill, padding);
}

void gx_box_add (GxPaintBox *container, GtkWidget *widget)
{
	gtk_container_add (GTK_CONTAINER(container), widget);
}

void gx_box_remove (GxPaintBox *container,GtkWidget *widget)
{
	gtk_container_remove (GTK_CONTAINER(container), widget);
}

void gx_box_set_border_width (GxPaintBox *container, guint border_width)
{
	gtk_container_set_border_width (GTK_CONTAINER(container), border_width);
}

GList*  gx_box_get_children(GxPaintBox *container)
{
	return gtk_container_get_children (GTK_CONTAINER(container));
}

GtkWidget *gx_paint_box_new (gboolean homogeneous, gint spacing)
{
	return (GtkWidget*)g_object_new(
		GX_TYPE_PAINT_BOX,
		"spacing", spacing,
		"homogeneous", homogeneous ? TRUE : FALSE,
		NULL);
}

/****************************************************************
 ** Paint functions
 */

inline double cairo_clr(guint16 clr)
{
	return clr / 65535.0;
}

// set cairo color related to the used skin
static void set_skin_color(GtkWidget *wi, cairo_pattern_t *pat)
{
	GxGradient *grad;
	gtk_widget_style_get(wi, "skin-gradient", &grad, NULL);
	if (!grad) {
		GdkColor *p1 = &wi->style->bg[GTK_STATE_NORMAL];
		cairo_pattern_add_color_stop_rgba(
			pat, 0, cairo_clr(p1->red), cairo_clr(p1->green),
			cairo_clr(p1->blue), 0.8);
		GdkColor *p2 = &wi->style->fg[GTK_STATE_NORMAL];
		cairo_pattern_add_color_stop_rgba(
			pat, 1, (cairo_clr(p1->red)+cairo_clr(p2->red))/2,
			(cairo_clr(p1->green)+cairo_clr(p2->green))/2,
			(cairo_clr(p1->blue)+cairo_clr(p2->blue))/2, 0.8);
		return;
	}
	GSList *p;
	for (p = grad->colors; p; p = g_slist_next(p)) {
		GxGradientElement *el = (GxGradientElement*)p->data;
		cairo_pattern_add_color_stop_rgba(pat, el->offset, el->red, el->green, el->blue, el->alpha);
	}
	gx_gradient_free(grad);
}

//----- paint boxes with cairo -----
static gboolean amp_expose(GtkWidget *wi, GdkEventExpose *ev)
{
	cairo_t *cr;
	cr = gdk_cairo_create(wi->window);
	GdkRegion *region;
	region = gdk_region_rectangle (&wi->allocation);
	gdk_region_intersect (region, ev->region);
	gdk_cairo_region (cr, region);
	cairo_clip (cr);
	double x0      = wi->allocation.x+2;
	double y0      = wi->allocation.y+2;
	double rect_width  = wi->allocation.width-5;
	double rect_height = wi->allocation.height-5;
	double radius = 25.;
	double x1,y1;
	x1=x0+rect_width;
	y1=y0+rect_height;

	cairo_move_to  (cr, x0, y0 + radius);
	cairo_curve_to (cr, x0 , y0, x0 , y0, x0 + radius, y0);
	cairo_line_to (cr, x1 - radius, y0);
	cairo_curve_to (cr, x1, y0, x1, y0, x1, y0 + radius);
	cairo_line_to (cr, x1 , y1 - radius);
	cairo_curve_to (cr, x1, y1, x1, y1, x1 - radius, y1);
	cairo_line_to (cr, x0 + radius, y1);
	cairo_curve_to (cr, x0, y1, x0, y1, x0, y1- radius);
	cairo_close_path (cr);

	cairo_set_source_rgba (cr, 0, 0, 0, 0.6);
	cairo_set_line_width (cr, 5.0);
	cairo_stroke (cr);

	cairo_move_to  (cr, x0, y0 + radius);
	cairo_curve_to (cr, x0 , y0, x0 , y0, x0 + radius, y0);
	cairo_line_to (cr, x1 - radius, y0);
	cairo_curve_to (cr, x1, y0, x1, y0, x1, y0 + radius);
	cairo_line_to (cr, x1 , y1 - radius);
	cairo_curve_to (cr, x1, y1, x1, y1, x1 - radius, y1);
	cairo_line_to (cr, x0 + radius, y1);
	cairo_curve_to (cr, x0, y1, x0, y1, x0, y1- radius);
	cairo_close_path (cr);

	cairo_set_source_rgb (cr, 0.02, 0.02, 0.02);
	cairo_set_line_width (cr, 1.0);
	cairo_stroke (cr);

	cairo_destroy(cr);
	gdk_region_destroy (region);
	return FALSE;
}

static gboolean conv_widget_expose(GtkWidget *wi, GdkEventExpose *ev)
{
	cairo_t *cr;
	cairo_pattern_t *pat;

	/* create a cairo context */
	cr = gdk_cairo_create(wi->window);
	GdkRegion *region;
	region = gdk_region_rectangle (&wi->allocation);
	gdk_region_intersect (region, ev->region);
	gdk_cairo_region (cr, region);
	cairo_clip (cr);

	double x0      = wi->allocation.x+5;
	double y0      = wi->allocation.y+5;
	double rect_width  = wi->allocation.width-10;
	double rect_height = wi->allocation.height-10;
	double radius = 36.;
	double x1,y1;
	x1=x0+rect_width;
	y1=y0+rect_height;

	cairo_move_to  (cr, x0, y0 + radius);
	cairo_curve_to (cr, x0 , y0, x0 , y0, x0 + radius, y0);
	cairo_line_to (cr, x1 - radius, y0);
	cairo_curve_to (cr, x1, y0, x1, y0, x1, y0 + radius);
	cairo_line_to (cr, x1 , y1 - radius);
	cairo_curve_to (cr, x1, y1, x1, y1, x1 - radius, y1);
	cairo_line_to (cr, x0 + radius, y1);
	cairo_curve_to (cr, x0, y1, x0, y1, x0, y1- radius);
	cairo_close_path (cr);

	pat = cairo_pattern_create_linear (0, y0, 0, y1);
    cairo_pattern_add_color_stop_rgba (pat, 1, 0., 0., 0., 0.8);
    cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0, 0.4);
    cairo_set_source (cr, pat);
    cairo_fill_preserve (cr);

	cairo_set_source_rgba (cr, 0, 0, 0, 0.8);
	cairo_set_line_width (cr, 9.0);
	cairo_stroke (cr);

	cairo_move_to  (cr, x0, y0 + radius);
	cairo_curve_to (cr, x0 , y0, x0 , y0, x0 + radius, y0);
	cairo_line_to (cr, x1 - radius, y0);
	cairo_curve_to (cr, x1, y0, x1, y0, x1, y0 + radius);
	cairo_line_to (cr, x1 , y1 - radius);
	cairo_curve_to (cr, x1, y1, x1, y1, x1 - radius, y1);
	cairo_line_to (cr, x0 + radius, y1);
	cairo_curve_to (cr, x0, y1, x0, y1, x0, y1- radius);
	cairo_close_path (cr);

	cairo_set_source_rgb (cr, 0.2, 0.2, 0.2);
	cairo_set_line_width (cr, 1.0);
	cairo_stroke (cr);

	cairo_pattern_destroy (pat);
	cairo_destroy(cr);
	gdk_region_destroy (region);
	return FALSE;
}

static gboolean upper_widget_expose(GtkWidget *wi, GdkEventExpose *ev)
{
	cairo_t *cr;
	cairo_pattern_t *pat;

	gint x, y;
	gint w, h;
	/* get the dimensions */
	x = wi->allocation.x+2;
	y = wi->allocation.y+2;
	w = wi->allocation.width-4;
	h = wi->allocation.height-4;

	/* create a cairo context */
	cr = gdk_cairo_create(wi->window);
	GdkRegion *region;
	region = gdk_region_rectangle (&wi->allocation);
	gdk_region_intersect (region, ev->region);
	gdk_cairo_region (cr, region);
	cairo_clip (cr);

	cairo_move_to (cr, x, y);
	cairo_curve_to (cr, x+w*0.66, y, x+w*0.33, y+h, x+w, y+h);
	cairo_line_to (cr, x+w , y);
	cairo_set_line_width (cr, 3.0);
	cairo_close_path (cr);

	pat = cairo_pattern_create_linear (0, y, 0, y+h);
	cairo_pattern_add_color_stop_rgba (pat, 1, 0, 0, 0, 0.8);
	cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0, 0);
	cairo_set_source (cr, pat);
	cairo_fill_preserve (cr);
	cairo_stroke (cr);

	cairo_pattern_destroy (pat);
	cairo_destroy(cr);
	gdk_region_destroy (region);
	return FALSE;

}

static gboolean rectangle_expose(GtkWidget *wi, GdkEventExpose *ev)
{
	cairo_t *cr;
	/* create a cairo context */
	cr = gdk_cairo_create(wi->window);
	GdkRegion *region;
	region = gdk_region_rectangle (&wi->allocation);
	gdk_region_intersect (region, ev->region);
	gdk_cairo_region (cr, region);
	cairo_clip (cr);

	double x0      = wi->allocation.x+1;
	double y0      = wi->allocation.y+1;
	double rect_width  = wi->allocation.width-2;
	double rect_height = wi->allocation.height-11;

	cairo_rectangle (cr, x0,y0,rect_width,rect_height+3);
	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_fill (cr);

	cairo_pattern_t*pat =
		cairo_pattern_create_radial (-50, y0, 5,rect_width+100,  rect_height, 0.0);
	cairo_pattern_add_color_stop_rgb (pat, 0, 0.2, 0.2, 0.3);
	cairo_pattern_add_color_stop_rgb (pat, 1, 0.05, 0.05, 0.05);
	set_skin_color(wi, pat);
	cairo_set_source (cr, pat);
	cairo_rectangle (cr, x0+1,y0+1,rect_width-2,rect_height-1);
	cairo_fill (cr);

	cairo_pattern_destroy (pat);
	cairo_destroy(cr);
	gdk_region_destroy (region);
	return FALSE;
}

static gboolean led_expose(GtkWidget *wi, GdkEventExpose *ev)
{
	cairo_t *cr;
	/* create a cairo context */
	cr = gdk_cairo_create(wi->window);
	GdkRegion *region;
	region = gdk_region_rectangle (&wi->allocation);
	gdk_region_intersect (region, ev->region);
	gdk_cairo_region (cr, region);
	cairo_clip (cr);

	double x0      = wi->allocation.x+1;
	double y0      = wi->allocation.y+1;
	double rect_width  = wi->allocation.width-2;
	double rect_height = wi->allocation.height-11;
	double x1 = x0+rect_width*0.5;
	double y1 = y0+rect_height*0.5;
	
	
	cairo_arc(cr,x1,y1, 5, 0, 2 * M_PI );

	//cairo_rectangle (cr, x0,y0,rect_width,rect_height+3);
	cairo_set_source_rgb (cr, 0.3, 0.8, 0.3);
	cairo_fill_preserve (cr);
	cairo_set_source_rgb (cr, 0., 0., 0.);
	cairo_stroke (cr);

	cairo_destroy(cr);
	gdk_region_destroy (region);
	return FALSE;
}

static gboolean rectangle_skin_color_expose(GtkWidget *wi, GdkEventExpose *ev)
{
	cairo_t *cr;
	/* create a cairo context */
	cr = gdk_cairo_create(wi->window);
	GdkRegion *region;
	region = gdk_region_rectangle (&wi->allocation);
	gdk_region_intersect (region, ev->region);
	gdk_cairo_region (cr, region);
	cairo_clip (cr);

	double x0      = wi->allocation.x+1;
	double y0      = wi->allocation.y+1;
	double rect_width  = wi->allocation.width-2;
	double rect_height = wi->allocation.height-2;

	cairo_rectangle (cr, x0,y0,rect_width,rect_height+3);
	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_fill (cr);

	cairo_pattern_t*pat =
	cairo_pattern_create_linear (0, y0, 0, y0+rect_height);
		//cairo_pattern_create_radial (-50, y0, 5,rect_width-10,  rect_height, 20.0);
	set_skin_color(wi, pat);
	cairo_set_source (cr, pat);
	cairo_rectangle (cr, x0+1,y0+1,rect_width-2,rect_height-1);
	cairo_fill (cr);

	cairo_pattern_destroy (pat);
	cairo_destroy(cr);
	gdk_region_destroy (region);
	return FALSE;
}

static gboolean convolver_icon_expose(GtkWidget *wi, GdkEventExpose *ev)
{
	cairo_t *cr;
	/* create a cairo black arc to given widget */
	cr = gdk_cairo_create(wi->window);
	GdkRegion *region;
	region = gdk_region_rectangle (&wi->allocation);
	gdk_region_intersect (region, ev->region);
	gdk_cairo_region (cr, region);
	cairo_clip (cr);

	double x0      = wi->allocation.x+1;
	double y0      = wi->allocation.y+1;
	double rect_width  = wi->allocation.width-2;
	double rect_height = wi->allocation.height-2;

	/* create a cairo context */
	cr = gdk_cairo_create(wi->window);

	cairo_rectangle (cr, x0,y0,rect_width,rect_height+3);
	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_fill (cr);

	cairo_pattern_t*pat =
		cairo_pattern_create_radial (-50, y0, 5,rect_width+100,  rect_height, 0.0);
	set_skin_color(wi, pat);
	cairo_set_source (cr, pat);
	cairo_rectangle (cr, x0+1,y0+1,rect_width-2,rect_height-1);
	cairo_fill (cr);

	cairo_move_to (cr, x0+10, y0 + (rect_height*0.5));
	cairo_curve_to (cr, x0+30,y0 + (rect_height*0.005), x0+50, y0 + (rect_height*0.995), x0+70, y0 + (rect_height*0.5));
	cairo_set_source_rgb (cr, 1, 1, 1);
	cairo_set_line_width (cr, 1.0);
	cairo_stroke (cr);

	cairo_move_to (cr, x0+10, y0 + (rect_height*0.5));
	cairo_line_to (cr, x0+75 , y0 + (rect_height*0.5));
	cairo_move_to (cr, x0+10, y0 + (rect_height*0.2));
	cairo_line_to (cr, x0+10 , y0 + (rect_height*0.8));
	cairo_set_source_rgb (cr, 0.2, 0.8, 0.2);
	cairo_set_line_width (cr, 1.0);
	cairo_stroke (cr);

	cairo_pattern_destroy (pat);
	cairo_destroy(cr);
	gdk_region_destroy (region);
	return FALSE;
}

static gboolean eq_expose(GtkWidget *wi, GdkEventExpose *ev)
{
	//float p = gx_gui::parameter_map["eq.f31_25"].getFloat().value;
    cairo_t *cr;
    cairo_text_extents_t extents;
	/* create a cairo context */
	cr = gdk_cairo_create(wi->window);
	GdkRegion *region;
	region = gdk_region_rectangle (&wi->allocation);
	gdk_region_intersect (region, ev->region);
	gdk_cairo_region (cr, region);
	cairo_clip (cr);

	double x0      = wi->allocation.x+1;
	double y0      = wi->allocation.y+1;
	double rect_width  = wi->allocation.width-2;
	double rect_height = wi->allocation.height-3;

	cairo_rectangle (cr, x0,y0,rect_width,rect_height+3);
	cairo_pattern_t*pat =
	cairo_pattern_create_linear (0, y0, 0, y0+rect_height);
		//cairo_pattern_create_radial (x0+200,y0+ rect_height*0.5, 5,x0+800, y0+ rect_height*0.5, 200.0);
	set_skin_color(wi, pat);
	//cairo_pattern_add_color_stop_rgb (pat, 0, 0.3, 0.3, 0.3);
	//cairo_pattern_add_color_stop_rgb (pat, 0.5, 0.15, 0.15, 0.15);
	//cairo_pattern_add_color_stop_rgb (pat, 1, 0.1, 0.1, 0.1);
	cairo_set_source (cr, pat);
	cairo_fill (cr);
	
	cairo_set_source_rgb(cr,  0.2, 0.2, 0.2);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr,x0+rect_width-3, y0+3);
    cairo_line_to(cr, x0+rect_width-3, y0+rect_height-2);
    cairo_line_to(cr, x0+2, y0+rect_height-2);
    cairo_stroke(cr);

    cairo_set_source_rgb(cr,  0.1, 0.1, 0.1);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr,x0+3, y0+rect_height-1);
    cairo_line_to(cr, x0+3, y0+3);
    cairo_line_to(cr, x0+rect_width-3, y0+3);
    cairo_stroke(cr);
    
    const gchar * title = "faust";
    cairo_select_font_face (cr, "sans", CAIRO_FONT_SLANT_NORMAL,
                               CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size (cr, 10);
	cairo_text_extents (cr,title , &extents);
	double x = x0+rect_width-extents.height - 3 ;
	double y = y0+rect_height*0.9+extents.height/2 - 3;
	cairo_move_to(cr,x, y);
	cairo_rotate (cr,270* M_PI/180);
	cairo_text_path (cr,title);
	cairo_set_source_rgb (cr, 0.3, 0.3, 0.3);
    cairo_fill (cr);

	cairo_pattern_destroy (pat);
	cairo_destroy(cr);
	gdk_region_destroy (region);
	return FALSE;
}

static gboolean zac_expose(GtkWidget *wi, GdkEventExpose *ev)
{
	cairo_t *cr;
	/* create a cairo context */
	cr = gdk_cairo_create(wi->window);
	GdkRegion *region;
	region = gdk_region_rectangle (&wi->allocation);
	gdk_region_intersect (region, ev->region);
	gdk_cairo_region (cr, region);
	cairo_clip (cr);

	double x0      = wi->allocation.x+1;
	double y0      = wi->allocation.y+1;
	double rect_width  = wi->allocation.width-2;
	double rect_height = wi->allocation.height-3;

	cairo_rectangle (cr, x0,y0,rect_width,rect_height+3);
	cairo_pattern_t*pat =
		cairo_pattern_create_radial (200, rect_height*0.5, 5,200,  rect_height*0.5, 200.0);
	cairo_pattern_add_color_stop_rgb (pat, 0, 0.8, 0.8, 0.8);
	cairo_pattern_add_color_stop_rgb (pat, 1, 0.3, 0.3, 0.3);
	cairo_set_source (cr, pat);
	cairo_fill (cr);

	cairo_pattern_destroy (pat);
	cairo_destroy(cr);
	gdk_region_destroy (region);
	return FALSE;
}

static gboolean AmpBox_expose(GtkWidget *wi, GdkEventExpose *ev)
{
	cairo_t *cr;
	/* create a cairo context */
	cr = gdk_cairo_create(wi->window);
	GdkRegion *region;
	region = gdk_region_rectangle (&wi->allocation);
	gdk_region_intersect (region, ev->region);
	gdk_cairo_region (cr, region);
	cairo_clip (cr);

	double x0      = wi->allocation.x+1;
	double y0      = wi->allocation.y+1;
	double rect_width  = wi->allocation.width-2;
	double rect_height = wi->allocation.height-2;

    cairo_rectangle (cr, x0-1,y0-1,rect_width+2,rect_height+2);
            cairo_set_source_rgb (cr, 0, 0, 0);
            cairo_fill (cr);

	cairo_pattern_t*pat =
		cairo_pattern_create_radial (-50, y0, 5,rect_width-10,  rect_height, 20.0);
	set_skin_color(wi, pat);
	cairo_set_source (cr, pat);
	cairo_rectangle (cr, x0+2,y0+2,rect_width-4,rect_height-4);
	cairo_fill (cr);

    cairo_set_source_rgb(cr,  0.2, 0.2, 0.2);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr,x0+rect_width-3, y0+3);
    cairo_line_to(cr, x0+rect_width-3, y0+rect_height-2);
    cairo_line_to(cr, x0+2, y0+rect_height-2);
    cairo_stroke(cr);

    cairo_set_source_rgb(cr,  0.1, 0.1, 0.1);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr,x0+3, y0+rect_height-1);
    cairo_line_to(cr, x0+3, y0+3);
    cairo_line_to(cr, x0+rect_width-3, y0+3);
    cairo_stroke(cr);

    cairo_pattern_destroy (pat);
	cairo_destroy(cr);
	gdk_region_destroy (region);
	return FALSE;
}
/*
static void set_rack_color(const gchar * title, cairo_pattern_t *pat)
{
	if(strcmp(title,"low high pass")==0) {
		
		cairo_pattern_add_color_stop_rgba (pat, 1, 0, 0., 0.4, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0.8, 0, 0, 0, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0, 0);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0., 0.4, 0.2);
	}else if(strcmp(title,"Scaleable EQ")==0) {
		
		cairo_pattern_add_color_stop_rgba (pat, 1, 0., 0.4, 0.4, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0.8, 0, 0, 0, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0, 0);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0., 0.4, 0.4, 0.2);
	}else if(strcmp(title,"Crybaby")==0) {
		
		cairo_pattern_add_color_stop_rgba (pat, 1, 0., 0.4, 0., 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0.8, 0, 0, 0, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0, 0);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0., 0.4, 0., 0.2);
	}else if(strcmp(title,"Multi Band Distortion")==0) {
		
		cairo_pattern_add_color_stop_rgba (pat, 1, 0.4, 0.4, 0., 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0.8, 0, 0, 0, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0, 0);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0.4, 0.4, 0., 0.2);
	}else if(strcmp(title,"ImpulseResponse")==0) {
		
		cairo_pattern_add_color_stop_rgba (pat, 1, 0.4, 0., 0., 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0.8, 0, 0, 0, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0, 0);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0.4, 0., 0., 0.2);
	}else if(strcmp(title,"Compressor")==0) {
		
		cairo_pattern_add_color_stop_rgba (pat, 1, 0.4, 0., 0.4, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0.8, 0, 0, 0, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0, 0);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0.4, 0., 0.4, 0.2);
	}else if(strcmp(title,"Overdrive")==0) {
		
		cairo_pattern_add_color_stop_rgba (pat, 1, 0., 0., 0.6, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0.8, 0, 0, 0, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0, 0);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0., 0., 0.6, 0.2);
	}else if(strcmp(title,"Echo")==0) {
		
		cairo_pattern_add_color_stop_rgba (pat, 1, 0., 0.6, 0.6, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0.8, 0, 0, 0, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0, 0);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0., 0.6, 0.6, 0.2);
	}else if(strcmp(title,"Delay")==0) {
		
		cairo_pattern_add_color_stop_rgba (pat, 1, 0., 0.6, 0., 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0.8, 0, 0, 0, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0, 0);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0., 0.6, 0., 0.2);
	}else if(strcmp(title,"Freeverb")==0) {
		
		cairo_pattern_add_color_stop_rgba (pat, 1, 0.6, 0.6, 0., 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0.8, 0, 0, 0, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0, 0);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0.6, 0.6, 0., 0.2);
	}else if(strcmp(title,"Chorus")==0) {
		
		cairo_pattern_add_color_stop_rgba (pat, 1, 0.6, 0., 0., 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0.8, 0, 0, 0, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0, 0);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0.6, 0., 0., 0.2);
	}else if(strcmp(title,"Flanger")==0) {
		
		cairo_pattern_add_color_stop_rgba (pat, 1, 0.6, 0., 0.6, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0.8, 0, 0, 0, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0, 0);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0.6, 0., 0.6, 0.2);
	}else if(strcmp(title,"Phaser")==0) {
		
		cairo_pattern_add_color_stop_rgba (pat, 1, 0., 0., 0.8, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0.8, 0, 0, 0, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0, 0);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0., 0., 0.8, 0.2);
	}else if(strcmp(title,"Stereo Delay")==0) {
		
		cairo_pattern_add_color_stop_rgba (pat, 1, 0., 0.8, 0.8, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0.8, 0, 0, 0, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0, 0);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0., 0.8, 0.8, 0.2);
	}else if(strcmp(title,"Stereo Echo")==0) {
		
		cairo_pattern_add_color_stop_rgba (pat, 1, 0., 0.8, 0., 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0.8, 0, 0, 0, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0, 0);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0., 0.8, 0., 0.2);
	}else if(strcmp(title,"Moog Filter")==0) {
		
		cairo_pattern_add_color_stop_rgba (pat, 1, 0.8, 0.8, 0., 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0.8, 0, 0, 0, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0, 0);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0.8, 0.8, 0., 0.2);
	}else{
		
		cairo_pattern_add_color_stop_rgba (pat, 1, 0, 0, 0.2, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0.8, 0, 0, 0, 0.8);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0, 0);
		cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0.2, 0.2);
	}
	
}
*/
static gboolean RackBox_expose(GtkWidget *wi, GdkEventExpose *ev)
{
	cairo_t *cr;
	cairo_text_extents_t extents;

	/* create a cairo context */
	cr = gdk_cairo_create(wi->window);
	GdkRegion *region;
	region = gdk_region_rectangle (&wi->allocation);
	gdk_region_intersect (region, ev->region);
	gdk_cairo_region (cr, region);
	cairo_clip (cr);
	const gchar * title = gtk_widget_get_name(GTK_WIDGET(wi));

	double x0      = wi->allocation.x;
	double y0      = wi->allocation.y;
	double rect_width  = wi->allocation.width;
	double rect_height = wi->allocation.height;
	double x,y;
	
	
	cairo_select_font_face (cr, "URW Chancery L", CAIRO_FONT_SLANT_NORMAL,
                               CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size (cr, rect_width/12);
	cairo_text_extents (cr,title , &extents);
	x = x0+rect_width/2-extents.width/2 ;
	y = y0+rect_height/2+extents.height/2 ;
	cairo_move_to(cr,x, y);
	cairo_text_path (cr,title);
	cairo_set_source_rgba (cr, 0., 0., 0., 0.1);
	cairo_fill_preserve (cr);
	cairo_set_source_rgba (cr, 0.7, 0.7, 0.7, 0.05);
    cairo_stroke (cr);

    cairo_rectangle (cr, x0,y0,rect_width,rect_height);
    cairo_set_line_width(cr, 3.0);
    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_stroke (cr);
    
	cairo_rectangle (cr, x0+4,y0+4,rect_width-8,rect_height-8);
	cairo_pattern_t*pat = cairo_pattern_create_linear (0, y0, 0, y0+rect_height);
	//set_rack_color(title, pat);
	cairo_pattern_add_color_stop_rgba (pat, 1, 0, 0, 0.2, 0.8);
	cairo_pattern_add_color_stop_rgba (pat, 0.8, 0, 0, 0, 0.8);
	//cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0, 0);
	cairo_pattern_add_color_stop_rgba (pat, 0.2, 0, 0, 0.2, 0.2);
	cairo_set_source (cr, pat);
	cairo_fill(cr);

    cairo_set_source_rgb(cr,  0.2, 0.2, 0.2);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr,x0+rect_width-3, y0+3);
    cairo_line_to(cr, x0+rect_width-3, y0+rect_height-2);
    cairo_line_to(cr, x0+2, y0+rect_height-2);
    cairo_stroke(cr);

    cairo_set_source_rgb(cr,  0.1, 0.1, 0.1);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr,x0+3, y0+rect_height-1);
    cairo_line_to(cr, x0+3, y0+3);
    cairo_line_to(cr, x0+rect_width-3, y0+3);
    cairo_stroke(cr);

    cairo_pattern_destroy (pat);
	cairo_destroy(cr);
	gdk_region_destroy (region);
	return FALSE;
}

static gboolean crybaby_expose(GtkWidget *wi, GdkEventExpose *ev)
{
	cairo_t *cr;
	cairo_text_extents_t extents;

	/* create a cairo context */
	cr = gdk_cairo_create(wi->window);
	GdkRegion *region;
	region = gdk_region_rectangle (&wi->allocation);
	gdk_region_intersect (region, ev->region);
	gdk_cairo_region (cr, region);
	cairo_clip (cr);
	const gchar * title = gtk_widget_get_name(GTK_WIDGET(wi));

	double x0      = wi->allocation.x;
	double y0      = wi->allocation.y;
	double rect_width  = wi->allocation.width;
	double rect_height = wi->allocation.height;
	double x,y;
	
	
	

    cairo_rectangle (cr, x0,y0,rect_width,rect_height);
    cairo_set_line_width(cr, 3.0);
    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_stroke (cr);
    
	cairo_rectangle (cr, x0+4,y0+4,rect_width-8,rect_height-8);
	cairo_pattern_t*pat = cairo_pattern_create_linear (0, y0, 0, y0+rect_height);
	//set_rack_color(title, pat);
	cairo_pattern_add_color_stop_rgb (pat, 1, 0, 0, 0.2);
	cairo_pattern_add_color_stop_rgb (pat, 0.8, 0, 0, 0);
	cairo_pattern_add_color_stop_rgb (pat, 0, 0.5, 0.5, 0.5);
	cairo_pattern_add_color_stop_rgb (pat, 0, 0.6, 0.6, 0.8);
	cairo_set_source (cr, pat);
	cairo_fill(cr);
	

    cairo_set_line_width(cr, 2.0);
	cairo_move_to  (cr, x0+rect_width*0.43, y0+4 );
	cairo_curve_to (cr, x0+rect_width*0.4 , y0+rect_height*0.5, x0+rect_width*0.4  , y0+rect_height*0.5, x0+rect_width*0.43  , y0+rect_height-6);
	cairo_line_to(cr, x0+rect_width*0.93, y0+rect_height-6);
	cairo_line_to(cr, x0+rect_width*0.93, y0+rect_height-6);
	cairo_curve_to (cr, x0+rect_width*0.95 , y0+rect_height*0.5, x0+rect_width*0.95  , y0+rect_height*0.5, x0+rect_width*0.93  , y0+4);
	
	//cairo_line_to(cr, x0+rect_width-3, y0+4);
	cairo_line_to(cr, x0+rect_width*0.92, y0+4);
	pat = cairo_pattern_create_linear (0, y0, 0, y0+rect_height);
	//set_rack_color(title, pat);
	cairo_pattern_add_color_stop_rgba (pat, 1, 0, 0, 0.2, 0.8);
	cairo_pattern_add_color_stop_rgba (pat, 0.8, 0, 0, 0, 0.8);
	cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0, 0);
	cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0.2, 0.2);
	cairo_set_source (cr, pat);
	cairo_fill_preserve(cr);
	cairo_set_source_rgb (cr, 0., 0., 0.);
    cairo_stroke (cr);
    
    cairo_set_line_width(cr, 1.0);
	cairo_move_to  (cr, x0+2+rect_width*0.43, y0+4 );
	cairo_curve_to (cr, x0+2+rect_width*0.4 , y0+rect_height*0.5, x0+2+rect_width*0.4  , y0+rect_height*0.5, x0+2+rect_width*0.43  , y0+rect_height-6);
	cairo_line_to(cr, x0-2+rect_width*0.93, y0+rect_height-6);
	cairo_curve_to (cr, x0-2+rect_width*0.95 , y0+rect_height*0.5, x0-2+rect_width*0.95  , y0+rect_height*0.5, x0-2+rect_width*0.93  , y0+4);
	cairo_set_source_rgb (cr, 0.2, 0.2, 0.2);
    cairo_stroke (cr);
    
    cairo_select_font_face (cr, "URW Chancery L", CAIRO_FONT_SLANT_NORMAL,
                               CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size (cr, 16);
	cairo_text_extents (cr,title , &extents);
	x = x0+10+extents.width/2 ;
	y = y0+rect_height*0.8+extents.height/2 ;
	cairo_move_to(cr,x, y);
	cairo_text_path (cr,title);
    cairo_set_line_width(cr, 1.0);
	cairo_set_source_rgb (cr, 0.7, 0.7, 0.7);
    cairo_fill (cr);
    
    cairo_set_source_rgb(cr,  0.2, 0.2, 0.2);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr,x0+rect_width-3, y0+3);
    cairo_line_to(cr, x0+rect_width-3, y0+rect_height-2);
    cairo_line_to(cr, x0+2, y0+rect_height-2);
    cairo_stroke(cr);

    cairo_set_source_rgb(cr,  0.1, 0.1, 0.1);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr,x0+3, y0+rect_height-1);
    cairo_line_to(cr, x0+3, y0+3);
    cairo_line_to(cr, x0+rect_width-3, y0+3);
    cairo_stroke(cr);

    cairo_pattern_destroy (pat);
	cairo_destroy(cr);
	gdk_region_destroy (region);
	return FALSE;
}

static gboolean ir_expose(GtkWidget *wi, GdkEventExpose *ev)
{
	cairo_t *cr;
	cairo_text_extents_t extents;

	/* create a cairo context */
	cr = gdk_cairo_create(wi->window);
	GdkRegion *region;
	region = gdk_region_rectangle (&wi->allocation);
	gdk_region_intersect (region, ev->region);
	gdk_cairo_region (cr, region);
	cairo_clip (cr);
	const gchar * title = gtk_widget_get_name(GTK_WIDGET(wi));

	double x0      = wi->allocation.x;
	double y0      = wi->allocation.y;
	double rect_width  = wi->allocation.width;
	double rect_height = wi->allocation.height;
	double x,y;
	
	
	

    cairo_rectangle (cr, x0,y0,rect_width,rect_height);
    cairo_set_line_width(cr, 3.0);
    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_stroke (cr);
    
	cairo_rectangle (cr, x0+4,y0+4,rect_width-8,rect_height-8);
	cairo_pattern_t*pat = cairo_pattern_create_linear (0, y0, 0, y0+rect_height);
	//set_rack_color(title, pat);
	cairo_pattern_add_color_stop_rgb (pat, 1, 0, 0, 0.2);
	cairo_pattern_add_color_stop_rgb (pat, 0.8, 0, 0.2, 0);
	cairo_pattern_add_color_stop_rgb (pat, 0.2, 0.2, 0.2, 0.2);
	cairo_pattern_add_color_stop_rgb (pat, 0, 0., 0., 0.);
	cairo_set_source (cr, pat);
	cairo_fill(cr);
	

    cairo_set_line_width(cr, 2.0);
	cairo_move_to  (cr, x0+rect_width*0.43, y0+4 );
	cairo_curve_to (cr, x0+rect_width*0.4 , y0+rect_height*0.5, x0+rect_width*0.4  , y0+rect_height*0.5, x0+rect_width*0.43  , y0+rect_height-6);
	cairo_line_to(cr, x0+rect_width*0.93, y0+rect_height-6);
	cairo_line_to(cr, x0+rect_width*0.93, y0+rect_height-6);
	cairo_curve_to (cr, x0+rect_width*0.95 , y0+rect_height*0.5, x0+rect_width*0.95  , y0+rect_height*0.5, x0+rect_width*0.93  , y0+4);
	
	//cairo_line_to(cr, x0+rect_width-3, y0+4);
	cairo_line_to(cr, x0+rect_width*0.92, y0+4);
	pat = cairo_pattern_create_linear (0, y0, 0, y0+rect_height);
	//set_rack_color(title, pat);
	cairo_pattern_add_color_stop_rgba (pat, 1, 0, 0, 0.2, 0.8);
	cairo_pattern_add_color_stop_rgba (pat, 0.8, 0, 0, 0, 0.8);
	cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0, 0);
	cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0.2, 0.2);
	cairo_set_source (cr, pat);
	cairo_fill_preserve(cr);
	cairo_set_source_rgb (cr, 0., 0., 0.);
    cairo_stroke (cr);
    
    cairo_set_line_width(cr, 1.0);
	cairo_move_to  (cr, x0+2+rect_width*0.43, y0+4 );
	cairo_curve_to (cr, x0+2+rect_width*0.4 , y0+rect_height*0.5, x0+2+rect_width*0.4  , y0+rect_height*0.5, x0+2+rect_width*0.43  , y0+rect_height-6);
	cairo_line_to(cr, x0-2+rect_width*0.93, y0+rect_height-6);
	cairo_curve_to (cr, x0-2+rect_width*0.95 , y0+rect_height*0.5, x0-2+rect_width*0.95  , y0+rect_height*0.5, x0-2+rect_width*0.93  , y0+4);
	cairo_set_source_rgb (cr, 0.2, 0.2, 0.2);
    cairo_stroke (cr);
    
    cairo_select_font_face (cr, "URW Chancery L", CAIRO_FONT_SLANT_NORMAL,
                               CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size (cr, 16);
	cairo_text_extents (cr,title , &extents);
	x = x0+25 ;
	y = y0+rect_height*0.78+extents.height/2 ;
	cairo_move_to(cr,x, y);
	cairo_text_path (cr,title);
    cairo_set_line_width(cr, 1.0);
	cairo_set_source_rgb (cr, 0.7, 0.7, 0.7);
    cairo_fill (cr);
    
    cairo_set_source_rgb(cr,  0.2, 0.2, 0.2);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr,x0+rect_width-3, y0+3);
    cairo_line_to(cr, x0+rect_width-3, y0+rect_height-2);
    cairo_line_to(cr, x0+2, y0+rect_height-2);
    cairo_stroke(cr);

    cairo_set_source_rgb(cr,  0.1, 0.1, 0.1);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr,x0+3, y0+rect_height-1);
    cairo_line_to(cr, x0+3, y0+3);
    cairo_line_to(cr, x0+rect_width-3, y0+3);
    cairo_stroke(cr);

    cairo_pattern_destroy (pat);
	cairo_destroy(cr);
	gdk_region_destroy (region);
	return FALSE;
}

static gboolean compressor_expose(GtkWidget *wi, GdkEventExpose *ev)
{
	cairo_t *cr;
	cairo_text_extents_t extents;

	/* create a cairo context */
	cr = gdk_cairo_create(wi->window);
	GdkRegion *region;
	region = gdk_region_rectangle (&wi->allocation);
	gdk_region_intersect (region, ev->region);
	gdk_cairo_region (cr, region);
	cairo_clip (cr);
	const gchar * title = gtk_widget_get_name(GTK_WIDGET(wi));

	double x0      = wi->allocation.x;
	double y0      = wi->allocation.y;
	double rect_width  = wi->allocation.width;
	double rect_height = wi->allocation.height;
	double x,y;
	
	
	

    cairo_rectangle (cr, x0,y0,rect_width,rect_height);
    cairo_set_line_width(cr, 3.0);
    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_stroke (cr);
    
	cairo_rectangle (cr, x0+4,y0+4,rect_width-8,rect_height-8);
	cairo_pattern_t*pat = cairo_pattern_create_linear (0, y0, 0, y0+rect_height);
	//set_rack_color(title, pat);
	cairo_pattern_add_color_stop_rgb (pat, 1, 0, 0, 0.2);
	cairo_pattern_add_color_stop_rgb (pat, 0.8, 0., 0, 0);
	cairo_pattern_add_color_stop_rgb (pat, 0.2, 0.5, 0.5, 0.5);
	cairo_pattern_add_color_stop_rgb (pat, 0, 0., 0., 0.);
	cairo_set_source (cr, pat);
	cairo_fill(cr);
	

    cairo_set_line_width(cr, 2.0);
	cairo_move_to  (cr, x0+rect_width*0.09, y0+4 );
	cairo_curve_to (cr, x0+rect_width*0.07 , y0+rect_height*0.5, x0+rect_width*0.07  , y0+rect_height*0.5, x0+rect_width*0.09 , y0+rect_height-6);
	cairo_line_to(cr, x0+rect_width*0.92, y0+rect_height-6);
	cairo_curve_to (cr, x0+rect_width*0.94 , y0+rect_height*0.5, x0+rect_width*0.94  , y0+rect_height*0.5, x0+rect_width*0.92  , y0+4);
	
	//cairo_line_to(cr, x0+rect_width-3, y0+4);
	cairo_line_to(cr, x0+rect_width*0.92, y0+4);
	pat = cairo_pattern_create_linear (0, y0, 0, y0+rect_height);
	//set_rack_color(title, pat);
	cairo_pattern_add_color_stop_rgba (pat, 1, 0, 0, 0.2, 0.8);
	cairo_pattern_add_color_stop_rgba (pat, 0.8, 0, 0, 0, 0.8);
	cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0, 0);
	cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0.2, 0.2);
	cairo_set_source (cr, pat);
	cairo_fill_preserve(cr);
	cairo_set_source_rgb (cr, 0., 0., 0.);
    cairo_stroke (cr);
    
    cairo_set_line_width(cr, 1.0);
	cairo_move_to  (cr, x0+2+rect_width*0.09, y0+4 );
	cairo_curve_to (cr, x0+2+rect_width*0.07 , y0+rect_height*0.5, x0+2+rect_width*0.07  , y0+rect_height*0.5, x0+2+rect_width*0.09 , y0+rect_height-6);
	cairo_line_to(cr, x0-2+rect_width*0.92, y0+rect_height-6);
	cairo_curve_to (cr, x0-2+rect_width*0.94 , y0+rect_height*0.5, x0-2+rect_width*0.94  , y0+rect_height*0.5, x0-2+rect_width*0.92  , y0+4);
	cairo_set_source_rgb (cr, 0.2, 0.2, 0.2);
    cairo_stroke (cr);
    
    cairo_select_font_face (cr, "URW Chancery L", CAIRO_FONT_SLANT_NORMAL,
                               CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size (cr, 12);
	cairo_text_extents (cr,title , &extents);
	x = x0+21 ;
	y = y0+rect_height*0.8+extents.height/2 ;
	
	

	cairo_move_to(cr,x, y);
	cairo_rotate (cr,270* M_PI/180);
	cairo_text_path (cr,title);
	
	cairo_set_line_width(cr, 1.0);
	cairo_set_source_rgb (cr, 0.9, 0.9, 1);
    cairo_fill (cr);
    
    cairo_set_source_rgb(cr,  0.2, 0.2, 0.2);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr,x0+rect_width-3, y0+3);
    cairo_line_to(cr, x0+rect_width-3, y0+rect_height-2);
    cairo_line_to(cr, x0+2, y0+rect_height-2);
    cairo_stroke(cr);

    cairo_set_source_rgb(cr,  0.1, 0.1, 0.1);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr,x0+3, y0+rect_height-1);
    cairo_line_to(cr, x0+3, y0+3);
    cairo_line_to(cr, x0+rect_width-3, y0+3);
    cairo_stroke(cr);

    cairo_pattern_destroy (pat);
	cairo_destroy(cr);
	gdk_region_destroy (region);
	return FALSE;
}

static gboolean seq_expose(GtkWidget *wi, GdkEventExpose *ev)
{
	cairo_t *cr;
	cairo_text_extents_t extents;

	/* create a cairo context */
	cr = gdk_cairo_create(wi->window);
	GdkRegion *region;
	region = gdk_region_rectangle (&wi->allocation);
	gdk_region_intersect (region, ev->region);
	gdk_cairo_region (cr, region);
	cairo_clip (cr);
	const gchar * title = gtk_widget_get_name(GTK_WIDGET(wi));

	double x0      = wi->allocation.x;
	double y0      = wi->allocation.y;
	double rect_width  = wi->allocation.width;
	double rect_height = wi->allocation.height;
	double x,y;
	
    cairo_rectangle (cr, x0,y0,rect_width,rect_height);
    cairo_set_line_width(cr, 3.0);
    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_stroke (cr);
    
	cairo_rectangle (cr, x0+4,y0+4,rect_width-8,rect_height-8);
	cairo_pattern_t*pat = cairo_pattern_create_linear (0, y0, 0, y0+rect_height);
	//set_rack_color(title, pat);
	cairo_pattern_add_color_stop_rgb (pat, 1, 0, 0, 0.);
	cairo_pattern_add_color_stop_rgb (pat, 0.8, 0, 0, 0.1);
	cairo_pattern_add_color_stop_rgb (pat, 0.2, 0.2, 0.2, 0.2);
	cairo_pattern_add_color_stop_rgb (pat, 0, 0., 0, 0.1);
	cairo_set_source (cr, pat);
	cairo_fill(cr);
	

    cairo_set_line_width(cr, 2.0);
	cairo_move_to  (cr, x0+rect_width*0.05, y0+rect_height-6 );
	cairo_curve_to (cr, x0+rect_width*0.03 , y0+rect_height*0.8, x0+rect_width*0.03  , y0+rect_height*0.8, x0+rect_width*0.05 , y0+rect_height*0.6);
	cairo_line_to(cr, x0+rect_width*0.95, y0+rect_height*0.6);
	cairo_curve_to (cr, x0+rect_width*0.97 , y0+rect_height*0.8, x0+rect_width*0.97  , y0+rect_height*0.8, x0+rect_width*0.95  , y0+rect_height-6);
	
	//cairo_line_to(cr, x0+rect_width-3, y0+4);
	cairo_line_to(cr, x0+rect_width*0.92, y0+rect_height-6);
	pat = cairo_pattern_create_linear (0, y0, 0, y0+rect_height);
	//set_rack_color(title, pat);
	cairo_pattern_add_color_stop_rgb (pat, 1, 0, 0, 0.2);
	cairo_pattern_add_color_stop_rgb (pat, 0.8, 0, 0, 0);
	cairo_pattern_add_color_stop_rgb (pat, 0, 0, 0, 0);
	cairo_pattern_add_color_stop_rgb (pat, 0, 0, 0, 0.2);
	cairo_set_source (cr, pat);
	cairo_fill_preserve(cr);
	cairo_set_source_rgb (cr, 0., 0., 0.);
    cairo_stroke (cr);
    
    cairo_set_line_width(cr, 1.0);
	cairo_move_to  (cr, x0+2+rect_width*0.05, y0+rect_height-6 );
	cairo_curve_to (cr, x0+2+rect_width*0.03 , y0+rect_height*0.8, x0+2+rect_width*0.03  , y0+rect_height*0.8, x0+2+rect_width*0.05 , y0+rect_height*0.6);
	cairo_line_to(cr, x0-2+rect_width*0.95, y0+rect_height*0.6);
	cairo_curve_to (cr, x0-2+rect_width*0.97 , y0+rect_height*0.8, x0-2+rect_width*0.97  , y0+rect_height*0.8, x0-2+rect_width*0.95  , y0+rect_height-6);
	
	cairo_set_source_rgb (cr, 0.2, 0.2, 0.2);
    cairo_stroke (cr);
    
    cairo_select_font_face (cr, "URW Chancery L", CAIRO_FONT_SLANT_NORMAL,
                               CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size (cr, 12);
	cairo_text_extents (cr,title , &extents);
	x = x0+21 ;
	y = y0+rect_height*0.5+extents.height/2 ;
	
	

	cairo_move_to(cr,x, y);
	cairo_rotate (cr,270* M_PI/180);
	cairo_text_path (cr,title);
	
	cairo_set_line_width(cr, 1.0);
	cairo_set_source_rgb (cr, 0.9, 0.9, 1);
    cairo_fill (cr);
    
    cairo_set_source_rgb(cr,  0.2, 0.2, 0.2);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr,x0+rect_width-3, y0+3);
    cairo_line_to(cr, x0+rect_width-3, y0+rect_height-2);
    cairo_line_to(cr, x0+2, y0+rect_height-2);
    cairo_stroke(cr);

    cairo_set_source_rgb(cr,  0.1, 0.1, 0.1);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr,x0+3, y0+rect_height-1);
    cairo_line_to(cr, x0+3, y0+3);
    cairo_line_to(cr, x0+rect_width-3, y0+3);
    cairo_stroke(cr);

    cairo_pattern_destroy (pat);
	cairo_destroy(cr);
	gdk_region_destroy (region);
	return FALSE;
}

static gboolean lhfilter_expose(GtkWidget *wi, GdkEventExpose *ev)
{
	cairo_t *cr;
	/* create a cairo context */
	cr = gdk_cairo_create(wi->window);
	GdkRegion *region;
	region = gdk_region_rectangle (&wi->allocation);
	gdk_region_intersect (region, ev->region);
	gdk_cairo_region (cr, region);
	cairo_clip (cr);

	double x0      = wi->allocation.x+1;
	double y0      = wi->allocation.y+1;
	double rect_width  = wi->allocation.width-2;
	double rect_height = wi->allocation.height-2;
	GdkPixbuf  *stock_image, *frame;	
	stock_image = gtk_widget_render_icon(wi,"gxhead_face",(GtkIconSize)-1,NULL);
	
    cairo_rectangle (cr, x0,y0,rect_width,rect_height);
    cairo_set_line_width(cr, 3.0);
    cairo_set_source_rgb (cr, 0., 0., 0.);
    cairo_stroke (cr);

    cairo_set_source_rgb(cr,  0.3, 0.3, 0.3);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr,x0+rect_width-3, y0+3);
    cairo_line_to(cr, x0+rect_width-3, y0+rect_height-2);
    cairo_line_to(cr, x0+2, y0+rect_height-2);
    cairo_stroke(cr);

    cairo_set_source_rgb(cr,  0.4, 0.4, 0.4);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr,x0+3, y0+rect_height-1);
    cairo_line_to(cr, x0+3, y0+3);
    cairo_line_to(cr, x0+rect_width-3, y0+3);
    cairo_stroke(cr);
    
 
	
	frame = gdk_pixbuf_scale_simple(
			stock_image, rect_width-8, rect_height-8, GDK_INTERP_NEAREST);
	gdk_draw_pixbuf(GDK_DRAWABLE(wi->window), gdk_gc_new(GDK_DRAWABLE(wi->window)),
	                frame, 0, 0,
	                x0+4, y0+4, rect_width-8,rect_height-8,
	                GDK_RGB_DITHER_NORMAL, 0, 0);
	
    g_object_unref(stock_image);
	g_object_unref(frame);
	cairo_destroy(cr);
	gdk_region_destroy (region);
	return FALSE;
}

static gboolean tribal_box_expose(GtkWidget *wi, GdkEventExpose *ev)
{
	GdkPixbuf *_image, *stock_image;
	cairo_t *cr;
	/* create a cairo context */
	cr = gdk_cairo_create(wi->window);
	GdkRegion *region;
	region = gdk_region_rectangle (&wi->allocation);
	gdk_region_intersect (region, ev->region);
	gdk_cairo_region (cr, region);
	cairo_clip (cr);

	gint x0      = wi->allocation.x+1;
	gint y0      = wi->allocation.y+1;
	gint rect_width  = wi->allocation.width-2;
	gint rect_height = wi->allocation.height-3;

	stock_image = gtk_widget_render_icon(wi,"guitar",(GtkIconSize)-1,NULL);
	_image = gdk_pixbuf_scale_simple(
		stock_image, rect_width, rect_height, GDK_INTERP_HYPER);

	cairo_pattern_t*pat;

	double radius = 38.;
	if (rect_width<38) radius = rect_width;
	else if (rect_height<38) radius = rect_height;
	double x1,y1;

	x1=x0+rect_width;
	y1=y0+rect_height;

	cairo_move_to  (cr, x0, y0 + radius);
	cairo_curve_to (cr, x0 , y0, x0 , y0, x0 + radius, y0);
	cairo_line_to (cr, x1 - radius, y0);
	cairo_curve_to (cr, x1, y0, x1, y0, x1, y0 + radius);
	cairo_line_to (cr, x1 , y1 - radius);
	cairo_curve_to (cr, x1, y1, x1, y1, x1 - radius, y1);
	cairo_line_to (cr, x0 + radius, y1);
	cairo_curve_to (cr, x0, y1, x0, y1, x0, y1- radius);
	cairo_close_path (cr);
	pat = cairo_pattern_create_linear (0, y0, 0, y1);
	cairo_pattern_add_color_stop_rgba (pat, 1, 0, 0, 0, 0.8);
	cairo_pattern_add_color_stop_rgba (pat, 0.5, 0.05, 0.05, 0.05, 0.6);
	cairo_pattern_add_color_stop_rgba (pat, 0, 0.2, 0.2, 0.2, 0.4);
	cairo_set_source (cr, pat);
	cairo_fill (cr);

	cairo_pattern_destroy (pat);
	cairo_destroy(cr);

	gdk_draw_pixbuf(GDK_DRAWABLE(wi->window), gdk_gc_new(GDK_DRAWABLE(wi->window)),
	                _image, 0, 0,
	                x0, y0, rect_width,rect_height,
	                GDK_RGB_DITHER_NORMAL, 0, 0);

	g_object_unref(_image);
	gdk_region_destroy (region);
	return FALSE;
}

static gboolean vbox_expose(GtkWidget *wi, GdkEventExpose *ev)
{
	cairo_t *cr;
	/* create a cairo context */
	cr = gdk_cairo_create(wi->window);
	GdkRegion *region;
	region = gdk_region_rectangle (&wi->allocation);
	gdk_region_intersect (region, ev->region);
	gdk_cairo_region (cr, region);
	cairo_clip (cr);

	double x0      = wi->allocation.x+1;
	double y0      = wi->allocation.y+1;
	double rect_width  = wi->allocation.width-2;
	double rect_height = wi->allocation.height-3;

	cairo_rectangle (cr, x0,y0,rect_width,rect_height+3);
	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_fill (cr);

	cairo_pattern_t*pat =
		cairo_pattern_create_radial (-50, y0, 5,rect_width+100,  rect_height, 0.0);
	cairo_pattern_add_color_stop_rgb (pat, 0, 0.2, 0.2, 0.3);
	cairo_pattern_add_color_stop_rgb (pat, 1, 0.05, 0.05, 0.05);

	cairo_set_source (cr, pat);
	cairo_rectangle (cr, x0+1,y0+1,rect_width-2,rect_height-1);
	cairo_fill (cr);

	cairo_pattern_destroy (pat);
	cairo_destroy(cr);
	gdk_region_destroy (region);
	return FALSE;
}

static gboolean filter_box_expose(GtkWidget *wi, GdkEventExpose *ev)
{
	GdkPixbuf *_image, *stock_image;
	cairo_t *cr;
	/* create a cairo context */
	cr = gdk_cairo_create(wi->window);
	GdkRegion *region;
	region = gdk_region_rectangle (&wi->allocation);
	gdk_region_intersect (region, ev->region);
	gdk_cairo_region (cr, region);
	cairo_clip (cr);

	gint x0      = wi->allocation.x+1;
	gint y0      = wi->allocation.y+1;
	gint rect_width  = wi->allocation.width-2;
	gint rect_height = wi->allocation.height-3;

	stock_image = gtk_widget_render_icon(wi,"tribe",(GtkIconSize)-1,NULL);
	_image = gdk_pixbuf_scale_simple(
		stock_image, rect_width, rect_height, GDK_INTERP_HYPER);

	cairo_pattern_t*pat;

	gdk_draw_pixbuf(GDK_DRAWABLE(wi->window), gdk_gc_new(GDK_DRAWABLE(wi->window)),
	                _image, 0, 0,
	                x0, y0, rect_width,rect_height,
	                GDK_RGB_DITHER_NORMAL, 0, 0);

	double radius = 38.;
	if (rect_width<38) radius = rect_width;
	else if (rect_height<38) radius = rect_height;
	double x1,y1;

	x1=x0+rect_width;
	y1=y0+rect_height;

	cairo_move_to  (cr, x0, y0 + radius);
	cairo_curve_to (cr, x0 , y0, x0 , y0, x0 + radius, y0);
	cairo_line_to (cr, x1 - radius, y0);
	cairo_curve_to (cr, x1, y0, x1, y0, x1, y0 + radius);
	cairo_line_to (cr, x1 , y1 - radius);
	cairo_curve_to (cr, x1, y1, x1, y1, x1 - radius, y1);
	cairo_line_to (cr, x0 + radius, y1);
	cairo_curve_to (cr, x0, y1, x0, y1, x0, y1- radius);
	cairo_close_path (cr);

	pat = cairo_pattern_create_linear (0, y0, 0, y1);
	cairo_pattern_add_color_stop_rgba (pat, 1, 0, 0, 0, 0.8);
	cairo_pattern_add_color_stop_rgba (pat, 0.5, 0.05, 0.05, 0.05, 0.6);
	cairo_pattern_add_color_stop_rgba (pat, 0, 0.2, 0.2, 0.2, 0.4);
	cairo_set_source (cr, pat);
	cairo_fill (cr);

	cairo_pattern_destroy (pat);
	cairo_destroy(cr);
	g_object_unref(_image);
	gdk_region_destroy (region);
	return FALSE;
}

static gboolean plug_box_expose(GtkWidget *wi, GdkEventExpose *ev)
{
	cairo_t *cr;
	/* create a cairo context */
	cr = gdk_cairo_create(wi->window);
	GdkRegion *region;
	region = gdk_region_rectangle (&wi->allocation);
	gdk_region_intersect (region, ev->region);
	gdk_cairo_region (cr, region);
	cairo_clip (cr);

	double x0      = wi->allocation.x+1;
	double y0      = wi->allocation.y+1;
	double rect_width  = wi->allocation.width-2;
	double rect_height = wi->allocation.height-2;

    cairo_rectangle (cr, x0-1,y0-1,rect_width+2,rect_height+2);
            cairo_set_source_rgb (cr, 0, 0, 0);
            cairo_fill (cr);

	cairo_pattern_t*pat =
		cairo_pattern_create_radial (-50, y0, 5,rect_width-10,  rect_height, 20.0);
	set_skin_color(wi, pat);
	cairo_set_source (cr, pat);
	cairo_rectangle (cr, x0+2,y0+2,rect_width-4,rect_height-4);
	cairo_fill (cr);

    cairo_set_source_rgb(cr,  0.2, 0.2, 0.2);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr,x0+rect_width-3, y0+3);
    cairo_line_to(cr, x0+rect_width-3, y0+rect_height-2);
    cairo_line_to(cr, x0+2, y0+rect_height-2);
    cairo_stroke(cr);

    cairo_set_source_rgb(cr,  0.1, 0.1, 0.1);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr,x0+3, y0+rect_height-1);
    cairo_line_to(cr, x0+3, y0+3);
    cairo_line_to(cr, x0+rect_width-3, y0+3);
    cairo_stroke(cr);

    cairo_set_source_rgb(cr,  0.5, 0.5, 0.5);
    cairo_set_line_width(cr, 1.5);
    cairo_arc (cr, x0+7, y0+7, 1.5, 0, 2*M_PI);
    cairo_move_to(cr,x0+rect_width-8, y0+7);
    cairo_arc (cr, x0+rect_width-7, y0+7, 1.5, 0, 2*M_PI);
    cairo_move_to(cr,x0+rect_width-7, y0+rect_height-6);
    cairo_arc (cr, x0+rect_width-7, y0+rect_height-6, 1.5, 0, 2*M_PI);
    cairo_move_to(cr,x0+7, y0+rect_height-6);
    cairo_arc (cr, x0+7, y0+rect_height-6, 1.5, 0, 2*M_PI);
    cairo_stroke_preserve(cr);
    cairo_set_source_rgb(cr,  0.1, 0.1, 0.1);
    cairo_fill (cr);

    cairo_pattern_destroy (pat);
	cairo_destroy(cr);
	gdk_region_destroy (region);
	return FALSE;
}


static gboolean info_box_expose_base(
	GtkWidget *wi, GdkEventExpose *ev,
	void (*set_grad)(GtkWidget*,cairo_pattern_t*))
{
	cairo_t *cr;
	/* create a cairo context */
	cr = gdk_cairo_create(wi->window);
	GdkRegion *region;
	region = gdk_region_rectangle (&wi->allocation);
	gdk_region_intersect (region, ev->region);
	gdk_cairo_region (cr, region);
	cairo_clip (cr);

	double x0      = wi->allocation.x+1;
	double y0      = wi->allocation.y+1;
	double rect_width  = wi->allocation.width-2;
	double rect_height = wi->allocation.height-2;

    cairo_rectangle (cr, x0-1,y0-1,rect_width+2,rect_height+2);
    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_set_line_width(cr, 2.0);
    cairo_stroke(cr);

	cairo_pattern_t*pat = cairo_pattern_create_linear (x0, y0+50,x0, y0);
    cairo_pattern_set_extend(pat, CAIRO_EXTEND_REFLECT);
    set_grad(wi, pat);
	cairo_set_source (cr, pat);
	cairo_rectangle (cr, x0+2,y0+2,rect_width-4,rect_height-4);
	cairo_fill (cr);

	cairo_rectangle (cr, x0+8,y0+31,rect_width-16,rect_height-60);
	cairo_set_source_rgb (cr, 0.5, 0.5, 0.5);
    cairo_set_line_width(cr, 2.0);
	cairo_stroke_preserve(cr);
	pat = cairo_pattern_create_linear (x0, y0+50,x0, y0);
    cairo_pattern_set_extend(pat, CAIRO_EXTEND_REFLECT);
    set_skin_color(wi, pat);
    cairo_set_source (cr, pat);
	cairo_fill (cr);

    cairo_set_source_rgb(cr,  0.2, 0.2, 0.2);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr,x0+rect_width-3, y0+3);
    cairo_line_to(cr, x0+rect_width-3, y0+rect_height-2);
    cairo_line_to(cr, x0+2, y0+rect_height-2);
    cairo_stroke(cr);

    cairo_set_source_rgb(cr,  0.1, 0.1, 0.1);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr,x0+3, y0+rect_height-1);
    cairo_line_to(cr, x0+3, y0+3);
    cairo_line_to(cr, x0+rect_width-3, y0+3);
    cairo_stroke(cr);

    cairo_set_source_rgb(cr,  0.5, 0.5, 0.5);
    cairo_set_line_width(cr, 1.5);
    cairo_arc (cr, x0+7, y0+7, 1.5, 0, 2*M_PI);
    cairo_move_to(cr,x0+rect_width-8, y0+7);
    cairo_arc (cr, x0+rect_width-7, y0+7, 1.5, 0, 2*M_PI);
    cairo_move_to(cr,x0+rect_width-7, y0+rect_height-6);
    cairo_arc (cr, x0+rect_width-7, y0+rect_height-6, 1.5, 0, 2*M_PI);
    cairo_move_to(cr,x0+7, y0+rect_height-6);
    cairo_arc (cr, x0+7, y0+rect_height-6, 1.5, 0, 2*M_PI);
    cairo_stroke_preserve(cr);
    cairo_set_source_rgb(cr,  0.1, 0.1, 0.1);
    cairo_fill (cr);

    cairo_pattern_destroy (pat);
	cairo_destroy(cr);
	gdk_region_destroy (region);
	return FALSE;
}

static void set_grad_info_on(GtkWidget *wi, cairo_pattern_t *pat)
{
	cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0.8, 0, 0.8);
	cairo_pattern_add_color_stop_rgba (pat, 0.5, 0.05, 0.8, 0.05, 0.6);
	cairo_pattern_add_color_stop_rgba (pat, 1, 0.2, 0.8, 0.2, 0.4);
}

static gboolean info_box_expose_on(GtkWidget *wi, GdkEventExpose *ev)
{
	return info_box_expose_base(wi, ev, set_grad_info_on);
}

static gboolean info_box_expose_off(GtkWidget *wi, GdkEventExpose *ev)
{
	return info_box_expose_base(wi, ev, set_skin_color);
}


static gboolean slooper_expose(GtkWidget *wi, GdkEventExpose *ev)
{
	cairo_t *cr;
	/* create a cairo context */
	cr = gdk_cairo_create(wi->window);
	GdkRegion *region;
	region = gdk_region_rectangle (&wi->allocation);
	gdk_region_intersect (region, ev->region);
	gdk_cairo_region (cr, region);
	cairo_clip (cr);

	double x0      = wi->allocation.x+1;
	double y0      = wi->allocation.y+1;
	double rect_width  = wi->allocation.width-2;
	double rect_height = wi->allocation.height-2;

    cairo_rectangle (cr, x0-1,y0-1,rect_width+2,rect_height+2);
            cairo_set_source_rgb (cr, 0, 0, 0);
            cairo_fill (cr);

	cairo_pattern_t*pat =
		cairo_pattern_create_radial (-50, y0, 5,rect_width-10,  rect_height, 20.0);
	set_skin_color(wi, pat);
	cairo_set_source (cr, pat);
	cairo_rectangle (cr, x0+2,y0+2,rect_width-4,rect_height-27);
	cairo_fill (cr);

    cairo_set_source_rgb(cr,  0.2, 0.2, 0.2);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr,x0+rect_width-3, y0+3);
    cairo_line_to(cr, x0+rect_width-3, y0+rect_height-2);
    cairo_line_to(cr, x0+2, y0+rect_height-2);
    cairo_move_to(cr,x0+2, y0+rect_height-25);
    cairo_line_to(cr, x0+rect_width-3, y0+rect_height-25);
    cairo_stroke(cr);

    cairo_set_source_rgb(cr,  0.1, 0.1, 0.1);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr,x0+3, y0+rect_height-1);
    cairo_line_to(cr, x0+3, y0+3);
    cairo_line_to(cr, x0+rect_width-3, y0+3);
    cairo_move_to(cr,x0+2, y0+rect_height-27);
    cairo_line_to(cr, x0+rect_width-3, y0+rect_height-27);
    cairo_stroke(cr);


    cairo_pattern_destroy (pat);
	cairo_destroy(cr);
	gdk_region_destroy (region);
	return FALSE;
}

static gboolean gxhead_expose(GtkWidget *wi, GdkEventExpose *ev)
{
	GxPaintBoxClass *klass =  GX_PAINT_BOX_CLASS(GTK_OBJECT_GET_CLASS(wi));
	
	gint x0      = wi->allocation.x+1;
	gint y0      = wi->allocation.y+1;
	gint rect_width  = wi->allocation.width-2;
	gint rect_height = wi->allocation.height-3;
	
	static int nf = 0;
	static double ne_w1 = 0.;
	static int spf;
	
	gtk_widget_style_get(GTK_WIDGET(wi), "icon-set", &spf, NULL);
	
	if (nf != spf || ne_w1 != rect_width*rect_height || !(GDK_IS_PIXBUF (klass-> gxh_image))) {
		ne_w1 = rect_width*rect_height;
		nf = spf;
		if (G_IS_OBJECT(klass-> gxh_image)) {
			g_object_unref(klass->gxh_image);
		}
		GdkPixbuf  *stock_image, *frame;
		stock_image = gtk_widget_render_icon(wi,get_stock_id(wi),(GtkIconSize)-1,NULL);
		double scalew = rect_width/double(gdk_pixbuf_get_width(stock_image)-48);
		double scaleh = rect_height/double(gdk_pixbuf_get_height(stock_image)-48);
		
		klass->gxh_image = gdk_pixbuf_scale_simple(
			stock_image, rect_width, rect_height, GDK_INTERP_NEAREST);
		// upper border
		frame = gdk_pixbuf_new_subpixbuf(
			stock_image,24,0,gdk_pixbuf_get_width(stock_image)-48,12);
		gdk_pixbuf_scale (
			frame, klass->gxh_image,0,0,rect_width,12,0,0,scalew,1,GDK_INTERP_BILINEAR);
		// under border
		frame = gdk_pixbuf_new_subpixbuf(
			stock_image,24,gdk_pixbuf_get_height(stock_image)-12,
			gdk_pixbuf_get_width(stock_image)-48,12);
		gdk_pixbuf_scale (
			frame,klass->gxh_image,0,gdk_pixbuf_get_height(klass->gxh_image)-12,
			rect_width,12,0,gdk_pixbuf_get_height(klass->gxh_image)-12,
			scalew,1,GDK_INTERP_BILINEAR);
		// left border
		frame = gdk_pixbuf_new_subpixbuf(
			stock_image,0,24,12,gdk_pixbuf_get_height(stock_image)-48);	
		gdk_pixbuf_scale(
			frame, klass->gxh_image,0,12,12,rect_height-24,0,0,1,scaleh,GDK_INTERP_BILINEAR);
		// right border	
		frame = gdk_pixbuf_new_subpixbuf(
			stock_image,gdk_pixbuf_get_width(stock_image)-12,
			24,12,gdk_pixbuf_get_height(stock_image)-48);
		gdk_pixbuf_scale(
			frame,klass->gxh_image,gdk_pixbuf_get_width(klass->gxh_image)-12,
			12,12,rect_height-24,gdk_pixbuf_get_width(klass->gxh_image)-12,
			0,1,scaleh,GDK_INTERP_BILINEAR);
		//left upper corner
		frame = gdk_pixbuf_new_subpixbuf(
			stock_image,0,0,20,20);
		gdk_pixbuf_scale (
			frame, klass->gxh_image,0,0,20,20,0,0,1,1,GDK_INTERP_BILINEAR);
		//right upper corner
		frame = gdk_pixbuf_new_subpixbuf(
			stock_image,gdk_pixbuf_get_width(stock_image)-20,0,20,20);
		gdk_pixbuf_scale (
			frame, klass->gxh_image,gdk_pixbuf_get_width(klass->gxh_image)-20,
			0,20,20,gdk_pixbuf_get_width(klass->gxh_image)-20,0,1,1,
			GDK_INTERP_BILINEAR);
		//left under corner
		frame = gdk_pixbuf_new_subpixbuf(
			stock_image,0,gdk_pixbuf_get_height(stock_image)-20,20,20);
		gdk_pixbuf_scale (
			frame, klass->gxh_image,0,gdk_pixbuf_get_height(klass->gxh_image)-20,
			20,20,0,gdk_pixbuf_get_height(klass->gxh_image)-20,1,1,
			GDK_INTERP_BILINEAR);
		//right under corner
		frame = gdk_pixbuf_new_subpixbuf(
			stock_image,gdk_pixbuf_get_width(stock_image)-20,
			gdk_pixbuf_get_height(stock_image)-20,20,20);
		gdk_pixbuf_scale (
			frame, klass->gxh_image,gdk_pixbuf_get_width(klass->gxh_image)-20,
			gdk_pixbuf_get_height(klass->gxh_image)-20,
			20,20,gdk_pixbuf_get_width(klass->gxh_image)-20,
			gdk_pixbuf_get_height(klass->gxh_image)-20,1,1,
			GDK_INTERP_BILINEAR);
				
		// base 
		frame = gdk_pixbuf_new_subpixbuf(
			stock_image,24,24,gdk_pixbuf_get_width(stock_image)-68,
			gdk_pixbuf_get_height(stock_image)-48);
		gdk_pixbuf_scale (
			frame, klass->gxh_image,12,12,rect_width-24,rect_height-24,
			12,12,scalew,scaleh,GDK_INTERP_TILES);
		//g_object_unref(_image);
		g_object_unref(stock_image);
		g_object_unref(frame);
	}
	// draw to display
	gdk_draw_pixbuf(GDK_DRAWABLE(wi->window), gdk_gc_new(GDK_DRAWABLE(wi->window)),
	                klass->gxh_image, 0, 0,
	                x0, y0, rect_width,rect_height,
	                GDK_RGB_DITHER_NORMAL, 0, 0);
	
	return FALSE;
}

static gboolean gxrack_expose(GtkWidget *wi, GdkEventExpose *ev)
{
	GxPaintBoxClass *klass =  GX_PAINT_BOX_CLASS(GTK_OBJECT_GET_CLASS(wi));
	cairo_t *cr;
	/* create a cairo context */
	cr = gdk_cairo_create(wi->window);
	GdkRegion *region;
	region = gdk_region_rectangle (&wi->allocation);
	gdk_region_intersect (region, ev->region);
	gdk_cairo_region (cr, region);
	cairo_clip (cr);
	
	gint x0      = wi->allocation.x+1;
	gint y0      = wi->allocation.y+1;
	gint rect_width  = wi->allocation.width-2;
	gint rect_height = wi->allocation.height-3;
	
	static double ne_w = 0.;
	if (ne_w != rect_width*rect_height || !(GDK_IS_PIXBUF (klass-> gxr_image))) {
		ne_w = rect_width*rect_height;
		if (G_IS_OBJECT(klass-> gxr_image)) {
			g_object_unref(klass->gxr_image);
		}
		GdkPixbuf  *stock_image, *frame;
		stock_image = gtk_widget_render_icon(wi,get_stock_id(wi),(GtkIconSize)-1,NULL);
		double scalew = rect_width/double(gdk_pixbuf_get_width(stock_image)-48);
		double scaleh = rect_height/double(gdk_pixbuf_get_height(stock_image)-48);
		
		klass->gxr_image = gdk_pixbuf_scale_simple(
			stock_image, rect_width, rect_height, GDK_INTERP_NEAREST);
		// upper border
		frame = gdk_pixbuf_new_subpixbuf(
			stock_image,24,0,gdk_pixbuf_get_width(stock_image)-48,12);
		gdk_pixbuf_scale (
			frame, klass->gxr_image,0,0,rect_width,12,0,0,scalew,1,GDK_INTERP_BILINEAR);
		// under border
		frame = gdk_pixbuf_new_subpixbuf(
			stock_image,24,gdk_pixbuf_get_height(stock_image)-12,
			gdk_pixbuf_get_width(stock_image)-48,12);
		gdk_pixbuf_scale (
			frame,klass->gxr_image,0,gdk_pixbuf_get_height(klass->gxr_image)-12,
			rect_width,12,0,gdk_pixbuf_get_height(klass->gxr_image)-12,
			scalew,1,GDK_INTERP_BILINEAR);
		// left border
		frame = gdk_pixbuf_new_subpixbuf(
			stock_image,0,24,12,gdk_pixbuf_get_height(stock_image)-48);
		gdk_pixbuf_scale(
			frame, klass->gxr_image,0,12,12,rect_height-24,0,0,1,scaleh,GDK_INTERP_BILINEAR);
		// right border	
		frame = gdk_pixbuf_new_subpixbuf(
			stock_image,gdk_pixbuf_get_width(stock_image)-12,
			24,12,gdk_pixbuf_get_height(stock_image)-48);
		gdk_pixbuf_scale(
			frame,klass->gxr_image,gdk_pixbuf_get_width(klass->gxr_image)-12,
			12,12,rect_height-24,gdk_pixbuf_get_width(klass->gxr_image)-12,
			0,1,scaleh,GDK_INTERP_BILINEAR);
		//left upper corner
		frame = gdk_pixbuf_new_subpixbuf(
			stock_image,0,0,20,20);
		gdk_pixbuf_scale (
			frame, klass->gxr_image,0,0,20,20,0,0,1,1,GDK_INTERP_BILINEAR);
		//right upper corner
		frame = gdk_pixbuf_new_subpixbuf(
			stock_image,gdk_pixbuf_get_width(stock_image)-20,0,20,20);
		gdk_pixbuf_scale (
			frame, klass->gxr_image,gdk_pixbuf_get_width(klass->gxr_image)-20,
			0,20,20,gdk_pixbuf_get_width(klass->gxr_image)-20,0,1,1,
			GDK_INTERP_BILINEAR);
		//left under corner
		frame = gdk_pixbuf_new_subpixbuf(
			stock_image,0,gdk_pixbuf_get_height(stock_image)-20,20,20);
		gdk_pixbuf_scale (
			frame, klass->gxr_image,0,gdk_pixbuf_get_height(klass->gxr_image)-20,
			20,20,0,gdk_pixbuf_get_height(klass->gxr_image)-20,1,1,
			GDK_INTERP_BILINEAR);
		//right under corner
		frame = gdk_pixbuf_new_subpixbuf(
			stock_image,gdk_pixbuf_get_width(stock_image)-20,
			gdk_pixbuf_get_height(stock_image)-20,20,20);
		gdk_pixbuf_scale (
			frame, klass->gxr_image,gdk_pixbuf_get_width(klass->gxr_image)-20,
			gdk_pixbuf_get_height(klass->gxr_image)-20,
			20,20,gdk_pixbuf_get_width(klass->gxr_image)-20,
			gdk_pixbuf_get_height(klass->gxr_image)-20,1,1,
			GDK_INTERP_BILINEAR);
		g_object_unref(stock_image);
		g_object_unref(frame);
	}
	
	// draw to display
	gdk_draw_pixbuf(GDK_DRAWABLE(wi->window), gdk_gc_new(GDK_DRAWABLE(wi->window)),
	                klass->gxr_image, 0, 0,
	                x0, y0, rect_width,rect_height,
	                GDK_RGB_DITHER_NORMAL, 0, 0);
	                
	// base 
	x0      += 12;
	y0      += 12;
	rect_width  -= 24;
	rect_height -= 24;
	
	cairo_rectangle (cr, x0-1,y0-1,rect_width+2,rect_height+2);
	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_fill (cr);

	cairo_pattern_t*pat =
	cairo_pattern_create_linear (0, y0, 0, y0+rect_height);
	//cairo_pattern_create_radial (-50, y0, 5,rect_width-10,  rect_height, 20.0);
	set_skin_color(wi, pat);
	cairo_set_source (cr, pat);
	cairo_rectangle (cr, x0+2,y0+2,rect_width-4,rect_height-4);
	cairo_fill (cr);

	cairo_set_source_rgb(cr,  0.2, 0.2, 0.2);
	cairo_set_line_width(cr, 2.0);
	cairo_move_to(cr,x0+rect_width-3, y0+3);
	cairo_line_to(cr, x0+rect_width-3, y0+rect_height-2);
	cairo_line_to(cr, x0+2, y0+rect_height-2);
	cairo_stroke(cr);

	cairo_set_source_rgb(cr,  0.1, 0.1, 0.1);
	cairo_set_line_width(cr, 2.0);
	cairo_move_to(cr,x0+3, y0+rect_height-1);
	cairo_line_to(cr, x0+3, y0+3);
	cairo_line_to(cr, x0+rect_width-3, y0+3);
	cairo_stroke(cr);

	cairo_pattern_destroy (pat);
	cairo_destroy(cr);
	gdk_region_destroy (region);                
	return FALSE;
}

static gboolean main_expose(GtkWidget *wi, GdkEventExpose *ev)
{
	
	cairo_t *cr;
	/* create a cairo context */
	cr = gdk_cairo_create(wi->window);
	GdkRegion *region;
	region = gdk_region_rectangle (&wi->allocation);
	gdk_region_intersect (region, ev->region);
	gdk_cairo_region (cr, region);
	cairo_clip (cr);
	static int w,h;
	
	gtk_widget_style_get(GTK_WIDGET(wi), "width", &w, NULL);
	gtk_widget_style_get(GTK_WIDGET(wi), "height", &h, NULL);
	gtk_widget_set_size_request (GTK_WIDGET (wi),w,h);
	gint x0      = wi->allocation.x;
	gint y0      = wi->allocation.y;
	
	
	GdkPixbuf  *main_image = gtk_widget_render_icon(wi,get_main_image_id(wi),(GtkIconSize)-1,NULL);
	
	gdk_draw_pixbuf(GDK_DRAWABLE(wi->window), gdk_gc_new(GDK_DRAWABLE(wi->window)),
	                main_image, 0, 0,
	                x0, y0, w,h,
	                GDK_RGB_DITHER_NORMAL, 0, 0);
	return FALSE;
}
static void set_expose_func(GxPaintBox *paint_box, const gchar *paint_func)
{
	if (strcmp(paint_func, "amp_expose") == 0) {
		paint_box->expose_func = amp_expose;
	} else if (strcmp(paint_func, "conv_widget_expose") == 0) {
		paint_box->expose_func = conv_widget_expose;
	} else if (strcmp(paint_func, "upper_widget_expose") == 0) {
		paint_box->expose_func = upper_widget_expose;
	} else if (strcmp(paint_func, "rectangle_expose") == 0) {
		paint_box->expose_func = rectangle_expose;
	} else if (strcmp(paint_func, "rectangle_skin_color_expose") == 0) {
		paint_box->expose_func = rectangle_skin_color_expose;
	} else if (strcmp(paint_func, "convolver_icon_expose") == 0) {
		paint_box->expose_func = convolver_icon_expose;
	} else if (strcmp(paint_func, "AmpBox_expose") == 0) {
		paint_box->expose_func = AmpBox_expose;
	} else if (strcmp(paint_func, "tribal_box_expose") == 0) {
		paint_box->expose_func = tribal_box_expose;
	} else if (strcmp(paint_func, "vbox_expose") == 0) {
		paint_box->expose_func = vbox_expose;
	} else if (strcmp(paint_func, "filter_box_expose") == 0) {
		paint_box->expose_func = filter_box_expose;
	} else if (strcmp(paint_func, "plug_box_expose") == 0) {
		paint_box->expose_func = plug_box_expose;
	} else if (strcmp(paint_func, "info_box_expose_on") == 0) {
		paint_box->expose_func = info_box_expose_on;
	} else if (strcmp(paint_func, "info_box_expose_off") == 0) {
		paint_box->expose_func = info_box_expose_off;
	} else if (strcmp(paint_func, "slooper_expose") == 0) {
		paint_box->expose_func = slooper_expose;
	} else if (strcmp(paint_func, "zac_expose") == 0) {
		paint_box->expose_func = zac_expose;
	} else if (strcmp(paint_func, "gxhead_expose") == 0) {
		paint_box->expose_func = gxhead_expose;
	} else if (strcmp(paint_func, "RackBox_expose") == 0) {
		paint_box->expose_func = RackBox_expose;
	} else if (strcmp(paint_func, "gxrack_expose") == 0) {
		paint_box->expose_func = gxrack_expose;
	} else if (strcmp(paint_func, "lhfilter_expose") == 0) {
		paint_box->expose_func = lhfilter_expose;
	} else if (strcmp(paint_func, "crybaby_expose") == 0) {
		paint_box->expose_func = crybaby_expose;
	} else if (strcmp(paint_func, "compressor_expose") == 0) {
		paint_box->expose_func = compressor_expose;
	} else if (strcmp(paint_func, "eq_expose") == 0) {
		paint_box->expose_func = eq_expose;
	} else if (strcmp(paint_func, "led_expose") == 0) {
		paint_box->expose_func = led_expose;
	} else if (strcmp(paint_func, "seq_expose") == 0) {
		paint_box->expose_func = seq_expose;
	} else if (strcmp(paint_func, "ir_expose") == 0) {
		paint_box->expose_func = ir_expose;
	} else if (strcmp(paint_func, "main_expose") == 0) {
		paint_box->expose_func = main_expose;
	} else {
		paint_box->expose_func = 0;
	}
}
