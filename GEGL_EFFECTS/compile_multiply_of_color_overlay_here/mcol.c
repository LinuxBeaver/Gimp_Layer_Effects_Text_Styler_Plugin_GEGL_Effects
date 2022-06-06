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

property_color  (value, _("Color"), "#ffffff")



#else

#define GEGL_OP_META
#define GEGL_OP_NAME     mcol
#define GEGL_OP_C_SOURCE mcol.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *multiply, *blur2;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");

  multiply = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);

  blur2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);

  gegl_node_link_many (input, multiply, output, NULL);
  gegl_node_link (input, blur2);
  gegl_node_connect_from (multiply, "aux", blur2, "output");

  gegl_node_connect_from (multiply, "aux", blur2, "output");  


  gegl_operation_meta_redirect (operation, "value", blur2, "value");

}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "gegl:mcol",
    "title",       _("Multiplied Color-Overlay"),
    "categories",  "hidden",
    "reference-hash", "45ed5656a238a5125700fc254001b2ac",
    "description", _("This filter only exist to power gegl:effects "
                     ""),
    NULL);
}

#endif
