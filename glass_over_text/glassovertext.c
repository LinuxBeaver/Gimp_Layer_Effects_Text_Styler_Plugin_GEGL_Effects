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
 * Glass over Text 2023 Beaver
 */

/*GEGL Glass Over Text was once just a stand alone plugin, but now it is also part of GEGL Effects. The stand alone version still exist and does more then the GEGL Effects implementation of it. */


/*
GEGL Graph Below
 */

/*
color-overlay  value=#ffffff
gaussian-blur std-dev-x=1 std-dev-y=1
emboss azimuth=44 depth=29
id=1
gimp:layer-mode layer-mode=erase composite-mode=auto aux=[ ref=1 dst-over aux=[ color value=#000000  ]  crop  color-to-alpha opacity-threshold=0.14 color-overlay value=#000000 ]  ]
color-overlay value=#ffffff
gaussian-blur std-dev-x=0 std-dev-y=0
 */


/*GEGL Graph here. NOTE FOR DEVS replacing"gimp:layer-mode layer-mode=erase with gegl:xor does not work proper. I tried and you can try it too and watch it fail. GEGL blend modes are not as good as
Gimp blend modes  */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

/*This is the enum list of glass text's blend mode switcher.*/
enum_start (gegl_blend_mode_type_glasstext)
  enum_value (GEGL_BLEND_MODE_TYPE_GLASSOVER,      "Replace",
              N_("Replace"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGEGLASS,      "GrainMerge",
              N_("GrainMerge"))
  enum_value (GEGL_BLEND_MODE_TYPE_LINEARLIGHTGLASS,      "LinearLight",
              N_("LinearLight"))
  enum_value (GEGL_BLEND_MODE_TYPE_SUBTRACTGLASS,      "Subtract",
              N_("Subtract"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAYGLASS, "Overlay",
              N_("Overlay"))
enum_end (geglblendmodetypeglasstext)



property_enum (glassover, _("Blend Mode of Glass on Text"),
 geglblendmodetypeglasstext, gegl_blend_mode_type_glasstext,
    GEGL_BLEND_MODE_TYPE_GLASSOVER)
    ui_meta     ("role", "output-extent")


property_boolean (enableglasstext, _("Enable Glass on Text for GEGL Effects"), TRUE)
  description    (_("This option is only for GEGL Effects"))
    ui_meta     ("role", "output-extent")
/*     ui_meta     ("role", "output-extent") hides this from showing up in the GUI */





#define TUTORIALGLASSTEXT \
" id=1 gimp:layer-mode layer-mode=erase composite-mode=auto aux=[ ref=1 dst-over aux=[ color value=#000000  ]   color-to-alpha opacity-threshold=0.14 color-overlay value=#000000 ]  ]  "\

property_double (azimuth, _("Azimuth"), 30.0)
    description (_("Light angle (degrees)"))
    value_range (0, 360)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")
  ui_steps      (0.5, 0.50)


property_int (depth, _("Depth"), 20.0)
    description (_("Filter width"))
    value_range (10, 100)

property_double (elevation, _("Elevation"), 45.0)
    description (_("Elevation angle (degrees)"))
    value_range (40, 46)
    ui_meta ("unit", "degree")
  ui_steps      (0.1, 0.50)


property_double (std_dev, _("Retract Shine"), 3.0)
   description (_("Add a Gaussian Blur"))
   value_range (1, 3)
   ui_range    (1, 3)
   ui_gamma    (3.0)
   ui_meta     ("unit", "pixel-distance")
  ui_steps      (0.1, 0.50)

property_double (blur, _("Blur Shine"), 0.5)
   description (_("Add a Gaussian Blur"))
   value_range (0.5, 1)
   ui_range    (0.5, 1)
   ui_gamma    (3.0)
  ui_steps      (0.1, 0.50)
   ui_meta     ("unit", "pixel-distance")

property_color (color, _("Color"), "#ffffff")
    description (_("The color to paint over the input"))

property_double (hyperopacity, _("Hyper Opacity"), 1.0)
    description (_("Opacity Above 100 to make effect more noticable"))
    value_range (0.3, 1.5)
    ui_range    (1.0, 1.5)
  ui_steps      (0.1, 0.50)


#else

#define GEGL_OP_META
#define GEGL_OP_NAME     glassovertext
#define GEGL_OP_C_SOURCE glassovertext.c

#include "gegl-op.h"

typedef struct
{
  GeglNode *input;
  GeglNode *emboss;
  GeglNode *color;
  GeglNode *white;
  GeglNode *retract;
  GeglNode *gaussian;
  GeglNode *hyperopacity;
  GeglNode *string;
  GeglNode *glassover;
  GeglNode *glassgrainmerge;
  GeglNode *glasslinearlight;
  GeglNode *glasssubtract;
  GeglNode *glassoverlay;
  GeglNode *output;
}State;



static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;

  GeglNode *blendglass = state->glassover; /* the default */
  switch (o->glassover) {
    case GEGL_BLEND_MODE_TYPE_GLASSOVER: blendglass = state->glassover; break;
    case GEGL_BLEND_MODE_TYPE_GRAINMERGEGLASS: blendglass = state->glassgrainmerge; break;
    case GEGL_BLEND_MODE_TYPE_LINEARLIGHTGLASS: blendglass = state->glasslinearlight; break;
    case GEGL_BLEND_MODE_TYPE_SUBTRACTGLASS: blendglass = state->glasssubtract; break;
    case GEGL_BLEND_MODE_TYPE_OVERLAYGLASS: blendglass = state->glassoverlay; break;
default: blendglass = state->glassover;

}
  if (o->enableglasstext)
  {
  gegl_node_link_many (state->input, blendglass, state->output,  NULL);
  gegl_node_link_many (state->input, state->white, state->retract, state->emboss, state->string, state->color, state->hyperopacity, state->gaussian, NULL);
  gegl_node_connect (blendglass, "aux", state->gaussian, "output");
  }
else
  gegl_node_link_many (state->input, state->output,  NULL);
}


static void attach (GeglOperation *operation)
{
GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *emboss, *color, *glassover,  *glassgrainmerge, *glasslinearlight, *glasssubtract, *glassoverlay, *white, *retract, *gaussian, *hyperopacity, *string;
  GeglColor *white_color = gegl_color_new ("rgb(1.1,1.1,1.1)");


  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");


  emboss = gegl_node_new_child (gegl,
                                  "operation", "gegl:emboss",
                                  NULL);

  string = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", TUTORIALGLASSTEXT,
                                  NULL);

  hyperopacity = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

  retract    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur", "clip-extent", FALSE,  "abyss-policy", 0,
                                  NULL);

  gaussian    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur", "clip-extent", FALSE,  "abyss-policy", 0,
                                  NULL);


  color    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);


  white     = gegl_node_new_child (gegl, "operation", "gegl:color-overlay",
                                   "value", white_color,
                                  NULL);


  /* Glass Over Text blend modes for GEGL Effects */

  glassover = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-in",
                                  NULL);

glasslinearlight = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 50, "composite-mode", 0, "blend-space", 3, NULL);

glassgrainmerge = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 47, "composite-mode", 0, "blend-space", 3, NULL);

glasssubtract = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 34, "composite-mode", 0, NULL);

glassoverlay = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 23, "composite-mode", 0, NULL);



  gegl_operation_meta_redirect (operation, "std_dev", retract, "std-dev-x");
  gegl_operation_meta_redirect (operation, "std_dev", retract, "std-dev-y");
  gegl_operation_meta_redirect (operation, "blur", gaussian, "std-dev-x");
  gegl_operation_meta_redirect (operation, "blur", gaussian, "std-dev-y");
  gegl_operation_meta_redirect (operation, "azimuth", emboss, "azimuth");
  gegl_operation_meta_redirect (operation, "elevation", emboss, "elevation");
  gegl_operation_meta_redirect (operation, "depth", emboss, "depth");
  gegl_operation_meta_redirect (operation, "color", color, "value");
  gegl_operation_meta_redirect (operation, "hyperopacity", hyperopacity, "value");


  /* now save references to the gegl nodes so we can use them
   * later, when update_graph() is called
   */
  State *state = g_malloc0 (sizeof (State));
  state->input = input;
  state->white = white;
  state->retract = retract;
  state->emboss = emboss;
  state->string = string;
  state->color = color;
  state->hyperopacity = hyperopacity;
  state->gaussian = gaussian;
  state->glassover = glassover;
  state->glassgrainmerge = glassgrainmerge;
  state->glasslinearlight = glasslinearlight;
  state->glasssubtract = glasssubtract;
  state->glassoverlay = glassoverlay;
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
    "name",        "lb:glassovertext",
    "title",       _("Glass Over Text"),
    "reference-hash", "stglass238a5125700fc254001b2ac",
    "description", _("Glass Over Text Effect - Filter expects user to place effect on layer above or use the normal blend mode. It may even be a good idea to experiment with other blend modes."),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Glass over text (to blend)..."),
    NULL);
}

#endif
