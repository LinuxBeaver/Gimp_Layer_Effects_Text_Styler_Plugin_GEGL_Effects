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

July 13 2023 update giving Glossy Balloon an optional HSL color mode. Beaver spent several hours trying to figure out why the trivial "gegl_node_pads_exist: Can't find sink property input of gegl:color 0x55da255f3590" ERROR exist. This error causes zero issue with GEGL and should be ignored. Please tell CMYK student to make sure that non-destructive filters work even if they have warnings. I don't see
the point in fixing meaningless errors.*/


#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES


#define TUTORIAL2 \
" id=3 screen aux=[   ref=3 emboss  type=bumpmap azimuth=30  elevation=15 ] median-blur abyss-policy=none  percentile=90 alpha-percentile=100 gaussian-blur  abyss-policy=none  clip-extent=false std-dev-x=1 std-dev-y=1 filter=fir id=3 screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] median-blur abyss-policy=none  percentile=50 alpha-percentile=100 screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ] median-blur abyss-policy=none  percentile=50 alpha-percentile=100 screen aux=[ ref=3  emboss  type=bumpmap azimuth=90  elevation=15 ]  "\
/* This GEGL graph is a instruction for emboss to fuss with screen like a normal bevel but done five times over*/


/* Above are two GEGL Graphs being called by Glossy Balloon */

property_double (gaus, _("Balloonification of text"), 6.0)
   description  (_("Makes the text/shapes blow up in size using an internal gaussian blur"))
  value_range (0.5, 20.0)
  ui_range (0.5, 12)
  ui_gamma (1.5)

property_double (hue, _("Color rotation"),  0.0)
   description  (_("Hue rotation to recolor - when this plugin first debuted in Summer 2022 author did not know how to do a color widget in GEGL, but did know how to call hue rotation"))
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
    description (_("Upload an image, files permitted are (png, jpg, raw, svg, bmp, tif, ...) )"))
  ui_meta     ("sensitive", "! wildcolor")


property_boolean (wildcolor, _("Enable alternative HSL Color mode"), FALSE)
  description    (_("Allow Glossy Balloon to use HSL Color instead of its default Hue Rotaton for color choice. This overwrites the hue rotation, lightness and image file overlays."))

property_color  (hslcolor, _("HSL Color Overlay"), "#f88cfb")
    description (_("Optional HSL Color overlay for Glossy Balloon. Is only enabled if its checkbox is on."))
  ui_meta     ("sensitive", "wildcolor")

property_int (thin, _("Thin"), 0)
   description  (_("Make Glossy Balloon thinner. Making it too thin might disrupt the text/shapes. This works by internalling thinning the text/shape before Glossy Balloon is applied. Works best when the balloonification slider is lower"))
   value_range  (-8, 0)
  ui_range (-5, 0)

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
  GeglNode *pink;
  GeglNode *huelight;
  GeglNode *huelightlock;
  GeglNode *graph;
  GeglNode *blur;
  GeglNode *graph2;
  GeglNode *saturation;
  GeglNode *opacityall;
  GeglNode *smooth;
  GeglNode *ta2;
  GeglNode *multiply;
  GeglNode *repairgeglgraph;
  GeglNode *layer;
  GeglNode *gray;
  GeglNode *color;
  GeglNode *hslcolor;
  GeglNode *nop;
  GeglNode *mediancontrol;
} State;



static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglColor *specialpink = gegl_color_new ("#f587ff");

State *state = g_malloc0 (sizeof (State));
  o->user_data = state;


    state->input    = gegl_node_get_input_proxy (gegl, "input");
    state->output   = gegl_node_get_output_proxy (gegl, "output");



    state->blur    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur", "abyss-policy", 0,  "clip-extent", FALSE,
   "filter", 1,
                                  NULL);
/* Filter one is code for Gaussian Blur Fir*/


   state->pink   = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay", "value", specialpink,  NULL);
/* this is the base pink color that is needed to bring out glossy balloon's glossy shine  */

   state->graph2   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl",  "string", TUTORIAL2,
                                  NULL);
/*JUNE 24 2023 I FINALLY FIGURED OUT HOW TO HIDE GEGL GRAPH STRINGS. My Plugins have never been more professional*/



   state->ta2   = gegl_node_new_child (gegl,
                                  "operation", "lb:threshold-alpha", "alphadegree", 0.2,
                                  NULL);
/*This is a plugin I made to further reduce edge puff. (july 2023)*/


   state->huelight   = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);

   state->huelightlock   = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma", "hue", 0.0, "lightness", -7.0,
                                  NULL);

   state->layer   = gegl_node_new_child (gegl,
                                  "operation", "port:load",
                                  NULL);

   state->opacityall  = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity", "value", 10.0,
                                  NULL);


   state->saturation   = gegl_node_new_child (gegl,
                                  "operation", "gegl:saturation",
                                  NULL);

   state->multiply   = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);

   state->nop   = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);


   state->smooth   = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-reduction", "iterations", 1,
                                  NULL);
/* When two text edges of glossy balloon touch they have rough edges. This solves that problem*/


   state->gray   = gegl_node_new_child (gegl,
                                  "operation", "gimp:desaturate", "mode", 2,
                                  NULL);


   state->color   = gegl_node_new_child (gegl,
                                  "operation", "gegl:color",
                                  NULL);

   state->mediancontrol   = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "percentile", 50.0, "alpha-percentile", 100.0, "radius", 0, "abyss-policy", 0, 
                                  NULL);

  state->hslcolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 39, "composite-mode", 0, "blend-space", 1, NULL);


    state->repairgeglgraph      = gegl_node_new_child (gegl, "operation", "gegl:median-blur",     "abyss-policy",     GEGL_ABYSS_NONE,
                                         "radius",       0,
                                         NULL);

 /*Repair GEGL Graph is a critical operation for Gimp's non-destructive future.
A median blur at zero radius is confirmed to make no changes to an image.
This option resets gegl:opacity's value to prevent a known bug where
plugins like clay, glossy balloon and custom bevel glitch out when
drop shadow is applied in a gegl graph below them.*/

  gegl_operation_meta_redirect (operation, "gaus", state->blur, "std-dev-x");
  gegl_operation_meta_redirect (operation, "gaus", state->blur, "std-dev-y");
  gegl_operation_meta_redirect (operation, "hue", state->huelight, "hue");
  gegl_operation_meta_redirect (operation, "lightness", state->huelight, "lightness");
  gegl_operation_meta_redirect (operation, "src", state->layer, "src");
  gegl_operation_meta_redirect (operation, "saturation", state->saturation, "scale");
  gegl_operation_meta_redirect (operation, "hslcolor",  state->color, "value");
  gegl_operation_meta_redirect (operation, "thin",  state->mediancontrol, "radius");

}


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
      gegl_node_link_many (state->input, state->pink, state->mediancontrol, state->blur, state->graph2, state->huelight,  state->saturation, state->opacityall, state->smooth, state->multiply, state->repairgeglgraph,  state->ta2, state->output, NULL);
/* Multiply blend mode fuses with image file upload*/
  gegl_node_connect (state->multiply, "aux", state->layer, "output");
/* Image file upload is given a trivial no operation to prevent a trivial warning */
  gegl_node_link_many (state->layer, state->nop,  NULL);
  }
  else
  {
/* This graph list everything in glossy balloon  */
      gegl_node_link_many (state->input, state->pink, state->mediancontrol, state->blur, state->graph2, state->huelight,  state->saturation, state->opacityall, state->smooth, state->multiply, state->repairgeglgraph, state->output, NULL);
/* Multiply blend mode fuses with image file upload*/
  gegl_node_connect (state->multiply, "aux", state->layer, "output");
/* Image file upload is given a trivial no operation to prevent a trivial warning */
  gegl_node_link_many (state->layer, state->nop,  NULL);
  }
  if (o->wildcolor)
/* Enable HSL wild color mode for Glossy Balloon.  */
      gegl_node_link_many (state->input, state->pink, state->mediancontrol, state->blur, state->graph2, state->huelightlock, state->opacityall, state->smooth,  state->gray, state->multiply, state->hslcolor, state->saturation, state->repairgeglgraph,  state->output, NULL);
  gegl_node_connect (state->hslcolor, "aux", state->color, "output");
/* Multiply blend mode fuses with image file upload*/
  gegl_node_connect (state->multiply, "aux", state->layer, "output");
/* Image file upload is given a trivial no operation to prevent a trivial warning */
  gegl_node_link_many (state->layer, state->nop,  NULL);
if (o->wildcolor)
  if (o->gaus > 3.1)
/* Enable HSL wild color mode for Glossy Balloon with threshold alpha like graph that kicks in at 3.1.  */
      gegl_node_link_many (state->input, state->pink, state->mediancontrol, state->blur, state->graph2, state->huelightlock,  state->opacityall, state->smooth,  state->gray, state->multiply, state->hslcolor, state->repairgeglgraph,  state->saturation, state->ta2,  state->output, NULL);
  gegl_node_connect (state->hslcolor, "aux", state->color, "output");
/* Multiply blend mode fuses with image file upload*/
  gegl_node_connect (state->multiply, "aux", state->layer, "output");
/* Image file upload is given a trivial no operation to prevent a trivial warning */
  gegl_node_link_many (state->layer, state->nop,  NULL);
if (!o->gbstatus)
/* Disable's Glossy Balloon*/
  gegl_node_link_many (state->input, state->output,  NULL);

 /* ORIGINAL GEGL GRAPH BEFORE THE THRESHOLD ALPHA 2 UPDATE
  gegl_node_link_many (input, graph, blur, graph2, hue, saturation, opacityall, smooth, multiply, repairgeglgraph, output, NULL);
  gegl_node_connect (multiply, "aux", layer, "output");
  gegl_node_link_many (layer, nop,  NULL);*/
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
    "reference-hash", "branch45a35416a10a512570f0f25sb2ac",
    "description", _("Make inflatable glossy bumped text - Doesn't work well on small text"
                     ""),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Glossy Balloon..."),
    NULL);
}

#endif
