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
 * 2022 Beaver (GEGL rock text)
 */

/*
GEGL Graph recreation of Rock Text so users can test without installing.
This may not be 100% accurate but it is close enough. 

#rock text
median-blur radius=10 alpha-percentile=70
noise-spread amount-x=22 amount-y=42
gaussian-blur std-dev-y=2.5 std-dev-x=2.5
color-overlay value=#ff8100
id=1
src-atop aux=[ ref=1 layer ]
gimp:layer-mode layer-mode=multiply aux=[ ref=1 emboss depth=12 elevation=9 azimuth=4 ]
gimp:threshold-alpha
unsharp-mask scale=1.2

Yes, Rock Text and Rock Text 2 should co-exist and they are different filters.
 */


#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES


property_int  (size, _("Internal Median Blur Radius"), 1)
  value_range (0, 10)
  ui_range    (0, 10)
  ui_meta     ("unit", "pixel-distance")
  description (_("Neighborhood radius, a negative value will calculate with inverted percentiles"))


property_double  (alpha_percentile, _("Internal Median Blur Alpha percentile"), 48)
  value_range (0, 100)
  description (_("Neighborhood alpha percentile"))


property_int    (amountx, _("Rockification Horizontal"), 30)
    description (_("Horizontal spread amount"))
    value_range (0, 112)
    ui_meta     ("unit", "pixel-distance")
    ui_meta     ("axis", "x")

property_int    (amounty, _("Rockification Vertical"), 22)
    description (_("Vertical spread amount"))
    value_range (0, 112)
    ui_meta     ("unit", "pixel-distance")
    ui_meta     ("axis", "y")

property_seed (seed, _("Random seed"), rand)
    description(_("Seed of the Rock Text"))


property_double (gaussian, _("Internal Gaussian Blur"), 1.8)
   description (_("Gaussian Blur to expand the rock text"))
   value_range (0.24, 4.0)
   ui_range    (0.24, 4.0)
   ui_gamma    (3.0)
   ui_meta     ("unit", "pixel-distance")
   ui_meta     ("axis", "x")

property_int  (shift, _("Horizontal Shift"), 3)
    description(_("Apply the shift filter on the rock text"))
    value_range (0, 10)
    ui_meta    ("unit", "pixel-distance")

property_seed (seed2, _("Random seed"), rand2)
    description(_("Another Seed of the rock text"))


property_int  (size2, _("Internal Median Blur Radius"), 5)
  value_range (0, 10)
  ui_range    (0, 10)
  ui_meta     ("unit", "pixel-distance")
  description (_("Neighborhood radius, a negative value will calculate with inverted percentiles"))

property_double (azimuth, _("Light Rotation"), 180.0)
    description (_("Light angle (degrees)"))
    value_range (0, 360)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")

property_double (elevation, _("Elevation of Emboss"), 97.0)
    description (_("Elevation angle (degrees)"))
    value_range (0, 180)
    ui_meta ("unit", "degree")



property_color  (mvalue, _("Color Overlay (for white text)"), "#ffffff")
    description (_("This uses the multiply blend mode so it will only work proper on white and grayscale text."))

property_file_path(src, _("Optional Image file overlay"), "")
    description (_("Source image file path (png, jpg, raw, svg, bmp, tif, ...)"))


property_double (x, _("X outline"), 1.0)
  description   (_("Horizontal shadow offset"))
  ui_range      (-6.0, 6.0)
  ui_steps      (1, 10)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "x")

property_double (y, _("Y outline"), 1.0)
  description   (_("Vertical shadow offset"))
  ui_range      (-6.0, 6.0)
  ui_steps      (1, 10)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "y")

property_double (radius, _("Outline Blur radius"), 0.5)
  value_range   (0.0, G_MAXDOUBLE)
  ui_range      (0.0, 1.0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")

property_double (grow_radius, _("Outline Grow radius"), 0)
  value_range   (-5.0, 5.0)
  ui_range      (-5.0, 5.0)
  ui_digits     (0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the shadow before blurring; a negative value will contract the shadow instead"))

property_color  (color, _("Outline Color"), "black")
    /* TRANSLATORS: the string 'black' should not be translated */
  description   (_("The shadow's color (defaults to 'black')"))

/* It does make sense to sometimes have opacities > 1 (see GEGL logo
 * for example)
 */
property_double (opacity, _("Outline Opacity"), 0.2)
  value_range   (0.0, 2.0)
  ui_steps      (0.01, 0.10)

property_double (exposure, _("Darkness to light"), 0.5)
    description (_("Exposure's darkness to light"))
    ui_range    (-2.0, 2.0)



#else

#define GEGL_OP_META
#define GEGL_OP_NAME     rock_text
#define GEGL_OP_C_SOURCE rock-text.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *color, *median, *noise, *gaussian, *shift, *median2, *imagefileupload, *nop, *coloroverlay, *emboss, *alpha, *mcol, *image, *outline, *exposure,  *output;
  GeglColor *rock_hidden_color = gegl_color_new ("#ffffff");

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");


  color    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay", "value", rock_hidden_color,
                                  NULL);

  median    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur",
                                  NULL);


  median2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur",
                                  NULL);


  gaussian    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
                                  NULL);

  noise    = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-spread",
                                  NULL);



  alpha    = gegl_node_new_child (gegl,
                                  "operation", "gimp:threshold-alpha",
                                  NULL);

  shift   = gegl_node_new_child (gegl,
                                  "operation", "gegl:shift",
                                  NULL);

 emboss    = gegl_node_new_child (gegl,
                                  "operation", "gegl:emboss",
                                  NULL);

 mcol    = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);

 image    = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);

 nop    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);



 imagefileupload    = gegl_node_new_child (gegl,
                                  "operation", "gegl:layer",
                                  NULL);

 coloroverlay    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay", 
                                  NULL);


 exposure    = gegl_node_new_child (gegl,
                                  "operation", "gegl:exposure",
                                  NULL);

 outline    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow",
                                  NULL);



  gegl_operation_meta_redirect (operation, "coloroverlay", color, "value");
  gegl_operation_meta_redirect (operation, "size", median, "radius");
  gegl_operation_meta_redirect (operation, "shift", shift, "shift");
  gegl_operation_meta_redirect (operation, "seed2", shift, "seed");
  gegl_operation_meta_redirect (operation, "amountx", noise, "amount-x");
  gegl_operation_meta_redirect (operation, "amounty", noise, "amount-y");
  gegl_operation_meta_redirect (operation, "seed", noise, "seed");
  gegl_operation_meta_redirect (operation, "size2", median2, "radius");
  gegl_operation_meta_redirect (operation, "gaussian", gaussian, "std-dev-x");
  gegl_operation_meta_redirect (operation, "gaussian", gaussian, "std-dev-y");
  gegl_operation_meta_redirect (operation, "azimuth", emboss, "azimuth");
  gegl_operation_meta_redirect (operation, "elevation", emboss, "elevation");
  gegl_operation_meta_redirect (operation, "alpha-percentile", median, "alpha-percentile");
  gegl_operation_meta_redirect (operation, "radius", outline, "radius");
  gegl_operation_meta_redirect (operation, "opacity", outline, "opacity");
  gegl_operation_meta_redirect (operation, "x", outline, "x");
  gegl_operation_meta_redirect (operation, "y", outline, "y");
  gegl_operation_meta_redirect (operation, "grow_radius", outline, "grow-radius");
  gegl_operation_meta_redirect (operation, "color", outline, "color");
  gegl_operation_meta_redirect (operation, "exposure", exposure, "exposure");
  gegl_operation_meta_redirect (operation, "mvalue", coloroverlay, "value");
  gegl_operation_meta_redirect (operation, "src", imagefileupload, "src");

  gegl_node_link_many (input, color, median, noise, gaussian, shift, median2, emboss, alpha, image, outline, exposure, nop, mcol, output, NULL);
  gegl_node_connect_from (image, "aux", imagefileupload, "output"); 
  gegl_node_connect_from (mcol, "aux", coloroverlay, "output"); 
  gegl_node_link_many (nop, coloroverlay, NULL);

}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "gegl:rock-text",
    "title",       _("Rock Text"),
    "categories",  "Aristic",
    "reference-hash", "30do6a1h24f10jcjf25sb2ac",
    "description", _("Rock Text with GEGL. Color and Image overlay work best if the rock text is white.  "
                     ""),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Rock Text (original)..."),
    NULL);
}

#endif
