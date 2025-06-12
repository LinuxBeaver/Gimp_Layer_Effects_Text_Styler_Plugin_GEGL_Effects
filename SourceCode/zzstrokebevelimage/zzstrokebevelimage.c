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
 * Credit to Øvind Kolas (pippin) for major GEGL contributions
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

property_boolean (clipbugpolicy, _("Disable Clipping Compromise"), FALSE)
  description    (_("This checkbox removes the shadow clip bug for compliance with Gimp 3's non-destructive text editing. If enabled this will triger another bug only seen after using GEGL Effects heavily, said bug appears usually after a few minutes of usage and will cause GEGL Effects to delay a selected color update until another slider is moved. It is suggested to enable this once one applies the filter. But keep it disabled while editing GEGL Effects unless you can tolerate a delayed color update."))


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

property_double (opacity, _("Opacity"), 1.0)
  value_range   (0.0, 1.0)
  ui_steps      (0.01, 0.10)

property_double (opacitybevel, _("Opacity Bevel Outline"), 1.0)
  value_range   (0.2, 1.0)
  ui_steps      (0.01, 0.10)

property_double (radius1, _("Bevel Radius"), 7.0)
  value_range (1.0, 40.0)
  ui_range (1.0, 12)
  ui_gamma (1.5)

property_double (bevel1, _("Bevel Depth Angle"), 90.0)
    description (_("Elevation angle (degrees)"))
    value_range (0.0, 180.0)
    ui_meta ("unit", "degree")

property_int (bevel2, _("Bevel Depth"), 1)
    description (_("Filter width"))
    value_range (1, 100)

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
  GeglNode *output;
  GeglNode *behind;
  GeglNode *opacitybevel;
  GeglNode *multiplyge;
  GeglNode *multiplygepending;
  GeglNode *atop;
  GeglNode *layer;
  GeglNode *color;
  GeglNode *nop;
  GeglNode *newnop;
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
  GeglNode *nothingcubism;
  GeglNode *invisibleblend2;
  GeglNode *invisibleblend;
  GeglNode *bevel;
} State;


static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);

  State *state = o->user_data = g_malloc0 (sizeof (State));



  state->input    = gegl_node_get_input_proxy (gegl, "input");
  state->output   = gegl_node_get_output_proxy (gegl, "output");

   state->ontop   = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-in",
                                  NULL);

    state->color   = gegl_node_new_child (gegl,
                                  "operation", "gegl:color",
                                  NULL);


    state->median      = gegl_node_new_child (gegl, "operation", "gegl:median-blur",
                                         "percentile",       100.0,
                                         "alpha-percentile", 100.0,
                                         "abyss-policy",     GEGL_ABYSS_NONE,
                                         NULL);
/*Example of a baked in median blur setting*/

    state->gaussian    = gegl_node_new_child (gegl, "operation", "gegl:gaussian-blur",
                                         "clip-extent", FALSE,
                                         "abyss-policy", 0,
                                         NULL);
/*Example of a baked in gaussian blur setting*/

    state->move   = gegl_node_new_child (gegl,
                                  "operation", "gegl:translate",
                                  NULL);

    state->crop   = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);


    state->disablebevel   = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst",
                                  NULL);

    state->blurshadowimage   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
                                         "clip-extent", FALSE,
                                         "abyss-policy", 0,
                                  NULL);

    state->nop2   = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

    state->newnop   = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst",
                                  NULL);



    state->nop   = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);


    state->behind   = gegl_node_new_child (gegl,
                                  "operation", "gegl:src",
                                  NULL);



    state->graph    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl",
                                  NULL);

    state->atop    = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);

    state->layer    = gegl_node_new_child (gegl,
                                  "operation", "port:load",
                                  NULL);

    state->cubismglow    = gegl_node_new_child (gegl,
                                  "operation", "gegl:cubism",
                                  NULL);


    state->huelight    = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);



    state->opacity    = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

    state->opacitybevel    = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

    state->multiplygepending    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst",
                                  NULL);


  state->multiplyge = gegl_node_new_child (gegl,
                                    "operation", "gimp:layer-mode", "layer-mode", 30,  "composite-mode", 0, NULL);

  state->grainmergege = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 47, "composite-mode", 0, NULL);

  state->shinygmge = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 47, "composite-space", 2, "composite-mode", 0, "blend-space", 2, NULL);

  state->colordodgege = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 42, "composite-mode", 0, "composite-space", 1, "blend-space", 1, NULL);

  state->hardlightge = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 44, "composite-mode", 0, "composite-space", 1, "blend-space", 0, NULL);

    state->nothingcubism    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

    state->invisibleblend    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst",
                                  NULL);

    state->invisibleblend2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst",
                                  NULL);

  state->bevel    = gegl_node_new_child (gegl,
                                  "operation", "lb:bevel",
                                  NULL);

}

static void
update_graph (GeglOperation *operation)
{

  GeglProperties *o = GEGL_PROPERTIES (operation);

  State *state = o->user_data;

  GeglNode *multiplyge = state->multiplyge;
  GeglNode *crop = state->crop;
/*
  GeglNode *atop;
  GeglNode *cubismglow; */
  if (!state) return;

  if (o->clipbugpolicy) crop = state->newnop;
  if (!o->clipbugpolicy) crop = state->crop;

  multiplyge = state->multiplyge; /* the default outline bevel that is meant to be switched into other blend modes.  */
  switch (o->blendmodebeveloutline) {
    case GEGL_BLEND_MODE_TYPE_MULTIPLYGE: multiplyge = state->multiplyge; break;
    case GEGL_BLEND_MODE_TYPE_GRAINMERGEGE: multiplyge = state->grainmergege; break;
    case GEGL_BLEND_MODE_TYPE_SHINYGMGE: multiplyge = state->shinygmge; break;
    case GEGL_BLEND_MODE_TYPE_COLORDODGEGE: multiplyge = state->colordodgege; break;
    case GEGL_BLEND_MODE_TYPE_HARDLIGHTGE: multiplyge = state->hardlightge; break;
    case GEGL_BLEND_MODE_TYPE_DISABLEBEVELGE: multiplyge = state->disablebevel; break;

 /* This is a really complex if else scenario Beaver did all by themselves. Beaver wishes in the future that this will become more simple.  */


/*
  if (o->enableaura) cubismglow = state->cubismglow;
  if (!o->enableaura) cubismglow = state->nothingcubism;

  if (!o->specialoutline) gegl_node_disconnect(state->atop, "aux");
  if (o->specialoutline)  gegl_node_connect(state->huelight, "output", state->atop, "aux");
  if (!o->specialoutline) atop = state->invisibleblend;
  if (o->specialoutline) atop = state->atop;
  if (!o->specialoutline) gegl_node_disconnect(multiplyge, "aux");
  if (o->specialoutline)  gegl_node_connect(state->opacitybevel, "output", state->multiplyge, "aux");
  if (!o->specialoutline) multiplyge = state->invisibleblend2;
  if (o->specialoutline) multiplyge = state->multiplyge;

*/

}
 if (o->enableoutline)
 if (o->specialoutline)
 if (o->enableaura)
  {
  gegl_node_link_many (state->input, state->median, state->cubismglow, state->gaussian,   state->ontop, state->atop, state->nop2, multiplyge, crop,  state->move, state->output, NULL);
  gegl_node_link_many (state->layer, state->blurshadowimage, state->huelight,  NULL);
  gegl_node_link_many (state->nop2, state->bevel, state->opacitybevel,  NULL);
  gegl_node_connect (multiplyge, "aux", state->opacitybevel, "output");
  gegl_node_connect (state->atop, "aux", state->huelight, "output");
  gegl_node_link_many (state->color,  state->opacity, NULL);
  gegl_node_connect (state->ontop, "aux", state->opacity, "output");
  }
else
  {
  gegl_node_link_many (state->input, state->median,  state->gaussian, state->ontop, state->atop, state->nop2, multiplyge, crop, state->move,   state->output, NULL);
  gegl_node_link_many (state->layer, state->blurshadowimage, state->huelight,  NULL);
  gegl_node_link_many (state->nop2, state->bevel, state->opacitybevel,  NULL);
  gegl_node_connect (multiplyge, "aux", state->opacitybevel, "output");
  gegl_node_connect (state->atop, "aux", state->huelight, "output");
  gegl_node_link_many (state->color,  state->opacity, NULL);
  gegl_node_connect (state->ontop, "aux", state->opacity, "output");
  }
else
  {
  gegl_node_link_many (state->input, state->median,  state->gaussian,  state->ontop, state->nop2,  crop, state->move,  state->output, NULL);
  gegl_node_link_many (state->color,  state->opacity, NULL);
  gegl_node_connect (state->ontop, "aux", state->opacity, "output");
  }
else
  {
    gegl_node_link_many (state->input, state->graph, state->output, NULL);
  }


  gegl_operation_meta_redirect (operation, "radius1", state->bevel, "radius1");
  gegl_operation_meta_redirect (operation, "bevel1", state->bevel, "bevel1");
  gegl_operation_meta_redirect (operation, "bevel2", state->bevel, "bevel2");
  gegl_operation_meta_redirect (operation, "azimuth", state->bevel, "azimuth");
  gegl_operation_meta_redirect (operation, "tile_saturation", state->cubismglow, "tile-saturation");
  gegl_operation_meta_redirect (operation, "tile_size", state->cubismglow, "tile-size");
  gegl_operation_meta_redirect (operation, "seed", state->cubismglow, "seed");
  gegl_operation_meta_redirect (operation, "blurshadowimage", state->blurshadowimage, "std-dev-x");
  gegl_operation_meta_redirect (operation, "blurshadowimage", state->blurshadowimage, "std-dev-y");
  gegl_operation_meta_redirect (operation, "stroke", state->median, "radius");
  gegl_operation_meta_redirect (operation, "blurstroke", state->gaussian, "std-dev-x");
  gegl_operation_meta_redirect (operation, "blurstroke", state->gaussian, "std-dev-y");
  gegl_operation_meta_redirect (operation, "x", state->move, "x");
  gegl_operation_meta_redirect (operation, "y", state->move, "y");
  gegl_operation_meta_redirect (operation, "grow_shape", state->median, "neighborhood");
  gegl_operation_meta_redirect (operation, "opacity", state->opacity, "value");
  gegl_operation_meta_redirect (operation, "opacitybevel", state->opacitybevel, "value");
  gegl_operation_meta_redirect (operation, "color", state->color, "value");
  gegl_operation_meta_redirect (operation, "src", state->layer, "src");
  gegl_operation_meta_redirect (operation, "hue", state->huelight, "hue");
  gegl_operation_meta_redirect (operation, "lightness", state->huelight, "lightness");
  gegl_operation_meta_redirect (operation, "string",  state->graph, "string");

}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class = GEGL_OPERATION_CLASS (klass);
   GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);

  operation_class->attach = attach;
  operation_meta_class->update = update_graph;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:zzstrokebevelimage",
    "title",       _("Hidden Operation to outline, shadow, glow in GE"),
    "categories",  "hidden",
    "reference-hash", "33234v25str2ac",
    "description", _("Hidden Operation for GEGL Effects - This is a rough reimagination of Gimp's drop shadow filter with many new things and technical options that drop shadow was not capable of doing. GEGL Effects no longer calls Gimp's GEGL drop shadow filter. It uses this under the hood."
                     ""),
    NULL);
}
 /* This operation is hidden from Gimp's GUI because the "categories" is set to "hidden".  */
#endif
