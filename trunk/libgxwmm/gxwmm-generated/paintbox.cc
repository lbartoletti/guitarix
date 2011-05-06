// Generated by gtkmmproc -- DO NOT MODIFY!


#include <gxwmm/paintbox.h>
#include <gxwmm/private/paintbox_p.h>

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

#include <gxw/GxPaintBox.h>

namespace
{
} // anonymous namespace


namespace Glib
{

Gxw::PaintBox* wrap(GxPaintBox* object, bool take_copy)
{
  return dynamic_cast<Gxw::PaintBox *> (Glib::wrap_auto ((GObject*)(object), take_copy));
}

} /* namespace Glib */

namespace Gxw
{


/* The *_Class implementation: */

const Glib::Class& PaintBox_Class::init()
{
  if(!gtype_) // create the GType if necessary
  {
    // Glib::Class has to know the class init function to clone custom types.
    class_init_func_ = &PaintBox_Class::class_init_function;

    // This is actually just optimized away, apparently with no harm.
    // Make sure that the parent type has been created.
    //CppClassParent::CppObjectType::get_type();

    // Create the wrapper type, with the same class/instance size as the base type.
    register_derived_type(gx_paint_box_get_type());

    // Add derived versions of interfaces, if the C type implements any interfaces:

  }

  return *this;
}


void PaintBox_Class::class_init_function(void* g_class, void* class_data)
{
  BaseClassType *const klass = static_cast<BaseClassType*>(g_class);
  CppClassParent::class_init_function(klass, class_data);

#ifdef GLIBMM_VFUNCS_ENABLED
#endif //GLIBMM_VFUNCS_ENABLED

#ifdef GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
#endif //GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
}

#ifdef GLIBMM_VFUNCS_ENABLED
#endif //GLIBMM_VFUNCS_ENABLED

#ifdef GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
#endif //GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED


Glib::ObjectBase* PaintBox_Class::wrap_new(GObject* o)
{
  return manage(new PaintBox((GxPaintBox*)(o)));

}


/* The implementation: */

PaintBox::PaintBox(const Glib::ConstructParams& construct_params)
:
  Gtk::Box(construct_params)
{
  }

PaintBox::PaintBox(GxPaintBox* castitem)
:
  Gtk::Box((GtkBox*)(castitem))
{
  }

PaintBox::~PaintBox()
{
  destroy_();
}

PaintBox::CppClassType PaintBox::paintbox_class_; // initialize static member

GType PaintBox::get_type()
{
  return paintbox_class_.init().get_type();
}


GType PaintBox::get_base_type()
{
  return gx_paint_box_get_type();
}


PaintBox::PaintBox()
:
  // Mark this class as non-derived to allow C++ vfuncs to be skipped.
  Glib::ObjectBase(0),
  Gtk::Box(Glib::ConstructParams(paintbox_class_.init()))
{
  

}

void PaintBox::pack_start(Gtk::Widget& child, gboolean expand, gboolean fill, guint padding)
{
gx_box_pack_start(gobj(), (child).gobj(), expand, fill, padding); 
}

void PaintBox::pack_end(Gtk::Widget& child, gboolean expand, gboolean fill, guint padding)
{
gx_box_pack_end(gobj(), (child).gobj(), expand, fill, padding); 
}

void PaintBox::set_border_width(guint border_width)
{
gx_box_set_border_width(gobj(), border_width); 
}

void PaintBox::add(Gtk::Widget& child)
{
gx_box_add(gobj(), (child).gobj()); 
}

void PaintBox::remove(Gtk::Widget& child)
{
gx_box_remove(gobj(), (child).gobj()); 
}

GList* PaintBox::get_children()
{
  return gx_box_get_children(gobj());
}


#ifdef GLIBMM_PROPERTIES_ENABLED
Glib::PropertyProxy<Glib::ustring> PaintBox::property_paint_func() 
{
  return Glib::PropertyProxy<Glib::ustring>(this, "paint-func");
}
#endif //GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
Glib::PropertyProxy_ReadOnly<Glib::ustring> PaintBox::property_paint_func() const
{
  return Glib::PropertyProxy_ReadOnly<Glib::ustring>(this, "paint-func");
}
#endif //GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
Glib::PropertyProxy<guint> PaintBox::property_border_width() 
{
  return Glib::PropertyProxy<guint>(this, "border-width");
}
#endif //GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
Glib::PropertyProxy_ReadOnly<guint> PaintBox::property_border_width() const
{
  return Glib::PropertyProxy_ReadOnly<guint>(this, "border-width");
}
#endif //GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
Glib::PropertyProxy<guint> PaintBox::property_spacing() 
{
  return Glib::PropertyProxy<guint>(this, "spacing");
}
#endif //GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
Glib::PropertyProxy_ReadOnly<guint> PaintBox::property_spacing() const
{
  return Glib::PropertyProxy_ReadOnly<guint>(this, "spacing");
}
#endif //GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
Glib::PropertyProxy<guint> PaintBox::property_homogeneous() 
{
  return Glib::PropertyProxy<guint>(this, "homogeneous");
}
#endif //GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
Glib::PropertyProxy_ReadOnly<guint> PaintBox::property_homogeneous() const
{
  return Glib::PropertyProxy_ReadOnly<guint>(this, "homogeneous");
}
#endif //GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
Glib::PropertyProxy<guint> PaintBox::property_icon_set() 
{
  return Glib::PropertyProxy<guint>(this, "icon-set");
}
#endif //GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
Glib::PropertyProxy_ReadOnly<guint> PaintBox::property_icon_set() const
{
  return Glib::PropertyProxy_ReadOnly<guint>(this, "icon-set");
}
#endif //GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
Glib::PropertyProxy<guint> PaintBox::property_width() 
{
  return Glib::PropertyProxy<guint>(this, "width");
}
#endif //GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
Glib::PropertyProxy_ReadOnly<guint> PaintBox::property_width() const
{
  return Glib::PropertyProxy_ReadOnly<guint>(this, "width");
}
#endif //GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
Glib::PropertyProxy<guint> PaintBox::property_height() 
{
  return Glib::PropertyProxy<guint>(this, "height");
}
#endif //GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
Glib::PropertyProxy_ReadOnly<guint> PaintBox::property_height() const
{
  return Glib::PropertyProxy_ReadOnly<guint>(this, "height");
}
#endif //GLIBMM_PROPERTIES_ENABLED


#ifdef GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
#endif //GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED

#ifdef GLIBMM_VFUNCS_ENABLED
#endif //GLIBMM_VFUNCS_ENABLED


} // namespace Gxw


