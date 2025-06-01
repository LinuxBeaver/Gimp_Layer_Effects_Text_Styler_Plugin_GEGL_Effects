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
 * 2022 LongShadow Pixel Data (Beaver)
 */

/*

Recreation of GEGL Graph - May not be 100% accurate or complete but shows the process of how I forked long shadow.
If you feed this to Gimp's GEGL Graph filter you can test this plugin without installing it.

id=0
dst-over aux=[ ref=0 
id=1
long-shadow composition=shadow-only angle=44 length=19
src-atop aux=[ ref=1 motion-blur angle=44 length=39 ]
]
 */


#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

property_double (angle, _("Angle"), 45.0)
  description (_("Shadow angle"))
  value_range (-180.0, 180.0)
  ui_meta     ("unit", "degree")
  ui_meta     ("direction", "cw")

property_double (length, _("Length"), 40)
  description (_("Shadow length"))
  value_range (0, 55)
  ui_range    (0, 55)


property_double (lengthblur, _("Length of Pixel Data colors"), 100.0)
    description (_("Length of blur in pixels. Recommended to keep this over double the length of the Long Shadow"))
    value_range (95, 200.0)
    ui_range    (95, 200.0)
    ui_gamma    (1.5)
    ui_meta     ("unit", "pixel-distance")

property_boolean (ls2, _("Fading Long Shadow mode"), FALSE)
  description    (_("Enable a Fading Long Shadow"))

property_double (chroma, _("Chroma"), 0.0)
   description  (_("Chroma adjustment"))
   value_range  (0.0, 30.0)

property_double (lightness, _("Lightness"), 0.0)
   description  (_("Lightness adjustment"))
   value_range  (-30.0, 30.0)

enum_start (gegl_blend_mode_typelspd)
  enum_value (GEGL_BLEND_MODE_TYPE_STANDALONE,      "standalone",
              N_("Stand Alone"))
  enum_value (GEGL_BLEND_MODE_TYPE_BEHIND, "behind",
              N_("Behind"))
enum_end (GeglBlendModeTypelspd)

property_enum (extract_or_behind, _("Position of Extrusion"),
    GeglBlendModeTypelspd, gegl_blend_mode_typelspd,
    GEGL_BLEND_MODE_TYPE_BEHIND)

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     longshadowpd
#define GEGL_OP_C_SOURCE longshadowpd.c

#include "gegl-op.h"

typedef struct
{
  GeglNode *input;
  GeglNode *alock;
  GeglNode *ls;
  GeglNode *ls2;
  GeglNode *lmb;
  GeglNode *behind;
  GeglNode *normal;
  GeglNode *lightchroma;
  GeglNode *output;
} State; 


static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;

  GeglNode *aloneorbehind = state->normal; /* the default */
  switch (o->extract_or_behind) {
    case GEGL_BLEND_MODE_TYPE_STANDALONE: aloneorbehind = state->normal; break;
    case GEGL_BLEND_MODE_TYPE_BEHIND: aloneorbehind = state->behind; break;
;default: aloneorbehind = state->behind;

}

  if (o->ls2)
  {
         gegl_node_link_many (state->input, aloneorbehind, state->output, NULL);
      gegl_node_connect (aloneorbehind, "aux", state->lightchroma, "output");
         gegl_node_link_many (state->input, state->ls2, state->alock, state->lightchroma, NULL);
      gegl_node_link_many (state->input, state->lmb, NULL);
      gegl_node_connect (state->alock, "aux", state->lmb, "output");
  }
  else
  {
         gegl_node_link_many (state->input, aloneorbehind, state->output, NULL);
      gegl_node_connect (aloneorbehind, "aux", state->lightchroma, "output");
         gegl_node_link_many (state->input, state->ls, state->alock, state->lightchroma, NULL);
      gegl_node_link_many (state->input, state->lmb, NULL);
      gegl_node_connect (state->alock, "aux", state->lmb, "output");
  }
}





static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglNode *input, *output, *ls, *ls2, *alock, *behind, *normal, *lightchroma, *lmb;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");


  alock    = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);

  ls    = gegl_node_new_child (gegl,
                                  "operation", "gegl:long-shadow", "style", 0, "composition", 1,
                                  NULL);


  ls2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:long-shadow", "style", 3, "composition", 1,
                                  NULL);


  lmb    = gegl_node_new_child (gegl,
                                  "operation", "gegl:motion-blur-linear",
                                  NULL);

  lightchroma    = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);

  behind    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst-over",
                                  NULL);

  normal    = gegl_node_new_child (gegl,
                                  "operation", "gegl:over",
                                  NULL);






  gegl_operation_meta_redirect (operation, "angle", ls2, "angle");
  gegl_operation_meta_redirect (operation, "angle", ls, "angle"); 
  gegl_operation_meta_redirect (operation, "length", ls, "length"); 
  gegl_operation_meta_redirect (operation, "angle", lmb, "angle"); 
  gegl_operation_meta_redirect (operation, "lengthblur", lmb, "length"); 
  gegl_operation_meta_redirect (operation, "chroma", lightchroma, "chroma"); 
  gegl_operation_meta_redirect (operation, "lightness", lightchroma, "lightness"); 




 /* Now save points to the various gegl nodes so we can rewire them in
   * update_graph() later
   */
  State *state = g_malloc0 (sizeof (State));
  state->input = input;
  state->ls = ls;
  state->ls2 = ls2;
  state->alock = alock;
  state->lmb = lmb;
  state->lightchroma = lightchroma;
  state->behind = behind;
  state->normal = normal;
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
    "name",        "lb:long-shadow-pd",
    "title",       _("Extrusion via Long Shadow"),
    "reference-hash", "45ed1636v1f3agbr2549lfl5s001b2ac",
    "description", _("Fork of the existing Long Shadow filter but it uses Pixel Data. "
                     ""),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Extrusion 2 (long shadow pixel data)..."),
    NULL);
}

#endif
