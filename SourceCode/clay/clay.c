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
 * 2022 Beaver (GEGL clay bevel)
 */
/*
Recreation of GEGL Graph from a early 2022 study. May not be 100% accurate.
Run this in Gimp's GEGL Graph filter to test without installing.

gegl:color-overlay value=#ffec00
median-blur percentile=50 alpha-percentile=2
gaussian-blur #stays locked at 0.50 in default. Never changes.
emboss  type=bumpmap azimuth=70  elevation=40 depth=3
median-blur radius=10 percentile=39 alpha-percentile=80 high-precision=yes
opacity value=2
median-blur radius=0
 */


#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES


#define TUTORIAL \
" median-blur percentile=80 alpha-percentile=2 gaussian-blur std-dev-x=1.50 std-dev-y=1.50 "\

enum_start (gegl_emboss_typex)
  enum_value (GEGL_EMBOSS_TYPE_EMBOSSx,  "embossx",  N_("Recolor and Image file overlay mode"))
  enum_value (GEGL_EMBOSS_TYPE_BUMPMAPx, "bumpmapx", N_("Preserve original color mode"))
enum_end (GeglEmbossTypex)

property_enum (type, _("Emboss Type"),
               GeglEmbossTypex, gegl_emboss_typex, GEGL_EMBOSS_TYPE_BUMPMAPx)
    description(_("Rendering type"))


property_double (opacity, _("Widness of Bevel"), 7.0)
    description (_("Global opacity to make clay wider or"))
    value_range (1.0, 10.0)
    ui_range    (1.5, 10.0)


property_double (azimuth, _("Azimuth"), 50.0)
    description (_("Light angle (degrees)"))
    value_range (30.0, 90.0)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")

property_double (elevation, _("Elevation"), 32.0)
    description (_("Elevation angle (degrees). This along with gaussian blur is primarily what brings out the clay effect. "))
    value_range (25.0, 90.0)
    ui_meta ("unit", "degree")

property_int (depth, _("Depth"), 36)
    description (_("Bevel Filter width. This will Make Bevel Darker"))
    value_range (1, 100)

property_int  (size, _("Internal Median Blur Radius"), 3)
  value_range (-10, 10)
  ui_range    (-10, 10)
  ui_meta     ("unit", "pixel-distance")
  description (_("Neighborhood radius, a negative value will calculate with inverted percentiles"))

property_double  (percentile, _("Internal Median Blur Percentile"), 65.0)
  value_range (20.0, 80.0)
  description (_("Neighborhood color percentile"))

property_double  (alpha_percentile, _("Internal Median Blur Alpha percentile"), 75.0)
  value_range (21.0, 100.0)
  description (_("Neighborhood alpha percentile"))

property_double (gaus, _("Expand Bevel"), 3.0)
   description (_("An internal Gaussian blur to expand the bevel. Larger text can benefit from a large gaussian, On small text keep this low."))
   value_range (0.0, 12.0)
   ui_range    (0.0, 7.0)

property_double (lightness, _("Lightness"), 0.0)
   description  (_("Lightness adjustment"))
   value_range  (-18.0, 18.0)


property_file_path(src, _("Image file Overlay - Works best with Recolor and Image overlay mode"), "")
    description (_("Source image file path (png, jpg, raw, svg, bmp, tif, ...)"))

property_double (hue, _("Hue rotation for Image File Overlay"),  0.0)
   description  (_("Hue adjustment for an image file uploaded. Without an image file this will do nothing"))
   value_range  (-180.0, 180.0)

property_color  (mcol, _("Recolor white text"), "#ffffff")
    description (_("If the text is white/grayscale or you are using --Recolor and Image file overlay mode-- you can make the text any color. If the text is not white it will be as if you are applying the multiply blend mode over text."))


#else

#define GEGL_OP_META
#define GEGL_OP_NAME     clay
#define GEGL_OP_C_SOURCE clay.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *col, *gray, *graph1, *emboss, *median, *median2, *gaussian, *multiply, *hue, *multiply2, *nop, *nop2, *mcol2, *median3, *lightness, *imagefileoverlay, *opacity, *repairgeglgraph, *output;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");
  GeglColor *hidden_color_clay = gegl_color_new ("#ffffff");



  median    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur",
                                  NULL);

  col    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay", "value", hidden_color_clay,
                                  NULL);

  mcol2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);

  multiply    = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);

  nop    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  nop2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  opacity   = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

  multiply2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);


  gaussian    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
   "filter", 1,
                                  NULL);

  median2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur",
                                  NULL);

  median3    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur",
                                  NULL);

  emboss    = gegl_node_new_child (gegl,
                                  "operation", "gegl:emboss",
                                  NULL);

  graph1    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", TUTORIAL,
                                  NULL);

  gray    = gegl_node_new_child (gegl,
                                  "operation", "gegl:saturation",
                                  NULL);

  imagefileoverlay    = gegl_node_new_child (gegl,
                                  "operation", "port:load",
                                  NULL);

  lightness    = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);

  hue    = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);

  repairgeglgraph      = gegl_node_new_child (gegl, "operation", "gegl:median-blur",
                                         "radius",       0,
                                         NULL);

 /*Repair GEGL Graph is a critical operation for Gimp's non-destructive future.
A median blur at zero radius is confirmed to make no changes to an image.
This option resets gegl:opacity's value to prevent a known bug where
plugins like clay, glossy balloon and custom bevel glitch out when
drop shadow is applied in a gegl graph below them.*/

  gegl_operation_meta_redirect (operation, "size", median, "radius");
  gegl_operation_meta_redirect (operation, "gaus", gaussian, "std-dev-x");
  gegl_operation_meta_redirect (operation, "gaus", gaussian, "std-dev-y");
  gegl_operation_meta_redirect (operation, "azimuth", emboss, "azimuth");
  gegl_operation_meta_redirect (operation, "elevation", emboss, "elevation");
  gegl_operation_meta_redirect (operation, "depth", emboss, "depth");
  gegl_operation_meta_redirect (operation, "type", emboss, "type");
  gegl_operation_meta_redirect (operation, "percentile", median, "percentile");
  gegl_operation_meta_redirect (operation, "alpha-percentile", median, "alpha-percentile");
  gegl_operation_meta_redirect (operation, "mcol", mcol2, "value");
  gegl_operation_meta_redirect (operation, "src", imagefileoverlay, "src");
  gegl_operation_meta_redirect (operation, "lightness", lightness, "lightness");
  gegl_operation_meta_redirect (operation, "hue", hue, "hue");
  gegl_operation_meta_redirect (operation, "opacity", opacity, "value");


/*mcol is color overlay and it needs to be blended with multiply blend mode. I introduced multiply2 for this reason as multiply is already occupied with the imagefileoverlay node  */

  gegl_node_link_many (input, graph1, emboss, median, median2, gaussian, median3, opacity, gray, nop, multiply, lightness, nop2, multiply2, repairgeglgraph, output, NULL);
  gegl_node_connect (multiply, "aux", hue, "output");
  gegl_node_connect (multiply2, "aux", mcol2, "output");
  gegl_node_link_many (nop2, mcol2, NULL);
  gegl_node_link_many (imagefileoverlay, hue, NULL);

}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:clay",
    "title",       _("Clay Bevel"),
    "categories",  "Artistic",
    "reference-hash", "33do6a1h24fk10fjf25sb2ac",
    "description", _("Clay Bevel Text styling using GEGL. This does not well on small text and will not work at all on very dark/black text.  "
                     ""),
    NULL);
}

#endif
