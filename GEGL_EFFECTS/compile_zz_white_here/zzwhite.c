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
*beaver zzcrap
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#define TUTORIAL \
"# uncomment a set of lines below by removing the\n"\
"# leading to test and modify an example, use\n"\
"# use ctrl+a before typing to select all, if you\n"\
"# want a blank slate.\n"\
"#\n"\
"id=in # name a reference to the input buffer 'in'\n"\
"\n"\
"\n"\
"# adaptive threshold:\n"\
"#\n"\
"#threshold aux=[ ref=in gaussian-blur  std-dev-x=0.2rel std-dev-y=0.2rel ]\n"\
"\n"\
"# local white balance and contrast stretching\n"\
"#\n"\
"#divide aux=[  ref=in  median-blur radius=.25rel percentile=100  gaussian-blur std-dev-x=.5rel std-dev-y=.5rel ]\n"\
"\n"\
" color-overlay value=#ffffff   :\n"\
"#\n"\
"#add aux=[  ref=in subtract aux=[ ref=in  median-blur radius=5  ] ] \n"\
"\n"\
"# styled text overlay\n"\
"#\n"\
"#over aux=[ text wrap=1.0rel  color=rgb(0.1,0.1,.3) size=.1rel string=\"ipsum sic amet deliriarium mic sel adendum. Mic fubar bax qux facilium dhat.\" dropshadow radius=.01rel  grow-radius=0.0065rel color=white x=0 y=0 border-align x=0.5 y=0.33  ] # try x=1 y=1\n"\
"\n"\
"\n"\
"# thumbs with misc filters along bottom of image:\n"\
"#\n"\
"#over aux=[\n"\
"#  ref=in scale-ratio x=0.20 y=0.20 newsprint period=0.01rel period2=0.01rel period3=0.01rel period4=0.01rel color-model=cmyk aa-samples=64 pattern=pssquare pattern2=pssquare pattern3=pssquare pattern4=pssquare \n"\
"#  pack gap=0.05rel aux=[ ref=in scale-ratio x=0.20 y=0.20 newsprint period=0.01rel period2=0.01rel period3=0.01rel period4=00.01rel color-model=rgb aa-samples=64  ] \n"\
"#  pack gap=0.05rel aux=[ ref=in scale-ratio x=0.20 y=0.20 id=scaled snn-mean snn-mean crop aux=[ ref=scaled ] ] \n"\
"#  pack gap=0.05rel aux=[ ref=in scale-ratio x=0.20 y=0.20 mosaic tile-size=0.03rel ] \n"\
"#  border-align x=0.5 y=0.9\n"\
"#]\n"\
"\n"\
"# All the examples can be expanded to be on\n"\
"# multiple lines, this graph description\n"\
"# language is not whitespace sensitive, the\n"\
"# rel suffix is relative to image height\n"




#ifdef GEGL_PROPERTIES

property_string (string, _("Circular Pixel Stretch"), TUTORIAL)




#else

#define GEGL_OP_META
#define GEGL_OP_NAME     zzwhite
#define GEGL_OP_C_SOURCE zzwhite.c

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
    "name",        "gegl:zzwhite",
    "title",       _("zzwhite"),
    "categories",  "hidden",
    "reference-hash", "29b15002avbxa4egh8a7ba97b41d8d4e",
    "description", _("Filter only exist to power GEGL Frame"),
    NULL);
}

#endif
