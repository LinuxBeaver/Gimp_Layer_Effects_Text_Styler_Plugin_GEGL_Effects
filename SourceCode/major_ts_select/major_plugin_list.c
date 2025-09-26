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
 * 2023 Beaver (GEGL Major Plugin list) -- a hidden operation for GEGL Effects to switch between all major text styling plugins. It also helps Gimp 2.10 users find my plugins instead
of having to search through the GEGL operations menu. I used the name "Amazing" so it would appear on the top of the GEGL Operations drop down list, as it is listed alphabetically.   
 */

/* August 5th 2023, This operation is instructing most major GEGL plugins of mine to be put in the same drop down list. They are all controlled by universal sliders they share.  */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

property_enum (list, _("Select a Major Text Plugin"),
    mymajorplugins, my_major_plugins,
    GEGL_GLOSSY_BALLOON_TIME)


enum_start (my_major_plugins)
  enum_value (GEGL_GLOSSY_BALLOON_TIME,      "glossyballoon",
              N_("Glossy Balloon"))
  enum_value (GEGL_CUSTOM_BEVEL_TIME,      "custombevel",
              N_("Custom Bevel"))
  enum_value (GEGL_CLAY_TIME,      "clay",
              N_("Clay"))
  enum_value (GEGL_SHARP_BEVEL_TIME,      "sharpbevel",
              N_("Sharp Bevel"))
  enum_value (GEGL_GLASS_TIME,      "glass",
              N_("GMM Glass"))
  enum_value (GEGL_METAL_TIME, "metal",
              N_("GMM Metal"))
  enum_value (GEGL_MARBLE_TIME, "marble",
              N_("GMM Marble"))
  enum_value (GEGL_ROCK_TEXT_TIME, "rocktext",
              N_("Rock Text"))
  enum_value (GEGL_CHROME_TIME, "chrome",
              N_("Chrome"))
enum_end (mymajorplugins)

property_double (universalgaussian, _("Internal Gaussian Size (NA sharpbevel, rock text)"), 6.0)
   description  (_("Universal internal Gaussian blur for all major text styling plugins that use it. Text styling plugins that do not use this filter will have no effect when this slider is used. They are listed under NA."))
  value_range (0.5, 20.0)
  ui_range (0.5, 12)
  ui_gamma (1.5)


property_double (universalazimuth, _("Azimuth - (NA glossyballoon)"), 50.0)
    description (_("Universal azimuth for all major text styling plugins that use it. Text styling plugins that do not use this filter will have no effect when this slider is used. They are listed under NA."))
    value_range (30, 90)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")

property_double (universalelevation, _("Elevation (NA glossyballoon)"), 56.0)
    description (_("Universal elevation for all major text styling plugins that use it. Text styling plugins that do not use this filter will have no effect when this slider is used. They are listed under NA."))
    value_range (25, 90)
    ui_meta ("unit", "degree")

property_int (universaldepth, _("Depth (NA glossyballoon, rocktext) "), 36)
    description (_("Universal depth for all major text styling plugins that use it. Text styling plugins that do not use this filter will have no effect when this slider is used. They are listed under NA."))
    value_range (1, 100)

property_int  (universalmedianradius, _("Median Radius (NA glossyballoon)"), 3)
  value_range (-10, 20)
  ui_range    (-10, 20)
  ui_meta     ("unit", "pixel-distance")
  description (_("Universal Median Radius for all major text styling plugins that use it. Text styling plugins that do not use this filter will have no effect when this slider is used. They are listed under NA."))

/*
property_double  (universalmedianpercentile, _("Median Percentile (NA glossyballoon)"), 65)
  value_range (20, 80)
  description (_("Universal Median Percentile for all major text styling plugins that use it. Text styling plugins that do not use this filter will have no effect when this slider is used. They are listed under NA."))
*/

property_double  (universalmedianalphapercentile, _("Median Alpha percentile (NA glossyballoon)"), 75)
  value_range (21, 100)
  description (_("Universal Median Alpha Percentile for all major text styling plugins that use it. Text styling plugins that do not use this filter will have no effect when this slider is used. They are listed under NA."))


property_color  (universalcolor, _("Color Selection (white is usually transparency)"), "#00c8ff")
    description (_("Universal Color Selection for all major text styling plugins that use it. Text styling plugins that do not use this filter will have no effect when this slider is used. They are listed under NA. On many plugins white equals transparency. If white doesn't give transparency left click the color changer and move its A slider to 0."))

property_file_path(universalimage, _("Image Upload (NA gmm, chrometext)"), "")
    description (_("Source image file path (png, jpg, raw, svg, bmp, tif, ...) Universal Image file Upload for all major text styling plugins that use it. Text styling plugins that do not use this filter will have no effect when this is used. They are listed under NA."))


enum_start (gegl_blend_mode_typecbsharp)
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHT, "Hardlight",
              N_("HardLight"))
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLY,      "Multiply",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_COLORDODGE,      "ColorDodge",
              N_("ColorDodge"))
  enum_value (GEGL_BLEND_MODE_TYPE_PLUS,      "Plus",
              N_("Plus"))
  enum_value (GEGL_BLEND_MODE_TYPE_DARKEN,      "Darken",
              N_("Darken"))
  enum_value (GEGL_BLEND_MODE_TYPE_LIGHTEN,      "Lighten",
              N_("Lighten"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAY,      "Overlay",
              N_("Overlay"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGE,      "GrainMerge",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOFTLIGHT,      "Softlight",
              N_("Soft Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_ADDITION,      "Addition",
              N_("Addition"))
  enum_value (GEGL_BLEND_MODE_TYPE_EMBOSSBLEND,      "EmbossBlend",
              N_("Grayscale Multiply for Image uploads"))
enum_end (GeglBlendModeTypecbsharp)

property_enum (universalblendmode, _("Blend of Custom Bevel, GMM, Sharp Bevel (NA everything else)"),
    GeglBlendModeTypecbsharp, gegl_blend_mode_typecbsharp,
    GEGL_BLEND_MODE_TYPE_GRAINMERGE)
    description (_("Universal Blend Mode switcher for all major text styling plugins that use it. Text styling plugins that do not use this filter will have no effect when this slider is used. They are listed under NA. In This case Custom Bevel, GMM, and Sharp Bevel are the only plugins this works with and everything else does nothing."))


property_boolean (policy, _("Enable/Disable major text styling plugins"), TRUE)
  description    (_("Most major GEGL text styling plugins are here. Enabling major text styling plugins in GEGL Effects will disable many other options. So if you are not using the major text styling plugins keep this checkbox disabled, or once again it will disable a ton of other options."))


/*On August 6th 2023 Beaver found out these hidden GUI properties below couldn't be embedded into a GEGL Operation's property listing. If you look at lb:gmm's GEGL node listing you will notice that many properties are already embedded. (12 exactly) It doesn't allow anymore, and if I did add a 13th one Gimp 2.10 would crash. We believe a GEGL operations property list caps out at 12 settings or when it reaches a certain point in memory. The problem may be caused by either or hypothesis, or something else. Due to this, Beaver is forced to use "output-extent" to hide additional baked in settings*/

property_double (hidden1, _("Opacity of linear light blended bevel"), 0.23)
   description (_("A bevel bumpmap fused with linear light at very low opacity is above the main bevel. This controls the opacity of said linear light blended bevel."))
   value_range (0.00, 0.35)
    ui_meta    ("role", "output-extent")


property_double (hidden2, _("Opacity of edge sobel"), 0.11)
   description (_("Edge sobel is running at a very low value. This increases or decreases its opacity presence."))
   value_range (0.00, 0.20)
    ui_meta    ("role", "output-extent")


property_double (hidden3, _("Opacity of Solarization Effect"), 0.77)
   description (_("Alien Map (three solarizations) are being blended above the original layer. This controls their opacity."))
   value_range (0.7, 1.0)
   ui_steps (0.1, 0.1)
    ui_meta    ("role", "output-extent")

property_double (hidden4, _("Saturation"), 1.22)
    description(_("Saturation to control the intensity of the color"))
    value_range (0.0, 2.0)
    ui_range (0.0, 2.0)
    ui_meta    ("role", "output-extent")

property_int (hidden5, _("Depth"), 28)
    description (_("Emboss depth -Brings out depth and detail of the bevel"))
    value_range (1, 100)
    ui_meta    ("role", "output-extent")

property_double (hidden6, _("Opacity of linear light blended bevel"), 0.29)
   description (_("A bevel bumpmap fused with linear light at very low opacity is above the main bevel. This controls the opacity of said linear light blended bevel."))
   value_range (0.00, 0.35)
    ui_meta    ("role", "output-extent")


property_double (hidden7, _("Opacity of edge sobel"), 0.14)
   description (_("Edge sobel is running at a very low value. This increases or decreases its opacity presence."))
   value_range (0.00, 0.20)
    ui_meta    ("role", "output-extent")


property_double (hidden8, _("Opacity of Solarization Effect"), 1.0)
   description (_("Alien Map (three solarizations) are being blended above the original layer. This controls their opacity."))
   value_range (0.7, 1.0)
   ui_steps (0.1, 0.1)
    ui_meta    ("role", "output-extent")

property_double (hidden9, _("Opacity of linear light blended bevel"), 0.19)
   description (_("A bevel bumpmap fused with linear light at very low opacity is above the main bevel. This controls the opacity of said linear light blended bevel."))
   value_range (0.00, 0.35)
    ui_meta    ("role", "output-extent")


property_double (hidden10, _("Opacity of edge sobel"), 0.13)
   description (_("Edge sobel is running at a very low value. This increases or decreases its opacity presence."))
   value_range (0.00, 0.20)
    ui_meta    ("role", "output-extent")


property_double (hidden11, _("Opacity of Solarization Effect"), 0.7)
   description (_("Alien Map (three solarizations) are being blended above the original layer. This controls their opacity."))
   value_range (0.7, 1.0)
   ui_steps (0.1, 0.1)
    ui_meta    ("role", "output-extent")



#else

#define GEGL_OP_META
#define GEGL_OP_NAME     major_plugin_list
#define GEGL_OP_C_SOURCE major_plugin_list.c

#include "gegl-op.h"


typedef struct
{
  GeglNode *input;
  GeglNode *glossyballoon;
  GeglNode *clay;
  GeglNode *custombevel;
  GeglNode *sharpbevel;
  GeglNode *gmm;
  GeglNode *gmm2;
  GeglNode *gmm3;
  GeglNode *nr;
  GeglNode *chrome;
  GeglNode *rocktext;
  GeglNode *edgesmooth;
  GeglNode *color;
  GeglNode *hslcolor;
  GeglNode *forcewhite;
  GeglNode *nop;
  GeglNode *adjustchrome;
  GeglNode *output;
} State; 


static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglNode *input, *output,  *gmm, *gmm2, *gmm3, *clay, *chrome, *hslcolor, *adjustchrome, *nr, *color, *nop, *forcewhite, *edgesmooth, *rocktext, *custombevel, *sharpbevel, *glossyballoon;
  GeglColor *embedded_color = gegl_color_new ("#ef43f6");
  GeglColor *embedded_color_glass = gegl_color_new ("#00a3ff");
  GeglColor *white_color_embedded = gegl_color_new ("#ffffff");

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");

  glossyballoon = gegl_node_new_child (gegl,
                                  "operation", "lb:glossy-balloon", "wildcolor", TRUE, "hslcolor", embedded_color, "lightness", 3.0,
                                  NULL);

/*On August 6th 2023 Beaver learned that the operations property list as a memory or number cap. It doesn't allow more then 12-13 numbers in a list.

The goal is to make all three GMM's true to the style of Glass Metal and Marble. The first is Glass, The second is Metal, the third is Marble.*/
  gmm = gegl_node_new_child (gegl,
                                  "operation", "lb:gmm", "blendmode", 6,  "solar1", 4.5, "solar2", 2.9, "solar3", 4.5, "radius", 2, "azimuth", 40.5, "elevation", 52.1,  "gaus", 4.0, "box", 5, "color", embedded_color_glass,  "alphapercentile", 6.1, 
NULL);
                                 

/*metal GMM */ 
  gmm2 = gegl_node_new_child (gegl,
                                  "operation", "lb:gmm", "blendmode", 6,  "solar1", 1.5, "solar2", 0.75, "solar3", 3.8, "radius", 15, "azimuth", 48.0, "elevation", 56.0,  "gaus", 3.0, "box", 6,  
NULL);
          
/*marble GMM */
  gmm3 = gegl_node_new_child (gegl,
                                  "operation", "lb:gmm", "blendmode", 6,  "solar1", 2.3, "solar2", 2.5, "solar3", 24.1, "radius", 10, "azimuth", 59.0, "elevation", 20.0,  "gaus", 4.0, "box", 5,  "alphapercentile", 23.1, 
NULL);
         
  clay = gegl_node_new_child (gegl,
                                  "operation", "lb:clay",
                                  NULL);

  sharpbevel = gegl_node_new_child (gegl,
                                  "operation", "lb:sharpbevel", "bevelcolorpolicy", TRUE,
                                  NULL);

  custombevel = gegl_node_new_child (gegl,
                                  "operation", "lb:custom-bevel",  
                                  NULL);

  chrome = gegl_node_new_child (gegl,
                                  "operation", "lb:chrome",   NULL);


  rocktext = gegl_node_new_child (gegl,
                                  "operation", "lb:rock-text", "opacity", 0.0,   NULL);
/*Without this opacity 0.0, rock text would have a faint outline around it.*/

  edgesmooth = gegl_node_new_child (gegl,
                                  "operation", "lb:edgesmooth",   NULL);
/*An exclusive edge smooth for rock text*/

  nop = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  forcewhite = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay", "value", white_color_embedded,
                                  NULL);


  adjustchrome = gegl_node_new_child (gegl,
                                  "operation", "gegl:levels", "in-high", 0.8,
                                  NULL);

  color = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);

  nr = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-reduction", "iterations", 9,
                                  NULL);


hslcolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 39, "composite-mode", 0, NULL);

/*An exclusive color overlay for chrome text

Below is a list of all shared GUI properties. They use the name universal because they are all sharing the same GUI options*/

  gegl_operation_meta_redirect (operation, "universalgaussian", glossyballoon, "gaus");
  gegl_operation_meta_redirect (operation, "universalgaussian", clay, "gaus");
  gegl_operation_meta_redirect (operation, "universalgaussian", custombevel, "gaus");
  gegl_operation_meta_redirect (operation, "universalgaussian", chrome, "bvradius");
  gegl_operation_meta_redirect (operation, "universalgaussian", gmm, "gaus");
  gegl_operation_meta_redirect (operation, "universalgaussian", gmm2, "gaus");
  gegl_operation_meta_redirect (operation, "universalgaussian", gmm3, "gaus");

  gegl_operation_meta_redirect (operation, "universalmedianradius", clay, "size");
  gegl_operation_meta_redirect (operation, "universalmedianradius", custombevel, "size");
  gegl_operation_meta_redirect (operation, "universalmedianradius", rocktext, "size");
  gegl_operation_meta_redirect (operation, "universalmedianradius", gmm, "radius");
  gegl_operation_meta_redirect (operation, "universalmedianradius", gmm2, "radius");
  gegl_operation_meta_redirect (operation, "universalmedianradius", gmm3, "radius");
  gegl_operation_meta_redirect (operation, "universalmedianradius", sharpbevel, "size");

  gegl_operation_meta_redirect (operation, "universalmedianalphapercentile", clay, "alpha-percentile");
  gegl_operation_meta_redirect (operation, "universalmedianalphapercentile", custombevel, "alphapercentile");
  gegl_operation_meta_redirect (operation, "universalmedianalphapercentile", gmm, "alphapercentile");
  gegl_operation_meta_redirect (operation, "universalmedianalphapercentile", gmm2, "alphapercentile");
  gegl_operation_meta_redirect (operation, "universalmedianalphapercentile", gmm3, "alphapercentile");
  gegl_operation_meta_redirect (operation, "universalmedianalphapercentile", rocktext, "alpha-percentile");

  gegl_operation_meta_redirect (operation, "universalcolor", clay, "mcol");
  gegl_operation_meta_redirect (operation, "universalcolor", sharpbevel, "color");
  gegl_operation_meta_redirect (operation, "universalcolor", glossyballoon, "hslcolor");
  gegl_operation_meta_redirect (operation, "universalcolor", gmm, "color");
  gegl_operation_meta_redirect (operation, "universalcolor", gmm2, "color");
  gegl_operation_meta_redirect (operation, "universalcolor", gmm3, "color");
  gegl_operation_meta_redirect (operation, "universalcolor", custombevel, "coloroverlay");
  gegl_operation_meta_redirect (operation, "universalcolor", rocktext, "mvalue");
  gegl_operation_meta_redirect (operation, "universalcolor", color, "value");

  gegl_operation_meta_redirect (operation, "universalazimuth", clay, "azimuth");
  gegl_operation_meta_redirect (operation, "universalazimuth", custombevel, "azimuth");
  gegl_operation_meta_redirect (operation, "universalazimuth", rocktext, "azimuth");
  gegl_operation_meta_redirect (operation, "universalazimuth", gmm, "azimuth");
  gegl_operation_meta_redirect (operation, "universalazimuth", gmm2, "azimuth");
  gegl_operation_meta_redirect (operation, "universalazimuth", gmm3, "azimuth");
  gegl_operation_meta_redirect (operation, "universalazimuth", chrome, "azimuth");
  gegl_operation_meta_redirect (operation, "universalazimuth", sharpbevel, "azimuth");

  gegl_operation_meta_redirect (operation, "universalelevation", clay, "elevation");
  gegl_operation_meta_redirect (operation, "universalelevation", custombevel, "elevation");
  gegl_operation_meta_redirect (operation, "universalelevation", gmm,  "elevation");
  gegl_operation_meta_redirect (operation, "universalelevation", gmm2, "elevation");
  gegl_operation_meta_redirect (operation, "universalelevation", gmm3, "elevation");
  gegl_operation_meta_redirect (operation, "universalelevation", sharpbevel, "elevation");
  gegl_operation_meta_redirect (operation, "universalelevation", rocktext, "elevation");
  gegl_operation_meta_redirect (operation, "universalelevation", chrome, "bevelconfig");


  gegl_operation_meta_redirect (operation, "universaldepth", clay, "depth");
  gegl_operation_meta_redirect (operation, "universaldepth", custombevel, "depth");
  gegl_operation_meta_redirect (operation, "universaldepth", gmm, "depth");
  gegl_operation_meta_redirect (operation, "universaldepth", gmm2, "depth");
  gegl_operation_meta_redirect (operation, "universaldepth", gmm3, "depth");
  gegl_operation_meta_redirect (operation, "universaldepth", sharpbevel, "depth");

  gegl_operation_meta_redirect (operation, "universalimage", glossyballoon, "src");
  gegl_operation_meta_redirect (operation, "universalimage", clay, "src");
  gegl_operation_meta_redirect (operation, "universalimage", custombevel, "src");
  gegl_operation_meta_redirect (operation, "universalimage", rocktext, "src");
  gegl_operation_meta_redirect (operation, "universalimage", sharpbevel, "src");

  gegl_operation_meta_redirect (operation, "universalblendmode", sharpbevel, "blendmode");
  gegl_operation_meta_redirect (operation, "universalblendmode", custombevel, "blendmode");
  gegl_operation_meta_redirect (operation, "universalblendmode", gmm, "blendmode");
  gegl_operation_meta_redirect (operation, "universalblendmode", gmm2, "blendmode");
  gegl_operation_meta_redirect (operation, "universalblendmode", gmm3, "blendmode");

/*There is no way to embed these operations to hide them. GMM's operation properties has reached its limit.*/
  gegl_operation_meta_redirect (operation, "hidden2", gmm, "sobelopacity");
  gegl_operation_meta_redirect (operation, "hidden3", gmm, "amopacity");
  gegl_operation_meta_redirect (operation, "hidden4", gmm, "saturation");
  gegl_operation_meta_redirect (operation, "hidden5", gmm, "depth");
  gegl_operation_meta_redirect (operation, "hidden6", gmm2, "llopacity");
  gegl_operation_meta_redirect (operation, "hidden7", gmm2, "sobelopacity");
  gegl_operation_meta_redirect (operation, "hidden8", gmm2, "amopacity");
  gegl_operation_meta_redirect (operation, "hidden9", gmm3, "llopacity");
  gegl_operation_meta_redirect (operation, "hidden10", gmm3, "sobelopacity");
  gegl_operation_meta_redirect (operation, "hidden11", gmm3, "amopacity");



 /* Now save points to the various gegl nodes so we can rewire them in
   * update_graph() later
   */
  State *state = g_malloc0 (sizeof (State));
  state->input = input;
  state->glossyballoon = glossyballoon;
  state->clay = clay;
  state->custombevel = custombevel;
  state->chrome = chrome;
  state->nr = nr;
  state->forcewhite = forcewhite;
  state->adjustchrome = adjustchrome;
  state->gmm = gmm;
  state->gmm2 = gmm2;
  state->gmm3 = gmm3;
  state->edgesmooth = edgesmooth;
  state->rocktext = rocktext;
  state->sharpbevel = sharpbevel;
  state->hslcolor = hslcolor;
  state->color = color;
  state->nop = nop;
  state->output = output;
  o->user_data = state;
}

static void

update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;

/*This is the switch between all plugins. This boots up each GEGL Graph for each respected plugin. Two of them (chrome text and rock text are given special additions. */
    if (o->policy)

switch (o->list) {
        break;
    case GEGL_GLOSSY_BALLOON_TIME:
    gegl_node_link_many (state->input, state->glossyballoon, state->output, NULL);
        break;
    case GEGL_CUSTOM_BEVEL_TIME:
    gegl_node_link_many (state->input, state->forcewhite, state->custombevel, state->output, NULL);
        break;
    case GEGL_CLAY_TIME:
    gegl_node_link_many (state->input, state->forcewhite, state->clay, state->output, NULL);
        break;
    case GEGL_SHARP_BEVEL_TIME:
    gegl_node_link_many (state->input, state->sharpbevel, state->output, NULL);
        break;
    case GEGL_GLASS_TIME:
    gegl_node_link_many (state->input, state->gmm, state->nr, state->output, NULL);
        break;
    case GEGL_METAL_TIME:
    gegl_node_link_many (state->input, state->gmm2,  state->output, NULL);
        break;
    case GEGL_MARBLE_TIME:
    gegl_node_link_many (state->input, state->gmm3, state->nr, state->output, NULL);
        break;
    case GEGL_ROCK_TEXT_TIME:
    gegl_node_link_many (state->input, state->forcewhite, state->rocktext, state->edgesmooth, state->output, NULL);
        break;
    case GEGL_CHROME_TIME:
    gegl_node_link_many (state->input, state->forcewhite, state->chrome, state->nop, state->hslcolor, state->adjustchrome, state->output, NULL);
  gegl_node_connect (state->hslcolor, "aux", state->color, "output");
  gegl_node_link_many (state->nop, state->color, NULL);
    }
 else
    {
    gegl_node_link_many (state->input, state->output, NULL);
    }
  }


/*fun fact, rock text in GEGL Effects has a edge smooth that isn't present in the default rock text filter

Chrome Text has a HSL Color overlay that is not found in its default plugin (though I am thinking about adding a color picker to it. August 7 2023.*/

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class = GEGL_OPERATION_CLASS (klass);
   GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);

  operation_class->attach = attach;
  operation_meta_class->update = update_graph;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:amazing",
    "title",       _("Amazing Text Styling Plugins"),
    "reference-hash", "53453maa2amacfxsxiwba30c",
    "categories",     "hidden",
    "description", _("Most major GEGL text styling plugins - all in once place - This is a operation that GEGL Effects Experimental depends on. It also helps Gimp 2.10 users easily access my plugins without having to search through the GEGL Operations list..."
                     ""),
    NULL);
}

#endif
