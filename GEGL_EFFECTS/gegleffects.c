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
 *2022 Beaver GEGL Effects 
 * 2022 Liam (for helping give Inner Glow a disable checkbox) 
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


property_double (xstroke, _("X"), 0.0)
  description   (_("Horizontal shadow offset"))
  ui_range      (-40.0, 40.0)
  ui_steps      (1, 10)
    ui_meta     ("role", "output-extent")


property_double (ystroke, _("Y"), 0.0)
  description   (_("Vertical shadow offset"))
  ui_range      (-40.0, 40.0)
  ui_steps      (1, 10)
    ui_meta     ("role", "output-extent")





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


property_boolean (innerglow, _("Enable Inner Glow"), FALSE)
  description   (_("Whether to add an inner glow effect, which can be slow"))



property_double (innergradius, _("Inner Glow's Blur radius"), 2.6)
  value_range   (0.0, 30.0)
  ui_range      (0.0, 30.0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")


property_double (innerggrow_radius, _("Inner Glow's 'Grow radius"), 7)
  value_range   (2, 30.0)
  ui_range      (2, 30.0)
  ui_digits     (0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the shadow before blurring; a negative value will contract the shadow instead"))

property_double (innergopacity, _("Inner Glow's opacity"), 1.4)
  value_range   (0.0, 2.0)
  ui_steps      (0.01, 0.10)



property_color (innergvalue, _("Inner Glow's Color"), "#ff8f00")
    description (_("The color to paint over the input"))

property_boolean (gradient, _("Enable Gradient"), FALSE)
  description   (_("Whether to add Gradient overlay"))


property_double (start_x, _("Gradient X1"), 659.0)
    ui_meta("unit", "pixel-coordinate")
    ui_meta("axis", "x")

property_double (start_y, _("Gradient Y1"), 49.0)
    ui_meta("unit", "pixel-coordinate")
    ui_meta("axis", "y")

property_double (end_x, _("Gradient X2"), 647.0)
    ui_meta("unit", "pixel-coordinate")
    ui_meta("axis", "x")

property_double (end_y, _("Gradient Y2"), 572.0)
    ui_meta ("unit", "pixel-coordinate")
    ui_meta ("axis", "y")

property_color (start_color, _("Gradient Start Color"), "#34ebd6")
    description (_("The color at (x1, y1)"))


property_color  (end_color, _("Gradient End Color"), "#fe18f2")
    description (_("The color at (x2, y2)"))
 

 

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     gegleffects
#define GEGL_OP_C_SOURCE gegleffects.c

#include "gegl-op.h"

typedef struct
{
  GeglNode *input;
  GeglNode *atop;
  GeglNode *crop;
  GeglNode *mbd;
  GeglNode *mcol;
  GeglNode *innerglow;
  GeglNode *stroke;
  GeglNode *ds;
  GeglNode *output;
  GeglNode *image;
  GeglNode *gradient;
} State;

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglNode *input, *output, *bc, *image, *atop, *mbd, *mcol, *stroke, *stroke2, *innerglow, *gradient, *crop, *ds;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");

  image = gegl_node_new_child (gegl,
                                  "operation", "gegl:layer",
                                  NULL);

  stroke = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow",
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

  gradient = gegl_node_new_child (gegl,
                                  "operation", "gegl:linear-gradient",
                                  NULL);

  crop = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop",
                                  NULL);

  gegl_node_link_many (input, atop, crop, mbd, mcol, /* innerglow, */ stroke, ds, output, NULL);
  gegl_node_link_many (input, image, /* gradient, */ NULL);
  gegl_node_connect_from (atop, "aux", image, "output");

  gegl_operation_meta_redirect (operation, "string", image, "string");
  gegl_operation_meta_redirect (operation, "x", ds, "x");
  gegl_operation_meta_redirect (operation, "y", ds, "y");
  gegl_operation_meta_redirect (operation, "ystroke", stroke, "y");
  gegl_operation_meta_redirect (operation, "xstroke", stroke, "x");
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
  gegl_operation_meta_redirect (operation, "start_x", gradient, "start-x");
  gegl_operation_meta_redirect (operation, "start_y", gradient, "start-y");
  gegl_operation_meta_redirect (operation, "end_x", gradient, "end-x");
  gegl_operation_meta_redirect (operation, "end_y", gradient, "end-y");
  gegl_operation_meta_redirect (operation, "start_color", gradient, "start-color");
  gegl_operation_meta_redirect (operation, "end_color", gradient, "end-color");


  /* Now save points to the various gegl nodes so we can rewire them in
   * update_graph() later
   */
  State *state = g_malloc0 (sizeof (State));
  o->user_data = state;

  state->input = input;
  state->atop = atop;
  state->crop = crop;
  state->mbd = mbd;
  state->mcol = mcol;
  state->innerglow = innerglow;
  state->stroke = stroke;
  state->ds = ds;
  state->output = output;
  state->image = image;
  state->gradient = gradient;
}

static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;

  if (o->innerglow)
  {
    if (o->gradient)
    {
      /* both innerglow and gradient */
      gegl_node_link_many (state->input, state->atop, state->crop, state->mbd, state->mcol, state->innerglow, state->stroke, state->ds, state->output, NULL);
      gegl_node_link_many (state->input, state->image, state->gradient, NULL);
      gegl_node_connect_from (state->atop, "aux", state->gradient, "output");
    }
    else
    {
      /* innerglow but no gradient */
      gegl_node_link_many (state->input, state->atop, state->crop, state->mbd, state->mcol, state->innerglow, state->stroke, state->ds, state->output, NULL);
      gegl_node_link_many (state->input, state->image, NULL);
      gegl_node_connect_from (state->atop, "aux", state->image, "output");
    }
  }
  else
  {
    if (o->gradient)
    {
      /* gradient but no innerglow */
      gegl_node_link_many (state->input, state->atop, state->crop, state->mbd, state->mcol, state->stroke, state->ds, state->output, NULL);
      gegl_node_link_many (state->input, state->image, state->gradient, NULL);
      gegl_node_connect_from (state->atop, "aux", state->gradient, "output");
    }
    else
    {
      /* neither gradient nor innerglow */
      gegl_node_link_many (state->input, state->atop, state->crop, state->mbd, state->mcol, state->stroke, state->ds, state->output, NULL);
      gegl_node_link_many (state->input, state->image, NULL);
      gegl_node_connect_from (state->atop, "aux", state->image, "output");
    }
  }
}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);
  GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);

  operation_class->attach = attach;
  operation_meta_class->update = update_graph;

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
