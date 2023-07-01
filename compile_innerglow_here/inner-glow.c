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

/*GEGL Inner Glow is a stand alone plugin but it is also part of GEGL Effects. The stand alone version does more then the GEGL Effects implementation of it. */

/*
Recreation of Inner Glow's GEGL Graph. May not be 100% accurate but you can test it without installing this way.

id=0 src-in aux=[ ref=0 id=1 dst-atop   aux=[  ref=1 distance-transform  ] xor srgb=true     aux=[ ref=1 ] color-overlay value=#000000 dropshadow x=0 y=0 grow-radius=3 radius=3 color-overlay value=#ff0000  ]
median-blur radius=3 alpha-percentile=94 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES


/*This is GEGL syntax I wrote. Its operation is gegl:gegl and it represents a GEGL Graph inside a GEGL Graph. */
#define GRAPHUSEDBYINNERGLOW \
"   id=1 dst-atop   aux=[  ref=1 distance-transform  ] xor srgb=true     aux=[ ref=1 ] color-overlay value=#000000   n"\

/*On June 24 2023 I finally figured out how to bake in GEGL Graphs*/

/*This is an enum list for the base shape of inner stroke/shadow/glow. It should not be renamed.*/
enum_start (gegl_dropshadow_grow_shapeig)
  enum_value (GEGL_DROPSHADOW_GROW_SHAPE_SQUAREig,  "squareig",  N_("Square"))
  enum_value (GEGL_DROPSHADOW_GROW_SHAPE_CIRCLEig,  "circleig",  N_("Circle"))
  enum_value (GEGL_DROPSHADOW_GROW_SHAPE_DIAMONDig, "diamondig", N_("Diamond"))
enum_end (GeglDropshadowGrowShapeig)


property_enum   (grow_shape, _("Grow shape"),
                 GeglDropshadowGrowShapeig, gegl_dropshadow_grow_shapeig,
                 GEGL_DROPSHADOW_GROW_SHAPE_CIRCLEig)
  description   (_("The shape to expand or contract the shadow in"))



property_double (x, _("X"), 0.0)
  description   (_("Horizontal shadow offset"))
  ui_range      (-20.0, 20.0)
  value_range   (-20.0, 20.0)
  ui_steps      (1, 2)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "x")

property_double (y, _("Y"), 0.0)
  description   (_("Vertical shadow offset"))
  ui_range      (-20.0, 20.0)
  value_range   (-20.0, 20.0)
  ui_steps      (1, 2)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "y")



property_double (radius, _("Blur radius"), 9)
  value_range   (0.0, 60.0)
  ui_range      (0.0, 40.0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")


property_double (grow_radius, _("Grow radius"), 4.0)
  value_range   (1, 40.0)
  ui_range      (1, 30.0)
  ui_digits     (0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the shadow before blurring; a negative value will contract the shadow instead"))

property_double (opacity, _("Opacity"), 1.2)
  value_range   (0.0, 2.0)
  ui_steps      (0.01, 0.10)


property_color (value2, _("Color"), "#fbff00")
    description (_("The color to paint over the input"))
    ui_meta     ("role", "color-primary")


property_double  (fixoutline, _("Median to fix non-effected pixels on edges"), 60)
  value_range (50, 100)
  description (_("Due to a bug I can't solve, not all pixels will be effected by inner glow. Median blur solves that problem.'"))


#else

#define GEGL_OP_META
#define GEGL_OP_NAME     inner_glow
#define GEGL_OP_C_SOURCE inner-glow.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *it, *shadow,   *color, *color2,  *atop, *median2, *in,  *output;
  GeglColor *hidden_color = gegl_color_new ("#00ffffAA");
/*This is a trick to bake in color nodes*/



  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");



  it    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", GRAPHUSEDBYINNERGLOW,
                                  NULL);
/*On June 24 2023 I finally figured out how to bake in GEGL Graphs*/

  in    = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-in",
                                  NULL);


  shadow    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow",
                                  NULL);

  color    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                   "value", hidden_color, NULL);
/*This is a trick to bake in color nodes*/

  color2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);



  atop    = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);


  median2     = gegl_node_new_child (gegl, "operation", "gegl:median-blur",
                                         "radius",       1,
                                         NULL);

gegl_operation_meta_redirect (operation, "grow_radius", shadow, "grow-radius");
gegl_operation_meta_redirect (operation, "radius", shadow, "radius");
gegl_operation_meta_redirect (operation, "opacity", shadow, "opacity");
gegl_operation_meta_redirect (operation, "grow-shape", shadow, "grow-shape");
gegl_operation_meta_redirect (operation, "value2", color2, "value");
gegl_operation_meta_redirect (operation, "x", shadow, "x");
gegl_operation_meta_redirect (operation, "y", shadow, "y");
gegl_operation_meta_redirect (operation, "fixoutline", median2, "alpha-percentile");
gegl_operation_meta_redirect (operation, "string", it, "string");

  gegl_node_link_many (input, it,  shadow, color, atop, in, median2, color2, output, NULL);
 gegl_node_connect_from (in, "aux", input, "output");
/*This is telling GEGL to put everything from input to atop inside the gegl:src-in blend mode. Src-In is like a combination between Gimp's alpha lock and replace blend mode.*/
/*Out of all the operations that ship with GEGL Effects. Inner Glow is the only one that has a simple GEGL Graph and it is the only one that is pure GEGL. As of June 7th 2023 */

}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:innerglow",
    "title",       _("Inner Glow (to blend)"),
    "categories",  "Aristic",
    "reference-hash", "g3do6aaoo1100g0fjf25sb2ac",
    "description", _("GEGL does an inner glow effect. For this filter to work you need to use the 'Normal' or other blending options. Or a duplicate layer on top.  "
                     ""),
    NULL);
}

#endif
