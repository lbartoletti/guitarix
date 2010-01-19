/*
 * Copyright (C) 2009 Hermann Meyer and James Warden
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
 * ---------------------------------------------------------------------------
 *
 *    This is the guitarix GUI related functionality
 *
 * ----------------------------------------------------------------------------
 */
#ifndef NJACKLAT
#define NJACKLAT (9)
#endif

#ifndef NUM_PORT_LISTS
#define NUM_PORT_LISTS (3)
#endif

#include <errno.h>

#include <assert.h>
#include <cstring>
#include <list>
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <cstdio>

using namespace std;

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <jack/jack.h>
#include <sndfile.h>

#include "guitarix.h"

using namespace gx_system;
using namespace gx_child_process;
using namespace gx_preset;


/* -------- helper for level meter display -------- */
inline float
log_meter (float db)
{
  gfloat def = 0.0f; /* Meter deflection %age */

  if (db < -70.0f)
    {
      def = 0.0f;
    }
  else if (db < -60.0f)
    {
      def = (db + 70.0f) * 0.25f;
    }
  else if (db < -50.0f)
    {
      def = (db + 60.0f) * 0.5f + 2.5f;
    }
  else if (db < -40.0f)
    {
      def = (db + 50.0f) * 0.75f + 7.5f;
    }
  else if (db < -30.0f)
    {
      def = (db + 40.0f) * 1.5f + 15.0f;
    }
  else if (db < -20.0f)
    {
      def = (db + 30.0f) * 2.0f + 30.0f;
    }
  else if (db < 6.0f)
    {
      def = (db + 20.0f) * 2.5f + 50.0f;
    }
  else
    {
      def = 115.0f;
    }

  /* 115 is the deflection %age that would be
     when db=6.0. this is an arbitrary
     endpoint for our scaling.
  */

  return def/115.0f;
}

/* --------- calculate power (percent) to decibel -------- */
// Note: could use fast_log10 (see ardour code) to make it faster
inline float power2db(float power)
{
  return  20.*log10(power);
}

/* --------------------------- gx_gui namespace ------------------------ */
namespace gx_gui
  {
    //***** the cairo callbacks *****//
#include "gx_cairo_callbacks.cpp"


    static const float falloff = meter_falloff * meter_display_timeout * 0.001;

    /* --------- menu function triggering engine on/off/bypass --------- */
    void gx_engine_switch (GtkWidget* widget, gpointer arg)
    {
      gx_engine::GxEngineState estate =
        (gx_engine::GxEngineState)gx_engine::checky;

      switch (estate)
        {
        case gx_engine::kEngineOn:
          estate = gx_engine::kEngineOff;
          if (arg)
            {
              // need to activate item
              gtk_check_menu_item_set_active(
                GTK_CHECK_MENU_ITEM(gx_engine_item), TRUE
              );
              estate = gx_engine::kEngineBypass;
            }

          break;

        case gx_engine::kEngineOff:
          if (!arg)
            estate = gx_engine::kEngineOn;
          break;

        default:
          estate = gx_engine::kEngineOn;
          gtk_check_menu_item_set_active(
            GTK_CHECK_MENU_ITEM(gx_engine_item), TRUE
          );
        }

      gx_engine::checky = (float)estate;
      gx_refresh_engine_status_display();
    }

    /* -------------- refresh engine status display ---------------- */
    void gx_refresh_engine_status_display()
    {
      gx_engine::GxEngineState estate =
        (gx_engine::GxEngineState)gx_engine::checky;

      string state;

      switch (estate)
        {

        case gx_engine::kEngineOff:
          gtk_widget_show(gx_engine_off_image);
          gtk_widget_hide(gx_engine_on_image);
          gtk_widget_hide(gx_engine_bypass_image);

          gtk_check_menu_item_set_active(
            GTK_CHECK_MENU_ITEM(gx_engine_item), FALSE
          );
          state = "OFF";
          break;

        case gx_engine::kEngineBypass:
          gtk_widget_show(gx_engine_bypass_image);
          gtk_widget_hide(gx_engine_off_image);
          gtk_widget_hide(gx_engine_on_image);

          gtk_check_menu_item_set_active(
            GTK_CHECK_MENU_ITEM(gx_gui::gx_engine_item), TRUE
          );
          state = "BYPASSED";
          break;

        case gx_engine::kEngineOn:
        default: // ON
          gtk_widget_show(gx_engine_on_image);
          gtk_widget_hide(gx_engine_off_image);
          gtk_widget_hide(gx_engine_bypass_image);

          gtk_check_menu_item_set_active(
            GTK_CHECK_MENU_ITEM(gx_gui::gx_engine_item), TRUE
          );
          state = "ON";
        }

      gx_print_info("Engine State: ", state);
    }

    /* ----------------- refresh GX level display function ---------------- */
    gboolean gx_refresh_meter_level(gpointer args)
    {
      if (gx_jack::client && gx_engine::buffers_ready)
        {

          GxMainInterface* gui = GxMainInterface::instance();

          // data holders for meters
          // Note: removed RMS calculation, we will only focus on max peaks
          float max_level  [2];
          (void)memset(max_level,   0, sizeof(max_level));
          float max_jclevel[2];
          (void)memset(max_jclevel, 0, sizeof(max_jclevel));

          static float old_peak_db  [2] = {-INFINITY, -INFINITY};
          static float old_jcpeak_db[2] = {-INFINITY, -INFINITY};

          jack_nframes_t nframes = gx_jack::jack_bs;

          // retrieve meter widgets
          GtkWidget* const* meters   = gui->getLevelMeters();
          GtkWidget* const* jcmeters = gui->getJCLevelMeters();

          // fill up from engine buffers
          for (int c = 0; c < 2; c++)
            {
              // guitarix output levels
              float data[nframes];

              // jconv output levels
              float jcdata[nframes];

              // need to differentiate between channels due to stereo
              switch (c)
                {
                default:
                case 0:
                  (void)memcpy(data, gx_engine::get_frame,  sizeof(data));
                  break;

                case 1:
                  (void)memcpy(data, gx_engine::get_frame1, sizeof(data));
                  break;
                }

              // turn channels for box_pack_end
              int b;
              if (c)b=1;
              else b=2;
              // jconv: note that jconv monitor channels are input[1] and [2]
              if (gx_jconv::jconv_is_running && gx_engine::is_setup)
                (void)memcpy(jcdata, gx_engine::gInChannel[b], sizeof(jcdata));

              // calculate  max peak
              for (guint f = 0; f < nframes; f++)
                {
                  max_level[c] = max(max_level[c], abs(data[f]));

                  if (gx_jconv::jconv_is_running && gx_engine::is_setup)
                    max_jclevel[c] = max(max_jclevel[c], abs(jcdata[f]));
                }


              // update meters (consider falloff as well)
              if (meters[c])
                {
                  // calculate peak dB and translate into meter
                  float peak_db = -INFINITY;
                  if (max_level[c] > 0.) peak_db = power2db(max_level[c]);

                  // retrieve old meter value and consider falloff
                  if (peak_db < old_peak_db[c])
                    peak_db = old_peak_db[c] - falloff;

                  gtk_fast_meter_set(GTK_FAST_METER(meters[c]), log_meter(peak_db));
                  old_peak_db[c] = max(peak_db, -INFINITY);
                }


              if (gx_jconv::jconv_is_running && jcmeters[c] && gx_engine::is_setup)
                {
                  // calculate peak dB and translate into meter
                  float peak_db = -INFINITY;
                  if (max_jclevel[c] > 0.) peak_db = power2db(max_jclevel[c]);

                  // retrieve old meter value and consider falloff
                  if (peak_db < old_jcpeak_db[c])
                    peak_db = old_jcpeak_db[c] - falloff;

                  gtk_fast_meter_set(GTK_FAST_METER(jcmeters[c]), log_meter(peak_db));
                  old_jcpeak_db[c] = max(peak_db, -INFINITY);
                }
            }
        }


      return TRUE;
    }

    /* -------------- refresh oscilloscope function -------------- */
    gboolean gx_refresh_oscilloscope(gpointer args)
    {
      if ((showwave == 1) &&
          ((wave_view_mode == kWvMode1) ||
           (wave_view_mode == kWvMode2)))
        gx_engine::GxEngine::instance()->viv = gx_engine::gOutChannel[0][0];
      return TRUE;
    }

gboolean gx_refresh_tuner(gpointer args)
    {
    gx_engine::GxEngine::instance()->fConsta1t = gx_engine::GxEngine::instance()->fConsta1;
return TRUE;
    }

    /* -------------- timeout for jconv startup when guitarix init -------------- */
    gboolean gx_check_startup(gpointer args)
    {
      gx_survive_jack_shutdown(NULL);
      gx_monitor_jack_ports(NULL);
      gx_engine::is_setup = 1;
      if (gx_jconv::GxJConvSettings::checkbutton7 == 1)
        {
          gx_start_stop_jconv(NULL,NULL);
        }
      return FALSE;

    }

    /* -------------- for thread that checks jackd liveliness -------------- */
    gboolean gx_survive_jack_shutdown(gpointer arg)
    {
      GtkWidget* wd = GxMainInterface::instance()->getJackConnectItem();

      // return if jack is not down
      if (gx_system_call("pgrep", "jackd", true) == SYSTEM_OK)
        {
          if (gx_jack::jack_is_down)
            {
              // let's make sure we get out of here
              if (!gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(wd)))
                gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(wd), TRUE);

              // revive existing client menus
              GxMainInterface::instance()->initClientPortMaps();

              return TRUE;
            }
        }
      else
        {
          // set jack client to NULL
          gx_jack::client = 0;

          // refresh some stuff. Note that it can be executed
          // more than once, no harm here
          gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(wd), FALSE);
          GxMainInterface::instance()->deleteAllClientPortMaps();

          gx_jconv::GxJConvSettings::checkbutton7 = 0;
          gx_jconv::jconv_is_running = false;
          gx_jack::jack_is_down = true;
        }
      return TRUE;
    }

    /* --------- queue up new client ports as they are registered -------- */
    void gx_queue_client_port(const string name,
                              const string type,
                              const int flags)
    {
      // add the port
      gx_client_port_queue.insert(pair<string, int>(name, flags));
    }

    /* --------- dequeue client ports as they are deregistered -------- */
    void gx_dequeue_client_port(const string name,
                                const string type,
                                const int flags)
    {
      // remove the port
      gx_client_port_dequeue.insert(pair<string, int>(name, flags));
    }


    /* ---------------------- monitor jack ports  items ------------------ */
    // we also refresh the connection status of these buttons
    gboolean gx_monitor_jack_ports(gpointer args)
    {
      // get gui instance
      GxMainInterface* gui = GxMainInterface::instance();

      // don't bother if we are not a valid client or if we are in the middle
      // of deleting stuff
      // if we are off jack or jack is down, delete everything
      if (!gx_jack::client || gx_jack::jack_is_down)
        {
          gui->deleteAllClientPortMaps();
          gx_client_port_dequeue.clear();
          gx_client_port_queue.clear();
          return TRUE;
        }

      // if the external client left without "unregistering" its ports
      // (yes, it happens, shame on the devs ...), we catch it here
      if (!gx_jack::client_out_graph.empty())
        {
          gx_client_port_dequeue.clear();
          gui->deleteClientPortMap(gx_jack::client_out_graph);
          gx_jack::client_out_graph = "";
          return TRUE;
        }

      // browse queue of added ports and update if needed
      gui->addClientPorts();

      // browse queue of removed ports and update if needed
      gui->deleteClientPorts();

      // loop over all existing clients
      set<GtkWidget*>::iterator cit = gui->fClientPortMap.begin();
      while (cit != gui->fClientPortMap.end())
        {
          // fetch client port map
          GtkWidget* portmap = *cit;
          GList* list = gtk_container_get_children(GTK_CONTAINER(portmap));

          guint len = g_list_length(list);
          if (len == NUM_PORT_LISTS) // something weird ...
            for (guint i = 0; i < len; i++)
              {
                // fetch client table
                GtkWidget* table = (GtkWidget*)g_list_nth_data(list, i);

                // check port connection status for each port
                gtk_container_foreach(GTK_CONTAINER(table),
                                      gx_refresh_portconn_status,
                                      GINT_TO_POINTER(i));
              }
          g_list_free(list);
          // next client
          cit++;
        }

      return TRUE;
    }

    /* --------------- refresh port connection button status -------------- */
    void gx_refresh_portconn_status(GtkWidget* button, gpointer data)
    {
      if (GTK_IS_CHECK_BUTTON(button) == FALSE) return;

      // our ports
      jack_port_t* ports[] =
      {
        gx_jack::input_ports [0],
        gx_jack::output_ports[0],
        gx_jack::output_ports[1]
      };

      int index = GPOINTER_TO_INT(data);

      // fetch port name from widget name
      const string port_name = gtk_widget_get_name(button);

      // delete unaktive clients from portmap
      if (!jack_port_by_name(gx_jack::client, port_name.c_str()))
        {
          gx_dequeue_client_port(port_name.c_str(),JACK_DEFAULT_MIDI_TYPE,0);
          return;
        }

      // connection to port
      int nconn = jack_port_connected_to(ports[index], port_name.c_str());

      // update status
      if (nconn == 0) // no connection
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), FALSE);

      else if (nconn > 0)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button),  TRUE);

    }

    /* ----- cycle through client tabs in client portmaps notebook  ------*/
    void gx_cycle_through_client_tabs(GtkWidget* item, gpointer data)
    {
      GxMainInterface* gui = GxMainInterface::instance();

      // current page
      int page = gtk_notebook_get_current_page(gui->getPortMapTabs());

      // next page
      gtk_notebook_next_page(gui->getPortMapTabs());

      // are we reaching the end of the notebook ?
      if (page == gtk_notebook_get_current_page(gui->getPortMapTabs()))
        gtk_notebook_set_current_page(gui->getPortMapTabs(), 0);
    }

    // get the last used skin as default
    void gx_set_skin_change(float fskin)
    {
      last_skin = int(fskin);
    }

    // save the current used skin as default
    void gx_get_skin_change(float *fskin)
    {
      *fskin  = float(gx_current_skin);
    }

    //----menu function gx_show_oscilloscope
    void gx_show_oscilloscope (GtkCheckMenuItem *menuitem, gpointer checkplay)
    {
      if (gtk_check_menu_item_get_active(menuitem) == TRUE)
        {
          showwave = 1;
          gtk_widget_show(livewa);
        }
      else
        {
          showwave = 0;
          gtk_widget_hide(livewa);
        }
    }

    //----menu function gx_tuner
    void gx_tuner (GtkCheckMenuItem *menuitem, gpointer checkplay)
    {
      if (gtk_check_menu_item_get_active(menuitem) == TRUE)
        {
          shownote = 1;
          gtk_widget_show(pb);
        }
      else
        {
          shownote = 0;
          gtk_widget_hide(pb);
        }
    }

    //---- menu function gx_midi_out
    void gx_midi_out (GtkCheckMenuItem *menuitem, gpointer checkplay)
    {
      if (gtk_check_menu_item_get_active(menuitem) == TRUE)
        {
          gx_engine::dsp::turnOnMidi();
          gtk_widget_show(midibox);
        }
      else
        {
          gx_engine::dsp::turnOffMidi();
          gtk_widget_hide(midibox);
        }
    }

    //---- menu function gx_midi_out
    void gx_log_window (GtkWidget* menuitem, gpointer arg)
    {
      GtkExpander* const exbox = GxMainInterface::instance()->getLoggingBox();

      // we could be called before UI is built up
      if (!exbox) return;

      // open it
      if (gtk_expander_get_expanded(exbox) ==  FALSE ||
          gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(menuitem)) == TRUE)
        gtk_expander_set_expanded(exbox, TRUE);

      // close it
      else
        gtk_expander_set_expanded(exbox, FALSE);

      // in any case, reset handle color
      GdkColor exp_color;
      gdk_color_parse("#ffffff", &exp_color);
      gtk_widget_modify_fg (GTK_WIDGET(exbox), GTK_STATE_NORMAL, &exp_color);
    }

    //----menu funktion about
    void gx_show_about( GtkWidget *widget, gpointer data )
    {
      static string about;
      if (about.empty())
        {
          about +=
            "\n  This Aplication is to a large extent provided"
            "\n  with the marvelous faust compiler.Yann Orlary"
            "\n  <http://faust.grame.fr/>"
            "\n  A large part is based on the work of Julius Orion Smith"
            "\n <http://ccrma.stanford.edu/realsimple/faust/>"
            "\n  and Albert Graef\n  <http://www.musikwissenschaft.uni-mainz.de/~ag/ag.html>  "
            "\n\n\n  guitarix ";

          about += GX_VERSION;

          about +=
            " use jack_capture >= 0.9.30for record"
            "\n  by Kjetil S. Matheussen "
            "\n  http://old.notam02.no/arkiv/src/?M=D"
            "\n  it will allways record to ~/guitarix_sessionX.xxx "
            "\n  for impulse response it use jconv "
            "\n  byFons Adriaensen "
            "\n  http://www.kokkinizita.net/linuxaudio/index.html "
            "\n\n  authors: Hermann Meyer <brummer-@web.de>"
            "\n  authors: James Warden <warjamy@yahoo.com>"
            "\n  home: http://guitarix.sourceforge.net/\n";
        }

      gx_message_popup(about.c_str());
    }


    //----- change the jack buffersize on the fly is still experimental, give a warning
    gint gx_wait_latency_warn()
    {
      GtkWidget* warn_dialog = gtk_dialog_new();
      gtk_window_set_destroy_with_parent(GTK_WINDOW(warn_dialog), TRUE);

      GtkWidget* box     = gtk_vbox_new (0, 4);
      GtkWidget* labelt  = gtk_label_new("\nWARNING\n");
      GtkWidget* labelt1 = gtk_label_new("CHANGING THE JACK_BUFFER_SIZE ON THE FLY \n"
                                         "MAY CAUSE UNPREDICTABLE EFFECTS \n"
                                         "TO OTHER RUNNING JACK APPLICATIONS. \n"
                                         "DO YOU WANT TO PROCEED ?");
      GdkColor colorGreen;
      gdk_color_parse("#969292", &colorGreen);
      gtk_widget_modify_fg (labelt1, GTK_STATE_NORMAL, &colorGreen);

      GtkStyle *style1 = gtk_widget_get_style(labelt1);
      pango_font_description_set_size(style1->font_desc, 10*PANGO_SCALE);
      pango_font_description_set_weight(style1->font_desc, PANGO_WEIGHT_BOLD);
      gtk_widget_modify_font(labelt1, style1->font_desc);

      gdk_color_parse("#ffffff", &colorGreen);
      gtk_widget_modify_fg (labelt, GTK_STATE_NORMAL, &colorGreen);
      style1 = gtk_widget_get_style(labelt);
      pango_font_description_set_size(style1->font_desc, 14*PANGO_SCALE);
      pango_font_description_set_weight(style1->font_desc, PANGO_WEIGHT_BOLD);
      gtk_widget_modify_font(labelt, style1->font_desc);

      GtkWidget* button1 =
        gtk_dialog_add_button(GTK_DIALOG (warn_dialog),
                              "Yes", gx_jack::kChangeLatency);

      GtkWidget* button2 =
        gtk_dialog_add_button(GTK_DIALOG (warn_dialog),
                              "No",  gx_jack::kKeepLatency);


      GtkWidget* box1    = gtk_hbox_new (0, 4);
      GtkWidget* box2    = gtk_hbox_new (0, 4);

      GtkWidget* disable_warn = gtk_check_button_new();
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(disable_warn), FALSE);
      g_signal_connect(disable_warn, "clicked",
                       G_CALLBACK(gx_user_disable_latency_warn), NULL);

      GtkWidget * labelt2 =
        gtk_label_new ("Don't bother me again with such a question, "
                       "I know what I am doing");

      gtk_container_add (GTK_CONTAINER(box),  labelt);
      gtk_container_add (GTK_CONTAINER(box),  labelt1);
      gtk_container_add (GTK_CONTAINER(box),  box2);
      gtk_container_add (GTK_CONTAINER(box),  box1);
      gtk_container_add (GTK_CONTAINER(box1), disable_warn);
      gtk_container_add (GTK_CONTAINER(box1), labelt2);
      gtk_container_add (GTK_CONTAINER(GTK_DIALOG(warn_dialog)->vbox), box);

      gtk_widget_modify_fg (labelt2, GTK_STATE_NORMAL, &colorGreen);

      GtkStyle *style = gtk_widget_get_style(labelt2);
      pango_font_description_set_size(style->font_desc, 8*PANGO_SCALE);
      pango_font_description_set_weight(style->font_desc, PANGO_WEIGHT_NORMAL);
      gtk_widget_modify_font(labelt2, style->font_desc);

      g_signal_connect_swapped(button1, "clicked",
                               G_CALLBACK (gtk_widget_destroy), warn_dialog);
      g_signal_connect_swapped(button2, "clicked",
                               G_CALLBACK (gtk_widget_destroy), warn_dialog);

      gtk_widget_show_all(box);

      return gtk_dialog_run (GTK_DIALOG (warn_dialog));
    }

    // check user's decision to turn off latency change warning
    void gx_user_disable_latency_warn(GtkWidget* wd, gpointer arg)
    {
      GtkToggleButton* button = GTK_TOGGLE_BUTTON(wd);
      gx_engine::fwarn_swap = (int)gtk_toggle_button_get_active(button);
    }

    void gx_reset_effects( GtkWidget *widget, gpointer data )
    {
      string filename = gx_user_dir + guitarix_reset;
      GxMainInterface* interface = GxMainInterface::instance();
      interface->recalladState(filename.c_str(),  121,  130, 7);
    }

    // reset the extended sliders to default settings
    void gx_reset_units( GtkWidget *widget, gpointer data )
    {
      const char* witchres = gtk_window_get_title(GTK_WINDOW(data));

      string filename = gx_user_dir + guitarix_reset;
      GxMainInterface* interface = GxMainInterface::instance();

      // Note (thorgal): to be revisited, too fragile design
      // Introduce XML based config

      if (strcmp(witchres, "distortion") == 0)
        interface->recalladState(filename.c_str(),  4,  16, 0);

      else if (strcmp(witchres, "freeverb") == 0)
        interface->recalladState(filename.c_str(),  20,  24, 1);

      else if (strcmp(witchres, "ImpulseResponse") == 0)
        interface->recalladState(filename.c_str(),  28,  32, 2);

      else if (strcmp(witchres, "crybaby") == 0)
        interface->recalladState(filename.c_str(),  16,  20, 3);

      else if (strcmp(witchres, "midi out") == 0)
        interface->recalladState(filename.c_str(),  44,  50, 4);

      else if (strcmp(witchres, "compressor") == 0)
        interface->recalladState(filename.c_str(),  72,  78, 5);

      else if (strcmp(witchres, "chorus") == 0)
        interface->recalladState(filename.c_str(),  110,  115, 6);

      else if (strcmp(witchres, "jconv") == 0)
        interface->recalladState(filename.c_str(),  116,  121, 8);
    }

    //----- show extendend settings slider
    void gx_show_extended_settings(GtkWidget *widget, gpointer data)
    {
      if (gtk_range_get_value(GTK_RANGE(widget)))
        {
          gtk_widget_show(GTK_WIDGET(data));
          gint root_x, root_y;
          gtk_window_get_position (GTK_WINDOW(data), &root_x, &root_y);
          if (root_y>160)root_y -= 120;
          else root_y +=120;
          gtk_window_move(GTK_WINDOW(data), root_x, root_y);
        }

      else gtk_widget_hide(GTK_WIDGET(data));
    }

    //----- hide the extendend settings slider
    void gx_hide_extended_settings( GtkWidget *widget, gpointer data )
    {
      static bool showit = false;

      if (showit == false)
        {
          gtk_widget_hide(GTK_WIDGET(fWindow));
          showit = true;
        }
      else
        {
          gtk_widget_show(GTK_WIDGET(fWindow));
          showit = false;
        }
    }

    //----- systray menu
    void gx_systray_menu( GtkWidget *widget, gpointer data )
    {
      guint32 tim = gtk_get_current_event_time ();
      gtk_menu_popup (GTK_MENU(menuh),NULL,NULL,NULL,(gpointer) menuh,2,tim);
    }

    //---- choice dialog without text entry
    gint gx_nchoice_dialog_without_entry (
      const char* window_title,
      const char* msg,
      const guint nchoice,
      const char* label[],
      const gint  resp[],
      const gint default_response
    )
    {
      GtkWidget* dialog   = gtk_dialog_new();
      GtkWidget* text_label = gtk_label_new (msg);
      gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox), text_label);

      GdkColor colorGreen;
      gdk_color_parse("#969292", &colorGreen);
      gtk_widget_modify_fg (text_label, GTK_STATE_NORMAL, &colorGreen);

      GdkColor colorBlack;
      gdk_color_parse("#000000", &colorBlack);
      gtk_widget_modify_bg (dialog, GTK_STATE_NORMAL, &colorBlack);

      GtkStyle* text_style = gtk_widget_get_style(text_label);
      pango_font_description_set_size(text_style->font_desc, 10*PANGO_SCALE);
      pango_font_description_set_weight(text_style->font_desc, PANGO_WEIGHT_BOLD);

      gtk_widget_modify_font(text_label, text_style->font_desc);

      for (guint i = 0; i < nchoice; i++)
        {
          GtkWidget* button =
            gtk_dialog_add_button(GTK_DIALOG (dialog), label[i], resp[i]);

          gdk_color_parse("#555555", &colorBlack);
          gtk_widget_modify_bg(button, GTK_STATE_NORMAL, &colorBlack);

          g_signal_connect_swapped(button, "clicked",  G_CALLBACK (gtk_widget_destroy), dialog);
        }

      // set default
      gtk_dialog_set_has_separator(GTK_DIALOG(dialog), TRUE);
      gtk_dialog_set_default_response(GTK_DIALOG(dialog), default_response);
      gtk_window_set_title(GTK_WINDOW(dialog), window_title);

      gtk_widget_show(text_label);

      //--- run dialog and check response
      gint response = gtk_dialog_run (GTK_DIALOG (dialog));
      return response;
    }

    //---- choice dialog without text entry
    gint gx_choice_dialog_without_entry (
      const char* window_title,
      const char* msg,
      const char* label1,
      const char* label2,
      const gint resp1,
      const gint resp2,
      const gint default_response
    )
    {
      const guint nchoice     = 2;
      const char* labels[]    = {label1, label2};
      const gint  responses[] = {resp1, resp2};

      return gx_nchoice_dialog_without_entry(
               window_title,
               msg,
               nchoice,
               labels,
               responses,
               default_response);
    }

    //---- get text entry from dialog
    void gx_get_text_entry(GtkEntry* entry, string& output)
    {
      if (gtk_entry_get_text(entry)[0])
        output = gtk_entry_get_text(entry);
    }

    //---- choice dialog with text entry
    gint gx_choice_dialog_with_text_entry (
      const char* window_title,
      const char* msg,
      const char* label1,
      const char* label2,
      const gint resp1,
      const gint resp2,
      const gint default_response,
      GCallback func
    )
    {
      GtkWidget *dialog, *button1, *button2;
      dialog  = gtk_dialog_new();
      button1 = gtk_dialog_add_button(GTK_DIALOG (dialog), label1, resp1);
      button2 = gtk_dialog_add_button(GTK_DIALOG (dialog), label2, resp2);

      GtkWidget* text_label = gtk_label_new (msg);
      gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox), text_label);


      GtkWidget* gtk_entry = gtk_entry_new_with_max_length(32);
      gtk_entry_set_text(GTK_ENTRY(gtk_entry), "");
      gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox), gtk_entry);

      g_signal_connect_swapped (button1, "clicked",  G_CALLBACK (func), gtk_entry);

      gtk_dialog_set_has_separator(GTK_DIALOG(dialog), TRUE);
      gtk_dialog_set_default_response(GTK_DIALOG(dialog), default_response);
      gtk_entry_set_activates_default(GTK_ENTRY(gtk_entry), TRUE);
      GTK_BOX(GTK_DIALOG(dialog)->action_area)->spacing = 4;

      // some display style
      GdkColor colorGreen;
      gdk_color_parse("#969292", &colorGreen);
      gtk_widget_modify_fg (text_label, GTK_STATE_NORMAL, &colorGreen);

      GdkColor colorBlack;
      gdk_color_parse("#000000", &colorBlack);
      gtk_widget_modify_bg (dialog, GTK_STATE_NORMAL, &colorBlack);

      GtkStyle* text_style = gtk_widget_get_style(text_label);
      pango_font_description_set_size(text_style->font_desc, 10*PANGO_SCALE);
      pango_font_description_set_weight(text_style->font_desc, PANGO_WEIGHT_BOLD);

      gtk_widget_modify_font(text_label, text_style->font_desc);

      gdk_color_parse("#555555", &colorBlack);
      gtk_widget_modify_bg (button1, GTK_STATE_NORMAL, &colorBlack);

      gdk_color_parse("#555555", &colorBlack);
      gtk_widget_modify_bg (button2, GTK_STATE_NORMAL, &colorBlack);

      // display extra stuff
      gtk_widget_show (text_label);
      gtk_widget_show (gtk_entry);
      gtk_window_set_title(GTK_WINDOW(dialog), window_title);

      // run the dialog and wait for response
      gint response = gtk_dialog_run (GTK_DIALOG(dialog));

      if (dialog) gtk_widget_destroy(dialog);

      return response;
    }

    //---- retrive skin array index from skin name
    void gx_actualize_skin_index(const string& skin_name)
    {
      for (guint s = 0; s < skin_list.size(); s++)
        if (skin_name == skin_list[s])
          {
            gx_current_skin = s;
            return;
          }
    }

    //------- count the number of available skins
    unsigned int gx_fetch_available_skins()
    {
      string tmpfile = gx_user_dir + ".n_skins";

      // make sure user dir exists
      gx_system_call("mkdir -p", gx_user_dir);

      // create a tmpfile
      gx_system_call("rm -f", tmpfile.c_str());
      gx_system_call("touch", tmpfile.c_str());

      string filelist  =
        gx_style_dir + string("guitarix*.rc") + " > " + tmpfile;

      gx_system_call("ls -1", filelist.c_str(), false);

      // read out number of files
      ifstream f(tmpfile.c_str());

      string rcfile;
      if (f.good())
        {
          while (!f.eof())
            {
              // retrieve filename
              getline(f, rcfile);

              // trim it
              if (!rcfile.empty())
                {
                  rcfile.erase(rcfile.find(".rc"));
                  rcfile.erase(0, rcfile.find("_")+1);
                  skin_list.push_back(rcfile);
                }
            }

          f.close();
        }

      // remove tmp file
      gx_system_call("rm -f", tmpfile.c_str());

      return skin_list.size();
    }

    // ----- skin change
    void  gx_change_skin(GtkCheckMenuItem *menuitem, gpointer arg)
    {
      // no action needed on false
      if (gtk_check_menu_item_get_active(menuitem) == FALSE)
        return;

      // update the skin to the one picked by user
      const int idx = (int)GPOINTER_TO_INT(arg);

      (void)gx_update_skin(idx, "gx_change_skin");
    }

    // ----- cycling through skin
    void  gx_cycle_through_skin(GtkWidget *widget, gpointer arg)
    {

      gint idx = gx_current_skin + 1;
      idx %= skin_list.size();

      // did it work ? if yes, update current skin
      if (gx_update_skin(idx, "gx_cycle_through_skin"))
        gx_current_skin = idx;

      // update menu item state
      gx_update_skin_menu_item(gx_current_skin);
    }

    // ----- cycling through skin
    void  gx_update_skin_menu_item(const int index)
    {
      // update menu item state
      GxMainInterface* gui = GxMainInterface::instance();
      GtkWidget* skinmenu = gui->getMenu("Skin");

      GList*     list = gtk_container_get_children(GTK_CONTAINER(skinmenu));
      GtkWidget* item = (GtkWidget*)g_list_nth_data(list, index);
      g_list_free(list);
      if (item) gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item), TRUE);
    }

    // ---- skin changer, used internally frm callbacks
    bool gx_update_skin(const gint idx, const char* calling_func)
    {
      // check skin validity
      if (idx < 0 || idx >= (gint)skin_list.size())
        {
          gx_print_warning(calling_func, "skin index out of range, keeping actual skin");
          return false;
        }

      string rcfile = GX_STYLE_DIR + string("/") + "guitarix_";
      rcfile += skin_list[idx];
      rcfile += ".rc";

      gtk_rc_parse(rcfile.c_str());
      gtk_rc_reset_styles(gtk_settings_get_default());

      gx_current_skin = idx;

      // refresh wave view
      gx_waveview_refresh (GTK_WIDGET(livewa), NULL);
      if (int(float(gx_current_skin)==0))
        {
          if (set_knob !=1)
            {
              GtkRegler::gtk_regler_init_pixmaps(1);
              set_knob = 1;
            }
        }
      else if (int(float(gx_current_skin)==1))
        {
          if (set_knob !=2)
            {
              GtkRegler::gtk_regler_init_pixmaps(2);
              set_knob = 2;
            }
        }

      else if (int(float(gx_current_skin)==3))
        {
          if (set_knob !=3)
            {
              GtkRegler::gtk_regler_init_pixmaps(3);
              set_knob = 3;
            }
        }
      else if (int(float(gx_current_skin)==5))
        {
          if (set_knob !=4)
            {
              GtkRegler::gtk_regler_init_pixmaps(4);
              set_knob = 4;
            }
        }
      else if (int(float(gx_current_skin)==4))
        {
          if (set_knob !=5)
            {
              GtkRegler::gtk_regler_init_pixmaps(5);
              set_knob = 5;
            }
        }
      else
        {
          GtkRegler::gtk_regler_init_pixmaps(0);
          set_knob = 0;
        }

      // refresh latency check menu
      GxMainInterface* gui = GxMainInterface::instance();
      GtkWidget* wd = gui->getJackLatencyItem(gx_jack::jack_bs);
      if (wd) gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(wd), TRUE);

      return true;
    }

    // ---- set last used skin as default
    bool gx_set_skin(GtkWidget *widget, gpointer data)
    {
      return gx_update_skin(last_skin, "Set Skin");
    }

    //---- popup warning
    int gx_message_popup(const char* msg)
    {
      // check msg validity
      if (!msg)
        {
          gx_print_warning("Message Popup",
                           string("warning message does not exist"));
          return -1;
        }

      // build popup window
      GtkWidget *about;
      GtkWidget *label;
      GtkWidget *ok_button;

      about = gtk_dialog_new();
      ok_button  = gtk_button_new_from_stock(GTK_STOCK_OK);

      label = gtk_label_new (msg);

      GtkStyle *style = gtk_widget_get_style(label);

      pango_font_description_set_size(style->font_desc, 10*PANGO_SCALE);
      pango_font_description_set_weight(style->font_desc, PANGO_WEIGHT_BOLD);

      gtk_widget_modify_font(label, style->font_desc);

      gtk_label_set_selectable(GTK_LABEL(label), TRUE);

      gtk_container_add (GTK_CONTAINER (GTK_DIALOG(about)->vbox), label);

      GTK_BOX(GTK_DIALOG(about)->action_area)->spacing = 3;
      gtk_container_add (GTK_CONTAINER (GTK_DIALOG(about)->action_area), ok_button);

      g_signal_connect_swapped (ok_button, "clicked",
                                G_CALLBACK (gtk_widget_destroy), about);

      g_signal_connect(label, "expose-event", G_CALLBACK(box1_expose), NULL);
      gtk_widget_show (ok_button);
      gtk_widget_show (label);
      return gtk_dialog_run (GTK_DIALOG(about));
    }

    /* show jack portmap window  */
    void gx_show_portmap_window (GtkWidget* widget, gpointer arg)
    {
      GxMainInterface* gui = GxMainInterface::instance();
      if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget)) == TRUE)
        {
          if (!gui->fClientPortMap.empty())
            {
              // get main window position (north east gravity)
              gint x,  y;
              gint wx, wy;
              gtk_window_get_position(GTK_WINDOW(fWindow), &x, &y);
              gtk_window_get_size(GTK_WINDOW(fWindow), &wx, &wy);

              // set position of port map window (north west gravity)
              gtk_window_move(gui->getPortMapWindow(), x+wx, y);
              gtk_widget_set_size_request(GTK_WIDGET(gui->getPortMapWindow()),
                                          400, 200);

              gtk_widget_show(GTK_WIDGET(gui->getPortMapWindow()));
            }
          else
            {
              gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget), FALSE);
              gx_print_warning("Show Port Maps",
                               "No Clients to show, jackd either down or we are disconnected");
            }
        }
      else
        {
          gtk_widget_hide(GTK_WIDGET(gui->getPortMapWindow()));
          gtk_widget_grab_focus(fWindow);
        }
    }

    /* show jack portmap window  */
    void gx_hide_portmap_window (GtkWidget* widget, gpointer arg)
    {
      gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget), FALSE);
    }

    /* meter button release   */
    void gx_meter_button_release(GdkEventButton* ev, gpointer arg)
    {
      if (ev->button == 1)
        {
          cerr << " button event " << endl;
          GxMainInterface* gui = GxMainInterface::instance();

          GtkWidget* const*  meters = gui->getLevelMeters();
          GtkWidget* const* jmeters = gui->getJCLevelMeters();

          for (int i = 0; i < 2; i++)
            {
              if (meters[i])
                gtk_fast_meter_clear(GTK_FAST_METER(meters[i]));
              if (jmeters[i])
                gtk_fast_meter_clear(GTK_FAST_METER(jmeters[i]));
            }
        }
    }

    /* Update all user items reflecting zone z */
    gboolean gx_update_all_gui(gpointer)
    {
      gx_ui::GxUI::updateAllGuis();
      return TRUE;
    }


    /* ----- delete event ---- */
    gboolean gx_delete_event( GtkWidget *widget, gpointer   data )
    {
      gtk_range_set_value(GTK_RANGE(widget), 0);
      return TRUE;
    }

    gboolean gx_hide_eq( GtkWidget *widget, gpointer   obj )
    {
      show_eq = (int) GTK_ADJUSTMENT (widget)->value;
      GtkWidget *wi = (GtkWidget *)obj;
      GtkWidget *box1 = gtk_widget_get_parent(GTK_WIDGET(wi));
      GtkWidget *box = gtk_widget_get_parent(GTK_WIDGET(box1));
      GList*   child_list =  gtk_container_get_children(GTK_CONTAINER(box));
      GtkWidget *parent_eq = (GtkWidget *) g_list_nth_data(child_list,1);
      g_list_free(child_list);
      box1 = gtk_widget_get_parent(GTK_WIDGET(livewa));


      // gtk_widget_set_size_request (parent_eq, 280,80);
      if (show_eq)
        {
          gtk_widget_show(parent_eq);
          //gtk_widget_set_size_request (box, -1, -1);
          if (GDK_IS_WINDOW (box1->window))
            gdk_window_invalidate_rect(GDK_WINDOW(box1->window),NULL,TRUE);
        }
      else
        {
          gtk_widget_hide(parent_eq);
          //gtk_widget_set_size_request (box, -1,-1);
          if (GDK_IS_WINDOW (box1->window))
            gdk_window_invalidate_rect(GDK_WINDOW(box1->window),NULL,TRUE);
        }

      return false;
    }


  } /* end of gx_gui namespace */
