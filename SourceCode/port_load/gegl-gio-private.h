/* This file is part of GEGL
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
 * Copyright 2014 Jon Nordby, The Grid <jononor@gmail.com>
 */

#include <glib.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#ifndef __GEGL_GIO_PRIVATE_H__
#define __GEGL_GIO_PRIVATE_H__

GInputStream *
gegl_gio_open_input_stream(const gchar *uri, const gchar *path, GFile **out_file, GError **err);

GOutputStream *
gegl_gio_open_output_stream(const gchar *uri, const gchar *path, GFile **out_file, GError **err);

gboolean
gegl_gio_uri_is_datauri(const gchar *uri);

gchar *
gegl_gio_datauri_get_content_type(const gchar *uri);

#endif // __GEGL_GIO_PRIVATE_H__

G_END_DECLS
