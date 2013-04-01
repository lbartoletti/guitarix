/*
 * Copyright (C) 2012 Hermann Meyer, Andreas Degert, Pete Shorthose
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
 * --------------------------------------------------------------------------
 */


////////////////////////////// LOCAL INCLUDES //////////////////////////

#include "gx_common.h"      // faust support and denormal protection (SSE)
#include "gx_tremolo.h"        // define struct PortIndex
#include "gx_pluginlv2.h"   // define struct PluginLV2
#include "tremolo.cc"    // dsp class generated by faust -> dsp2cc

////////////////////////////// PLUG-IN CLASS ///////////////////////////

class Gx_tremolo
{
private:
  // internal stuff
  float*                       output;
  float*                       input;
  PluginLV2*                   tremolo_st;
public:

  inline void run_dsp_mono(uint32_t n_samples);
  inline void connect_mono(uint32_t port,void* data);
  inline void init_dsp_mono(uint32_t rate);
  inline void connect_all_mono_ports(uint32_t port, void* data);
  inline void activate_f();
  inline void clean_up();
  Gx_tremolo();
  ~Gx_tremolo();
};

// constructor
Gx_tremolo::Gx_tremolo() :
  output(NULL),
  input(NULL),
  tremolo_st(tremolo::plugin()) {};

// destructor
Gx_tremolo::~Gx_tremolo()
{
  // just to be sure the plug have given free the allocated mem
  // it didn't hurd if the mem is already given free by clean_up()
  //tremolo_st->activate_plugin(false, tremolo_st);
  // delete DSP class
  tremolo_st->delete_instance(tremolo_st);
};

////////////////////////////// PLUG-IN CLASS  FUNCTIONS ////////////////

void Gx_tremolo::init_dsp_mono(uint32_t rate)
{
  AVOIDDENORMALS(); // init the SSE denormal protection
  tremolo_st->set_samplerate(rate, tremolo_st); // init the DSP class
}

// connect the Ports used by the plug-in class
void Gx_tremolo::connect_mono(uint32_t port,void* data)
{
  switch ((PortIndex)port)
    {
    case EFFECTS_OUTPUT:
      output = static_cast<float*>(data);
      break;
    case EFFECTS_INPUT:
      input = static_cast<float*>(data);
      break;
    default:
      break;
    }
}

void Gx_tremolo::activate_f()
{
  // allocate the internal DSP mem
  //tremolo_st->activate_plugin(true, tremolo_st);
}

void Gx_tremolo::clean_up()
{
  // delete the internal DSP mem
  //tremolo_st->activate_plugin(false, tremolo_st);
}

void Gx_tremolo::run_dsp_mono(uint32_t n_samples)
{
  tremolo_st->mono_audio(static_cast<int>(n_samples), input,
                        output, tremolo_st);
}

void Gx_tremolo::connect_all_mono_ports(uint32_t port, void* data)
{
  // connect the Ports used by the plug-in class
  connect_mono(port,data); 
  // connect the Ports used by the DSP class
  tremolo_st->connect_ports(port,  data, tremolo_st);
}

///////////////////////////// LV2 defines //////////////////////////////

static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
            double                    rate,
            const char*               bundle_path,
            const LV2_Feature* const* features)
{
  // init the plug-in class
  Gx_tremolo *self = new Gx_tremolo();
  if (!self)
    {
      return NULL;
    }

  self->init_dsp_mono((uint32_t)rate);

  return (LV2_Handle)self;
}

static void
connect_port(LV2_Handle instance,
             uint32_t   port,
             void*      data)
{
  // connect all ports
  static_cast<Gx_tremolo*>(instance)->connect_all_mono_ports(port, data);
}

static void
activate(LV2_Handle instance)
{
  // allocate needed mem
  static_cast<Gx_tremolo*>(instance)->activate_f();
}

static void
run(LV2_Handle instance, uint32_t n_samples)
{
  // run dsp
  static_cast<Gx_tremolo*>(instance)->run_dsp_mono(n_samples);
}

static void
cleanup(LV2_Handle instance)
{
  // well, clean up after us
  Gx_tremolo* self = static_cast<Gx_tremolo*>(instance);
  self->clean_up();
  delete self;
}

///////////////////////////// LV2 DESCRIPTOR ///////////////////////////

static const LV2_Descriptor descriptor =
{
  GXPLUGIN_URI "#_tremolo",
  instantiate,
  connect_port,
  activate,
  run,
  NULL,
  cleanup,
  NULL
};

extern "C"
LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
  switch (index)
    {
    case 0:
      return &descriptor;
    default:
      return NULL;
    }
}

///////////////////////////// FIN //////////////////////////////////////
