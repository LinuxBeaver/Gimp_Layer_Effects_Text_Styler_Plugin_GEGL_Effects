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
 * 2023 Beaver (GEGL ZZStrokeBevelImageOverlay a hidden operation fork of drop shadow to give GEGL Effects outline the ability to add a bevel, image file overlay and more. May 16 23 it now gives GEGL Effects drop shadow a blurred image file overlay. May 17 2023 it now has an aura seed option for shadow glow )
 */

/*zzstrokebevelimage is a hidden operations for GEGL Effects. The goal is to eventually bake it inside GEGL Effects and get rid of this hidden operation. 
This hidden operation is meant to be a better drop shadow. Before it existed GEGL Effects called Gimp's normal drop shadow filter*/


#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES


property_double (tile_size, _("Glow Aura Intensity"), 10)
    description (_("Average diameter of each tile (in pixels)"))
    value_range (0.0, 10.0)
    ui_meta     ("unit", "pixel-distance")

property_double (tile_saturation, _("Glow Spacing"), 2.5)
    description (_("Expand tiles by this amount"))
    value_range (0.0, 4.0)

property_seed (seed, _("Seed of Glow Aura"), rand)


property_double (blurshadowimage, _("Blur Drop Shadow/Glow's Image file overlay"), 0)
   description (_("Standard deviation for the horizontal axis"))
   value_range (0.0, 40.0)
   ui_range    (0.24, 40.0)
   ui_gamma    (3.0)
   ui_meta     ("unit", "pixel-distance")


/*ENUM list of blend modes for Outline Bevel*/
enum_start (gegl_blend_mode_type_effectszzbevoutline)
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLYGE,      "MultiplyGE",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGEGE,      "GrainMergeGE",
              N_("GrainMerge"))
  enum_value (GEGL_BLEND_MODE_TYPE_SHINYGMGE,      "GrainMergealtGE",
              N_("GrainMergeAlt"))
  enum_value (GEGL_BLEND_MODE_TYPE_COLORDODGEGE,      "ColorDodgeGE",
              N_("ColorDodge"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHTGE,      "HardLightGE",
              N_("HardLight"))
  enum_value (GEGL_BLEND_MODE_TYPE_DISABLEBEVELGE,      "DisableBevelGE",
              N_("DisableOutlineBevel"))
enum_end (GeglBlendModeTypezzbevoutline)


property_enum (blendmodebeveloutline, _("Blend Mode of Bevel's emboss'"),
    GeglBlendModeTypezzbevoutline, gegl_blend_mode_type_effectszzbevoutline,
    GEGL_BLEND_MODE_TYPE_MULTIPLYGE)

property_boolean (enableaura, _("Enable Shadow/Glow Aura Mode"), FALSE)
  description    (_("Make Shadow Glow an Aura"))



property_boolean (specialoutline, _("Special Outline Switch"), FALSE)
  description    (_("Turn on special outline abilities"))

property_boolean (enableoutline, _("Enable normal outline switch"), FALSE)
  description    (_("Enable Outline"))

/*ENUM list of blend modes for Base Outline Shape*/
enum_start (gegl_stroke_grow_shapeszz2)
  enum_value (GEGL_stroke_GROW_SHAPE_SQUAREzz,  "square",  N_("Square"))
  enum_value (GEGL_stroke_GROW_SHAPE_CIRCLEzz,  "circle",  N_("Circle"))
  enum_value (GEGL_stroke_GROW_SHAPE_DIAMONDzz, "diamond", N_("Diamond"))
enum_end (GeglstrokeGrowShapeszz2)

property_enum   (grow_shape, _("Grow shape"),
                 GeglstrokeGrowShapeszz2, gegl_stroke_grow_shapeszz2,
                 GEGL_stroke_GROW_SHAPE_CIRCLEzz)
  description   (_("The shape to expand or contract the stroke in"))



property_file_path(src, _("Source"), "")
    description (_("Source image file path (png, jpg, raw, svg, bmp, tif, ...)"))




property_color (color, _("Color"), "#ffffff")


property_double (x, _("X"), 0.0)
  description   (_("Horizontal shadow offset"))
  value_range (-300, 300.0)
  ui_range      (-15.0, 15.0)
  ui_steps      (1, 10)
  ui_meta       ("axis", "x")

property_double (y, _("Y"), 0.0)
  description   (_("Vertical shadow offset"))
  value_range (-300, 300.0)
  ui_range      (-15.0, 15.0)
  ui_steps      (1, 10)
  ui_meta       ("axis", "y")


property_double (blurstroke, _("Blur radius"), 0.0)
  value_range   (0.0, G_MAXDOUBLE)
  ui_range      (0.0, 30.0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")


property_double (stroke, _("Grow radius"), 12.0)
  value_range   (0.0, G_MAXDOUBLE)
  ui_range      (0, 100.0)
  ui_digits     (0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the shadow before blurring; a negative value will contract the shadow instead"))


property_double (opacity, _("Opacity"), 1)
  value_range   (0.0, 1.0)
  ui_steps      (0.01, 0.10)

property_double (opacitybevel, _("Opacity Bevel Outline"), 1)
  value_range   (0.2, 1.0)
  ui_steps      (0.01, 0.10)





property_double (radius1, _("Bevel Radius"), 7.0)
  value_range (1.0, 40.0)
  ui_range (1.0, 12)
  ui_gamma (1.5)



property_double (bevel1, _("Bevel Depth Angle"), 90.0)
    description (_("Elevation angle (degrees)"))
    value_range (0, 180)
    ui_meta ("unit", "degree")

property_int (bevel2, _("Bevel Depth"), 1)
    description (_("Filter width"))
    value_range (1, 100)


property_double (th, _("Bevel Threshold of the Bevel's Transparency'"), 0.100)
  value_range (0.0, 1.0)
  ui_range (0.0, 0.5)

property_double (azimuth, _("Bevel Rotate Lighting"), 40.0)
    description (_("Light angle (degrees)"))
    value_range (0, 350)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")

property_double (hue, _("Hue of image file overlay"),  0.0)
   description  (_("Hue adjustment"))
   value_range  (-180.0, 180.0)


property_double (lightness, _("Lightness of image file overlay"), 0.0)
   description  (_("Lightness adjustment"))
   value_range  (-100.0, 100.0)


property_string (string, _("Graph1"), GEGLSYNTAXZONE)
    ui_meta     ("role", "output-extent")

/*GEGL Syntax here. Operation is gegl:gegl. This syntax is instructing zzstrokebevelimage to remove everything*/
#define GEGLSYNTAXZONE \
" id=1 clear aux=[ ref=1 ]  ] "\

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     zzstrokebevelimage
#define GEGL_OP_C_SOURCE zzstrokebevelimage.c

#include "gegl-op.h"

typedef struct
{
  GeglNode *input;
  GeglNode *behind;
  GeglNode *bevel;
  GeglNode *opacitybevel;
  GeglNode *multiplyge;
  GeglNode *atop;
  GeglNode *layer;
  GeglNode *stroke;
  GeglNode *color;
  GeglNode *nop;
  GeglNode *huelight;
  GeglNode *hardlightge;
  GeglNode *colordodgege;
  GeglNode *shinygmge;
  GeglNode *grainmergege;
  GeglNode *opacity;
  GeglNode *disablebevel;
  GeglNode *blurshadowimage;
  GeglNode *median;
  GeglNode *gaussian;
  GeglNode *cubismglow;
  GeglNode *move;
  GeglNode *ontop;
  GeglNode *nop2;
  GeglNode *crop;
  GeglNode *graph;
  GeglNode *output;
} State; 


static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node; 
  GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglNode *input, *output, *behind, *median, *cubismglow, *gaussian, *crop, *nop2, *move,   *nop, *opacity, *huelight, *graph, *opacitybevel, *bevel, *multiplyge, *disablebevel, *blurshadowimage, *atop, *layer, *stroke, *hardlightge, *shinygmge, *grainmergege, *colordodgege,  *color,  *ontop;


  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");


 ontop   = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-in",
                                  NULL);


  color   = gegl_node_new_child (gegl,
                                  "operation", "gegl:color",
                                  NULL);


  median      = gegl_node_new_child (gegl, "operation", "gegl:median-blur",
                                         "percentile",       100.0,
                                         "alpha-percentile", 100.0,
                                         "abyss-policy",     GEGL_ABYSS_NONE,
                                         NULL);
/*Example of a baked in median blur setting*/

  gaussian    = gegl_node_new_child (gegl, "operation", "gegl:gaussian-blur",
                                         "clip-extent", FALSE,
                                         "abyss-policy", 0,
                                         NULL);
/*Example of a baked in gaussian blur setting*/

  move   = gegl_node_new_child (gegl,
                                  "operation", "gegl:translate",
                                  NULL);

  crop   = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop",
                                  NULL);



  disablebevel   = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst",
                                  NULL);

  blurshadowimage   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
                                  NULL);



  nop2   = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);


  nop   = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);


  behind   = gegl_node_new_child (gegl,
                                  "operation", "gegl:src",
                                  NULL);


  stroke    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow",
                                  NULL);


  graph    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl",
                                  NULL);

  atop    = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);

  layer    = gegl_node_new_child (gegl,
                                  "operation", "gegl:layer",
                                  NULL);

  cubismglow    = gegl_node_new_child (gegl,
                                  "operation", "gegl:cubism",
                                  NULL);


  bevel    = gegl_node_new_child (gegl,
                                  "operation", "gegl:bevel",
                                  NULL);

  huelight    = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);

  opacity    = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

  opacitybevel    = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);


multiplyge = gegl_node_new_child (gegl,
                                    "operation", "gimp:layer-mode", "layer-mode", 30,  "composite-mode", 0, NULL);

grainmergege = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 47, "composite-mode", 0, NULL);

shinygmge = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 47, "composite-space", 2, "composite-mode", 0, "blend-space", 2, NULL);

colordodgege = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 42, "composite-mode", 0, "composite-space", 1, "blend-space", 1, NULL);

hardlightge = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 44, "composite-mode", 0, "composite-space", 1, "blend-space", 0, NULL);





  gegl_operation_meta_redirect (operation, "tile_saturation", cubismglow, "tile-saturation");
  gegl_operation_meta_redirect (operation, "tile_size", cubismglow, "tile-size");
  gegl_operation_meta_redirect (operation, "seed", cubismglow, "seed");
  gegl_operation_meta_redirect (operation, "blurshadowimage", blurshadowimage, "std-dev-x");
  gegl_operation_meta_redirect (operation, "blurshadowimage", blurshadowimage, "std-dev-y");
  gegl_operation_meta_redirect (operation, "stroke", median, "radius");
  gegl_operation_meta_redirect (operation, "blurstroke", gaussian, "std-dev-x");
  gegl_operation_meta_redirect (operation, "blurstroke", gaussian, "std-dev-y");
  gegl_operation_meta_redirect (operation, "x", move, "x");
  gegl_operation_meta_redirect (operation, "y", move, "y");
  gegl_operation_meta_redirect (operation, "grow_shape", median, "neighborhood");
  gegl_operation_meta_redirect (operation, "opacity", opacity, "value");
  gegl_operation_meta_redirect (operation, "opacitybevel", opacitybevel, "value");
  gegl_operation_meta_redirect (operation, "color", stroke, "color");
  gegl_operation_meta_redirect (operation, "color", color, "value");
  gegl_operation_meta_redirect (operation, "radius1", bevel, "radius1");
  gegl_operation_meta_redirect (operation, "th", bevel, "th");
  gegl_operation_meta_redirect (operation, "bevel1", bevel, "bevel1");
  gegl_operation_meta_redirect (operation, "bevel2", bevel, "bevel2");
  gegl_operation_meta_redirect (operation, "azimuth", bevel, "azimuth");
  gegl_operation_meta_redirect (operation, "src", layer, "src");
  gegl_operation_meta_redirect (operation, "hue", huelight, "hue");
  gegl_operation_meta_redirect (operation, "lightness", huelight, "lightness");
  gegl_operation_meta_redirect (operation, "string",  graph, "string");

 /* Now save points to the various gegl nodes so we can rewire them in
   * update_graph() later
   */
  State *state = g_malloc0 (sizeof (State));
  state->input = input;
  state->behind = behind;
  state->cubismglow = cubismglow;
  state->bevel = bevel;
  state->opacitybevel = opacitybevel;
  state->disablebevel = disablebevel;
  state->blurshadowimage = blurshadowimage;
  state->multiplyge = multiplyge;
  state->atop = atop;
  state->layer = layer;
  state->stroke = stroke;
  state->color = color;
  state->nop = nop;
  state->huelight = huelight;
  state->hardlightge = hardlightge;
  state->shinygmge = shinygmge;
  state->grainmergege = grainmergege;
  state->colordodgege = colordodgege;
  state->opacity = opacity;
  state->output = output;
  state->median = median;
  state->gaussian = gaussian;
  state->move = move;
  state->ontop = ontop;
  state->nop2 = nop2;
  state->graph = graph;
  state->crop = crop;
  o->user_data = state;
}

static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  GeglNode *multiplyge;
  if (!state) return;

  multiplyge = state->multiplyge; /* the default outline bevel that is meant to be switched into other blend modes.  */
  switch (o->blendmodebeveloutline) {
    case GEGL_BLEND_MODE_TYPE_MULTIPLYGE: multiplyge = state->multiplyge; break;
    case GEGL_BLEND_MODE_TYPE_GRAINMERGEGE: multiplyge = state->grainmergege; break;
    case GEGL_BLEND_MODE_TYPE_SHINYGMGE: multiplyge = state->shinygmge; break;
    case GEGL_BLEND_MODE_TYPE_COLORDODGEGE: multiplyge = state->colordodgege; break;
    case GEGL_BLEND_MODE_TYPE_HARDLIGHTGE: multiplyge = state->hardlightge; break;
    case GEGL_BLEND_MODE_TYPE_DISABLEBEVELGE: multiplyge = state->disablebevel; break;

 /* This is a really complex if else scenario Beaver did all by themselves. Beaver wishes in the future that this will become more simple.  */
 }


 if (o->enableoutline)
 if (o->specialoutline)
 if (o->enableaura)
  {
    gegl_node_link_many (state->input, state->median, state->cubismglow, state->gaussian, state->move,  state->ontop, state->crop, state->atop, multiplyge, state->output, NULL);
    gegl_node_link_many (state->nop, state->layer, state->blurshadowimage, state->huelight,  NULL); 
    gegl_node_link_many (state->atop, state->bevel, state->opacitybevel,  NULL);
    gegl_node_connect_from (multiplyge, "aux", state->opacitybevel, "output");
    gegl_node_connect_from (state->atop, "aux", state->huelight, "output");
      gegl_node_link_many (state->color, state->nop2, state->opacity, NULL);
      gegl_node_connect_from (state->ontop, "aux", state->opacity, "output");
  }
else
  {
    gegl_node_link_many (state->input, state->median, state->gaussian, state->move,  state->ontop, state->crop, state->atop, multiplyge,  state->output, NULL);
    gegl_node_link_many (state->nop, state->layer, state->blurshadowimage, state->huelight,  NULL); 
    gegl_node_link_many (state->atop, state->bevel, state->opacitybevel,  NULL);
    gegl_node_connect_from (multiplyge, "aux", state->opacitybevel, "output");
    gegl_node_connect_from (state->atop, "aux", state->huelight, "output");
      gegl_node_link_many (state->color, state->nop2,  state->opacity, NULL);
      gegl_node_connect_from (state->ontop, "aux", state->opacity, "output");
  }
else
  {
    gegl_node_link_many (state->input, state->median, state->gaussian,  state->move, state->ontop, state->crop, state->output, NULL);
      gegl_node_link_many (state->color, state->nop2, state->opacity, NULL);
      gegl_node_connect_from (state->ontop, "aux", state->opacity, "output");
  }
else
  {
    gegl_node_link_many (state->input, state->graph, state->output, NULL);
  }
}




static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class = GEGL_OPERATION_CLASS (klass);
   GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);

  operation_class->attach = attach;
  operation_meta_class->update = update_graph;

  gegl_operation_class_set_keys (operation_class,
    "name",        "gegl:zzstrokebevelimage",
    "title",       _("Hidden Operation to bevel and image file overlay a stroke for GEGL Effects. GEGL Effects will not work at all without this hidden operation."),
    "categories",  "hidden",
    "reference-hash", "33234v25str2ac",
    "description", _("Hidden Operation for GEGL Effects - This is a rough reimagination of Gimp's drop shadow filter with many new things and technical options that drop shadow was not capable of doing. GEGL Effects no longer calls Gimp's GEGL drop shadow filter. It uses this under the hood."
                     ""),
    NULL);
}
 /* This operation is hidden from Gimp's GUI because the "categories" is set to "hidden".  */
#endif
