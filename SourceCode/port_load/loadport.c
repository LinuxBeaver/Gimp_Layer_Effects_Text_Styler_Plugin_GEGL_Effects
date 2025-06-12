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
 * Credit to Øvind Kolas (pippin) for major GEGL contributions
 */

#include "config.h"
#include <glib/gi18n-lib.h>
#include <gegl-metadata.h>

#ifdef G_OS_WIN32
#define realpath(a,b) _fullpath(b,a,_MAX_PATH)
#endif

#ifdef GEGL_PROPERTIES

property_file_path (src, _("File"), "")
    description (_("Path of file to load."))
property_uri (uri, _("URI"), "")
    description (_("URI of file to load."))
property_object (metadata, _("Metadata"), GEGL_TYPE_METADATA)
    description (_("Object to supply image metadata"))

#else

#define GEGL_OP_NAME     loadport
#define GEGL_OP_C_SOURCE loadport.c

#include <gegl-plugin.h>
#include <gegl-gio-private.h>

struct _GeglOp
{
  GeglOperationMeta parent_instance;
  gpointer          properties;

  GeglNode *output;
  GeglNode *load;
};

typedef struct
{
  GeglOperationMetaClass parent_class;
} GeglOpClass;

#include <gegl-op.h>
GEGL_DEFINE_DYNAMIC_OPERATION(GEGL_TYPE_OPERATION_META)

#include <stdio.h>
#include <stdlib.h>
#define SNIFFING_LENGTH 4096

static gboolean
read_from_stream (GInputStream *stream,
                  guchar      **buffer,
                  gsize        *read,
                  GError      **error)
{
  const gsize size = SNIFFING_LENGTH;

  *read = 0;
  *buffer = g_try_new (guchar, size);

  g_assert (buffer != NULL);

  return g_input_stream_read_all (stream, *buffer, size, read, NULL, error);
}

static void
do_setup (GeglOperation *operation, const gchar *path, const gchar *uri)
{
  GeglOp  *self = GEGL_OP (operation);
  GeglProperties *o = GEGL_PROPERTIES (operation);
  const gchar *handler = NULL;
  gchar *content_type = NULL, *filename = NULL, *message;
  gboolean load_from_uri, uncertain;
  GInputStream *stream = NULL;
  GError *error = NULL;
  GFile *file = NULL;
  guchar *buffer = NULL;
  gsize size;

  if (uri != NULL && strlen (uri) > 0)
    {
      if (!gegl_gio_uri_is_datauri (uri))
        filename = g_filename_display_name (uri);
      stream = gegl_gio_open_input_stream (uri, NULL, &file, &error);
      if (stream == NULL || (file == NULL && !gegl_gio_uri_is_datauri (uri)))
        {
          if (!gegl_gio_uri_is_datauri (uri))
            {
              if (g_error_matches (error, G_IO_ERROR, G_IO_ERROR_NOT_FOUND))
                {
                  message = g_strdup_printf ("%s does not exist", filename);
                  gegl_node_set (self->load,
                                 "operation", "gegl:text",
                                 "string", message,
                                 "size", 0.0,
                                 NULL);
                  g_free (message);
                }

              g_warning ("%s does not exist or could not be opened", filename);
            }
          else
            g_warning ("datauri could not be parsed");

          g_clear_error (&error);
          goto cleanup;
        }
      load_from_uri = TRUE;
    }
  else if (path != NULL && strlen (path) > 0)
    {
      gchar *resolved_path = realpath (path, NULL);
      if (resolved_path)
        {
          filename = g_filename_display_name (resolved_path);

          stream = gegl_gio_open_input_stream (NULL, resolved_path, &file, &error);
          if (stream == NULL || file == NULL)
            {
              if (g_error_matches (error, G_IO_ERROR, G_IO_ERROR_NOT_FOUND))
                {
                  message = g_strdup_printf ("%s does not exist", filename);
                  gegl_node_set (self->load,
                                 "operation", "gegl:text",
                                 "string", message,
                                 "size", 0.0,
                                 NULL);
                  g_free (message);
                }
              g_warning ("%s does not exist or could not be opened", filename);
              g_clear_error (&error);
              free (resolved_path);
              goto cleanup;
            }
          load_from_uri = FALSE;
          free (resolved_path);
        }
      else
        {
          gegl_node_set (self->load,
                         "operation", "gegl:text",
                         "string", "load failed",
                         "size", 0.0,
                         NULL);
          goto cleanup;
        }
    }
  else
    {
      gegl_node_set (self->load,
                     "operation", "gegl:text",
                     "string", "",
                     "size", 0.0,
                     NULL);
/*No path or URI specified*/
      return;
    }

  g_assert (stream != NULL);

  if (load_from_uri)
    {
      if (!read_from_stream (stream, &buffer, &size, &error))
        {
          g_warning ("%s", error->message);
          g_clear_error (&error);
          goto cleanup;
        }

      content_type = g_content_type_guess (NULL, buffer, size, &uncertain);
      if ((!g_str_has_prefix (content_type, "image/") &&
           !g_str_has_prefix (content_type, ".")) || uncertain)
        {
          g_free (content_type);

          if (gegl_gio_uri_is_datauri (uri))
            {
              content_type = gegl_gio_datauri_get_content_type (uri);
            }
          else
            {
              content_type = g_content_type_guess (filename,
                                                   buffer,
                                                   size,
                                                   NULL);
            }
        }
    }
  else
    {
      /* This should match the logic in glib/gio/glocalfileinfo.c for local
       * files. Otherwise, our interpretation of the content won't match
       * with those of other components. Contrary to what we might expect,
       * GLib first looks at the filename, and sniffs the content only
       * if it is inconclusive.
       */

      content_type = g_content_type_guess (filename, NULL, 0, &uncertain);
      if ((!g_str_has_prefix (content_type, "image/") &&
           !g_str_has_prefix (content_type, ".")) || uncertain)
        {
          if (!read_from_stream (stream, &buffer, &size, &error))
            {
              g_warning ("%s", error->message);
              g_clear_error (&error);
              goto cleanup;
            }
          g_free (content_type);
          content_type = g_content_type_guess (filename, buffer, size, NULL);
        }
    }

  if (!gegl_gio_uri_is_datauri (uri) &&
      !g_str_has_prefix (content_type, "image/") &&
      !g_str_has_prefix (content_type, "."))
    {
      g_free (content_type);

      if (g_strrstr (filename, ".") != NULL)
        content_type = g_strdup (g_strrstr (filename, "."));
      else
        content_type = NULL;
    }

  if (content_type == NULL)
    {
      gegl_node_set (self->load,
                     "operation", "gegl:text",
                     "string", "Failed to detect content type",
                     "size", 0.0,
                     NULL);
      goto cleanup;
    }

  handler = gegl_operation_handlers_get_loader (content_type);
  if (handler == NULL)
    {
      gegl_node_set (self->load,
                     "operation", "gegl:text",
                     "string", "Failed to find a loader",
                     "size", 0.0,
                     NULL);
      goto cleanup;
    }

  gegl_node_set (self->load, "operation", handler, NULL);

  if (o->metadata &&
      gegl_operation_find_property (handler, "metadata") != NULL)
    gegl_node_set (self->load, "metadata", o->metadata, NULL);

  if (load_from_uri == TRUE)
    gegl_node_set (self->load, "uri", uri, NULL);
  else
    gegl_node_set (self->load, "path", path, NULL);

cleanup:

  if (stream != NULL)
    {
      g_input_stream_close (stream, NULL, NULL);
      g_object_unref (stream);
    }

  g_clear_object (&file);

  g_free (buffer);

  g_free (content_type);
  g_free (filename);
}

static void
attach (GeglOperation *operation)
{
  GeglOp         *self = GEGL_OP (operation);
  GeglProperties *o    = GEGL_PROPERTIES (operation);

  self->output = gegl_node_get_output_proxy (operation->node, "output");

  self->load = gegl_node_new_child (operation->node,
                                    "operation", "gegl:text",
                                    NULL);

  do_setup (operation, o->src, o->uri);

  gegl_node_link (self->load, self->output);
}

static GeglNode *
detect (GeglOperation *operation,
        gint           x,
        gint           y)
{
  GeglOp *self = GEGL_OP (operation);
  GeglNode *output = self->output;
  GeglRectangle bounds;

  bounds = gegl_node_get_bounding_box (output); /* hopefully this is
                                                   as correct as original
                                                   which was peeking
                                                   directly into output->have_rect
                                                   */

  if (x >= bounds.x &&
      y >= bounds.y &&
      x  < bounds.x + bounds.width &&
      y  < bounds.y + bounds.height)
    return operation->node;

  return NULL;
}

static void
my_set_property (GObject      *gobject,
                 guint         property_id,
                 const GValue *value,
                 GParamSpec   *pspec)
{
  GeglOperation  *operation = GEGL_OPERATION (gobject);
  GeglOp         *self      = GEGL_OP (operation);
  GeglProperties *o         = GEGL_PROPERTIES (operation);

  gchar *old_path = g_strdup (o->src);
  gchar *old_uri = g_strdup (o->uri);
  void  *old_metadata = o->metadata;

  gboolean props_changed;

  /* The set_property provided by the chant system does the
   * storing and reffing/unreffing of the input properties
   */
  set_property (gobject, property_id, value, pspec);
  props_changed = g_strcmp0 (o->src, old_path) || g_strcmp0 (o->uri, old_uri) || (old_metadata != o->metadata);

  if (self->load && props_changed)
    do_setup (operation, o->src, o->uri);
  g_free (old_path);
  g_free (old_uri);
}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GObjectClass       *object_class    = G_OBJECT_CLASS (klass);
  GeglOperationClass *operation_class = GEGL_OPERATION_CLASS (klass);

  object_class->set_property = my_set_property;

  operation_class->attach = attach;
  operation_class->detect = detect;

  gegl_operation_class_set_keys (operation_class,
    "name"       , "port:load",
    "title",       "Load Image",
    "categories" , "meta:input",
    "description",
          _("Multipurpose file loader, that uses other native handlers, and "
            "fallback conversion using Image Magick's convert."),
    NULL);

}

#endif
