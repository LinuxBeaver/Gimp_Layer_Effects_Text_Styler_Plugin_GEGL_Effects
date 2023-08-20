/*This file is an image processing operation for GEGL
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
 * 2023 Beaver (GEGL sharp bevel) 
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

/*
THIS FILTER IS NOT A CLONE OF CUSTOM BEVEL. IT IS A HEAVY MODIFICATION TO IT THAT MAKES AN ORIGINAL BEVEL EFFECT. 
This plugin is very resource intensive because I was going towards quality.

The sharp bevel graph recreation. You can change hardlight to other blend modes. 
If you feed this to Gimp's GEGL Graph filter you can get a static preview of this filter without installing it.

This requires another plugin of mine lb:threshold-alpha that is included. 

Both Median Blur radius=6's and neighborhood are meant to update together and they can't do this in a graph.

The id and ref named "originalbg" are part of a special technique I found to preserve the original color of the image.

GRAPH 1

median-blur radius=6 neighborhood=diamond
id=color
id=originalbg
median-blur radius=6 neighborhood=diamond
distance-transform 
denoise-dct sigma=10
median-blur radius=0
color-to-alpha color=#000000
id=0 gimp:layer-mode layer-mode=lighten-only  aux=[ ref=0 emboss azimuth=83 depth=63 ]
opacity value=2.5
median-blur radius=0 
lb:threshold-alpha
gegl:gray hue-chroma lightness=3
#gimp:layer-mode layer-mode=burn composite-mode=auto opacity=0.95 aux=[ layer src=/home/USERNAME/Pictures/Magic/3_angular_w2aves.png ]
multiply aux=[ ref=originalbg ]
lb:edgesmooth alpha-percentile2=100 gaus=0.8
lb:threshold-alpha

END OF GRAPH 1, 

GRAPH 2 - WITH THE CHECKBOX FOR COLOR PRIORITY 

median-blur radius=6 neighborhood=diamond
distance-transform 
denoise-dct sigma=10
median-blur radius=0
color-to-alpha color=#000000
color-overlay value=#ff00f6
id=0 gimp:layer-mode layer-mode=lighten-only  aux=[ ref=0 emboss azimuth=83 depth=63 ]
opacity value=2.5
median-blur radius=0 
lb:threshold-alpha
lb:edgesmooth alpha-percentile2=100 gaus=0.8
lb:threshold-alpha
 */


#define EMBEDDEDGRAPH \
" gegl:gray  hue-chroma lightness=3 "\
/* This is an embedded gegl graph for the plugin*/

property_boolean (bevelcolorpolicy , _("Enable Color Priority mode"), FALSE)
  description    (_("This (when enabled) disables the bevel's ability to get the layers color information. But in return the color overlay will be enabled. Due to the logic of this filter very dark colors or black will not work with it."))


property_int  (size, _("Size of the Bevel"), 3)
  value_range (1, 9)
  ui_range    (1, 9)
  ui_meta     ("unit", "pixel-distance")
  description (_("Median Radius to control the size of the bevel"))

property_double (bevelcontrol, _("Bevel's Flat Surface control"), 1)
    value_range (1.0, 6.0)
  description (_("Moving this slider up will give the bevel a flat surface. At 1 it will be a default sharp surface."))

property_double (azimuth, _("Azimuth"), 67.0)
    description (_("Light angle (degrees)"))
    value_range (20, 90)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")

property_double (elevation, _("Elevation"), 25.0)
    description (_("Elevation angle (degrees). This appears to rotate the brightest pixels."))
    value_range (7, 140)
    ui_meta ("unit", "degree")

property_int (depth, _("Depth and or detail"), 24)
    description (_("Brings out depth and or detail of the bevel depending on the blend mode"))
    value_range (8, 100)


property_double (smooth, _("Smooth Bevel"), 9.0)
    description (_("Smooth the Bevel with Denoise DCT to make it less rough looking. If this is on very low values  the bevel will look very rough, but on very high values it will make the filter slow."))
    value_range (1, 20)

property_double (sharpen, _("Sharpen Radius"), 0.0)
    description (_("Unsharp mask radius that helps the bevel apper to have depth and structure to it. This is most useful on blend modes like plus and lighten when depth detail is high. But should be avoided on blend modes like darken only and multiply. As it will just make the already dark bevel darker. This effect is also color specific. Out of all the sliders in this filter this is the most situational one."))
    value_range (0, 100)



enum_start (gegl_blend_mode_typesharpbevel)
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHT, "hardlight",
              N_("HardLight"))
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLY,      "multiply",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_COLORDODGE,      "colordodge",
              N_("Color Dodge"))
  enum_value (GEGL_BLEND_MODE_TYPE_PLUS,      "plus",
              N_("Plus"))
  enum_value (GEGL_BLEND_MODE_TYPE_DARKEN,      "darken",
              N_("Darken"))
  enum_value (GEGL_BLEND_MODE_TYPE_LIGHTEN,      "lighten",
              N_("Lighten"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAY,      "overlay",
              N_("Overlay"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGE,      "grainmerge",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOFTLIGHT,      "softlight",
              N_("Soft Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_ADDITION,      "addition",
              N_("Addition"))
  enum_value (GEGL_BLEND_MODE_TYPE_EMBOSSBLEND,      "embossblend",
              N_("Grayscale Multiply for Image uploads"))
enum_end (GeglBlendModeTypesharpbevel)
/* This ENUM list has a unique name to avoid conflicts with other plugins or Gimp filters*/

property_enum (blendmode, _("Blend Mode of Internal Emboss"),
    GeglBlendModeTypesharpbevel, gegl_blend_mode_typesharpbevel,
    GEGL_BLEND_MODE_TYPE_HARDLIGHT)
    description (_("Blend mode of the internal emboss. Different blend modes will change the bevel, to make the bevel have more depth, more shine or lack of shine and much less depth."))


property_enum (metric, _("Distance Map Setting"),
               GeglDistanceMetric, gegl_distance_metric, GEGL_DISTANCE_METRIC_CHEBYSHEV)
    description (_("Distance Map has three settings that alter the structure of the bevel. Chebyshev is the default; due to it being the best. But try the other two. "))

/* In almost all cases ENUM list must be renamed. This seems to be an exception to the rule about ENUM list sharing the same name breaking GEGL plugins. It getting information from gegl-enums.c.
That might have something to do with this exception. I don't understand why this exception exist - but it does.*/


enum_start (gegl_median_blur_neighborhoodsharpbevel)
  enum_value (GEGL_MEDIAN_BLUR_NEIGHBORHOOD_SQUARE,  "square",  N_("Square"))
  enum_value (GEGL_MEDIAN_BLUR_NEIGHBORHOOD_CIRCLE,  "circle",  N_("Circle"))
  enum_value (GEGL_MEDIAN_BLUR_NEIGHBORHOOD_DIAMOND, "diamond", N_("Diamond"))
enum_end (GeglMedianBlurNeighborhoodsharpbevel)
/* This ENUM list has a unique name to avoid conflicts with other plugins or Gimp filters*/


property_enum (shape, _("Internal Median Shape"),
               GeglMedianBlurNeighborhoodsharpbevel, gegl_median_blur_neighborhoodsharpbevel,
               GEGL_MEDIAN_BLUR_NEIGHBORHOOD_CIRCLE)
  description (_("Base shape structure of the bevel. There is a problem where --Square-- leaves noticable black pixel artifact, which can be solved with a slider below."))


property_color (color, _("Color Overlay"), "#ffffff")
    description (_("If color priority mode is enabled this slider will overwrite the default color and make it whatever you the user wants. When the checkbox is disabled the bevel will resort to the default color and this option will be disabled. Due to the logic of this filter it will not work with black or very dark colors."))
  ui_meta     ("sensitive", " bevelcolorpolicy")

property_file_path(src, _("Image file Overlay - Make Color white for best results."), "")
    description (_("Source image file path (png, jpg, raw, svg, bmp, tif, ...) You can remove an image file overlay by going back to the image file select window, removing all text from --location-- then click open. Image file overlay will then go back to its default (None)"))

property_double (ollight, _("Levels - Low output (lighting control)"), 0.0)
    description (_("Levels Low Output slider in the negative values. Allow the user to have limited control of the lighting of the bevel. This is best used in combination with image file overlays."))
    value_range    (-0.2, 0.0)


property_double (transvalue, _("Black artifact fix via transparency threshold"), 0.05)
    description(_("On non color priority mode this filter currently has a bug where its internal settings generates a black artifact. This helps fix that problem. On color priority mode this is still useful as it decreases the size of the bevel by shrinking its borders."))
    value_range (0.0, 0.3)

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     sharpbevel
#define GEGL_OP_C_SOURCE sharpbevel.c

#include "gegl-op.h"

typedef struct
{
  GeglNode *input;
  GeglNode *median;
  GeglNode *fix;
  GeglNode *sharpen;
  GeglNode *fix3;
  GeglNode *multiply3;
  GeglNode *multiply4;
  GeglNode *idref;
  GeglNode *graph;
  GeglNode *fix2;
  GeglNode *ta2;
  GeglNode *ta2again;
  GeglNode *c2a;
  GeglNode *smooth;
  GeglNode *edgesmooth;
  GeglNode *dt;
  GeglNode *hardlight;
  GeglNode *multiply;
  GeglNode *colordodge;
  GeglNode *emboss;
  GeglNode *plus;
  GeglNode *darken;
  GeglNode *lighten;
  GeglNode *opacity;
  GeglNode *multiply2;
  GeglNode *multiply5;
  GeglNode *nop;
  GeglNode *levels;
  GeglNode *col;
  GeglNode *allowblack;
  GeglNode *imagefileoverlay;
  GeglNode *grainmerge;
  GeglNode *overlay;
  GeglNode *softlight;
  GeglNode *addition;
  GeglNode *embossblend;
  GeglNode *output;
}State;

static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;


  GeglNode *usethis = state->hardlight; /* the default */
  switch (o->blendmode) {
    case GEGL_BLEND_MODE_TYPE_MULTIPLY: usethis = state->multiply; break;
    case GEGL_BLEND_MODE_TYPE_COLORDODGE: usethis = state->colordodge; break;
    case GEGL_BLEND_MODE_TYPE_PLUS: usethis = state->plus; break;
    case GEGL_BLEND_MODE_TYPE_DARKEN: usethis = state->darken; break;
    case GEGL_BLEND_MODE_TYPE_LIGHTEN: usethis = state->lighten; break;
    case GEGL_BLEND_MODE_TYPE_OVERLAY: usethis = state->overlay; break;
    case GEGL_BLEND_MODE_TYPE_GRAINMERGE: usethis = state->grainmerge; break;
    case GEGL_BLEND_MODE_TYPE_SOFTLIGHT: usethis = state->softlight; break;
    case GEGL_BLEND_MODE_TYPE_ADDITION: usethis = state->addition; break;
    case GEGL_BLEND_MODE_TYPE_EMBOSSBLEND: usethis = state->embossblend; break;
default: usethis = state->hardlight;

}

  if (o->bevelcolorpolicy)
  {
  gegl_node_link_many (state->input, state->allowblack, state->median, state->dt, state->multiply5, state->smooth, state->fix, state->c2a, state->col, state->nop, usethis, state->opacity, state->fix2, state->ta2, state->levels, state->multiply2, state->edgesmooth, state->ta2again, state->sharpen, state->output, NULL);
/* Most of the GEGL nodes are here. usethis is a potential blend that users can choose. The nop behaves like a id/ref */
  gegl_node_connect_from (usethis, "aux", state->emboss, "output");
  gegl_node_link_many (state->nop, state->emboss,  NULL);
/* Emboss is being put inside a blend mode of the users choice. */
  gegl_node_connect_from (state->multiply2, "aux", state->imagefileoverlay, "output");
/* Image file overlay is being fused with the multiply blend mode. */ 
  }


else
  {
  gegl_node_link_many (state->input, state->allowblack, state->fix3, state->multiply4, state->idref, state->median, state->dt, state->multiply5, state->smooth, state->fix, state->c2a, state->nop, usethis, state->opacity, state->fix2, state->ta2, state->graph, state->multiply2,  state->multiply3, state->levels, state->edgesmooth, state->ta2again, state->sharpen, state->output, NULL);
/* Most of the GEGL nodes are here. usethis is a potential blend that users can choose. The nop behaves like a id/ref */
  gegl_node_connect_from (usethis, "aux", state->emboss, "output");
  gegl_node_link_many (state->nop, state->emboss,  NULL);
/* Emboss is being put inside a blend mode of the users choice. */
  gegl_node_connect_from (state->multiply2, "aux", state->imagefileoverlay, "output");
/* Image file overlay is being fused with the multiply blend mode. */
  gegl_node_connect_from (state->multiply3, "aux", state->idref, "output");
/*The idref node is a nop that was placed very early in the graph
to bookmark the original gimp layer content. Now it is being recalled 
to grab image data that will be median blurred and multiply blended on
a white bevel so that the bevel can maintain the images color.

To explain this in very simple terms. Without this the bevel (if applied on text of any color; such as red) would lose its original color and turn white.
This node grabs a copy of the layer before effects are applied, median blurs it, and fuses it with the multiply blend mode over a white bevel. */
  }
}

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglNode *input, *output, *fix, *fix2, *fix3, *smooth, *allowblack, *edgesmooth, *multiply3, *multiply4, *multiply5, *sharpen, *levels, *idref, *graph, *ta2, *ta2again, *opacity, *multiply, *dt, *c2a, *multiply2,  *imagefileoverlay, *median, *hardlight, *emboss,  *embossblend, *addition, *colordodge, *grainmerge, *softlight, *overlay, *darken, *lighten,  *col, *nop, *plus;
  GeglColor *embeddedcolorbevel2 = gegl_color_new ("#000000");
/* This is an embedded color (black) */

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");



  fix    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 0,
                                  NULL);

  fix2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 0,
                                  NULL);

  fix3    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur",
                                  NULL);
/* Fix 3 is a special case that updates with defined median. Its radius and neigborhood update together with "median"*/


 /*"Fix"is a critical operation for Gimp's non-destructive future.
A median blur at zero radius is confirmed to make no changes to an image. 
This option resets gegl:opacity's value to prevent many bugs.
plugins like clay, glossy balloon and custom bevel glitch out when
drop shadow is applied in a gegl graph below them. median 0 solves this.*/

  graph    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", EMBEDDEDGRAPH,
                                  NULL);



  median    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur",
                                  NULL);
/* This defined median updates with fix 3. Its radius and neigborhood update together with "fix3"*/


  ta2    = gegl_node_new_child (gegl,
                                  "operation", "lb:threshold-alpha",
                                  NULL);

 ta2again    = gegl_node_new_child (gegl,
                                  "operation", "lb:threshold-alpha",
                                  NULL);

/* This is a dependency I made to remove edge puff or semi transparent artifact around edges. In this case it is removing artfifact around the bevel.  */

  edgesmooth    = gegl_node_new_child (gegl,
                                  "operation", "lb:edgesmooth", "alphapercentile2", 100, "gaus", 0.7,
                                  NULL);
/* This is a dependency I made to smooth edges. In this case it is smoothing edges of the bevel */


  c2a    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-to-alpha", "color", embeddedcolorbevel2,
                                  NULL);
/* This is an embedded color (black) */


  imagefileoverlay    = gegl_node_new_child (gegl,
                                  "operation", "gegl:layer",
                                  NULL);
/* GEGL layer is best used for image file overlays */

  idref    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  sharpen    = gegl_node_new_child (gegl,
                                  "operation", "gegl:unsharp-mask",
                                  NULL);



  nop    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  levels    = gegl_node_new_child (gegl,
                                  "operation", "gegl:levels",
                                  NULL);

  allowblack    = gegl_node_new_child (gegl,
                                  "operation", "gegl:levels", "out-low", 0.006,
                                  NULL);

  emboss    = gegl_node_new_child (gegl,
                                  "operation", "gegl:emboss",
                                  NULL);

  smooth    = gegl_node_new_child (gegl,
                                  "operation", "gegl:denoise-dct",
                                  NULL);
/* This is a new operation that will only work in Gimp 2.10.32 and up */

  col   = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);


  dt    = gegl_node_new_child (gegl,
                                  "operation", "gegl:distance-transform", "metric", 0,
                                  NULL);
/* This is the operation that makes the bevel sharp. Looking at Oyvind's modification to my "custom bevel" (now gegl:chamfer in Gimp 2.99) gave me this idea.
Though I must clarify that I went much further with it. */



  opacity   = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity", "value", 2.5,
                                  NULL);
 /*Opacities settings are baked in*/


 /*GEGL blend modes listed here*/

  multiply    = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);

  multiply5    = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);



 /*This blend mode is used by image file upload. All the other blend modes are reversed for emboss.*/

  hardlight    = gegl_node_new_child (gegl,
                                  "operation", "gegl:hard-light",
                                  NULL);

  colordodge    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-dodge",
                                  NULL);

  darken    = gegl_node_new_child (gegl,
                                  "operation", "gegl:darken",
                                  NULL);

  lighten    = gegl_node_new_child (gegl,
                                  "operation", "gegl:lighten",
                                  NULL);

  plus    = gegl_node_new_child (gegl,
                                  "operation", "gegl:plus",
                                  NULL);
 /*Plus is an example of a blend mode unique to GEGL. It is like Gimp's addition but not the same*/


  multiply3    = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);

  multiply4    = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);
 /*This blend mode is used to restore the original color from the input image. Without it the bevel will not be able to be the original color.*/

 /*This is the replace blend mode for GEGL that shows the emboss alone. Emboss alone behaves like an alt multiply blend mode suited for image file overlays*/
  embossblend   = gegl_node_new_child (gegl,
                                  "operation", "gegl:src", 
                                  NULL);



 /*These are Gimp only blend modes and they can only be called through numbers*/

grainmerge = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 47, "composite-mode", 1, NULL);

 /*As of now (july 2023) Gimp's Overlay blend mode is 23, if Gimp ever gets new blend modes this will break and it will either be 22-24 or some other number. Future GEGL maintainers
need to know this.*/
overlay = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 23, "composite-mode", 1, NULL);

 /*As of now (july 2023) Gimp's Softlight blend mode is 45, if Gimp ever gets new blend modes this will break and it will either be 44-46 or some other number. Future GEGL maintainers
need to know this.*/
softlight = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 45, "composite-mode", 1, NULL);


 /*As of now (july 2023) Gimp's addition blend mode is 33, if Gimp ever gets new blend modes this will break and it will either be 32-34 or some other number. Future GEGL maintainers
need to know this.*/
addition = gegl_node_new_child (gegl,
                                  "operation", "gimp:layer-mode", "layer-mode", 33, "composite-mode", 1, NULL); 

 /*THIS (multiply2) USES GIMP'S LEGACY MULTIPLY BLEND MODE. In early development it called gegl:multiply but it was switched later*/


 /*As of now (july 2023) Gimp's legacy multiply blend mode is 3, if Gimp ever gets new blend modes this will break and it will either be 2-4 or some other number. Future GEGL maintainers
need to know this.*/
multiply2 = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 3, "blend-space", 1, "composite-mode", 0, NULL);



  gegl_operation_meta_redirect (operation, "size", median, "radius");
  gegl_operation_meta_redirect (operation, "size", fix3, "radius");
  gegl_operation_meta_redirect (operation, "shape", median, "neighborhood");
  gegl_operation_meta_redirect (operation, "shape", fix3, "neighborhood");
/* This is a special instruction two median blur radius's and their neighborhood base shapes udpate together. They fix a bug relating to anti erasing and a unwanted outline. */
  gegl_operation_meta_redirect (operation, "smooth", smooth, "sigma");
  gegl_operation_meta_redirect (operation, "azimuth", emboss, "azimuth");
  gegl_operation_meta_redirect (operation, "elevation", emboss, "elevation");
  gegl_operation_meta_redirect (operation, "depth", emboss, "depth");
  gegl_operation_meta_redirect (operation, "src", imagefileoverlay, "src");
  gegl_operation_meta_redirect (operation, "color", col, "value");
  gegl_operation_meta_redirect (operation, "metric", dt, "metric");
  gegl_operation_meta_redirect (operation, "ollight", levels, "out-low");
  gegl_operation_meta_redirect (operation, "transvalue", c2a, "transparency-threshold");
  gegl_operation_meta_redirect (operation, "sharpen", sharpen, "std-dev");
  gegl_operation_meta_redirect (operation, "bevelcontrol", multiply5, "value");

  /* now save references to the gegl nodes so we can use them
   * later, when update_graph() is called
   */
  State *state = g_malloc0 (sizeof (State));
  state->hardlight = hardlight;
  state->multiply = multiply;
  state->colordodge = colordodge;
  state->embossblend = embossblend;
  state->addition = addition;
  state->plus = plus;
  state->darken = darken;
  state->lighten = lighten;
  state->grainmerge = grainmerge;
  state->overlay = overlay;
  state->softlight = softlight;
  state->input = input;
  state->median = median;
  state->dt = dt;
  state->smooth = smooth;
  state->fix = fix;
  state->fix3 = fix3;
  state->sharpen = sharpen;
  state->c2a = c2a;
  state->col = col;
  state->nop = nop;
  state->opacity = opacity;
  state->fix2 = fix2;
  state->graph = graph;
  state->ta2 = ta2;
  state->ta2again = ta2again;
  state->emboss = emboss;
  state->edgesmooth = edgesmooth;
  state->nop = nop;
  state->levels = levels;
  state->idref = idref;
  state->allowblack = allowblack;
  state->multiply3 = multiply3;
  state->multiply4 = multiply4;
  state->imagefileoverlay = imagefileoverlay;
  state->multiply2 = multiply2;
  state->multiply5 = multiply5;
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
    "name",        "lb:sharpbevel",
    "title",       _("Sharp Bevel"),
    "categories",  "Artistic",
    "reference-hash", "v76ao6gk4321vyeef625362f2ag",
    "description", _("A heavy modification of my existing custom bevel plugin. GEGL makes a bevel allowing choice of bevel size, and internal blend modes.  Different blend modes do different things regarding detail, depth or presence of a shine effect."
                     ""),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Sharp Bevel..."),
    NULL);
}

#endif
