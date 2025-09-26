/* This file is the public operation GEGL API, this API will change to much
 * larger degrees than the api provided by gegl.h
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
 * 2000-2008 Øyvind Kolås.
 */

#ifndef __GEGL_PLUGIN_H__
#define __GEGL_PLUGIN_H__

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <math.h>
#include <string.h>
#include <glib-object.h>
#include <gmodule.h>
#include <gegl.h>
#include <gegl-math.h>
#include <gegl-types.h>
#include <gegl-paramspecs.h>
#include <gegl-audio-fragment.h>

G_BEGIN_DECLS

/* Extra types needed when coding operations */
typedef struct _GeglModule     GeglModule;
typedef struct _GeglModuleInfo GeglModuleInfo;
typedef struct _GeglModuleDB   GeglModuleDB;

/***
 * Writing GEGL operations
 *
 */

/*  increment the ABI version each time one of the following changes:
 *
 *  - the libgeglmodule implementation (if the change affects modules).
 *  - GeglOperation or one of it's base classes changes. (XXX:-
 *    should be extended so a range of abi versions are accepted.
 */

#define GEGL_MODULE_ABI_VERSION 0x000A

struct _GeglModuleInfo
{
  guint32  abi_version;
};

GType gegl_module_register_type (GTypeModule     *module,
                                 GType            parent_type,
                                 const gchar     *type_name,
                                 const GTypeInfo *type_info,
                                 GTypeFlags       flags);

#include <operation/gegl-operation.h>
#include <operation/gegl-operation-context.h>
#include <operation/gegl-operation-filter.h>
#include <operation/gegl-operation-area-filter.h>
#include <operation/gegl-operation-point-filter.h>
#include <operation/gegl-operation-composer.h>
#include <operation/gegl-operation-composer3.h>
#include <operation/gegl-operation-point-composer.h>
#include <operation/gegl-operation-point-composer3.h>
#include <operation/gegl-operation-point-render.h>
#include <operation/gegl-operation-temporal.h>
#include <operation/gegl-operation-source.h>
#include <operation/gegl-operation-sink.h>
#include <operation/gegl-operation-meta.h>
#include <operation/gegl-extension-handler.h>
#include <operation/gegl-operation-handlers.h>
#include <operation/gegl-operation-property-keys.h>

G_END_DECLS

#endif  /* __GEGL_PLUGIN_H__ */
