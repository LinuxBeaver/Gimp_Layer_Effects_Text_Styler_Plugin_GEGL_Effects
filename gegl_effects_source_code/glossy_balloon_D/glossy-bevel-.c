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
 * Copyright 2006 Øyvind Kolås <pippin@gimp.org>
 * 2022 Beaver GEGL Glossy Balloon *2023 Beaver Glossy Balloon (branch with advance modifications, can co-exist with default plugin)
 */

/* This is a Glossy Balloon's GEGL Graph made in mid 2022 with some modifications.
 It is not 100% the same but the closest I can find in my archives. So this is how you can test this filter without installing it.

 
color-overlay value=#f587ff median-blur  percentile=90 alpha-percentile=100 
median-blur  percentile=90 alpha-percentile=290
gaussian-blur std-dev-x=4 std-dev-y=4
id=3
screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] 
median-blur  percentile=90 alpha-percentile=290
gaussian-blur std-dev-x=4 std-dev-y=4
id=3
screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] 
median-blur  percentile=90 alpha-percentile=290
screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] 
median-blur  percentile=90 alpha-percentile=290
screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] 
opacity value=10
median-blur radius=0


Fun Fact, besides GEGL Effects, Glossy Balloon has gone under more radical change then any other plugin of mine. I have spent countless hours rearranging Glossy Balloon's GEGL nodes. Changing its under
the hood stuff while still keeping it roughly the same from a user's perspective. At one point (early 2023) Glossy Balloon had a disable puff checkbox that worked by stacking layer copies on top of each other. 
It was removed mid 2023. During the debut of Glossy Balloon late may 2022) it required several mandatory dependencies when I knew little about GEGL plugin development. The GEGL Graphs were not embedded but 
rather seperate binary dependencies. In the summer of 2022 to early 2023 Glossy Balloon had a sharpen feature and seperate hue rotation meant for its image file overlay. Both removed, Now - as of July 2023 Glossy Balloon once again has a dependency but this time it is ENTIRELY OPTIONAL and designed to remove edge puff. It was also granted the ability to use the HSL Color blend mode with an optional checkbox.

Glossy Balloon came into GEGL Graph conception probably in April-May 2022 BEFORE I MADE PLUGINS. 

July 13 2023 update giving Glossy Balloon an optional HSL color mode. Beaver spent several hours trying to figure out why the trivial "gegl_node_pads_exist: Can't find sink property input of gegl:color 0x55da255f3590" ERROR exist. This error causes zero issue with GEGL and should be ignored. Please tell CMYK student to make sure that non-destructive filters work even if they have warnings. I don't see
the point in fixing meaningless errors.*/


#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

#define TUTORIAL \
" color-overlay value=#f587ff median-blur  percentile=90 alpha-percentile=100 "\
/* This GEGL Graph is the base pink color that is needed to bring out glossy balloon's glossy shine and a median blur to make it "fatter" like a balloon. */


#define TUTORIAL2 \
" id=3 screen aux=[   ref=3 emboss  type=bumpmap azimuth=30  elevation=15 ] median-blur  percentile=90 alpha-percentile=100 gaussian-blur std-dev-x=1 std-dev-y=1 filter=fir id=3 screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] median-blur  percentile=50 alpha-percentile=100 screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] median-blur  percentile=50 alpha-percentile=100 screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ]  "\
/* This GEGL graph is a instruction for emboss to fuss with screen like a normal bevel but done five times over*/


/* Above are two GEGL Graphs being called by Glossy Balloon */

property_double (gaus, _("Balloonification of text"), 6.0)
   description  (_("Makes the text blow up in size using an internal gaussian blur.'"))
  value_range (0.5, 20.0)
  ui_range (0.5, 12)
  ui_gamma (1.5)

property_double (hue, _("Color rotation"),  0.0)
   description  (_("Color rotation. Manually apply a color overlay after applying to make it any color you want.'"))
   value_range  (-180.0, 180.0)
  ui_meta     ("sensitive", "! wildcolor")

property_double (lightness, _("Lightness"), -7)
   description  (_("Lightness adjustment"))
   value_range  (-15.0, 15)
  ui_meta     ("sensitive", "! wildcolor")

property_double (saturation, _("Saturation"), 1.2)
   description  (_("Saturation control - for image file overlays full desaturation is recommended."))
  ui_range (0.0, 2)
   value_range  (0.0, 2)

property_file_path(src, _("Image file overlay (Desaturation and bright light recommended)"), "")
    description (_("Upload an image, files permitted are (png, jpg, raw, svg, bmp, tif, ...) Does not work proper with HSL Color mode. You can remove an image file overlay by going back to the image file select window, removing all text from --location-- then click open. Image file overlay will then go back to its default (None)"))
  ui_meta     ("sensitive", "! wildcolor")


property_boolean (wildcolor, _("Enable alternative HSL Color mode for Glossy Balloon"), FALSE)
  description    (_("Allow Glossy Balloon to use HSL Color instead of its default Hue Rotaton. This overwrites the hue rotation, lightness and image file overlays."))

property_color  (hslcolor, _("HSL Color Overlay"), "#f88cfb")
    description (_("Optional HSL Color overlay for Glossy Balloon. Is only enabled if its checkbox is on."))
  ui_meta     ("sensitive", "wildcolor")

property_boolean (gbstatus, _("Glossy Balloon on/off switch"), TRUE)
  description    (_("Disable Glossy Balloon. This is useful for other plugins that call Glossy Balloon but useful for stand alone Glossy Baloon. Thus this option is hidden from the GUI. The output-extent tag in ui_meta is what hides it."))
    ui_meta     ("role", "output-extent")


#else

#define GEGL_OP_META
#define GEGL_OP_NAME     glossy_bevel_
#define GEGL_OP_C_SOURCE glossy-bevel-.c

#include "gegl-op.h"

typedef struct
{
  GeglNode *input;
  GeglNode *output;
  GeglNode *graph;
  GeglNode *blur;
  GeglNode *graph2;
  GeglNode *hue;
  GeglNode *fix;
  GeglNode *huelock;
  GeglNode *lightlock;
  GeglNode *saturation;
  GeglNode *opacityall;
  GeglNode *smooth;
  GeglNode *ta2;
  GeglNode *light;
  GeglNode *multiply;
  GeglNode *repairgeglgraph;
  GeglNode *layer;
  GeglNode *gray;
  GeglNode *color;
  GeglNode *hslcolor;
  GeglNode *nop;
  GeglNode *nop2;
  GeglNode *nop3;
} State;

static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;
/* This is the most complex part of GEGL Plugin development. As of July 2023 Beaver understands the if else stuff. */
  if (o->gaus > 3.1)
  {
/* This graph list everything in glossy balloon with an additional threshold-alpha like filter that kicks in at 3.1 "Balloonification/gaussian blur". state->ta2 is the only addition.  */
      gegl_node_link_many (state->input, state->graph, state->blur, state->graph2, state->hue, state->light, state->saturation, state->opacityall, state->smooth, state->multiply, state->repairgeglgraph,  state->ta2, state->output, NULL);
/* Multiply blend mode fuses with image file upload*/
  gegl_node_connect_from (state->multiply, "aux", state->layer, "output");
/* Image file upload is given a trivial no operation to prevent a trivial warning */
  gegl_node_link_many (state->layer, state->nop,  NULL); 
  }
  else 
  {
/* This graph list everything in glossy balloon  */
      gegl_node_link_many (state->input, state->graph, state->blur, state->graph2, state->hue, state->light, state->saturation, state->opacityall, state->smooth, state->multiply, state->repairgeglgraph, state->output, NULL);
/* Multiply blend mode fuses with image file upload*/
  gegl_node_connect_from (state->multiply, "aux", state->layer, "output");
/* Image file upload is given a trivial no operation to prevent a trivial warning */
  gegl_node_link_many (state->layer, state->nop,  NULL); 
  }
  if (o->wildcolor)
/* Enable HSL wild color mode for Glossy Balloon.  */
      gegl_node_link_many (state->input, state->graph, state->blur, state->graph2, state->huelock, state->lightlock,  state->opacityall, state->smooth,  state->gray, state->multiply, state->hslcolor, state->saturation, state->repairgeglgraph,  state->output, NULL);
  gegl_node_connect_from (state->hslcolor, "aux", state->nop2, "output");
  gegl_node_link_many (state->color, state->nop2, NULL);
/* Multiply blend mode fuses with image file upload*/
  gegl_node_connect_from (state->multiply, "aux", state->layer, "output");
/* Image file upload is given a trivial no operation to prevent a trivial warning */
  gegl_node_link_many (state->layer, state->nop,  NULL); 
if (o->wildcolor)
  if (o->gaus > 3.1)
/* Enable HSL wild color mode for Glossy Balloon with threshold alpha like graph that kicks in at 3.1.  */
      gegl_node_link_many (state->input, state->graph, state->blur, state->graph2, state->huelock, state->lightlock, state->opacityall, state->smooth,  state->gray, state->multiply, state->hslcolor, state->repairgeglgraph,  state->saturation, state->ta2,  state->output, NULL);
  gegl_node_connect_from (state->hslcolor, "aux", state->nop2, "output");
  gegl_node_link_many (state->color, state->nop2, NULL);
/* Multiply blend mode fuses with image file upload*/
  gegl_node_connect_from (state->multiply, "aux", state->layer, "output");
/* Image file upload is given a trivial no operation to prevent a trivial warning */
  gegl_node_link_many (state->layer, state->nop,  NULL); 
if (!o->gbstatus)
/* Disable's Glossy Balloon*/
  gegl_node_link_many (state->input, state->output,  NULL); 
}

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglNode *input, *output, *blur, *graph, *graph2, *fix, *nop, *nop2, *nop3, *hue, *light, *lightlock, *huelock, *gray, *color, *hslcolor,  *layer, *smooth, *saturation, *ta2, *opacityall, *multiply, *repairgeglgraph;


  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");

  blur    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
   "filter", 1,
                                  NULL);
/* Filter one is code for Gaussian Blur Fir*/


 graph   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", TUTORIAL,  NULL);
                             

 graph2   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl",  "string", TUTORIAL2, 
                                  NULL);
/*JUNE 24 2023 I FINALLY FIGURED OUT HOW TO HIDE GEGL GRAPH STRINGS. My Plugins have never been more professional*/



 ta2   = gegl_node_new_child (gegl,
                                  "operation", "lb:threshold-alpha", "alphadegree", 0.2,
                                  NULL);
/*This is an exeripmental plugin I made that is completly optional and Glossy Balloon will work fine without it.
Most users will not have this addition plugin. This plugin is meant to further reduce edge puff. (july 2023)*/


 hue   = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);

 light   = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);

 lightlock   = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma", "lightness", -7.0,
                                  NULL);

 huelock   = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma", "hue", 0.0,
                                  NULL);

 layer   = gegl_node_new_child (gegl,
                                  "operation", "gegl:layer",
                                  NULL);



 opacityall  = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity", "value", 10.0,
                                  NULL);


 saturation   = gegl_node_new_child (gegl,
                                  "operation", "gegl:saturation",
                                  NULL);

 fix   = gegl_node_new_child (gegl,
                                  "operation", "gegl:rgb-clip",
                                  NULL);

 multiply   = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);

 nop   = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);
/* I don't know why but this NOP removes a trivial warning in GEGL. This is a common theme that many of my plugins have*/

 nop2   = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

 nop3   = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);
/* I don't know why but this NOP removes a trivial warning in GEGL. This is a common theme that many of my plugins have*/

 smooth   = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-reduction", "iterations", 1,
                                  NULL);
/* When two text edges of glossy balloon touch they have rough edges. This solves that problem*/


 gray   = gegl_node_new_child (gegl,
                                  "operation", "gimp:desaturate", "mode", 2,
                                  NULL);


 color   = gegl_node_new_child (gegl,
                                  "operation", "gegl:color",
                                  NULL);

hslcolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 39, "composite-mode", 0, "blend-space", 1, NULL);


  repairgeglgraph      = gegl_node_new_child (gegl, "operation", "gegl:median-blur",
                                         "radius",       0,
                                         NULL);

 /*Repair GEGL Graph is a critical operation for Gimp's non-destructive future.
A median blur at zero radius is confirmed to make no changes to an image. 
This option resets gegl:opacity's value to prevent a known bug where
plugins like clay, glossy balloon and custom bevel glitch out when
drop shadow is applied in a gegl graph below them.*/
 
  gegl_operation_meta_redirect (operation, "gaus", blur, "std-dev-x");
  gegl_operation_meta_redirect (operation, "gaus", blur, "std-dev-y");
  gegl_operation_meta_redirect (operation, "hue", hue, "hue");
  gegl_operation_meta_redirect (operation, "lightness", light, "lightness");
  gegl_operation_meta_redirect (operation, "src", layer, "src");
  gegl_operation_meta_redirect (operation, "saturation", saturation, "scale");
  gegl_operation_meta_redirect (operation, "string",  graph, "string");
  gegl_operation_meta_redirect (operation, "string2",  graph2, "string");
  gegl_operation_meta_redirect (operation, "hslcolor",  color, "value");

 /* ORIGINAL GEGL GRAPH BEFORE THE THRESHOLD ALPHA 2 UPDATE
  gegl_node_link_many (input, graph, blur, graph2, hue, saturation, opacityall, smooth, multiply, repairgeglgraph, output, NULL);
  gegl_node_connect_from (multiply, "aux", layer, "output");
  gegl_node_link_many (layer, nop,  NULL);*/

  State *state = g_malloc0 (sizeof (State));
  o->user_data = state;
  state->input = input;
  state->output = output;
  state->graph = graph;
  state->blur = blur;
  state->graph2 = graph2;
  state->hue = hue;
  state->huelock = huelock;
  state->lightlock = lightlock;
  state->light = light;
  state->saturation = saturation;
  state->opacityall = opacityall;
  state->smooth = smooth;
  state->multiply = multiply;
  state->repairgeglgraph = repairgeglgraph;
  state->layer = layer;
  state->nop = nop;
  state->fix = fix;
  state->nop2 = nop2;
  state->nop3 = nop3;
  state->gray = gray;
  state->color = color;
  state->hslcolor = hslcolor;
  state->ta2 = ta2;

  o->user_data = state;
}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;
GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);
  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;
  operation_meta_class->update = update_graph;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:glossy-balloon",
    "title",       _("Glossy Balloon"),
    "categories",  "Artistic",
    "reference-hash", "branch45a35416a10a512570f0f25sb2ac",
    "description", _("Make Glossy Bevel text with GEGL. This will not work well on small text."
                     ""),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Glossy Balloon..."),
    NULL);
}

#endif
