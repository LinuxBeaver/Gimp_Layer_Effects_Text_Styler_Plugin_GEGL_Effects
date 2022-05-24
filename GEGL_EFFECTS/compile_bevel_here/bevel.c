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
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES



property_double (radius1, _("Radius"), 7.0)
  value_range (1.0, 40.0)
  ui_range (1.0, 12)
  ui_gamma (1.5)



property_double (bevel1, _("Depth Angle"), 90.0)
    description (_("Elevation angle (degrees)"))
    value_range (0, 180)
    ui_meta ("unit", "degree")

property_int (bevel2, _("Depth"), 40)
    description (_("Filter width"))
    value_range (1, 100)


property_double (th, _("Threshold of the Bevel's Transparency'"), 0.25)
  value_range (0.0, 1.0)
  ui_range (0.0, 0.5)

property_double (azimuth, _("Rotate Lighting"), 40.0)
    description (_("Light angle (degrees)"))
    value_range (0, 350)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")


#else

#define GEGL_OP_META
#define GEGL_OP_NAME     bevel
#define GEGL_OP_C_SOURCE bevel.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *blur, *emb, *th;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");

   blur = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
                                  NULL);


 emb   = gegl_node_new_child (gegl,
                                  "operation", "gegl:emboss",
                                  NULL);

  th = gegl_node_new_child (gegl,
                                  "operation", "gimp:threshold-alpha",
                                  NULL);



  gegl_node_link_many (input, blur, emb, th, output, NULL);

  gegl_operation_meta_redirect (operation, "radius1", blur, "std-dev-x");
  gegl_operation_meta_redirect (operation, "radius1", blur, "std-dev-y");

  gegl_operation_meta_redirect (operation, "bevelhidden", emb, "");

  gegl_operation_meta_redirect (operation, "bevel1", emb, "elevation");

  gegl_operation_meta_redirect (operation, "bevel2", emb, "depth");

  gegl_operation_meta_redirect (operation, "azimuth", emb, "azimuth");


  gegl_operation_meta_redirect (operation, "th", th, "value");


}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "gegl:bevel",
    "title",       _("Bevel"),
    "categories",  "Aristic",
    "reference-hash", "45ed5656a28a512570f0f25sb2ac",
    "description", _("Bevel Images using GEGL.  Use the multiply blend mode "
                     ""),
    NULL);
}

#endif
