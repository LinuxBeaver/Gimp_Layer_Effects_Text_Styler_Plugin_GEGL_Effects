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
*2022 Beaver GEGL
 */

#include "config.h"
#include <glib/gi18n-lib.h>


#ifdef GEGL_PROPERTIES


property_file_path(src, _("Optional image file overlay upload "), "")
    description (_("Source image file path (png, jpg, raw, svg, bmp, tif, ...)"))


property_color  (optioncolor, _("Optional Color Overlay"), "#ffffff")




property_int (depth, _("Bevel Depth"), 0)
    description (_("Filter width"))
    value_range (1, 100)





/* Should correspond to GeglMedianBlurNeighborhood in median-blur.c */
enum_start (gegl_stroke_grow_shape)
  enum_value (GEGL_stroke_GROW_SHAPE_SQUARE,  "square",  N_("Square"))
  enum_value (GEGL_stroke_GROW_SHAPE_CIRCLE,  "circle",  N_("Circle"))
  enum_value (GEGL_stroke_GROW_SHAPE_DIAMOND, "diamond", N_("Diamond"))
enum_end (GeglstrokeGrowShape)




property_enum   (grow_shape, _("Grow shape"),
                 GeglstrokeGrowShape, gegl_stroke_grow_shape,
                 GEGL_stroke_GROW_SHAPE_CIRCLE)
  description   (_("The shape to expand or contract the stroke in"))

property_double (radiusstroke, _("Outline's 'Blur radius"), 0.0)
  value_range   (0.0, 2)
  ui_range      (0.0, 300.0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")


property_double (grow_radiusstroke, _("Outline's 'Grow radius"), 12.0)
  value_range   (0.0, 100.0)
  ui_range      (0.0, 50.0)
  ui_digits     (0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the stroke before blurring; a negative value will contract the stroke instead"))

property_double (opacitystroke, _("Outline's 'Opacity"), 1.0)
  value_range   (0.0, 1.0)
  ui_steps      (0.01, 0.10)


property_color  (colorstroke, _("Outline's Color"), "white")
    /* TRANSLATORS: the string 'black' should not be translated */
  description   (_("The shadow's color (defaults to 'black')"))




enum_start (gegl_dropshadow_grow_shape)
  enum_value (GEGL_DROPSHADOW_GROW_SHAPE_SQUARE,  "square",  N_("Square"))
  enum_value (GEGL_DROPSHADOW_GROW_SHAPE_CIRCLE,  "circle",  N_("Circle"))
  enum_value (GEGL_DROPSHADOW_GROW_SHAPE_DIAMOND, "diamond", N_("Diamond"))
enum_end (GeglDropshadowGrowShape)

property_double (x, _("Shadow/Glow X"), 10.0)
  description   (_("Horizontal shadow offset"))
  ui_range      (-40.0, 40.0)
  ui_steps      (1, 10)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "x")

property_double (y, _("Shadow/Glow Y"), 10.0)
  description   (_("Vertical shadow offset"))
  ui_range      (-40.0, 40.0)
  ui_steps      (1, 10)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "y")




property_color  (color, _("Shadow/Glow Color"), "black")
    /* TRANSLATORS: the string 'black' should not be translated */
  description   (_("The shadow's color (defaults to 'black')"))

/* It does make sense to sometimes have opacities > 1 (see GEGL logo
 * for example)
 */



property_double (opacity, _("Shadow/Glow Opacity"), 0.5)
  value_range   (0.0, 1.0)
  ui_steps      (0.01, 0.10)

property_double (grow_radius, _("Shadow/Glow Grow radius"), 0.0)
  value_range   (-100.0, 100.0)
  ui_range      (-50.0, 50.0)
  ui_digits     (0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the shadow before blurring; a negative value will contract the shadow instead"))

property_double (radius, _("Shadow/Glow Blur radius"), 10.0)
  value_range   (0.0, G_MAXDOUBLE)
  ui_range      (0.0, 300.0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")


property_double (innergradius, _("Inner Glow's 'Blur radius"), 3.2)
  value_range   (0.0, 30.0)
  ui_range      (0.0, 30.0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")


property_double (innerggrow_radius, _("Inner Glow's 'Grow radius"), 6.0)
  value_range   (4, 30.0)
  ui_range      (4, 30.0)
  ui_digits     (0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the shadow before blurring; a negative value will contract the shadow instead"))

property_double (innergopacity, _("Inner Glow's opacity"), 0.9)
  value_range   (0.0, 2.0)
  ui_steps      (0.01, 0.10)


property_color (innergvalue, _("Inner Glow's Color"), "#797979")
    description (_("The color to paint over the input"))
 

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     gegleffects
#define GEGL_OP_C_SOURCE gegleffects.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *bc, *image, *atop, *mbd, *mcol, *stroke, *stroke2, *innerglow, *ds;




  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");

  image = gegl_node_new_child (gegl,
                                  "operation", "gegl:layer",
                                  NULL);


  stroke = gegl_node_new_child (gegl,
                                  "operation", "gegl:stroke",
                                  NULL);


  ds = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow",
                                  NULL);

  innerglow = gegl_node_new_child (gegl,
                                  "operation", "gegl:inner-glow",
                                  NULL);


  atop = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);



  mbd = gegl_node_new_child (gegl,
                                  "operation", "gegl:mbd",
                                  NULL);


  mcol = gegl_node_new_child (gegl,
                                  "operation", "gegl:mcol",
                                  NULL);





  gegl_node_link_many (input, atop, mbd, mcol, innerglow, stroke, ds, output, NULL);
  gegl_node_link (input, image);
  gegl_node_connect_from (atop, "aux", image, "output");

  gegl_operation_meta_redirect (operation, "string", image, "string");

  gegl_operation_meta_redirect (operation, "x", ds, "x");

   gegl_operation_meta_redirect (operation, "y", ds, "y");

   gegl_operation_meta_redirect (operation, "opacity", ds, "opacity");

   gegl_operation_meta_redirect (operation, "grow_radius", ds, "grow-radius");

gegl_operation_meta_redirect (operation, "radius", ds, "radius");

gegl_operation_meta_redirect (operation, "color", ds, "color");


  gegl_operation_meta_redirect (operation, "grow_shape", stroke, "grow-shape");

   gegl_operation_meta_redirect (operation, "opacitystroke", stroke, "opacity");

gegl_operation_meta_redirect (operation, "radiusstroke", stroke, "radius");

  gegl_operation_meta_redirect (operation, "grow_radiusstroke", stroke, "grow-radius");





   gegl_operation_meta_redirect (operation, "colorstroke", stroke, "color");


   gegl_operation_meta_redirect (operation, "depth", mbd, "radius2");

   gegl_operation_meta_redirect (operation, "optioncolor", mcol, "value");


   gegl_operation_meta_redirect (operation, "src", image, "src");




gegl_operation_meta_redirect (operation, "innerggrow_radius", innerglow, "grow-radius");

gegl_operation_meta_redirect (operation, "innergradius", innerglow, "radius");

  gegl_operation_meta_redirect (operation, "innergopacity", innerglow, "opacity");


  gegl_operation_meta_redirect (operation, "innergvalue", innerglow, "value");









}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "gegl:effects",
    "title",       _("GEGL Effects"),
    "categories",  "Generic",
    "reference-hash", "45ed565h8500fca01b2ac",
    "description", _("GEGL text stying and speciality image outlining filter. Text recoloring only works if the color is white "
                     ""),
    NULL);
}

#endif
