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
/*This is a list of the default inner glow plugin and its modern successors.*/

enum_start (inner_glow_list)
   enum_value (DEFAULT_IG,    "default",    N_("Default Inner Glow"))
   enum_value (INVERT_TRANSPARENCY_IG,  "invert",  N_("Invert Transparency"))
   enum_value (DEFAULT_IG_IMAGE_UPLOAD, "defaultimage", N_("Image Upload"))
   enum_value (INVERT_TRANSPARENCY_IG_IMAGE_UPLOAD,  "invertimage",  N_("Image Upload with Inverted Transparency"))
   enum_value (FEB_2024_IG,  "feb2024",  N_("Feb 2024 Inner Glow (adjustment layer group compatible) "))
   enum_value (GRAINY_IG,  "grainy",  N_("Grainy Inner Glow"))
   enum_value (BEVEL_IG,  "beveled",  N_("Beveled Inner Glow"))
enum_end (innerglowlist)

property_enum (mode, _("Mode:"),
               innerglowlist, inner_glow_list,
               DEFAULT_IG)
  description (_("Change the default mode of Inner Glow to an alternative mode where inner glow inverts transparency and or allows image file uploads over it."))

enum_start (gegl_blend_mode_typeig)
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHT, "hardlight",
              N_("Hard Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLY,      "multiply",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_SCREEN,      "screen",
              N_("Screen"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGE,      "grainmerge",
              N_("Grain Merge"))
 enum_end (GeglBlendModeTypeig0)


property_enum (bevelblendmode, _("Emboss blend mode:"),
    GeglBlendModeTypeig0, gegl_blend_mode_typeig,
    GEGL_BLEND_MODE_TYPE_HARDLIGHT)
ui_meta ("visible", "mode {beveled  }" )

/*This is GEGL syntax I wrote to invert transparency. Its operation is gegl:gegl and it represents a GEGL Graph inside a GEGL Graph.
please know distance transform is only here for filling the entire image with content. i could have used other
nodes like "solid-noise, cell-noise, ect... but distance transform was the fastest. gegl:color was faster but broke the graph */
#define GRAPHUSEDBYINNERGLOW \
"   id=1 dst-atop   aux=[  ref=1 distance-transform  ] xor srgb=true aux=[ ref=1 ] color-overlay value=#000000  "\

#define GRAPHUSEDBYINNERGLOW2 \
"  id=1 src-out aux=[ color value=black]  crop aux=[ ref=1 ] "\

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
  ui_meta       ("axis", "x")

property_double (y, _("Y"), 0.0)
  description   (_("Vertical shadow offset"))
  ui_range      (-20.0, 20.0)
  value_range   (-20.0, 20.0)
  ui_steps      (1, 2)
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

property_double (noise, _("Increase Noise"), 60.0)
    value_range (10.0, 100.0)
ui_meta ("visible", "mode {grainy  }" )

property_double (noiserepeat, _("Repetition of noise"), 8.0)
    value_range (5.0, 100.0)
ui_meta ("visible", "mode {grainy  }" )

property_seed (seed, _("Noise seed"), rand)
ui_meta ("visible", "mode {grainy  }" )

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
ui_meta ("visible", "!mode {feb2024, grainy, beveled   }" )

property_int  (fixoutline2, _("Median to fix non-effected pixels on edges 2"), 1)
  value_range (0, 5)
  description (_("Due to a bug I can't solve, not all pixels will be effected by inner glow. Median blur solves that problem."))
ui_meta ("visible", "mode {grainy, feb2024, beveled  }" )

property_file_path (image, _("Upload image file"), "")
    description (_("Upload image file for inner Glow"))
ui_meta ("visible", "!mode {default, grainy, feb2024, beveled, invert  }" )

property_double  (imageblur, _("Blur image uploaded"), 0.0)
  value_range (0.0, 20.0)
  description (_("Blur image file upload only"))
ui_meta ("visible", "!mode {default, invert, grainy, beveled, feb2024  }" )

property_boolean (clippolicy, _("Clip setting (bug trade off)"), TRUE)
  description    (_("Enable or disable the inner glow's clip. When disabled it will create a border bug when blur radius is high. When enabled it will clip Gimp's layers to image size setting."))



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
 GeglNode *src;
 GeglNode *pick;
 GeglNode *shadow;
 GeglNode *shadowopacitymax;
 GeglNode *color;
 GeglNode *color2;
 GeglNode *median2;
 GeglNode *medianset;
 GeglNode *in;
 GeglNode *in2;
 GeglNode *in3;
 GeglNode *in4;
 GeglNode *ingrainy;
 GeglNode *c2arevision;
 GeglNode *out;
 GeglNode *blurimage;
 GeglNode *idref;
 GeglNode *idref2;
 GeglNode *idref3;
 GeglNode *crop;
 GeglNode *atop;
 GeglNode *image;
 GeglNode *multiplybevel;
 GeglNode *screenbevel;
 GeglNode *hardlightbevel;
 GeglNode *grainmergebevel;
 GeglNode *nothing;
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
                                  "operation", "gegl:gegl", "string", GRAPHUSEDBYINNERGLOW2,
                                  NULL);
/*On June 24 2023 I finally figured out how to bake in GEGL Graphs
At Nov 20 2023 I learned that two nodes can call the same graph, also this graph inverts transparency.*/

  state->in    = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-in",
                                  NULL);

  state->in2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-in",
                                  NULL);

  state->in3    = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-in",
                                  NULL);

  state->in4    = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-in",
                                  NULL);


  state->ingrainy    = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-in",
                                  NULL);

  state->out    = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-out",
                                  NULL);

  state->src    = gegl_node_new_child (gegl,
                                  "operation", "gegl:src",
                                  NULL);

  state->pick    = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-pick",
                                  NULL);


  state->atop    = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);

  state->image    = gegl_node_new_child (gegl,
                                  "operation", "gegl:layer",
                                  NULL);

  state->idref3    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);



  state->idref2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
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


#define opacitymax \
"   median-blur radius=0 opacity value=2 median-blur radius=0  "\

  state->shadowopacitymax    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", opacitymax,
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


  state->medianset     = gegl_node_new_child (gegl, "operation", "gegl:median-blur",
                                         "radius",       1,
                                         "alpha-percentile", 100.0,
                                         NULL);

#define screen \
"   id=1 screen aux=[ ref=1  emboss opacity value=0.7 ]  "\


  state->screenbevel    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", screen,
                                  NULL);

#define hardlight \
"   id=1 hard-light aux=[ ref=1  emboss opacity value=0.2 ] "\

  state->hardlightbevel    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", hardlight,
                                  NULL);

#define grainmerge \
"   id=1 gimp:layer-mode layer-mode=grain-merge blend-space=rgb-perceptual aux=[ ref=1  emboss opacity value=0.5 ] "\

  state->grainmergebevel    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", grainmerge,
                                  NULL);
#define multiply \
"   id=1 gimp:layer-mode layer-mode=multiply aux=[ ref=1  emboss opacity value=0.5 ] "\

  state->multiplybevel    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", multiply,
                                  NULL);

#define c2afeb2024 \
"    color-to-alpha color=black   transparency-threshold=0 opacity-threshold=1  "\

  state->c2arevision    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", c2afeb2024,
                                  NULL);

  state->nothing    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst",
                                  NULL);


gegl_operation_meta_redirect (operation, "grow_radius",  state->shadow, "grow-radius");
gegl_operation_meta_redirect (operation, "radius",  state->shadow, "radius");
gegl_operation_meta_redirect (operation, "opacity",  state->shadow, "opacity");
gegl_operation_meta_redirect (operation, "grow-shape",  state->shadow, "grow-shape");
gegl_operation_meta_redirect (operation, "value2",  state->color2, "value");
gegl_operation_meta_redirect (operation, "x",  state->shadow, "x");
gegl_operation_meta_redirect (operation, "y",  state->shadow, "y");
gegl_operation_meta_redirect (operation, "fixoutline",  state->median2, "alpha-percentile");
gegl_operation_meta_redirect (operation, "fixoutline2",  state->medianset, "radius");
gegl_operation_meta_redirect (operation, "image", state->image, "src");
gegl_operation_meta_redirect (operation, "imageblur", state->blurimage, "std-dev-x");
gegl_operation_meta_redirect (operation, "imageblur", state->blurimage, "std-dev-y");
gegl_operation_meta_redirect (operation, "noise", state->pick, "pct-random");
gegl_operation_meta_redirect (operation, "noiserepeat", state->pick, "repeat");
gegl_operation_meta_redirect (operation, "seed", state->pick, "seed");

}

static void update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;

  GeglNode *usethis = state->multiplybevel; /* the blend mode switcher for the bevel applying to inner glow*/
  GeglNode *crop = state->crop;
  if (o->clippolicy) crop = state->crop;
  if (!o->clippolicy) crop = state->nothing;



  switch (o->bevelblendmode) {
    case GEGL_BLEND_MODE_TYPE_MULTIPLY: usethis = state->multiplybevel; break;
    case GEGL_BLEND_MODE_TYPE_GRAINMERGE: usethis = state->grainmergebevel; break;
    case GEGL_BLEND_MODE_TYPE_HARDLIGHT: usethis = state->hardlightbevel; break;
    case GEGL_BLEND_MODE_TYPE_SCREEN: usethis = state->screenbevel; break;
default: usethis = state->multiplybevel;
}

switch (o->mode) {
        break;
    case DEFAULT_IG:
 gegl_node_link_many (state->input, state->it,  state->shadow, state->color, state->in, state->median2, state->color2, crop, state->output, NULL);
 gegl_node_connect (state->in, "aux", state->input, "output");
        break;
    case INVERT_TRANSPARENCY_IG:
  gegl_node_link_many (state->input, state->it,  state->shadow,  state->it2,  state->color, state->in, state->median2, state->color2, crop, state->output, NULL);
 gegl_node_connect (state->in, "aux", state->input, "output");
 gegl_node_connect (state->crop, "aux", state->input, "output");
        break;
    case DEFAULT_IG_IMAGE_UPLOAD:
 gegl_node_link_many (state->input, state->it,  state->shadow, state->color, state->in, state->median2, state->color2, state->idref, state->atop, crop, state->output, NULL);
 gegl_node_link_many (state->idref, state->image, state->blurimage,  NULL);
 gegl_node_connect (state->in, "aux", state->input, "output");
 gegl_node_connect (state->atop, "aux", state->blurimage, "output");
 gegl_node_connect (state->crop, "aux", state->input, "output");
        break;
    case INVERT_TRANSPARENCY_IG_IMAGE_UPLOAD:
 gegl_node_link_many (state->input, state->it,  state->shadow, state->it2,  state->color, state->in, state->median2, state->color2,  state->idref, state->atop, crop, state->output, NULL);
 gegl_node_link_many (state->idref, state->image, state->blurimage,  NULL);
 gegl_node_connect (state->in, "aux", state->input, "output");
 gegl_node_connect (state->atop, "aux", state->blurimage, "output");
 gegl_node_connect (state->crop, "aux", state->input, "output");
        break;
    case FEB_2024_IG:
 gegl_node_link_many (state->input, state->medianset, state->idref, state->in2, crop, state->output, NULL);
 gegl_node_link_many  (state->idref, state->it2,  state->shadow, state->color2,   NULL);
 gegl_node_connect (state->in2, "aux", state->color2, "output");
 gegl_node_connect (state->crop, "aux", state->input, "output");
        break;
    case GRAINY_IG:
/*This is median blur followed by GEGL's src-in blend mode and a crop*/
 gegl_node_link_many (state->input, state->medianset, state->idref3, state->in3, crop, state->output, NULL);
/*Inside the src in blend mode we have a gegl graph, drop shadow, color overlay and another src-in blend mode*/
gegl_node_connect (state->in3, "aux", state->ingrainy, "output");
gegl_node_link_many  (state->idref3, state->it2,  state->shadow,  state->color2, state->idref2, state->ingrainy,  NULL);
/*Inside the second src in blend mode with have the pick filter.*/
 gegl_node_connect (state->ingrainy, "aux", state->pick, "output");
 gegl_node_link_many (state->idref2, state->pick, NULL);
 gegl_node_connect (state->crop, "aux", state->input, "output");
        break;
    case BEVEL_IG:
 gegl_node_link_many (state->input, state->medianset, state->idref, state->in4, crop, state->output, NULL);
 gegl_node_link_many  (state->idref, state->it2,  state->shadow, state->color2, usethis,  NULL);
 gegl_node_connect (state->in4, "aux", usethis, "output");
 gegl_node_connect (state->crop, "aux", state->input, "output");
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
