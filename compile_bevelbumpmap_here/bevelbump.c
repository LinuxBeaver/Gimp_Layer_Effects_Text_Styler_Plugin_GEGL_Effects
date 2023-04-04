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
 * 2022 Beaver (GEGL Bevel) *2023 Beaver (GEGL Bevel 2 for GEGL Effects Continual Edition) 
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

property_boolean (effectsswitchbevel, _("Enable Bevel (FOR GEGL EFFECTS ONLY)"), TRUE)
  description    (_("This switch exist for GEGL Effects. You won't find it in stand alone bevel'"))
    ui_meta     ("role", "output-extent")

property_boolean (embossmode, _("Emboss Mode (use Gimp's layer Grain Merge blend mode)"), FALSE)
  description    (_("Make an embossed bevel'"))








property_double (radius, _("Radius Normal Bevel"), 7.0)
  value_range (1.0, 40.0)
  ui_range (1.0, 12)
  ui_gamma (1.5)

property_int (radiussharp, _("Radius Sharp Bevel"), 0)
   description(_("Box Blur -0 means disabled by default"))
   value_range (0, 8)
   ui_range    (0, 8)
   ui_gamma   (1.5)


property_double (elevation, _("Depth Angle"), 90.0)
    description (_("Elevation angle (degrees)"))
    value_range (0, 180)
    ui_meta ("unit", "degree")

property_int (depth, _("Depth"), 40)
    description (_("Filter width"))
    value_range (1, 100)


property_double (th, _("Threshold of the Bevel's Transparency'"), 0.100)
  value_range (0.0, 1.0)
  ui_range (0.0, 0.5)

property_double (azimuth, _("Rotate Lighting"), 40.0)
    description (_("Light angle (degrees)"))
    value_range (0, 350)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")


#else

#define GEGL_OP_META
#define GEGL_OP_NAME     bevelbump
#define GEGL_OP_C_SOURCE bevelbump.c

#include "gegl-op.h"


typedef struct
{
  GeglNode *input;
  GeglNode *blur;
  GeglNode *boxblur;
  GeglNode *emb;
  GeglNode *th;
  GeglNode *output;
} State; 


static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglNode *input, *output, *boxblur, *blur, *emb, *th;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");

   blur = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
                                  NULL);

   boxblur = gegl_node_new_child (gegl,
                                  "operation", "gegl:box-blur",
                                  NULL);




 emb   = gegl_node_new_child (gegl,
                                  "operation", "gegl:emboss",
                                  NULL);

  th = gegl_node_new_child (gegl,
                                  "operation", "gimp:threshold-alpha",
                                  NULL);



  gegl_node_link_many (input, blur, boxblur, emb, th, output, NULL);
  gegl_operation_meta_redirect (operation, "radius", blur, "std-dev-x");
  gegl_operation_meta_redirect (operation, "radius", blur, "std-dev-y");
  gegl_operation_meta_redirect (operation, "elevation", emb, "elevation");
  gegl_operation_meta_redirect (operation, "depth", emb, "depth");
  gegl_operation_meta_redirect (operation, "azimuth", emb, "azimuth");
  gegl_operation_meta_redirect (operation, "radiussharp", boxblur, "radius");
  gegl_operation_meta_redirect (operation, "th", th, "value");




 /* Now save points to the various gegl nodes so we can rewire them in
   * update_graph() later
   */
  State *state = g_malloc0 (sizeof (State));
  state->input = input;
  state->blur = blur;
  state->boxblur = boxblur;
  state->emb = emb;
  state->th = th;
  state->output = output;
  o->user_data = state;
}

static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;

  if (o->effectsswitchbevel)
  if (o->embossmode)
  {
    gegl_node_link_many (state->blur, state->boxblur,  state->emb, state->output, NULL);
  }
else
  {
    gegl_node_link_many (state->blur, state->boxblur,  state->emb, state->th, state->output, NULL);
  }
else
  {
    gegl_node_link_many (state->input,  state->output, NULL);
  }
}





static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class = GEGL_OPERATION_CLASS (klass);
   GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);

  operation_class->attach = attach;
  operation_meta_class->update = update_graph;

  gegl_operation_class_set_keys (operation_class,
    "name",        "gegl:bevelbump",
    "title",       _("Bevel 2"),
    "categories",  "Aristic",
    "reference-hash", "bev45ed5656a28a512570f0f25sb2ac",
    "description", _("GEGL Effects Continual Edition REQUIRES Bevel 2. Many other Beaver Plugins (chrome, goo, and default GEGL Effects) require Bevel. So you have to keep both bevel plugins if you want everything to work. Bevel 2 has new abilities such as the sharp bevel mode and emboss mode. Works best wiht blend modes multiply and grain merge."
                     ""),
    NULL);
}

#endif
