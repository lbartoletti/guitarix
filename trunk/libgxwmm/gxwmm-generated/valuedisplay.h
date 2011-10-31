// -*- c++ -*-
// Generated by gtkmmproc -- DO NOT MODIFY!
#ifndef _GXWMM_VALUEDISPLAY_H
#define _GXWMM_VALUEDISPLAY_H


#include <glibmm.h>

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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <gxwmm/regler.h>
#include <gtkmm/adjustment.h>


#ifndef DOXYGEN_SHOULD_SKIP_THIS
typedef struct _GxValueDisplay GxValueDisplay;
typedef struct _GxValueDisplayClass GxValueDisplayClass;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */


namespace Gxw
{ class ValueDisplay_Class; } // namespace Gxw
namespace Gxw {


class ValueDisplay: public Gxw::Regler {
	public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  typedef ValueDisplay CppObjectType;
  typedef ValueDisplay_Class CppClassType;
  typedef GxValueDisplay BaseObjectType;
  typedef GxValueDisplayClass BaseClassType;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

  virtual ~ValueDisplay();

#ifndef DOXYGEN_SHOULD_SKIP_THIS

private:
  friend class ValueDisplay_Class;
  static CppClassType valuedisplay_class_;

  // noncopyable
  ValueDisplay(const ValueDisplay&);
  ValueDisplay& operator=(const ValueDisplay&);

protected:
  explicit ValueDisplay(const Glib::ConstructParams& construct_params);
  explicit ValueDisplay(GxValueDisplay* castitem);

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  static GType get_type()      G_GNUC_CONST;


  static GType get_base_type() G_GNUC_CONST;
#endif

  ///Provides access to the underlying C GtkObject.
  GxValueDisplay*       gobj()       { return reinterpret_cast<GxValueDisplay*>(gobject_); }

  ///Provides access to the underlying C GtkObject.
  const GxValueDisplay* gobj() const { return reinterpret_cast<GxValueDisplay*>(gobject_); }


public:
  //C++ methods used to invoke GTK+ virtual functions:

protected:
  //GTK+ Virtual Functions (override these to change behaviour):

  //Default Signal Handlers::


private:

	public:
	ValueDisplay();
	explicit ValueDisplay(Gtk::Adjustment& adjustment);


};

} // namespace Gxw


namespace Glib
{
  /** A Glib::wrap() method for this object.
   * 
   * @param object The C instance.
   * @param take_copy False if the result should take ownership of the C instance. True if it should take a new copy or ref.
   * @result A C++ instance that wraps this C instance.
   *
   * @relates Gxw::ValueDisplay
   */
  Gxw::ValueDisplay* wrap(GxValueDisplay* object, bool take_copy = false);
} //namespace Glib


#endif /* _GXWMM_VALUEDISPLAY_H */

