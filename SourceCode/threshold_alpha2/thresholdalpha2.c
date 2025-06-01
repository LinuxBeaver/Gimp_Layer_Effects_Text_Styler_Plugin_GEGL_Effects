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
 * 2023 Beaver Threshold Alpha 2
 */

/* 
This filter can be tested without installing by running this syntax inside Gimp's GEGL Graph filter.

id=0
gimp:layer-mode layer-mode=split composite-mode=auto aux=[ ref=0 
color-overlay value=#ffffff 
id=1 dst-over aux=[ ref=1 color value=#000000 ]
levels in-low=0.5
color-to-alpha  ]
 */


#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES


property_double (alphadegree, _("Value"), 0.4)
    description ( _("Increase the Alpha channel intensity"))
    value_range    (0.0, 0.95)
  ui_steps      (0.01, 0.01)


#else

#define GEGL_OP_META
#define GEGL_OP_NAME     thresholdalpha2
#define GEGL_OP_C_SOURCE thresholdalpha2.c

#include "gegl-op.h"



static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *black, *white, *alphaslider, *erase, *behind, *c2a, *output;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");
  GeglColor *threshold_alpha_embedded_color = gegl_color_new ("#ffffff");
  GeglColor *threshold_alpha_embedded_color2 = gegl_color_new ("#000000");


 alphaslider   = gegl_node_new_child (gegl,
                                  "operation", "gegl:levels",  NULL);

 behind   = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst-over",  NULL);

/*This is Gimp's split blend mode. Similar to erase except it gets rid of edge puff better. I prefer this over erase.
NOTE GEGL'S XOR (gegl:xor) WILL NOT WORK PROPER WITH THIS AS IT USES THE UNION BLEND MODE AND THAT BREAKS THINGS BY NOT MAKING IT TRANSPARENT.*/
erase = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 60, "composite-mode", 0, NULL);



 c2a   = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-to-alpha", "transparency-threshold", 0.1,  NULL);



 black   = gegl_node_new_child (gegl,
                                  "operation", "gegl:color", "value", threshold_alpha_embedded_color2,  NULL);



 white   = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay", "value", threshold_alpha_embedded_color,  NULL);

/*Everything is being put inside the split blend mode*/
  gegl_node_link_many (input, erase, output, NULL);
  gegl_node_connect (erase, "aux", c2a, "output");
/*Main contents inside split blend mode. (color overlay, behind blend mode, levels slider, color to alpha.  */
  gegl_node_link_many (input, white, behind, alphaslider, c2a, NULL);
/*Black color fill is put inside the behind blend mode. Below a white color overlay to make a "layer mask"  */
  gegl_node_connect (behind, "aux", black, "output"); 

 
/*The only instruction for the GUI is here. This filter has only one slider*/
  gegl_operation_meta_redirect (operation, "alphadegree", alphaslider, "in-low");

}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:threshold-alpha",
    "title",       _("Threshold Alpha 2"),
    "categories",  "plugins",
    "reference-hash", "threso1alphbgxxf34t27730vaefe2g4f1b2ac",
    "description", _("A better version of Gimp's threshold alpha filter to increase alpha channel intensity without killing transparency."
                     ""),
    NULL);
}

#endif
