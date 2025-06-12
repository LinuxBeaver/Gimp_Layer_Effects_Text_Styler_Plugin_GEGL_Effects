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
*2022 Beaver GEGL Effects First Ever released - Recreated

Syntax to recreate plugin

id=1 multiply aux=[ color value=red ] 
crop aux=[ ref=1 ]
id=2 
multiply aux=[ ref=2 bevel type=bump depth=100 ]
dropshadow x=0 y=0 radius=0.5 grow-radius=6 opacity=1 color=white
dropshadow
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

property_color  (color_fill, _("Optional Color Overlay"), "#ffffff")

property_int (depth_bevel, _("Bevel Depth"), 1)
    description (_("Bevel presence/Emboss Depth"))
    value_range (1, 100)

/* Should correspond to GeglMedianBlurNeighborhood in median-blur.c */
enum_start (gegl_stroke_grow_shape_ancient)
  enum_value (GEGL_stroke_GROW_SHAPE_SQUARE,  "square",  N_("Square"))
  enum_value (GEGL_stroke_GROW_SHAPE_CIRCLE,  "circle",  N_("Circle"))
  enum_value (GEGL_stroke_GROW_SHAPE_DIAMOND, "diamond", N_("Diamond"))
enum_end (GeglstrokeGrowShape_ancient)

property_enum   (grow_shape_stroke, _("Grow shape"),
                 GeglstrokeGrowShape_ancient, gegl_stroke_grow_shape_ancient,
                 GEGL_stroke_GROW_SHAPE_CIRCLE)
  description   (_("The shape to expand or contract the stroke in"))

property_double (radius_stroke, _("Outline's Blur radius"), 0.0)
  value_range   (0.0, 2.0)
  ui_range      (0.0, 2.0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")


property_int (grow_radius_stroke, _("Outline's Grow radius"), 12)
  value_range   (0, 100)
  ui_range      (0, 50)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the stroke before blurring; a negative value will contract the stroke instead"))

property_double (opacity_stroke, _("Outline's Opacity"), 1.0)
  value_range   (0.0, 1.0)
  ui_steps      (0.01, 0.10)

property_color  (color_stroke, _("Outline's Color"), "white")
  description   (_("The shadow's color (defaults to 'white')"))


property_double (x_shadow, _("Shadow/Glow X"), 10.0)
  description   (_("Horizontal shadow offset"))
  ui_range      (-40.0, 40.0)
  ui_steps      (1, 10)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "x")

property_double (y_shadow, _("Shadow/Glow Y"), 10.0)
  description   (_("Vertical shadow offset"))
  ui_range      (-40.0, 40.0)
  ui_steps      (1, 10)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "y")

property_color  (color_shadow, _("Shadow/Glow Color"), "black")
  description   (_("The shadow's color (defaults to 'black')"))

property_double (opacity_shadow, _("Shadow/Glow Opacity"), 0.5)
  value_range   (0.0, 1.0)
  ui_steps      (0.01, 0.10)

property_int (grow_radius_shadow, _("Shadow/Glow Grow radius"), 0)
  value_range   (-100, 100)
  ui_range      (-50, 50)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the shadow before blurring; a negative value will contract the shadow instead"))

property_double (radius_shadow, _("Shadow/Glow Blur radius"), 10.0)
  value_range   (0.0, G_MAXDOUBLE)
  ui_range      (0.0, 300.0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     original
#define GEGL_OP_C_SOURCE original.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;

    GeglNode *input    = gegl_node_get_input_proxy (gegl, "input");
    GeglNode *output   = gegl_node_get_output_proxy (gegl, "output");

    GeglNode *multiply1 = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);

    GeglNode *multiply2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);

    GeglNode *stroke = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow", "x", 0.0, "y", 0.0, "radius", 0.5, "opacity", 1.0, 
                                  NULL);

    GeglNode *ds = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow",
                                  NULL);

    GeglNode *blur = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur", "std-dev-x", 7.0, "std-dev-y", 7.0, "abyss-policy", 0, "clip-extent", 0,  
                                  NULL);

    GeglNode *mbd = gegl_node_new_child (gegl,
                                  "operation", "gegl:emboss", "elevation", 90.0, "azimuth", 40.0, 
                                  NULL);
#define bevelopacity \
" opacity value=1.5 median-blur radius=0 abyss-policy=none "\

    GeglNode *graphopacity = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", bevelopacity, 
                                  NULL);

    GeglNode *mcol = gegl_node_new_child (gegl,
                                  "operation", "gegl:color",
                                  NULL);

    GeglNode *crop = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop",
                                  NULL);\

    GeglNode *idref = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  gegl_node_link_many (input, multiply1, crop, idref, multiply2, stroke, ds, output, NULL);
  gegl_node_connect (multiply1, "aux", mcol, "output");
  gegl_node_connect (multiply2, "aux", graphopacity, "output");
  gegl_node_link_many (idref, blur, mbd, graphopacity, NULL);
  gegl_node_connect (crop, "aux", input, "output");

   gegl_operation_meta_redirect (operation, "x_shadow", ds, "x");
   gegl_operation_meta_redirect (operation, "y_shadow", ds, "y");
   gegl_operation_meta_redirect (operation, "opacity_shadow", ds, "opacity");
   gegl_operation_meta_redirect (operation, "grow_radius_shadow", ds, "grow-radius");
   gegl_operation_meta_redirect (operation, "radius_shadow", ds, "radius");
   gegl_operation_meta_redirect (operation, "color_shadow", ds, "color");
   gegl_operation_meta_redirect (operation, "grow_shape_stroke", stroke, "grow-shape");
   gegl_operation_meta_redirect (operation, "opacity_stroke", stroke, "opacity");
   gegl_operation_meta_redirect (operation, "radius_stroke", stroke, "radius");
   gegl_operation_meta_redirect (operation, "grow_radius_stroke", stroke, "grow-radius");
   gegl_operation_meta_redirect (operation, "color_stroke", stroke, "color");
   gegl_operation_meta_redirect (operation, "depth_bevel", mbd, "depth");
   gegl_operation_meta_redirect (operation, "color_fill", mcol, "value");

}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;
  operation_class = GEGL_OPERATION_CLASS (klass);
  operation_class->attach = attach;
  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:original",
    "title",       _("Day One"),
    "reference-hash", "45ed565h8500fca01b2acancient",
    "description", _("First version of Beaver's layerfx plugin (recreated). This is how Beaver's layerfx plugin appeared on its debut date in May 11 2022."
                     ""),
    NULL);
}

#endif
