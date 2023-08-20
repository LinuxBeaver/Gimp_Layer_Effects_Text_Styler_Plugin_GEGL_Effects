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
 * 2022 GEGL chrome metal text styler - Beaver
 */

/*
Test Plugins GEGL graph without installing. REQUIRES BEVEL AND EDGE SMOOTH

color-overlay value=#ffb386
lb:bevel azimuth=60 bevel1=58 th=0.210 radius2=2  radius1=7 bevel2=
metallic  solar1=27 solar2=2.8 solar3=2.1 
noise-reduction iterations=2
bloom strength=2

lb:edgesmooth
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

enum_start (guichangeenumchrometextlist)
enum_value   (CHROMETEXT_SHOW_DEFAULT, "defaultchrometext", N_("Basic Metal Sliders"))
enum_value   (CHHROMETEXT_SHOW_ADVANCE, "advancechrometext", N_("Advance Metal Sliders and Edge Smooth"))
  enum_end (guiendchrometextlist)

property_enum(guichange, _("Part of filter displayed"),
    guiendchrometextlist, guichangeenumchrometextlist,
    CHROMETEXT_SHOW_DEFAULT)
  description(_("Change the GUI option"))


property_double (bvradius, _("Radius of bevel"), 7.0)
    description (_("This setting should be higher for larger text and lower for smaller text"))
  value_range (5.0, 15.0)
  ui_range (5.0, 9)
  ui_gamma (1.5)

property_double (bevelconfig, _("Depth Angle"), 58.0)
    description (_("Emboss Elevation, Larger text should have lower values and smaller text can benefit from higher values."))
    value_range (45, 80)
    ui_meta ("unit", "degree")


property_double (azimuth, _("Rotate Lighting"), 60.0)
    description (_("Light angle (degrees)"))
    value_range (28, 350)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")

property_double (solar1, _("Solarization of Red Channel"), 2.7)
    description (_("Gimp's alien map filter adjusting Red Channel"))
  value_range (2, 2.8)
  ui_meta     ("sensitive", "! cpn-1-keep")
  ui_meta     ("label", "[color-model {rgb} : rgb-label,"
                        " color-model {hsl} : hsl-label]")
  ui_meta     ("rgb-label", _("Red frequency"))
  ui_meta     ("hsl-label", _("Hue frequency"))

property_double  (solar2, _("Solarization of Green Channel"), 2.8)
  value_range (2.2, 2.8)
    description (_("Gimp's alien map filter adjusting Green Channel"))
  ui_meta     ("sensitive", "! cpn-2-keep")
  ui_meta     ("label", "[color-model {rgb} : rgb-label,"
                        " color-model {hsl} : hsl-label]")
  ui_meta     ("rgb-label", _("Green frequency"))
  ui_meta     ("hsl-label", _("Saturation frequency"))

property_double  (solar3, _("Solarization of Blue Channel"), 2.1)
    description (_("Gimp's alien map filter adjusting Blue Channel"))
  value_range (0.0, 4.0)
  ui_meta     ("sensitive", "! cpn-3-keep")
  ui_meta     ("label", "[color-model {rgb} : rgb-label,"
                        " color-model {hsl} : hsl-label]")
  ui_meta     ("rgb-label", _("Blue frequency"))
  ui_meta     ("hsl-label", _("Lightness frequency"))

property_double (lightmetal, _("Darkness to Light"), 0.0)
   description  (_("Lightness adjustment"))
   value_range  (-10.0, 7.0)

property_int  (smoothmetal, _("Smooth metal"), 2)
  description (_("Smoothness via noise reduction"))
  value_range (0, 7)
  ui_range    (0, 7)
ui_meta ("visible", "guichange {advancechrometext}")

property_double (glow, _("Glow effect (bloom)"), 6.0)
    description (_("Glow strength via gegl:bloom"))
    value_range (0.0, G_MAXDOUBLE)
    ui_range    (0.0, 14.0)
ui_meta ("visible", "guichange {advancechrometext}")

property_double (softglow, _("Soft glow effect"), 3.0)
    description (_("Glow strength via gegl:softglow"))
    value_range (1.0, 7.0)
    ui_meta    ("unit", "pixel-distance")
ui_meta ("visible", "guichange {advancechrometext}")

property_double (sharpen, _("Sharpen Metal"), 0.2)
    description(_("Scaling factor for unsharp-mask, the strength of effect"))
    value_range (0.0, 0.9)
    ui_range    (0.0, 0.9)
    ui_gamma    (3.0)
ui_meta ("visible", "guichange {advancechrometext}")

property_double  (smoothedge, _("Median to smooth rough edges"), 50)
    description(_("Median blur on the edges to smooth them"))
  value_range (0, 100)
  description (_("Neighborhood alpha percentile"))
ui_meta ("visible", "guichange {advancechrometext}")

property_double (opacity, _("Hyper Opacity for edges"), 1.0)
    description (_("Above 100% opacity for edges"))
    value_range (1, 3)
    ui_range    (1, 3)
ui_meta ("visible", "guichange {advancechrometext}")

property_double (th, _("Threshold Alpha of Bevel"), 0.210)
    description(_("Bevel's threshold Alpha"))
  value_range (0.195, 0.230)
  ui_range (0.195, 0.230)
ui_meta ("visible", "guichange {advancechrometext}")



#else

#define GEGL_OP_META
#define GEGL_OP_NAME     geglchrome
#define GEGL_OP_C_SOURCE geglchrome.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *color, *bevel, *metallic, *glow, *sharpen, *softglow, *noisereduction, *smoothedge, *opacity;
  GeglColor *hidden_color_chrome_text = gegl_color_new ("#ffb386");

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");


  color    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                   "value", hidden_color_chrome_text, NULL);
                           

  bevel    = gegl_node_new_child (gegl,
                                  "operation", "lb:bevel",
                                  NULL);

  metallic    = gegl_node_new_child (gegl,
                                  "operation", "gegl:metallic",
                                  NULL);

  glow    = gegl_node_new_child (gegl,
                                  "operation", "gegl:bloom",
                                  NULL);

  sharpen    = gegl_node_new_child (gegl,
                                  "operation", "gegl:unsharp-mask",
                                  NULL);

  softglow    = gegl_node_new_child (gegl,
                                  "operation", "gegl:softglow",
                                  NULL);

  noisereduction    = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-reduction",
                                  NULL);

  smoothedge    = gegl_node_new_child (gegl,
                                  "operation", "lb:edgesmooth",
                                  NULL);

  opacity    = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

  gegl_node_link_many (input, color, bevel, metallic, glow, sharpen, softglow, noisereduction, smoothedge, opacity, output, NULL);

  gegl_operation_meta_redirect (operation, "value", color, "value");
  gegl_operation_meta_redirect (operation, "th", bevel, "th");
  gegl_operation_meta_redirect (operation, "azimuth", bevel, "azimuth");
  gegl_operation_meta_redirect (operation, "bvradius", bevel, "radius1");
  gegl_operation_meta_redirect (operation, "bevelconfig", bevel, "bevel1");
  gegl_operation_meta_redirect (operation, "solar1", metallic, "solar1");
  gegl_operation_meta_redirect (operation, "solar2", metallic, "solar2");
  gegl_operation_meta_redirect (operation, "solar3", metallic, "solar3");
  gegl_operation_meta_redirect (operation, "lightmetal", metallic, "light");
  gegl_operation_meta_redirect (operation, "smoothmetal", metallic, "smooth");
  gegl_operation_meta_redirect (operation, "glow", glow, "strength");
  gegl_operation_meta_redirect (operation, "sharpen", sharpen, "scale");
  gegl_operation_meta_redirect (operation, "softglow", softglow, "glow-radius");
  gegl_operation_meta_redirect (operation, "smoothedge", smoothedge, "alpha-percentile2");
  gegl_operation_meta_redirect (operation, "opacity", opacity, "value");
 
}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "gegl:chrome",
    "title",       _("Chrome metal text styler"),
    "categories",  "artistic",
    "reference-hash", "450056eat8b351dt25700ftevfe2g4fonf1c",
    "description", _(""
                     "Make metal chrome text styling effects with GEGL. Works best on 100-500p text. Fails on very small text and excessively wide fonts."),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Chrome Text..."),
    NULL);
}

#endif
