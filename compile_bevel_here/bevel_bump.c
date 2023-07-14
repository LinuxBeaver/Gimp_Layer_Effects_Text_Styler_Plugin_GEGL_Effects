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
 * 2022 Beaver (GEGL Bevel) 
 */

/*GEGL Bevel is a stand alone plugin but it is also part of GEGL Effects. The stand alone version does more then the GEGL Effects implementation of it. */


/*
Graph here to test without installing. 

id=1 gimp:layer-mode layer-mode=normal composite-mode=clip-to-backdrop aux=[color value=#ffffff  opacity value=0.1 ]
gaussian-blur std-dev-x=2 std-dev-y=2
emboss azimuth=44 depth=9
gimp:threshold-alpha value=0.03
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

property_boolean (effectsswitchbevel, _("Enable Bevel (FOR GEGL EFFECTS ONLY)"), TRUE)
  description    (_("This switch exist for GEGL Effects testing. You won't find it in stand alone bevel. Or even in current *June 7th 2023 GEGL Effects.'"))
    ui_meta     ("role", "output-extent")

property_boolean (embossmode, _("Emboss Mode (use Gimp's layer Grain Merge blend mode)"), FALSE)
  description    (_("Make an embossed bevel'"))



property_double (radius1, _("Radius Normal Bevel"), 7.0)
  value_range (0.5, 40.0)
  ui_range (1.0, 9.0)
  ui_gamma (1.5)

property_int (radius2, _("Radius Sharp Bevel"), 0)
   description(_("Box Blur -0 means disabled by default"))
   value_range (0, 10.0)
   ui_range    (0, 9.0)
   ui_gamma   (1.5)


property_double (bevel1, _("Depth Angle"), 90.0)
    description (_("Elevation angle (degrees)"))
    value_range (0, 180)
    ui_meta ("unit", "degree")

property_int (bevel2, _("Depth"), 40)
    description (_("Filter width"))
    value_range (1, 100)


property_double (th, _("Bevel's coverage threshold."), 0.100)
    description (_("Lower covers more and higher covers less."))
  value_range (0.0, 1.0)
  ui_range (0.0, 0.5)
  ui_meta     ("sensitive", "! embossmode")

property_double (azimuth, _("Rotate Lighting"), 40.0)
    description (_("Light angle (degrees)"))
    value_range (0, 350)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")


property_double (slideupblack, _("Black Bevel/Image Bevel mode."), 0.00)
    description (_("This slider allows GEGL bevel to works on black Bevels; but the user must still manually select blend modes like Grain Merge and Hardlight that are known to work with very dark Bevels. This also allows bevel to be applied on image file overlays without conforming to an image's details."))
  value_range   (0.00, 0.999)
  ui_steps      (0.01, 0.50)



#else

#define GEGL_OP_META
#define GEGL_OP_NAME     bevel_bump
#define GEGL_OP_C_SOURCE bevel_bump.c

#include "gegl-op.h"


typedef struct
{
  GeglNode *input;
  GeglNode *blur;
  GeglNode *boxblur;
  GeglNode *emb;
  GeglNode *th;
  GeglNode *whitecolor;
  GeglNode *normallayer;
  GeglNode *slideupblack;
  GeglNode *fix;
  GeglNode *output;
} State; 


static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglNode *input, *output, *boxblur, *blur, *emb, *th, *fix, *whitecolor, *normallayer, *slideupblack;
  GeglColor *white_color = gegl_color_new ("#ffffff");

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


  whitecolor    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                   "value", white_color, NULL);


normallayer = gegl_node_new_child (gegl,
                                    "operation", "gimp:layer-mode", "layer-mode", 28,  "composite-mode", 2, NULL);


 slideupblack   = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

 fix   = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop",
                                  NULL);

  gegl_operation_meta_redirect (operation, "radius1", blur, "std-dev-x");
  gegl_operation_meta_redirect (operation, "radius1", blur, "std-dev-y");
  gegl_operation_meta_redirect (operation, "bevel1", emb, "elevation");
  gegl_operation_meta_redirect (operation, "bevel2", emb, "depth");
  gegl_operation_meta_redirect (operation, "azimuth", emb, "azimuth");
  gegl_operation_meta_redirect (operation, "radius2", boxblur, "radius");
  gegl_operation_meta_redirect (operation, "th", th, "value");
  gegl_operation_meta_redirect (operation, "slideupblack", slideupblack, "value");


 /* Now save points to the various gegl nodes so we can rewire them in
   * update_graph() later
   */
  State *state = g_malloc0 (sizeof (State));
  state->input = input;
  state->blur = blur;
  state->boxblur = boxblur;
  state->emb = emb;
  state->th = th;
  state->whitecolor = whitecolor;
  state->normallayer = normallayer;
  state->slideupblack = slideupblack;
  state->fix = fix;
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
    gegl_node_link_many (state->input, state->normallayer, state->blur, state->boxblur,  state->emb, state->fix, state->output, NULL);
    gegl_node_link_many (state->input, state->whitecolor, state->slideupblack,  NULL);
    gegl_node_connect_from (state->normallayer, "aux", state->slideupblack, "output");
  }
else
  {
    gegl_node_link_many (state->input, state->normallayer, state->blur, state->boxblur,  state->emb, state->th, state->fix, state->output, NULL);
    gegl_node_link_many (state->input, state->whitecolor, state->slideupblack,  NULL);
    gegl_node_connect_from (state->normallayer, "aux", state->slideupblack, "output");
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
    "name",        "lb:bevel",
    "title",       _("Bevel (to blend)"),
    "categories",  "Artistic",
    "reference-hash", "45ed5656a28a512570f0f25sb2ac",
    "description", _("You are expected to use GEGL or Gimp blend modes with this plugin. Works best with blend modes multiply and grain merge. Emboss mode requires non-GEGL Gimp blend modes"
                     ""),
    NULL);
}

#endif
