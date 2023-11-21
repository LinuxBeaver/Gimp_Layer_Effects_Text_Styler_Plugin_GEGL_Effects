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

id=0 src-in aux=[ ref=0 id=1 dst-atop   aux=[  ref=1 distance-transform  ] xor srgb=true     aux=[ ref=1 ] color-overlay value=#000000 dropshadow x=0 y=0 grow-radius=5 color-overlay value=#ff0000  ]
median-blur radius=3 alpha-percentile=94 crop

 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES


enum_start (inner_glow_list)
   enum_value (DEFAULT_IG,    "default",    N_("Default Inner Glow"))
   enum_value (INVERT_TRANSPARENCY_IG,  "invert",  N_("Invert Transparency"))
   enum_value (DEFAULT_IG_IMAGE_UPLOAD, "defaultimage", N_("Image Upload"))
   enum_value (INVERT_TRANSPARENCY_IG_IMAGE_UPLOAD,  "invertimage",  N_("Image Upload with Inverted Transparency"))
enum_end (innerglowlist)

property_enum (mode, _("Mode:"),
               innerglowlist, inner_glow_list,
               DEFAULT_IG)
  description (_("Change the default mode of Inner Glow to an alternative mode where inner glow inverts transparency and or allows image file uploads over it."))


/*This is GEGL syntax I wrote to invert transparency. Its operation is gegl:gegl and it represents a GEGL Graph inside a GEGL Graph.
please know distance transform is only here for filling the entire image with content. i could have used other
nodes like "solid-noise, cell-noise, ect... but distance transform was the fastest. gegl:color was faster but broke the graph */
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
  description (_("The distance to expand the shadow before blurring. When using inverted modes this setting needs to be at a reasonable degree for it's effect to be noticable."))

property_double (opacity, _("Opacity"), 1.2)
  value_range   (0.0, 2.0)
  ui_steps      (0.01, 0.10)


property_color (value2, _("Color"), "#fbff00")
    description (_("The color to paint over the input"))
    ui_meta     ("role", "color-primary")
ui_meta ("visible", "!mode {defaultimage, invertimage  }" )

property_double  (fixoutline, _("Median to fix non-effected pixels on edges"), 60)
  value_range (50, 100)
  description (_("Due to a bug I can't solve, not all pixels will be effected by inner glow. Median blur solves that problem.'"))

property_file_path (image, _("Upload image file"), "")
    description (_("Upload image file for inner Glow"))
ui_meta ("visible", "!mode {default, invert  }" )

property_double  (imageblur, _("Blur image uploaded"), 0.0)
  value_range (0.0, 20.0)
  description (_("Blur image file upload only"))
ui_meta ("visible", "!mode {default, invert  }" )

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     inner_glow
#define GEGL_OP_C_SOURCE inner-glow.c

#include "gegl-op.h"

typedef struct
{
 GeglNode *input;
 GeglNode *it;
 GeglNode *it2;
 GeglNode *shadow;
 GeglNode *color;
 GeglNode *color2;
 GeglNode *median2;
 GeglNode *in;
 GeglNode *blurimage;
 GeglNode *idref;
 GeglNode *crop;
 GeglNode *atop;
 GeglNode *image;
 GeglNode *output;
}State;

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);

  State *state = o->user_data = g_malloc0 (sizeof (State));
  GeglColor *hidden_color = gegl_color_new ("#00ffffAA");
  state->input    = gegl_node_get_input_proxy (gegl, "input");
  state->output   = gegl_node_get_output_proxy (gegl, "output");



  state->it    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", GRAPHUSEDBYINNERGLOW,
                                  NULL);

  state->it2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", GRAPHUSEDBYINNERGLOW,
                                  NULL);
/*On June 24 2023 I finally figured out how to bake in GEGL Graphs
At Nov 20 2023 I learned that two nodes can call the same graph, also this graph inverts transparency.*/

  state->in    = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-in",
                                  NULL);

  state->atop    = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);

  state->image    = gegl_node_new_child (gegl,
                                  "operation", "gegl:layer",
                                  NULL);


  state->idref    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  state->crop    = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop",
                                  NULL);
/*This node was added November 20 2023 to fix another border bug that happens with layer to image size, where inner glow goes OUTSIDE the selection some how. This is a bug with Gimp internally
but somehow crop fixes it.*/

  state->blurimage    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
                                  NULL);


  state->shadow    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow",
                                  NULL);

  state->color    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                   "value", hidden_color, NULL);
/*This is a trick to bake in color nodes*/

  state->color2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);


  state->median2     = gegl_node_new_child (gegl, "operation", "gegl:median-blur",
                                         "radius",       1,
                                         NULL);

gegl_operation_meta_redirect (operation, "grow_radius",  state->shadow, "grow-radius");
gegl_operation_meta_redirect (operation, "radius",  state->shadow, "radius");
gegl_operation_meta_redirect (operation, "opacity",  state->shadow, "opacity");
gegl_operation_meta_redirect (operation, "grow-shape",  state->shadow, "grow-shape");
gegl_operation_meta_redirect (operation, "value2",  state->color2, "value");
gegl_operation_meta_redirect (operation, "x",  state->shadow, "x");
gegl_operation_meta_redirect (operation, "y",  state->shadow, "y");
gegl_operation_meta_redirect (operation, "fixoutline",  state->median2, "alpha-percentile");
gegl_operation_meta_redirect (operation, "image", state->image, "src");
gegl_operation_meta_redirect (operation, "imageblur", state->blurimage, "std-dev-x");
gegl_operation_meta_redirect (operation, "imageblur", state->blurimage, "std-dev-y");

} 

static void update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;

switch (o->mode) {
        break;
    case DEFAULT_IG:
 gegl_node_link_many (state->input, state->it,  state->shadow, state->color, state->in, state->median2, state->color2, state->crop, state->output, NULL);
 gegl_node_connect_from (state->in, "aux", state->input, "output");
        break;
    case INVERT_TRANSPARENCY_IG:
  gegl_node_link_many (state->input, state->it,  state->shadow,  state->it2,  state->color, state->in, state->median2, state->color2, state->crop, state->output, NULL);
 gegl_node_connect_from (state->in, "aux", state->input, "output");
        break;
    case DEFAULT_IG_IMAGE_UPLOAD:
 gegl_node_link_many (state->input, state->it,  state->shadow, state->color, state->in, state->median2, state->color2, state->idref, state->atop, state->crop, state->output, NULL);
 gegl_node_link_many (state->idref, state->image, state->blurimage,  NULL);
 gegl_node_connect_from (state->in, "aux", state->input, "output");
 gegl_node_connect_from (state->atop, "aux", state->blurimage, "output");
        break;
    case INVERT_TRANSPARENCY_IG_IMAGE_UPLOAD:
 gegl_node_link_many (state->input, state->it,  state->shadow, state->it2,  state->color, state->in, state->median2, state->color2,  state->idref, state->atop, state->crop, state->output, NULL);
 gegl_node_link_many (state->idref, state->image, state->blurimage,  NULL);
 gegl_node_connect_from (state->in, "aux", state->input, "output");
 gegl_node_connect_from (state->atop, "aux", state->blurimage, "output");
    }
  }

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;
GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);
  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;
  operation_meta_class->update = update_graph;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:innerglow",
    "title",       _("Inner Glow (to blend)"),
    "reference-hash", "g3do6aaoo1100g0fjf25sb2ac",
    "description", _("GEGL does an inner glow effect. For this filter to work you need to use the 'Normal' or other blending options. Or a duplicate layer on top.  "
                     ""),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Inner Glow (to blend)..."),
    NULL);
}

#endif
