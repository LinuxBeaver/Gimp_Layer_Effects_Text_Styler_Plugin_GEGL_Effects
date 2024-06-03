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
 * 2023 Beaver (GEGL Gold/Metallic Text)
 */

/*
Run this to test filter in GEGL Graph without installing. Requires custom bevel, and bevel.

median-blur radius=10 alpha-percentile=20 neighborhood=diamond

custom-bevel size=4 gaus=4 box=5  opacity=4.9 coloroverlay=#ffcc99 blendmode=Plus azimuth=54
alien-map cpn-1-frequency=5 cpn-2-frequency=3 cpn-2-frequency=13
gimp:desaturate mode=average
hue-chroma lightness=-17


opacity value=6.5


id=color
gimp:layer-mode layer-mode=hsl-color opacity=0.15  composite-mode=auto  aux=[ ref=color color-overlay value=#fff17f box-blur radius=30 ]
noise-reduction iterations=3
bloom strength=22

saturation scale=1.17

id=bll
gimp:layer-mode layer-mode=linear-light composite-mode=auto blend-space=rgb-perceptual opacity=0.05  aux=[ ref=bll lb:bevel ]

id=edge gimp:layer-mode layer-mode=normal opacity=0.06 aux=[ ref=edge edge algorithm=sobel gimp:desaturate  ]

median-blur radius=0



 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES



#define TUTORIAL1 \
" gimp:desaturate mode=average hue-chroma lightness=-17 opacity value=6.5 "\



#define TUTORIAL2 \
" id=1 gimp:layer-mode layer-mode=grain-merge opacity=0.05 aux=[ ref=1 ] "\

#define TUTORIAL3 \
" id=1 src-in aux=[ ref=1 gaussian-blur  abyss-policy=none  clip-extent=false std-dev-x=0.4 std-dev-y=0.4  gaussian-blur  abyss-policy=none  clip-extent=false std-dev-x=0.4 std-dev-y=0.4  ] "\



property_enum(guichange, _("Part of filter to be displayed"),
    guiendgmmlist, guichangeenumgmmlist,
    GMM_SHOW_DEFAULT)
  description(_("Change the GUI option"))


enum_start (guichangeenumgmmlist)
enum_value   (GMM_SHOW_DEFAULT, "defaultgmm", N_("Basic Sliders"))
enum_value   (GMM_SHOW_ADVANCE, "advancegmm", N_("All Sliders for technical users"))
  enum_end (guiendgmmlist)



property_enum (blendmode, _("Blend Mode of Internal Emboss"),
    GeglBlendModeTypecbevelgoldmeme, gegl_blend_mode_typecbevelgoldmeme,
    GEGL_BLEND_MODE_TYPE_GRAINMERGE3)


property_int  (smooth, _("Smooth Metal"), 12)
  description (_("Smooth the bevel using noise reduction"))
  value_range (0, 14)
  ui_range    (0, 14)
ui_meta ("visible", "guichange {advancegmm}")

property_double (solar1, _("Solarization of Red Channel"), 1.5)
  description (_("Alien Map on Red Channel to create a metallic effect"))
  value_range (0.5, 8)
ui_meta ("visible", "guichange {advancegmm}")


property_double  (solar2, _("Solarization of Green Channel"), 0.750)
  description (_("Alien Map on Green Channel to create a metallic effect"))
  value_range (0.5, 8)
ui_meta ("visible", "guichange {advancegmm}")

property_double  (solar3, _("Solarization of Blue Channel"), 3.8)
  description (_("Alien Map on Blue Channel to create a metallic effect"))
  value_range (1, 30)
ui_meta ("visible", "guichange {advancegmm}")



/*
property_double (bloom, _("Bloom Glow Strength"), 22)
    description(_("Runs GEGLs glow lighting effect/orton filter 'Bloom' on the bevel. There is no orton due to this being a bevel but it will create an intense lighting. "))
    value_range (0.0, 22.0)
    ui_range (0.0, 70.0) */



enum_start (gegl_blend_mode_typecbevelgoldmeme)
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHT3, "Hardlight",
              N_("HardLight"))
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLY3,      "Multiply",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_COLORDODGE3,      "ColorDodge",
              N_("ColorDodge"))
  enum_value (GEGL_BLEND_MODE_TYPE_PLUS3,      "Plus",
              N_("Plus"))
  enum_value (GEGL_BLEND_MODE_TYPE_DARKEN3,      "Darken",
              N_("Darken"))
  enum_value (GEGL_BLEND_MODE_TYPE_LIGHTEN3,      "Lighten",
              N_("Lighten"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAY3,      "Overlay",
              N_("Overlay"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGE3,      "GrainMerge",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOFTLIGHT3,      "Softlight",
              N_("Soft Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_ADDITION3,      "Addition",
              N_("Addition"))
  enum_value (GEGL_BLEND_MODE_TYPE_EMBOSSBLEND3,      "EmbossBlend",
              N_("ImageandColorOverlayMode"))
enum_end (GeglBlendModeTypecbevelgoldmeme)



property_int  (radius, _("Median's Radius (wideness control)"), 15)
  value_range (0, 30)
  ui_range    (0, 30)
  ui_meta     ("unit", "pixel-distance")
  description (_("Neighborhood radius, a negative value will calculate with inverted percentiles"))


property_double  (alphapercentile, _("Median's Alpha Percentile (wideness control)"), 30)
  value_range (0, 30)
  description (_("Neighborhood alpha percentile"))

property_double (azimuth, _("Azimuth"), 48.0)
    description (_("Light angle (degrees)"))
    value_range (30, 90)
    ui_meta ("unit", "degree")
ui_meta ("visible", "guichange {advancegmm}")

property_double (elevation, _("Elevation"), 29.0)
    description (_("Rotate the brightest pixels of the bevel"))
    value_range (20, 80)
    ui_meta ("unit", "degree")

property_double (depth, _("Depth and or detail of bevel"), 24.0)
    description (_("Emboss's depth setting to increase depth and detail to the bevel"))
    value_range (1, 99)
    ui_meta ("unit", "degree")


property_double (gaus, _("Internal Gaussian Blur (fatness of bevel)"), 3.0)
   description (_("Gaussian Blur makes a normal fat bevel"))
   value_range (0.0, 9.0)



property_int (box, _("Internal Box Blur (makes sharper bevel)"), 6)
   description(_("Box Blur makes a sharper bevel"))
   value_range (0, 6)
   ui_range    (0, 6)
   ui_gamma   (1.5)


property_color (color, _("Color Overlay "), "#ffb957")
    description (_("The color of the gold/metallic bevel"))

property_double (saturation, _("Saturation"), 0.70)
    description(_("Saturation to control the intensity of the color"))
    value_range (0.0, 2.0)
    ui_range (0.0, 2.0)
ui_meta ("visible", "guichange {advancegmm}")

property_double (amopacity, _("Opacity of Solarization Effect"), 1.0)
   description (_("Alien Map (three solarizations) are being blended above the original layer. This controls their opacity. "))
   value_range (0.7, 1.0)
   ui_steps (0.1, 0.1)
ui_meta ("visible", "guichange {advancegmm}")

property_double (llopacity, _("Opacity of linear light blended bevel"), 0.30)
   description (_("A bevel bumpmap fused with linear light at very low opacity is above the main bevel. This controls the opacity of said linear light blended bevel. "))
   value_range (0.00, 0.35)
ui_meta ("visible", "guichange {advancegmm}")

property_double (sobelopacity, _("Opacity of edge sobel"), 0.15)
   description (_("Edge sobel is running at a very low value. This increases or decreases its opacity presence."))
   value_range (0.00, 0.20)
ui_meta ("visible", "guichange {advancegmm}")

property_double (topbevelexpand, _("Top blended bevel expansion"), 7.0)
  value_range (0.5, 40.0)
  ui_range (1.0, 9.0)
  ui_gamma (1.5)
    description (_("Make a normal bevel bump map using Gaussian Blur"))
ui_meta ("visible", "guichange {advancegmm}")

property_double (topbevelelevation, _("Top blended bevel elevation"), 45.0)
    description (_("Elevation angle (degrees)"))
    value_range (0, 180)
    ui_meta ("unit", "degree")
ui_meta ("visible", "guichange {advancegmm}")


property_int (topbeveldepth, _("Top blended bevel depth"), 40)
    description (_("Emboss depth -Brings out depth and detail of the bevel"))
    value_range (1, 100)
ui_meta ("visible", "guichange {advancegmm}")

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     goldmetallicmeme
#define GEGL_OP_C_SOURCE goldmetallicmeme.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *alienmap, *white, *bloom, *boxblur, *bevel, *cb, *color, *graph1, *graph2, *graph3,  *desat, *replace2, *edge, *hslcolor, *idref, *idref2, *idref3, *idref4, *linearlight, *median, *nr, *replace, *fixgraph,  *saturation, *ta2, *output;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");
  GeglColor *embedded_color = gegl_color_new ("#ffcc99");
  GeglColor *embedded_color2 = gegl_color_new ("#ffb957");
  GeglColor *embedded_color3 = gegl_color_new ("#ffffff");



  cb    = gegl_node_new_child (gegl,
                                  "operation", "lb:custom-bevel", "blendmode", 3, "opacity", 4.9, "coloroverlay", embedded_color,
                                  NULL);

  bevel    = gegl_node_new_child (gegl,
                                  "operation", "lb:bevel",
                                  NULL);


  color    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay", "value", embedded_color2,
                                  NULL);

  boxblur    = gegl_node_new_child (gegl,
                                  "operation", "gegl:box-blur", "radius", 30,
                                  NULL);

  alienmap    = gegl_node_new_child (gegl,
                                  "operation", "gegl:alien-map",
                                  NULL);

  median    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "neighborhood", 2,  "abyss-policy",     GEGL_ABYSS_NONE,
                                  NULL);

replace = gegl_node_new_child (gegl,
                                    "operation", "gimp:layer-mode", "layer-mode", 62, "composite-mode", 0,  "blend-space", 0, NULL);

replace2 = gegl_node_new_child (gegl,
                                    "operation", "gimp:layer-mode", "layer-mode", 62, "composite-mode", 0,  "blend-space", 0, NULL);


  desat    = gegl_node_new_child (gegl,
                                  "operation", "gimp:desaturate",
                                  NULL);



  graph1    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", TUTORIAL1,
                                  NULL);


  graph2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", TUTORIAL2,
                                  NULL);

  graph3    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", TUTORIAL3,
                                  NULL);


white    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay", "value", embedded_color3,
                                  NULL);


  ta2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  idref    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  idref2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  idref3    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  idref4    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  edge    = gegl_node_new_child (gegl,
                                  "operation", "gegl:edge",
                                  NULL);


  saturation    = gegl_node_new_child (gegl,
                                  "operation", "gegl:saturation",
                                  NULL);

  nr    = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-reduction",
                                  NULL);

  bloom    = gegl_node_new_child (gegl,
                                  "operation", "gegl:bloom", "strength", 22.0,
                                  NULL);

linearlight = gegl_node_new_child (gegl,
                                    "operation", "gimp:layer-mode", "layer-mode", 50, "composite-mode", 0,  "blend-space", 2, "opacity", 0.05, NULL);

hslcolor = gegl_node_new_child (gegl,
                                    "operation", "gimp:layer-mode", "layer-mode", 39, "composite-mode", 0,  "blend-space", 2, "opacity", 0.15, NULL);

 /*Repair GEGL Graph is a critical operation for Gimp's non-destructive future.
A median blur at zero radius is confirmed to make no changes to an image.
This option resets gegl:opacity's value to prevent a known bug where
plugins like clay, glossy balloon and custom bevel glitch out when
drop shadow is applied in a gegl graph below them.*/
   fixgraph      = gegl_node_new_child (gegl, "operation", "gegl:median-blur",     "abyss-policy",     GEGL_ABYSS_NONE,
                                         "radius",       0,
                                         NULL);

  gegl_operation_meta_redirect (operation, "gaus", cb, "gaus");
  gegl_operation_meta_redirect (operation, "box", cb, "box");
  gegl_operation_meta_redirect (operation, "azimuth", cb, "azimuth");
  gegl_operation_meta_redirect (operation, "elevation", cb, "elevation");
  gegl_operation_meta_redirect (operation, "depth", cb, "depth");
  gegl_operation_meta_redirect (operation, "color", color, "value");
  gegl_operation_meta_redirect (operation, "blendmode", cb, "blendmode");
  gegl_operation_meta_redirect (operation, "radius", median, "radius");
  gegl_operation_meta_redirect (operation, "alphapercentile", median, "alpha-percentile");
  gegl_operation_meta_redirect (operation, "saturation", saturation, "scale");
  gegl_operation_meta_redirect (operation, "smooth", nr, "iterations");
  gegl_operation_meta_redirect (operation, "llopacity", linearlight, "opacity");
  gegl_operation_meta_redirect (operation, "amopacity", replace, "opacity");
  gegl_operation_meta_redirect (operation, "sobelopacity", replace2, "opacity");
  gegl_operation_meta_redirect (operation, "solar1", alienmap, "cpn-1-frequency");
  gegl_operation_meta_redirect (operation, "solar2", alienmap, "cpn-2-frequency");
  gegl_operation_meta_redirect (operation, "solar3", alienmap, "cpn-3-frequency");
  gegl_operation_meta_redirect (operation, "topbevelexpand", bevel, "radius1");
  gegl_operation_meta_redirect (operation, "topbevelelevation", bevel, "bevel1");
  gegl_operation_meta_redirect (operation, "topbeveldepth", bevel, "bevel2");



  gegl_node_link_many (input, white, median, cb, idref, replace, graph1, idref2, hslcolor, nr, bloom, saturation, idref3, linearlight, idref4, replace2, graph2, graph3, fixgraph, ta2, output, NULL);
  gegl_node_connect (replace, "aux", alienmap, "output");
  gegl_node_link_many (idref, alienmap, NULL);
  gegl_node_connect (hslcolor, "aux", boxblur, "output");
  gegl_node_link_many (idref2, color, boxblur, NULL);
  gegl_node_connect (linearlight, "aux", bevel, "output");
  gegl_node_link_many (idref3, bevel, NULL);
  gegl_node_connect (replace2, "aux", desat, "output");
  gegl_node_link_many (idref4, edge, desat,  NULL);


}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:gmm",
    "title",       _("Glass, Metallic, Marble and more Text Styler"),
    "reference-hash", "golod6r00350fmejftall124c",
    "description", _("Glass Metallic and Marble text generation - To use this filter optimally please import and save the provided presets and perhaps adjust them with internal gaussian/box, and wideness control to match your text.)"
                     ""),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Glass Metallic and Marble mega text styler..."),
    NULL);
}

#endif
