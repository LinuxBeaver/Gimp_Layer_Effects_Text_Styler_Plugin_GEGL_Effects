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
 * 2023 Beaver long shadow policy 
  Long Shadow Policy is a hidden operation for GEGL Effects that allows GEGL Effects to choose from Long Shadow and Long Shadow PD
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

property_boolean (longshadowpolicy, _("Enable/Disable major text styling plugins"), FALSE)
  description    (_("Enable Long Shadow and Long Shadow Pixel Data here."))

enum_start (long_shadow_type)
  enum_value (TYPE_LSPD, "longshadowpd",
              N_("Long Shadow Pixel Data"))
  enum_value (TYPE_LS,      "longshadow",
              N_("Long Shadow"))
  enum_value (TYPE_FFL, "longshadowffl",
              N_("Long Shadow Fading Fixed Length"))
enum_end (longshadowtype)

property_enum (longshadowtype, _("Long Shadow Type"),
    longshadowtype, long_shadow_type,
    TYPE_LSPD)

property_double (angle, _("Angle"), 45.0)
  description (_("Shadow angle"))
  value_range (-180.0, 180.0)
  ui_meta     ("unit", "degree")
  ui_meta     ("direction", "cw")

property_double (length, _("Length"), 40)
  description (_("Shadow length"))
  value_range (0, 85)
  ui_range    (0, 55)


property_double (lightness, _("Lightness"), 0.0)
   description  (_("Lightness adjustment"))
   value_range  (-30.0, 30.0)


property_double (lengthblur, _("Length of Pixel Data colors (PD Only)"), 100.0)
    description (_("Length of blur in pixels. Recommended to keep this over double the length of the Long Shadow. On Non PD mode this will do nothing."))
    value_range (95.0, 200.0)
    ui_range    (95.0, 200.0)
    ui_gamma    (1.5)
    ui_meta     ("unit", "pixel-distance")

property_double (midpoint, _("Midpoint (FFL Only)"), 0.4)
  description (_("Shadow fade midpoint. On non FFL (Fading Fixed Length) mode this will do nothing."))
  value_range (0.0, 1.0)
  ui_range    (0.0, 1.0)

property_color (color, _("Color (FFL and Normal only)"), "black")
  description (_("Shadow color. This will not work in Pixel Data Mode."))

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     long_shadow_policy
#define GEGL_OP_C_SOURCE long_shadow_policy.c

#include "gegl-op.h"


typedef struct
{
  GeglNode *input;
  GeglNode *fix;
  GeglNode *fix2;
  GeglNode *longshadow;
  GeglNode *longshadowpd;
  GeglNode *longshadowffl;
  GeglNode *lightadjust;
  GeglNode *nop;
  GeglNode *edgesmooth;
  GeglNode *behind;
  GeglNode *output;
} State; 


static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglNode *input, *output, *fix, *fix2, *lightadjust, *longshadow, *longshadowffl, *longshadowpd, *behind, *nop, *edgesmooth;


  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");


  fix = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 0,
                                  NULL);


  fix2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 0,
                                  NULL);
/*Median Blur at 0 is known to prevent many bugs relating to GEGL Graph glitching out. 
Another operation (alpha-clip) is known to solve some bugs relating to opacity, but it 
isn't as good as median at zero.*/

  longshadow = gegl_node_new_child (gegl,
                                  "operation", "gegl:long-shadow", 
                                  NULL);

 longshadowffl = gegl_node_new_child (gegl,
                                  "operation", "gegl:long-shadow", "style", 3, 
                                  NULL);
/*Style 3 is code for Fading Fixed Length*/

  longshadowpd = gegl_node_new_child (gegl,
                                  "operation", "lb:long-shadow-pd", "extract-or-behind", 0,
                                  NULL);

  lightadjust = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma", "lightness",  0.0,
                                  NULL);

  nop = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop", 
                                  NULL);

  edgesmooth = gegl_node_new_child (gegl,
                                  "operation", "lb:edgesmooth", "alpha-percentile2", 20.0,
                                  NULL);

  behind = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst-over", 
                                  NULL);


  gegl_operation_meta_redirect (operation, "angle", longshadow, "angle");
  gegl_operation_meta_redirect (operation, "angle", longshadowpd, "angle");
  gegl_operation_meta_redirect (operation, "angle", longshadowffl, "angle");
  gegl_operation_meta_redirect (operation, "length", longshadow, "length"); 
  gegl_operation_meta_redirect (operation, "length", longshadowpd, "length");
  gegl_operation_meta_redirect (operation, "length", longshadowffl, "length");  
  gegl_operation_meta_redirect (operation, "lightness", lightadjust, "lightness"); 
  gegl_operation_meta_redirect (operation, "lengthblur", longshadowpd, "lengthblur");
  gegl_operation_meta_redirect (operation, "midpoint", longshadowffl, "midpoint-rel");
  gegl_operation_meta_redirect (operation, "color", longshadow, "color");
  gegl_operation_meta_redirect (operation, "color", longshadowffl, "color");


 /* Now save points to the various gegl nodes so we can rewire them in
   * update_graph() later
   */
  State *state = g_malloc0 (sizeof (State));
  state->input = input;
  state->longshadow = longshadow;
  state->longshadowpd = longshadowpd;
  state->longshadowffl = longshadowffl;
  state->fix = fix;
  state->fix2 = fix2;
  state->edgesmooth = edgesmooth;
  state->behind = behind;
  state->nop = nop;
  state->edgesmooth = edgesmooth;
  state->lightadjust = lightadjust;
  state->output = output;
  o->user_data = state;
}

static void

update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;

/*This is the switch between Long Shadow and Long Shadow PD. This boots up each GEGL Graph for each respected meta operation. Else is an instruction to do nothing.
fix AKA median blur at zero is at the beginning and end of every graph to prevent a known bug.*/
    if (o->longshadowpolicy)

switch (o->longshadowtype) {
        break;
    case TYPE_LSPD:
    gegl_node_link_many (state->input,  state->edgesmooth, state->fix, state->nop, state->behind, state->fix2,  state->output, NULL);
  gegl_node_connect (state->behind, "aux", state->lightadjust, "output");
    gegl_node_link_many (state->nop, state->longshadowpd, state->lightadjust,  NULL);
        break;
    case TYPE_LS:
    gegl_node_link_many (state->input, state->fix,  state->longshadow, state->fix2,  state->output, NULL);
        break;
    case TYPE_FFL:
    gegl_node_link_many (state->input, state->fix, state->longshadowffl, state->fix2, state->output, NULL);
        break;
    }
 else
    {
    gegl_node_link_many (state->input, state->output, NULL);
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
    "name",        "lb:ls",
    "title",       _("Long Shadow Policy"),
    "reference-hash", "2po45345235amfaenick23",
    "categories",     "hidden",
    "description", _("Hidden Operation so GEGL Effects can use Long Shadows. This is literally just a selection switch with long shadow and extrusion 2. Other plugins may also use this to call long shadows."
                     ""),
    NULL);
}

#endif
