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
 * GEGL Shiny Text for GEGL Effects CE- Beaver 2023
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

/*
Search through GEGL's source code in /operations/common to find templates.

Example Templates below that call gaussian blur and hsl color overlay together'
 */

property_double (opacity, _("Opacity of Gloss Shine"), 1)
    description (_("Global opacity value that is always used on top of the optional auxiliary input buffer."))
    value_range (0.100, 1)
    ui_range    (0.1, 1)



property_double (x_scale, _("X Scale of Gloss Shine"), 28.00)
    description (_("Scale value for x axis"))
    value_range (15, 60.0)
    ui_range    (15, 60.0)
    ui_meta     ("unit", "pixel-distance")
    ui_meta     ("axis", "x")

property_double (y_scale, _("Y Scale of Gloss Shine"), 56.0)
    description (_("Scale value for y axis"))
    value_range (15, 150.0)
    ui_range    (15, 150.0)
    ui_meta     ("unit", "pixel-distance")
    ui_meta     ("axis", "y")

property_double (complexity, _("Complexity of Gloss Shine"), 0.8)
    description (_("Complexity factor"))
    value_range (0.1, 0.8)

property_seed (seed, _("Random seed"), rand)

property_boolean (tiling, _("Force tiling"), TRUE)
    description (_("If set, the pattern generated will tile"))
    ui_meta     ("role", "output-extent")

property_boolean (perturbation, _("Distorted"), TRUE)
    description (_("If set, the pattern will be a little more distorted"))
    ui_meta     ("role", "output-extent")

property_color  (color1, _("Color 1"), "#ffffff")
    ui_meta     ("role", "output-extent")

property_color  (color2, _("Color 2"), "#000000")
    ui_meta     ("role", "output-extent")

enum_start (gegl_sinus_gloss)
  enum_value (GEGL_SINUS_BLEND_LINEAR , "linear",   N_("Linear"))
  enum_value (GEGL_SINUS_BLEND_BILINEAR , "bilinear", N_("Bilinear"))
  enum_value (GEGL_SINUS_BLEND_SINUSOIDAL, "sinusoidal", N_("Sinusoidal"))
enum_end (GeglSinusgloss)


enum_start (gegl_blend_mode_typeshineg)
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGEALT,      "GrainMergeAlt",
              N_("GrainMergeAlt"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGE,      "GrainMerge",
              N_("GrainMerge"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHT,      "Hardlight",
              N_("Hardlight"))
  enum_value (GEGL_BLEND_MODE_TYPE_ADDITION,      "Addition",
              N_("Addition"))
  enum_value (GEGL_BLEND_MODE_TYPE_REPLACE, "Replace",
              N_("Multiply"))
enum_end (GeglBlendModeTypeshineg)

property_enum (blendmode, _("Blend Mode of Shine"),
    GeglBlendModeTypeshineg, gegl_blend_mode_typeshineg,
    GEGL_BLEND_MODE_TYPE_REPLACE)

property_boolean (enable, _("Enable Shine for GEGL Effects"), FALSE)
  description    (_("This option is only for GEGL Effects"))





property_enum (sinusmode, _("Sinus Mode"),
    GeglSinusgloss, gegl_sinus_gloss,
    GEGL_SINUS_BLEND_LINEAR)
    ui_meta     ("role", "output-extent")

property_double (blend_power, _("Dark to light intensity"), -0.6)
    description (_("Intensity of the Gloss Shine Effect"))
    value_range (-0.9, 0.0)


property_int    (width, _("Width"), 1024)
    description (_("Width of the generated buffer"))
    value_range (0, G_MAXINT)
    ui_range    (0, 4096)
    ui_meta     ("unit", "pixel-distance")
    ui_meta     ("axis", "x")
    ui_meta     ("role", "output-extent")

property_int    (height, _("Height"), 768)
    description (_("Height of the generated buffer"))
    value_range (0, G_MAXINT)
    ui_range    (0, 4096)
    ui_meta     ("unit", "pixel-distance")
    ui_meta     ("axis", "y")
    ui_meta     ("role", "output-extent")


#else

#define GEGL_OP_META
#define GEGL_OP_NAME     shinytext
#define GEGL_OP_C_SOURCE shinytext.c

#include "gegl-op.h"


typedef struct
{
  GeglNode *input;
  GeglNode *opacity;
  GeglNode *crop;
  GeglNode *crop2;
  GeglNode *nop0;
  GeglNode *nop;
  GeglNode *blend;
  GeglNode *sinus;
  GeglNode *addition;
  GeglNode *hardlight;
  GeglNode *grainmerge;
  GeglNode *grainmergealt;
  GeglNode *replace;
  GeglNode *output;
}State;

static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;

  GeglNode *blendchoice = state->replace; /* the default */
  switch (o->blendmode) {
    case GEGL_BLEND_MODE_TYPE_GRAINMERGEALT: blendchoice = state->grainmergealt; break;
    case GEGL_BLEND_MODE_TYPE_GRAINMERGE: blendchoice = state->grainmerge; break;
    case GEGL_BLEND_MODE_TYPE_HARDLIGHT: blendchoice = state->hardlight; break;
    case GEGL_BLEND_MODE_TYPE_ADDITION: blendchoice = state->addition; break;
    case GEGL_BLEND_MODE_TYPE_REPLACE: blendchoice = state->replace; break;
default: blendchoice = state->replace;

}
  if (o->enable)
  {
  gegl_node_link_many (state->input, blendchoice, state->crop2, state->output,  NULL);
  gegl_node_link_many (state->sinus, state->opacity, state->crop, NULL);
  gegl_node_connect_from (blendchoice, "aux", state->crop, "output");
  }
else
  gegl_node_link_many (state->input, state->output,  NULL);
}


static void attach (GeglOperation *operation)
{
GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *sinus, *replace, *addition, *crop, *crop2, *grainmerge, *grainmergealt, *blend, *nop0, *nop, *hardlight, *opacity;


  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");


  opacity = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);


  sinus    = gegl_node_new_child (gegl,
                                  "operation", "gegl:sinus",
                                  NULL);

  nop    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  crop    = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop",
                                  NULL);

  crop2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop",
                                  NULL);

  nop0    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);


replace = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 30, "composite-space", 2, "composite-mode", 0, "blend-space", 2, NULL);

hardlight = gegl_node_new_child (gegl,
                                  "operation", "gimp:layer-mode", "layer-mode", 44, "composite-mode", 0, "blend-space", 0, NULL);


addition = gegl_node_new_child (gegl,
                                  "operation", "gimp:layer-mode", "layer-mode", 33, "composite-mode", 0, NULL);

grainmerge = gegl_node_new_child (gegl,
                                  "operation", "gimp:layer-mode", "layer-mode", 47, "composite-mode", 0, NULL);

grainmergealt = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 47, "composite-space", 2, "composite-mode", 0, "blend-space", 2, NULL);




  blend    = gegl_node_new_child (gegl,
                                  "operation", "gegl:plus",
                                  NULL);

  gegl_operation_meta_redirect (operation, "opacity", opacity, "value");
  gegl_operation_meta_redirect (operation, "seed", sinus, "seed");
  gegl_operation_meta_redirect (operation, "complexity", sinus, "complexity");
  gegl_operation_meta_redirect (operation, "x_scale", sinus, "x-scale");
  gegl_operation_meta_redirect (operation, "y_scale", sinus, "y-scale");
  gegl_operation_meta_redirect (operation, "color1", sinus, "color1");
  gegl_operation_meta_redirect (operation, "color2", sinus, "color2");
  gegl_operation_meta_redirect (operation, "blend_power", sinus, "blend-power");




  

  /* now save references to the gegl nodes so we can use them
   * later, when update_graph() is called
   */
  State *state = g_malloc0 (sizeof (State));
  state->input = input;
  state->opacity = opacity;
  state->addition = addition;
  state->hardlight = hardlight;
  state->grainmerge = grainmerge;
  state->grainmergealt = grainmergealt;
  state->replace = replace;
  state->sinus = sinus;
  state->nop = nop;
  state->nop = nop0;
  state->crop = crop;
  state->crop2 = crop2;
  state->blend = blend;
  state->output = output;

  o->user_data = state;

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
    "name",        "gegl:shinytext",
    "title",       _("Shiny Text"),
    "categories",  "hidden",
    "reference-hash", "sglosa28a5125700fc254001b2ac",
    "description", _("GEGL renders a shiny effect on text. This won't work on normal images. Only Text! Users are specified to use Gimp/GEGL blend modes and opacity slider with this"),
    NULL);
}

#endif
