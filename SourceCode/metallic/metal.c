/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <https://www.gnu.org/licenses/>.
 *
 * Credit to Øyvind Kolås (pippin) for major GEGL contributions
 * 2022 Beaver (GEGL metallic)
 */

/*
June 25 2023- Recreation of Metallic's GEGL Graph.

alien-map cpn-1-frequency=6 cpn-2-frequency=5.9 cpn-3-frequency=3
hue-chroma lightness=-5
noise-reduction iterations=3
saturation scale=0

September 11 2023 gold metal update, (apply above graph to make gold metal)

gimp:layer-mode layer-mode=hsl-color opacity=0.8 composite-mode=auto  aux=[ color value=#ffdc00  ]
id=darken
gimp:layer-mode layer-mode=addition opacity=0.3 aux=[ ref=darken ]

--end of syntax and list of alternative blend modes for "addition" --
#addition graimerge linearburn multiply

Also, Syntax7 is a unique GEGL Graph that makes an amazing even better gold effect.

October 2 2023, This filter got a update that gives it new metal effects. This is an example of me updating a plugin without breaking presets by adding all the new
options in a drop down list. I, (Beaver) am doing things with GEGL that gimp's team or anyone has ever done. It is so fun to pioneer this software.


Below is a list of all the GEGL Graph strings being called by this filter. they are NOT all called together and some do radically different things.
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

#define syntax1 \
" id=darken gimp:layer-mode layer-mode=addition opacity=0.3 aux=[ ref=darken ]  "\

#define syntax2 \
" id=darken gimp:layer-mode layer-mode=grain-merge opacity=0.2 aux=[ ref=darken ] "\

#define syntax3 \
" id=darken gimp:layer-mode layer-mode=linear-burn opacity=0.08 aux=[ ref=darken ] "\

#define syntax4 \
" id=darken gimp:layer-mode layer-mode=multiply opacity=0.3 aux=[ ref=darken ] "\

#define syntax5 \
" mean-curvature-blur iterations=2 median-blur radius=0  abyss-policy=none alien-map cpn-1-frequency=6 cpn-2-frequency=5.9 cpn-3-frequency=3 hue-chroma lightness=-5 noise-reduction iterations=3 saturation scale=0 gimp:layer-mode layer-mode=hsl-color opacity=0.8 composite-mode=auto  aux=[ color value=#ffdc00  ] id=darken gimp:layer-mode layer-mode=addition opacity=0.3 aux=[ ref=darken ]  levels out-low=0.024 median-blur  abyss-policy=none radius=0 "\

#define syntax6 \
" bilateral-filter blur-radius=4 edge-preservation=9  bilateral-filter blur-radius=1 edge-preservation=3 mean-curvature-blur iterations=1   "\

#define syntax7 \
" id=dv gimp:layer-mode layer-mode=normal opacity=0 aux=[ ref=dv  emboss depth=3 elevation=25  id=divcall ] unsharp-mask scale=0.3 mean-curvature-blur iterations=1 id=1 gimp:layer-mode layer-mode=hsl-color aux=[ ref=1 color-overlay value=#ffbb00 ]  alien-map cpn-1-frequency=6 cpn-2-frequency=6 cpn-3-frequency=1 saturation scale=0 bloom strength=95 levels in-high=1.87  in-low=0.02 noise-reduction iterations=5 id=color  gimp:layer-mode layer-mode=hsl-color opacity=0.84 composite-mode=clip-to-backdrop aux=[ color value=#f4cd62] unsharp-mask scale=0.9 median-blur  abyss-policy=none radius=0  gimp:layer-mode layer-mode=divide opacity=0.03 aux=[ ref=divcall gaussian-blur  clip-extent=false  abyss-policy=none std-dev-x=0.5 std-dev-y=0.7 noise-reduction ] "\

#define syntax8 \
" gimp:desaturate mode=value invert value-invert "\



property_enum(guichange, _("Policy"),
    guiendmetalcolor, guichangemetalcolor,
    METAL_NO_COLOR)
  description(_("Enabling this will allow the metal to be gold or any color."))

enum_start (guichangemetalcolor)
enum_value   (METAL_NO_COLOR, "metal", N_("Metal Without Color Overlay"))
enum_value   (METAL_COLOR, "colormetal", N_("Metal With Color Overlay"))
enum_value   (FIXED_GOLD, "goldfixed", N_("Uneditable Gold Graph"))
enum_value   (FIXED_GOLD2, "goldfixed2", N_("Uneditable Gold Graph 2"))
enum_value   (OCT_2023_METAL, "oct2023metal", N_("Oct 2023 Metal"))
enum_value   (OCT_2023_METAL_COLOR, "oct2023metalcolor", N_("Oct 2023 Metal (with color)"))
  enum_end (guiendmetalcolor)

property_double (liquid, _("Liquidify Metal"), 0.0)
    description (_("In default this is disabled, but when enabled it will smoothly expand the metal and make it some what 'liquid-like'. "))
  value_range (0.0, 8)
  ui_steps      (0.5, 1)


property_double (solar1, _("Solarization of Red Channel"), 2.7)
    description (_("Solarization is another name for Gimp's 'alien map' filter effect, which at low values shifts tones. A method which can create a metallic effect when combined with desaturation. "))
  value_range (0, 6)
  ui_steps      (0.100, 1.00)
                 ui_meta ("visible", "guichange {metal, colormetal}")


property_double  (solar2, _("Solarization of Green Channel"), 2.8)
    description (_("Solarization is another name for Gimp's 'alien map' filter effect, which at low values shifts tones. A method which can create a metallic effect when combined with desaturation. "))
  value_range (2.2, 6)
  ui_steps      (0.100, 1.00)
                 ui_meta ("visible", "guichange {metal, colormetal}")

property_double  (solar3, _("Solarization of Blue Channel"), 2.1)
    description (_("Solarization is another name for Gimp's 'alien map' filter effect, which at low values shifts tones. A method which can create a metallic effect when combined with desaturation. "))
  value_range (0, 6)
  ui_steps      (0.100, 1.00)
                 ui_meta ("visible", "guichange {metal, colormetal}")


property_double (light, _("Lightness"), 0.0)
   description  (_("Lightness adjustment"))
   value_range  (-10.0, 10.0)
  ui_steps      (0.200, 1.00)
                 ui_meta ("visible", "guichange {metal, colormetal}")

property_int  (smooth, _("Smoothness"), 2)
  description (_("Smoothness"))
  value_range (0, 8)
  ui_range    (0, 8)
                 ui_meta ("visible", "guichange {metal, colormetal}")

property_double (value, _("Slide to invert colors on original image"), 0)
    description (_("Invert with Slider. This invert is a transition process from 0-100 percentage, and that is a good thing as sometimes in between lies the best results for metal subjects and objects."))
    value_range (0, 1)
  ui_steps      (1.00, 1.00)
                 ui_meta ("visible", "guichange {metal, colormetal}")



property_double (altsolar1, _("Alt Solar Red Channel"), 276)
   description  (_("Alien Map's red channel phase shift"))
   value_range  (240, 290)
                 ui_meta ("visible", "guichange {oct2023metal, oct2023metalcolor}")

property_double (altsolar2, _("Alt Solar Green Channel"), 145)
   description  (_("Alien Map's green channel phase shift"))
   value_range  (120, 180)
                 ui_meta ("visible", "guichange {oct2023metal, oct2023metalcolor}")

property_double (altsolar3, _("Alt Solar Blue Channel"), 13)
   description  (_("Alien Map's blue channel phase shift"))
   value_range  (8, 19)
                 ui_meta ("visible", "guichange {oct2023metal, oct2023metalcolor}")

property_int (altsmooth, _("Smooth alt metal"), 5)
   description  (_("Smooth the alt metal with dct denoise."))
   value_range  (1, 45)
                 ui_meta ("visible", "guichange {oct2023metal, oct2023metalcolor}")
    ui_meta    ("role", "output-extent")


property_color (color, _("Color Overlay of Metal"), "#fcf9eb")
                 ui_meta ("visible", "guichange {colormetal}")

property_color (color2, _("Color Overlay of Metal"), "#f3d758")
                 ui_meta ("visible", "guichange {oct2023metalcolor}")


property_double (coloropacity, _("Opacity of Color"), 1.00)
   description  (_("Opacity of Color"))
   value_range  (0.00, 1.00)
                 ui_meta ("visible", "guichange {colormetal, oct2023metalcolor}")



property_enum (blend, _("Blend Mode to modify colored metal"),
    metallicclownworld, metallic_clownworld,
    GRAPH_BLEND_MODE_TYPE_MULTIPLY)
    description (_("Blend Mode to modify colored metal. This fuses the colored metal layer with itself on a blend mode. The usually darkens the colored metal. "))
                 ui_meta ("visible", "guichange {colormetal}")


property_double (hue, _("Hue Rotation (0 resets)"),  0.0)
   description  (_("Hue adjustment for graphs with no other editable properties"))
   value_range  (-180.0, 180.0)
                 ui_meta ("visible", "guichange {goldfixed, goldfixed2}")






enum_start (metallic_clownworld)
  enum_value (GRAPH_BLEND_MODE_TYPE_NONE, "none",
              N_("None"))
  enum_value (GRAPH_BLEND_MODE_TYPE_ADDITION, "graph1",
              N_("Addition"))
  enum_value (GRAPH_BLEND_MODE_TYPE_GRAINMERGE,      "graph2",
              N_("Grain Merge"))
  enum_value (GRAPH_BLEND_MODE_TYPE_LINEARBURN,      "graph3",
              N_("LinearBurn"))
  enum_value (GRAPH_BLEND_MODE_TYPE_MULTIPLY,      "graph4",
              N_("Multiply"))
enum_end (metallicclownworld)
/* This is the first part of color overlay's blend mode switch. The ENUM list with all the blend modes needed by color overlay */


#else

#define GEGL_OP_META
#define GEGL_OP_NAME     metal
#define GEGL_OP_C_SOURCE metal.c

#include "gegl-op.h"

typedef struct
{
GeglNode *input;
GeglNode *output;
GeglNode *gaussian;
GeglNode *solar;
GeglNode *desat;
GeglNode *smooth;
GeglNode *light;
GeglNode *invert;
GeglNode *opacity;
GeglNode *hslcolor;
GeglNode *nop;
GeglNode *color;
GeglNode *color2;
GeglNode *over;
GeglNode *idref;
GeglNode *graph1;
GeglNode *graph2;
GeglNode *graph3;
GeglNode *graph4;
GeglNode *dctsmooth;
GeglNode *repair;
GeglNode *repair2;
GeglNode *smooth08;
GeglNode *none;
GeglNode *sl;
GeglNode *fixedgold;
GeglNode *fixedgold2;
GeglNode *huerotate;
GeglNode *solar2;
GeglNode *desatinverts;
GeglNode *dctsmooth2;
GeglNode *idref2;
} State;

static void attach (GeglOperation *operation)
{


}

static void update_graph (GeglOperation *operation)
{

  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);

  State *state = o->user_data = g_malloc0 (sizeof (State));

  if (!state) return;


  state->input    = gegl_node_get_input_proxy (gegl, "input");
  state->output   = gegl_node_get_output_proxy (gegl, "output");


  state->solar = gegl_node_new_child (gegl,
                                  "operation", "gegl:alien-map",
                                  NULL);

  state->solar2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:alien-map", "cpn-1-frequency", 1.0, "cpn-2-frequency", 1.0, "cpn-3-frequency", 1.0,
                                  NULL);


  state->desat = gegl_node_new_child (gegl,
                                  "operation", "gegl:saturation", "scale", 0.0,
                                  NULL);


  state->smooth = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-reduction",
                                  NULL);


   state->light = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);

   state->nop = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);


  state->opacity = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

   state->invert = gegl_node_new_child (gegl,
                                  "operation", "gegl:invert-gamma",
                                  NULL);

   state->over = gegl_node_new_child (gegl,
                                  "operation", "gegl:over",
                                  NULL);

state->hslcolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 39, "composite-mode", 0, "opacity", 0.90, NULL);

state->sl = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 45, "composite-mode", 0, "opacity", 1.00, "blend-space", 2, NULL);




   state->gaussian = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur", "clip-extent", FALSE,   "abyss-policy", 0,
                                  NULL);

   state->idref = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);


   state->none = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

   state->idref2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

   state->huerotate = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);


  state->color = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);


  state->color2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);





   state->dctsmooth2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:mean-curvature-blur", "iterations", 3,
                                  NULL);


  state->graph1 = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", syntax1,
                                  NULL);

  state->graph2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", syntax2,
                                  NULL);

  state->graph3 = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", syntax3,
                                  NULL);

  state->graph4 = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", syntax4,
                                  NULL);

  state->fixedgold = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", syntax5,
                                  NULL);

  state->fixedgold2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", syntax7,
                                  NULL);

  state->desatinverts = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", syntax8,
                                  NULL);

  state->smooth08 = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", syntax6,
                                  NULL);

   state->dctsmooth = gegl_node_new_child (gegl,
                                  "operation", "gegl:mean-curvature-blur", "iterations", 3,
                                  NULL);

   state->repair = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 0,  "abyss-policy",     GEGL_ABYSS_NONE,
                                  NULL);

   state->repair2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 0,  "abyss-policy",     GEGL_ABYSS_NONE,
                                  NULL);



  GeglNode *graphblendmode = state->graph4;
  switch (o->blend) {
    case GRAPH_BLEND_MODE_TYPE_NONE: graphblendmode = state->none; break;
    case GRAPH_BLEND_MODE_TYPE_ADDITION: graphblendmode = state->graph1; break;
    case GRAPH_BLEND_MODE_TYPE_GRAINMERGE: graphblendmode = state->graph2; break;
    case GRAPH_BLEND_MODE_TYPE_LINEARBURN: graphblendmode = state->graph3; break;
    case GRAPH_BLEND_MODE_TYPE_MULTIPLY: graphblendmode = state->graph4; break;

default: graphblendmode = state->graph4;

}
  if (o->value < 0.8)

switch (o->guichange) {
        break;
    case METAL_NO_COLOR:
  gegl_node_link_many (state->input, state->gaussian, state->repair2, state->over, state->solar, state->desat, state->smooth, state->light, state->output, NULL);
  gegl_node_connect (state->over, "aux", state->opacity, "output");
  gegl_node_link_many (state->input, state->invert, state->opacity, NULL);
        break;
    case METAL_COLOR:
  gegl_node_link_many (state->input, state->gaussian, state->repair2,  state->over, state->solar, state->desat, state->smooth, state->light, state->idref, state->hslcolor,  graphblendmode, state->repair, state->output, NULL);
  gegl_node_connect (state->over, "aux", state->opacity, "output");
  gegl_node_connect (state->hslcolor, "aux", state->color, "output");
  gegl_node_link_many (state->input, state->invert, state->opacity, NULL);
  gegl_node_link_many (state->idref,  state->color, NULL);
        break;
    case FIXED_GOLD:
    gegl_node_link_many (state->input, state->gaussian, state->fixedgold, state->huerotate, state->output, NULL);
        break;
    case FIXED_GOLD2:
    gegl_node_link_many (state->input, state->gaussian, state->fixedgold2, state->huerotate, state->output, NULL);
        break;
    case OCT_2023_METAL:
    gegl_node_link_many (state->input, state->gaussian, state->dctsmooth2, state->solar2, state->desatinverts, state->output, NULL);
        break;
    case OCT_2023_METAL_COLOR:
    gegl_node_link_many (state->input, state->gaussian, state->dctsmooth2, state->solar2, state->desatinverts,  state->idref, state->sl, state->output, NULL);
  gegl_node_connect (state->sl, "aux", state->color2, "output");
  gegl_node_link_many (state->idref,  state->color2,  NULL);
    }

else

switch (o->guichange) {
        break;
    case METAL_NO_COLOR:
  gegl_node_link_many (state->input, state->gaussian, state->repair2, state->over, state->solar, state->desat, state->smooth, state->light, state->smooth08, state->output, NULL);
  gegl_node_connect (state->over, "aux", state->opacity, "output");
  gegl_node_link_many (state->input, state->invert, state->opacity, NULL);
        break;
    case METAL_COLOR:
  gegl_node_link_many (state->input, state->gaussian, state->repair2, state->over, state->solar, state->desat, state->smooth, state->light, state->idref, state->hslcolor,  graphblendmode, state->smooth08, state->repair, state->output, NULL);
  gegl_node_connect (state->over, "aux", state->opacity, "output");
  gegl_node_connect (state->hslcolor, "aux", state->color, "output");
  gegl_node_link_many (state->input, state->invert, state->opacity, NULL);
  gegl_node_link_many (state->idref, state->color, NULL);
        break;
    case FIXED_GOLD:
    gegl_node_link_many (state->input, state->gaussian, state->fixedgold, state->huerotate, state->output, NULL);
        break;
    case FIXED_GOLD2:
    gegl_node_link_many (state->input, state->gaussian, state->fixedgold2, state->huerotate, state->output, NULL);
        break;
    case OCT_2023_METAL:
    gegl_node_link_many (state->input, state->gaussian, state->dctsmooth2, state->solar2, state->desatinverts, state->output, NULL);
        break;
    case OCT_2023_METAL_COLOR:
    gegl_node_link_many (state->input, state->gaussian, state->dctsmooth2, state->solar2, state->desatinverts, state->idref, state->sl, state->output, NULL);
  gegl_node_connect (state->sl, "aux", state->color2, "output");
  gegl_node_link_many (state->idref, state->color2,  NULL);
}


  gegl_operation_meta_redirect (operation, "solar1", state->solar, "cpn-1-frequency");
  gegl_operation_meta_redirect (operation, "solar2", state->solar, "cpn-2-frequency");
  gegl_operation_meta_redirect (operation, "solar3", state->solar, "cpn-3-frequency");
  gegl_operation_meta_redirect (operation, "altsolar1", state->solar2, "cpn-1-phaseshift");
  gegl_operation_meta_redirect (operation, "altsolar2", state->solar2, "cpn-2-phaseshift");
  gegl_operation_meta_redirect (operation, "altsolar3", state->solar2, "cpn-3-phaseshift");
  gegl_operation_meta_redirect (operation, "light", state->light, "lightness");
  gegl_operation_meta_redirect (operation, "smooth", state->smooth, "iterations");
/*  gegl_operation_meta_redirect (operation, "altsmooth", state->dctsmooth2, "sigma"); */
  gegl_operation_meta_redirect (operation, "value", state->opacity, "value");
  gegl_operation_meta_redirect (operation, "color", state->color, "value");
  gegl_operation_meta_redirect (operation, "color2", state->color2, "value");
  gegl_operation_meta_redirect (operation, "hue", state->huerotate, "hue");
  gegl_operation_meta_redirect (operation, "liquid", state->gaussian, "std-dev-x");
  gegl_operation_meta_redirect (operation, "liquid", state->gaussian, "std-dev-y");
  gegl_operation_meta_redirect (operation, "coloropacity", state->hslcolor, "opacity");
  gegl_operation_meta_redirect (operation, "coloropacity", state->sl, "opacity");

}



static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;
GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);
  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_meta_class->update = update_graph;
operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
/*If this filter ever breaks try changing the name gegl: or lb: to something else. This is because Gimp's team may want different name spaces.*/
    "name",        "lb:metallic",
    "title",       _("Metallic"),
    "reference-hash", "45ed5656h28ff20fjf25sb2ac",
    "description", _("Add a metal effect to subjects and objects."),
/*This is an instruction to put the filter in Gimp's menu. It only works in Gimp 2.99.16 and up. will not work in Gimp 2.10 as of August 28 2023*/
    "gimp:menu-path", "<Image>/Filters/Artistic",
    "gimp:menu-label", _("Metallic and Gold (for subjects and object)..."),
    NULL);
}

#endif
