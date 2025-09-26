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
 * 2022 Edge Smooth
 */

/*
Rough Recreation of GEGL Graph. This may not be 100% accurate but it is good enough. Using
this method you can run the filter without installing it.

id=1  gegl:over aux=[ ref=1  xor aux=[   median-blur radius=2.4 alpha-percentile=2 ]

id=2  gegl:dst-atop aux=[  ref=2 median-blur radius=2 alpha-percentile=-1 gaussian-blur std-dev-x=2 std-dev-y=2 opacity value=2.7 median-blur radius= percentile=2  alpha-percentile=73  ]
 */


#include "config.h"
#include <glib/gi18n-lib.h>


#ifdef GEGL_PROPERTIES

#define TUTORIAL \
"  id=1  gegl:over aux=[ ref=1  xor aux=[   median-blur radius=2.4 alpha-percentile=2 abyss-policy=none    ]  "\


/*It is mid 2023 and I still don't know how to hide these from the GEGL Graph. Everything else is easy to hide though*/


property_double  (alpha_percentile2, _("Median edge"), 73.0)
  value_range (0, 100)
  description (_("Apply a median blur only on the edges"))


property_double (gaus, _("Blur edge"), 1)
   description (_("Apply a gaussian blur only on the edges of an image"))
   value_range (0.0, 3.0)
   ui_range    (0.24, 3.0)
   ui_gamma    (3.0)
   ui_meta     ("unit", "pixel-distance")
   ui_meta     ("axis", "x")


property_double (value, _("Increase opacity"), 1.2)
    description (_("Increase the opacity only around the edges"))
    value_range (1, 6.0)
    ui_range    (1, 3.0)

enum_start (gegl_median_blur_abyss_policy_es)
   enum_value (GEGL_MEDIAN_BLUR_ABYSS_NONE,  "none",  N_("None"))
   enum_value (GEGL_MEDIAN_BLUR_ABYSS_CLAMP, "clamp", N_("Clamp"))
enum_end (GeglMedianBlurAbyssPolicyes)

property_enum (abyss_policy, _("Abyss Policy"), GeglMedianBlurAbyssPolicyes,
               gegl_median_blur_abyss_policy_es, GEGL_MEDIAN_BLUR_ABYSS_NONE)
  description (_("How image edges are handled. In default they don't clip but when applying this filter on subjects it makes sense for it to clip"))



#else

#define GEGL_OP_META
#define GEGL_OP_NAME     smoothedge
#define GEGL_OP_C_SOURCE smoothedge.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *graph, *behind, *median, *median2, *fixgraph,  *gaussian, *opacity;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");



  median    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 2, "alpha-percentile", 2.0, "abyss-policy",     GEGL_ABYSS_NONE,
                                  NULL);

  median2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 2, "percentile", 1.0, "abyss-policy",     GEGL_ABYSS_NONE,
                                  NULL);

  fixgraph    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 0, "abyss-policy",     GEGL_ABYSS_NONE,
                                  NULL);

/*
This "fixgraph" is for Gimp's non-destructive future. Median Blur at 0 makes no modifications to an image but solves a
bug by resetting gegl opacity. egl:opacity adjust the global opacity of an image; resulting in filters like "drop shadow" behaving
in a damaged way because there global opacity is way to high. Median blur radius=0 resets its global opacity.
 */


  gaussian    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur", "clip-extent", FALSE,
                                  NULL);

  opacity    = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

  behind    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst-atop",
                                  NULL);

  graph    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", TUTORIAL,
                                  NULL);


gegl_node_link_many(input, graph, behind, fixgraph, output, NULL);
gegl_node_link_many(input,  median, gaussian, opacity, median2,  NULL);
gegl_node_connect (behind, "aux", median2, "output");

  gegl_operation_meta_redirect (operation, "gaus", gaussian, "std-dev-x");
  gegl_operation_meta_redirect (operation, "gaus", gaussian, "std-dev-y");
  gegl_operation_meta_redirect (operation, "alpha_percentile2", median2, "alpha-percentile");
  gegl_operation_meta_redirect (operation, "value", opacity, "value");
  gegl_operation_meta_redirect (operation, "abyss_policy", median, "abyss-policy");
  gegl_operation_meta_redirect (operation, "abyss_policy", median2, "abyss-policy");
  gegl_operation_meta_redirect (operation, "abyss_policy", fixgraph, "abyss-policy");
}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:edgesmooth",
    "title",       _("Rough Edge Smoother"),
    "reference-hash", "45ed5656a11bgxxdt27730vaefe2g4f1b2ac",
    "description", _("GEGL will apply a median blur and a few other things around a transparent images edges to smooth edges "
                     ""),
    "gimp:menu-path", "<Image>/Filters/Blur",
    "gimp:menu-label", _("Smooth edges (blur on edges only)..."),
    NULL);
}

#endif
