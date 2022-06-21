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
 * 2022 Beaver (GEGL inner glow)
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

property_double (x, _("X"), 1.0)
  description   (_("Horizontal shadow offset"))
  ui_range      (-2.0, 2.0)
  value_range   (-2.0, 2.0)
  ui_steps      (1, 2)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "x")

property_double (y, _("Y"), 1.0)
  description   (_("Vertical shadow offset"))
  ui_range      (-2.0, 2.0)
  value_range   (-2.0, 2.0)
  ui_steps      (1, 104)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "y")



property_double (radius, _("Blur radius"), 9.0)
  value_range   (0.0, 30.0)
  ui_range      (0.0, 30.0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")


property_double (grow_radius, _("Grow radius"), 8.0)
  value_range   (2, 30.0)
  ui_range      (2, 30.0)
  ui_digits     (0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the shadow before blurring; a negative value will contract the shadow instead"))

property_double (opacity, _("Opacity"), 2)
  value_range   (0.0, 2.0)
  ui_steps      (0.01, 0.10)


property_color  (notouch, _("Don't touch. Needed for filter to work"), "#")


property_color (value, _("Color"), "#fbff00")
    description (_("The color to paint over the input"))
    ui_meta     ("role", "color-primary")



#else

#define GEGL_OP_META
#define GEGL_OP_NAME     innerglow
#define GEGL_OP_C_SOURCE innerglow.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *it, *shadow, *c2a, *white, *color, *nop, *atop, *output;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");



  it    = gegl_node_new_child (gegl,
                                  "operation", "gegl:it",
                                  NULL);

  shadow    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow",
                                  NULL);

  c2a    = gegl_node_new_child (gegl,
                                  "operation", "gegl:zzeraseblack",
                                  NULL);

  color    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);

  atop    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst-atop",
                                  NULL);

  white    = gegl_node_new_child (gegl,
                                  "operation", "gegl:zzwhite",
                                  NULL);

  nop    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);


gegl_operation_meta_redirect (operation, "grow_radius", shadow, "grow-radius");

gegl_operation_meta_redirect (operation, "radius", shadow, "radius");

  gegl_operation_meta_redirect (operation, "opacity", shadow, "opacity");


  gegl_operation_meta_redirect (operation, "value", color, "value");

gegl_operation_meta_redirect (operation, "x", shadow, "x");

gegl_operation_meta_redirect (operation, "y", shadow, "y");

gegl_operation_meta_redirect (operation, "notouch", shadow, "color");





  gegl_node_link_many (input, nop, it, shadow, white, color, atop, output, NULL);
 gegl_node_connect_from (atop, "aux", nop, "output");


}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "gegl:inner-glow",
    "title",       _("Inner Glow"),
    "categories",  "hidden",
    "reference-hash", "a3do6a1h11100g0fjf25sb2ac",
    "description", _("GEGL does an inner glow effect.  "
                     ""),
    NULL);
}

#endif
