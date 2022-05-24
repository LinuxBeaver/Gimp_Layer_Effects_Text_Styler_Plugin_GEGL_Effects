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
 * Copyright 2016 Øyvind Kolås <pippin@gimp.org>
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#define TUTORIAL \
" ADD IMAGE FILE HERE.\n"\
" remove the # symbol from svg:src-atop , \n"\
" #svg:src-atop aux=[gegl:layer src=/home/username/pictures/image.png] , \n"\

#ifdef GEGL_PROPERTIES

property_string (string, _("pipeline"), TUTORIAL)
    description(_("[op [property=value] [property=value]] [[op] [property=value]"))
    ui_meta ("multiline", "true")

property_string (error, _("Eeeeeek"), "")
    description (_("There is a problem in the syntax or in the application of parsed property values. Things might mostly work nevertheless."))
    ui_meta ("error", "true")

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     imagefile
#define GEGL_OP_C_SOURCE imagefile.c

#include "gegl-op.h"
#include <unistd.h>


#include <stdio.h>
#include <stdlib.h>

static void
prepare (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglNode *gegl, *input, *output;
  GError *error = NULL;

  gegl = operation->node;

  if (!o->user_data || !g_str_equal (o->user_data, o->string))
  {
    if (o->user_data)
      g_free (o->user_data);
    o->user_data = g_strdup (o->string);

  input  = gegl_node_get_input_proxy (gegl,  "input");
  output = gegl_node_get_output_proxy (gegl, "output");

  gegl_node_link_many (input, output, NULL);
  {
     gchar cwd[81920]; // XXX: should do better
     getcwd (cwd, sizeof(cwd));
  gegl_create_chain (o->string, input, output, 0.0,
                     gegl_node_get_bounding_box (input).height, cwd,
                     &error);
  }

  if (error)
  {
    gegl_node_set (gegl, "error", error->message, NULL);
    g_clear_error (&error);
  }
  else
  {
    g_object_set (operation, "error", "", NULL);
  }
  }
}

static void
attach (GeglOperation *operation)
{
  GeglNode *gegl, *input, *output;

  gegl    = operation->node;

  input  = gegl_node_get_input_proxy (gegl, "input");
  output = gegl_node_get_output_proxy (gegl, "output");

  gegl_node_link_many (input, output, NULL);
  prepare (operation);
}

static void
dispose (GObject *object)
{
  GeglProperties *o = GEGL_PROPERTIES (object);
  if (o->user_data)
  {
    g_free (o->user_data);
    o->user_data = NULL;
  }
}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GObjectClass       *object_class;
  GeglOperationClass *operation_class;

  object_class = G_OBJECT_CLASS  (klass);
  operation_class = GEGL_OPERATION_CLASS (klass);

  object_class->dispose = dispose;
  operation_class->attach = attach;
  operation_class->prepare = prepare;

  gegl_operation_class_set_keys (operation_class,
    "name",        "gegl:imageupload",
    "title",       _("Image file overlay with GEGL"),
    "categories",  "generic",
    "reference-hash", "29bf565ff2g93g18a7ba9cb41d8d4e",
    "description", _("Upload an image file"),
    NULL);
}

#endif
