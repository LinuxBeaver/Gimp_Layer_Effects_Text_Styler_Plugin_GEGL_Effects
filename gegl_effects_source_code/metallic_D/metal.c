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
 * 2022 Beaver (GEGL metallic)
 */

/*
June 25 2023- Recreation of Metallic's GEGL Graph.
alien-map cpn-1-frequency=6 cpn-2-frequency=5.9 cpn-3-frequency=3
hue-chroma lightness=-5
noise-reduction iterations=3
saturation scale=0
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES


property_double (solar1, _("Solarization of Red Channel"), 2.7)
  value_range (0, 4)
  ui_meta     ("sensitive", "! cpn-1-keep")
  ui_meta     ("label", "[color-model {rgb} : rgb-label,"
                        " color-model {hsl} : hsl-label]")
  ui_meta     ("rgb-label", _("Red frequency"))
  ui_meta     ("hsl-label", _("Hue frequency"))
  ui_steps      (0.100, 1.00)

property_double  (solar2, _("Solarization of Green Channel"), 2.8)
  value_range (2.2, 4)
  ui_meta     ("sensitive", "! cpn-2-keep")
  ui_meta     ("label", "[color-model {rgb} : rgb-label,"
                        " color-model {hsl} : hsl-label]")
  ui_meta     ("rgb-label", _("Green frequency"))
  ui_meta     ("hsl-label", _("Saturation frequency"))
  ui_steps      (0.100, 1.00)

property_double  (solar3, _("Solarization of Blue Channel"), 2.1)
  value_range (0, 4)
  ui_meta     ("sensitive", "! cpn-3-keep")
  ui_meta     ("label", "[color-model {rgb} : rgb-label,"
                        " color-model {hsl} : hsl-label]")
  ui_meta     ("rgb-label", _("Blue frequency"))
  ui_meta     ("hsl-label", _("Lightness frequency"))
  ui_steps      (0.100, 1.00)


property_double (light, _("Lightness"), 0.0)
   description  (_("Lightness adjustment"))
   value_range  (-10.0, 10.0)
  ui_steps      (0.200, 1.00)

property_int  (smooth, _("Smoothness"), 2)
  description (_("Smoothness"))
  value_range (0, 8)
  ui_range    (0, 8)

property_double (value, _("Slide to invert colors on original image"), 0)
    description (_("Invert with Slider"))
    value_range (0, 1)
  ui_steps      (1.00, 1.00)
	

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     metal
#define GEGL_OP_C_SOURCE metal.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *solar, *desat, *smooth, *light, *invert, *opacity, *over;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");


  solar = gegl_node_new_child (gegl,
                                  "operation", "gegl:alien-map",
                                  NULL);


  desat = gegl_node_new_child (gegl,
                                  "operation", "gegl:saturation", "scale", 0.0,
                                  NULL);


  smooth = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-reduction",
                                  NULL);


   light = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);


  opacity = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

   invert = gegl_node_new_child (gegl,
                                  "operation", "gegl:invert-gamma",
                                  NULL);

   over = gegl_node_new_child (gegl,
                                  "operation", "gegl:over",
                                  NULL);

  gegl_operation_meta_redirect (operation, "solar1", solar, "cpn-1-frequency");
  gegl_operation_meta_redirect (operation, "solar2", solar, "cpn-2-frequency");
  gegl_operation_meta_redirect (operation, "solar3", solar, "cpn-3-frequency");
  gegl_operation_meta_redirect (operation, "light", light, "lightness");
  gegl_operation_meta_redirect (operation, "smooth", smooth, "iterations");
  gegl_operation_meta_redirect (operation, "value", opacity, "value");


  gegl_node_link_many (input, over, solar, desat, smooth, light, output, NULL);
  gegl_node_connect_from (over, "aux", opacity, "output");
  gegl_node_link_many (input, invert, opacity, NULL);

}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "gegl:metallic",
    "title",       _("Metal Effect"),
    "categories",  "Aristic",
    "reference-hash", "45ed5656h28ff20fjf25sb2ac",
    "description", _("Metalize subjects and objects with GEGL (This filter works best on human skin).  "
                     ""),
    NULL);
}

#endif
