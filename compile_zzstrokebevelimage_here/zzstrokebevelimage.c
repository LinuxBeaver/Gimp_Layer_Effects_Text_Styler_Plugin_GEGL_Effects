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
 * 2023 Beaver (GEGL ZZStrokeBevelImageOverlay a hidden operation to give GEGL Effects outline the ability to add a bevel, image file overlay and more )
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES



property_boolean (specialoutline, _("Special Outline Switch"), FALSE)
  description    (_("Turn on special outline abilities"))

property_boolean (enableoutline, _("Enable normal outline switch"), TRUE)
  description    (_("Enable Outline"))


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
  ui_range      (-15.0, 15.0)
  ui_steps      (1, 10)
  ui_meta       ("axis", "x")

property_double (y, _("Y"), 0.0)
  description   (_("Vertical shadow offset"))
  ui_range      (-15.0, 15.0)
  ui_steps      (1, 10)
  ui_meta       ("axis", "y")


property_double (blurstroke, _("Blur radius"), 0.0)
  value_range   (0.0, 3)
  ui_range      (0.0, 3.0)
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
  GeglNode *multiply;
  GeglNode *atop;
  GeglNode *layer;
  GeglNode *stroke;
  GeglNode *color;
  GeglNode *nop;
  GeglNode *huelight;
  GeglNode *output;
} State; 


static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node; 
  GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglNode *input, *output, *behind, *nop, *huelight, *bevel, *multiply, *atop, *layer, *stroke,  *color;


  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");


  color   = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);


  nop   = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);


  behind   = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst-over",
                                  NULL);


  stroke    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow",
                                  NULL);

  multiply    = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);

  atop    = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);

  layer    = gegl_node_new_child (gegl,
                                  "operation", "gegl:layer",
                                  NULL);

  bevel    = gegl_node_new_child (gegl,
                                  "operation", "gegl:bevel",
                                  NULL);

  huelight    = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);


  gegl_node_link_many (input, behind, output, NULL);
  gegl_node_link_many (input, stroke, color, atop, multiply, NULL);
  /*gegl_node_link (layer, NULL);*/
  gegl_node_link_many (atop, bevel, NULL);

 gegl_node_connect_from (multiply, "aux", bevel, "output"); 
 gegl_node_connect_from (atop, "aux", layer, "output"); 
 gegl_node_connect_from (behind, "aux", multiply, "output"); 




  gegl_operation_meta_redirect (operation, "stroke", stroke, "grow-radius");

  gegl_operation_meta_redirect (operation, "blurstroke", stroke, "radius");

  gegl_operation_meta_redirect (operation, "x", stroke, "x");

  gegl_operation_meta_redirect (operation, "y", stroke, "y");

  gegl_operation_meta_redirect (operation, "grow_shape", stroke, "grow-shape");

  gegl_operation_meta_redirect (operation, "opacity", stroke, "opacity");

  gegl_operation_meta_redirect (operation, "color", color, "value");

  gegl_operation_meta_redirect (operation, "radius1", bevel, "radius1");

  gegl_operation_meta_redirect (operation, "th", bevel, "th");

  gegl_operation_meta_redirect (operation, "bevel1", bevel, "bevel1");

  gegl_operation_meta_redirect (operation, "bevel2", bevel, "bevel2");

  gegl_operation_meta_redirect (operation, "azimuth", bevel, "azimuth");

  gegl_operation_meta_redirect (operation, "src", layer, "src");

  gegl_operation_meta_redirect (operation, "hue", huelight, "hue");

  gegl_operation_meta_redirect (operation, "lightness", huelight, "lightness");





 /* Now save points to the various gegl nodes so we can rewire them in
   * update_graph() later
   */
  State *state = g_malloc0 (sizeof (State));
  state->input = input;
  state->behind = behind;
  state->bevel = bevel;
  state->multiply = multiply;
  state->atop = atop;
  state->layer = layer;
  state->stroke = stroke;
  state->color = color;
  state->nop = nop;
  state->huelight = huelight;
  state->output = output;
  o->user_data = state;
}

static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;


 if (o->enableoutline)
 if (o->specialoutline)
  {
    gegl_node_link_many (state->input, state->behind, state->output, NULL);
    gegl_node_link_many (state->input, state->stroke, state->color, state->atop, state->multiply,  NULL);
    gegl_node_link_many (state->nop, state->layer, state->huelight,  NULL); 
    gegl_node_link_many (state->atop, state->bevel,  NULL);
    gegl_node_connect_from (state->multiply, "aux", state->bevel, "output");
    gegl_node_connect_from (state->atop, "aux", state->huelight, "output");
    gegl_node_connect_from (state->behind, "aux", state->multiply, "output");
  }
else
  {
    gegl_node_link_many (state->input, state->behind, state->output, NULL);
    gegl_node_link_many (state->input, state->stroke, state->color, NULL);
     gegl_node_connect_from (state->behind, "aux", state->color, "output");
  }
else
  {
    gegl_node_link_many (state->input, state->output, NULL);
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
    "title",       _("Hidden Operation to bevel and image file overlay a stroke for GEGL Effects"),
    "categories",  "hidden",
    "reference-hash", "33234v25str2ac",
    "description", _("Hidden Operation for testing on GEGL Effects"
                     ""),
    NULL);
}

#endif
