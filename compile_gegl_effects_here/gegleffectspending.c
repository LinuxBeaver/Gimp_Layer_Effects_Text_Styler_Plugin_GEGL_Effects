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
 *2022 Beaver GEGL Effects  *2023 Beaver GEGL Effects CE with new features like bevel outlines, thin/bold text mode and shines and so much more.
 * 2022 BarefootLiam (for helping give Inner Glow a disable checkbox) 
 */

/*

To help future devs I will list predicted scenarios that I believe will lead to GEGL effects or any of my plugins breaking compatibility with future versions of Gimp. 
--
First example of how GEGL effects can and eventually break is if Gimp team adds new blend modes. 

In Example, Gimp's Split blend mode is 60

split = gegl_node_new_child (gegl,
"operation", "gimp:layer-mode", "layer-mode", 60,
"composite-mode", 0, "blend-space", 0, "composite-space", 0, NULL);

If Gimp's team adds a new blend mode this will throw off all the gimp exclusive blend modes called by one number.

Resulting in a scenario where all my plugins will not work (by using the wrong blend mode) until the number 60 is changed to 61 or 59. (we'll have to figure it out when it happens).
All Gimp exclusive blend modes will have to be updated when this happens. That will break a ton of my plugins but the fix should be trivial.
--
The second most common reason GEGL Effects will break is if a GEGL operation changes its value. In example gaussian blur's values are

gaussian-blur std-dev-x=30  std-dev-y=30

but lets say a hypothetical update from the gimp team comes and changes it to

gaussian-blur x=30  y=30

Everything that uses gaussian blur will then break.

Until std-dev-x and std-dev-y are replaced with x and y. all plugins of mine that call gaussian blur will remain broken. This can happen to any operation not just gaussian blur

The third most common reason GEGL Effects will break is if a operation is removed or renamed. It will then have to be rebuilt and shipped with GEGL Effects
hypothetical scenario would be  if gegl:emboss got renamed to gegl:embosser; we'd have to udpate all my filters that call emboss.

The fourth most common scenario is if Gimp's team adds a new filter that has the exact same name as one of mine in both the title and operation name. 
then the title and or gegl operation of name of one mf my plugins; example "gegl:bevel" would have to change to "bevel2" everywhere in my codebase
where it is used. 


*/


#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

/*This is the enum list of blend modes for Outline Bevel.  Outline Bevel is part of zzstrokebevelimage hidden operation*/
enum_start (gegl_blend_mode_type_effectszzbevoutlinege)
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLYGE,      "MultiplyGE",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGEGE,      "GrainMergeGE",
              N_("GrainMerge"))
  enum_value (GEGL_BLEND_MODE_TYPE_SHINYGMGE,      "GrainMergealtGE",
              N_("GrainMergeAlt"))
  enum_value (GEGL_BLEND_MODE_TYPE_COLORDODGEGE,      "ColorDodgeGE",
              N_("ColorDodge"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHTGE,      "HardLightGE",
              N_("HardLight"))
  enum_value (GEGL_BLEND_MODE_TYPE_DISABLEBEVELGE,      "DisableBevelGE",
              N_("Outline Bevel Off"))
enum_end (GeglBlendModeTypezzbevoutlinege)

/*This is the enum list of blend modes for Extra Outline Bevel. Outline Bevel is part of zzstrokebevelimage hidden operation*/

enum_start (gegl_blend_mode_type_effectszzbevoutlinegeextra)
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLYGEextra,      "MultiplyGE",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGEGEextra,      "GrainMergeGE",
              N_("GrainMerge"))
  enum_value (GEGL_BLEND_MODE_TYPE_SHINYGMGEextra,      "GrainMergealtGE",
              N_("GrainMergeAlt"))
  enum_value (GEGL_BLEND_MODE_TYPE_COLORDODGEGEextra,      "ColorDodgeGE",
              N_("ColorDodge"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHTGEextra,      "HardLightGE",
              N_("HardLight"))
  enum_value (GEGL_BLEND_MODE_TYPE_DISABLEBEVELGEextra,      "DisableBevelGE",
              N_("Outline Bevel Off"))
enum_end (GeglBlendModeTypezzbevoutlinegeextra)


/*This is the enum list of blend modes for bevel.*/
enum_start (gegl_blend_mode_type_effectszz)
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLY,      "Multiply",
              N_("Multiply Bevel"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGE,      "GrainMerge",
              N_("GrainMerge Bevel"))
  enum_value (GEGL_BLEND_MODE_TYPE_SHINYGM,      "GrainMergealt",
              N_("GrainMergeAlt Bevel"))
  enum_value (GEGL_BLEND_MODE_TYPE_SUBTRACT,      "Subtract",
              N_("Subtract Bevel"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINEXTRACT,      "GrainExtract",
              N_("GrainExtract Bevel"))
  enum_value (GEGL_BLEND_MODE_TYPE_COLORDODGE,      "ColorDodge",
              N_("ColorDodge Bevel"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHT,      "HardLight",
              N_("HardLight Bevel"))
  enum_value (GEGL_BLEND_MODE_TYPE_SCREEN,      "Screen",
              N_("Screen Bevel"))
  enum_value (GEGL_BLEND_MODE_TYPE_BEVELOFF,      "BevelOff",
              N_("Bevel off "))
enum_end (GeglBlendModeTypezz)



/*This is the enum list of blend modes for Inner Glow.*/
enum_start (gegl_blend_mode_type_effectsigzz)
  enum_value (GEGL_BLEND_MODE_TYPE_OVER,      "Over",
              N_("Over"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGEIG,      "GrainMerge",
              N_("GrainMerge"))
  enum_value (GEGL_BLEND_MODE_TYPE_ADDITION,      "Addition",
              N_("Addition"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOFTLIGHTIG,      "Softlight",
              N_("Softlight"))
  enum_value (GEGL_BLEND_MODE_TYPE_SCREENIG,      "Screen",
              N_("Screen"))
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLYIG,      "Multiply",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_HSLCOLORIG,      "HSLColor",
              N_("HSLColor"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAYIG,      "Overlay",
              N_("Overlay"))
  enum_value (GEGL_BLEND_MODE_TYPE_LINEARLIGHTIG,      "LinearLight",
              N_("LinearLight"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHTIG,      "HardLight",
              N_("HardLight"))
  enum_value (GEGL_BLEND_MODE_TYPE_BURNIG,      "Burn",
              N_("Burn"))
  enum_value (GEGL_BLEND_MODE_TYPE_LCHCOLORIG,      "LCHColor",
              N_("LCHColor"))
  enum_value (GEGL_BLEND_MODE_TYPE_HUEIG,      "Hue",
              N_("Hue"))
enum_end (GeglBlendModeTypeigzz)

/*This is the enum list of blend modes for Gradient.*/
enum_start (gegl_blend_mode_type_effectsgzz)
  enum_value (GEGL_BLEND_MODE_TYPE_ATOPG,      "Over",
              N_("Over"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGEG,      "GrainMerge",
              N_("GrainMerge"))
  enum_value (GEGL_BLEND_MODE_TYPE_ADDITIONG,      "Addition",
              N_("Addition"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOFTLIGHTG,      "Softlight",
              N_("Softlight"))
  enum_value (GEGL_BLEND_MODE_TYPE_SCREENG,      "Screen",
              N_("Screen"))
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLYG,      "Multiply",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_HSLCOLORG,      "HSLColor",
              N_("HSLColor"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAYG,      "Overlay",
              N_("Overlay"))
  enum_value (GEGL_BLEND_MODE_TYPE_LINEARLIGHTG,      "LinearLight",
              N_("LinearLight"))
  enum_value (GEGL_BLEND_MODE_TYPE_HSVHUEG,      "HSVHue",
              N_("HSVHue"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHTG,      "HardLight",
              N_("HardLight"))
  enum_value (GEGL_BLEND_MODE_TYPE_BURNG,      "Burn",
              N_("Burn"))
  enum_value (GEGL_BLEND_MODE_TYPE_LCHCOLORG,      "LCHColor",
              N_("LCHColor"))
enum_end (GeglBlendModeTypegzz)

/*This is the enum list of blend modes for Image File Overlay.*/
enum_start (gegl_blend_mode_type_effectsgzzimage)
  enum_value (GEGL_BLEND_MODE_TYPE_NORMALIMAGE,      "NormalImage",
              N_("NormalImage"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGEIMAGE,      "GrainMerge",
              N_("GrainMerge"))
  enum_value (GEGL_BLEND_MODE_TYPE_ADDITIONIMAGE,      "Addition",
              N_("Addition"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOFTLIGHTIMAGE,      "Softlight",
              N_("Softlight"))
  enum_value (GEGL_BLEND_MODE_TYPE_SCREENIMAGE,      "Screen",
              N_("Screen"))
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLYIMAGE,      "Multiply",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_HSLCOLORIMAGE,      "HSLColor",
              N_("HSLColor"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAYIMAGE,      "Overlay",
              N_("Overlay"))
  enum_value (GEGL_BLEND_MODE_TYPE_LINEARLIGHTIMAGE,      "LinearLight",
              N_("LinearLight"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHTIMAGE,      "HardLight",
              N_("HardLight"))
  enum_value (GEGL_BLEND_MODE_TYPE_LCHCOLORIMAGE,      "LCHColor",
              N_("LCHColor"))
  enum_value (GEGL_BLEND_MODE_TYPE_HSVHUEIMAGE,      "HSVHue",
              N_("HSVHue"))
enum_end (GeglBlendModeTypegzzimage)

/*This is the enum list of blend modes for Color Overlay*/
enum_start (gegl_blend_mode_type_effectsgzzcolor)
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLYCOLOR,      "Multiply",
              N_("Default Multiply; White is transparent"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOLIDCOLOR,      "SolidColor",
              N_("Solid Color"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGECOLOR,      "GrainMerge",
              N_("GrainMerge"))
  enum_value (GEGL_BLEND_MODE_TYPE_ADDITIONCOLOR,      "Addition",
              N_("Addition"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOFTLIGHTCOLOR,      "Softlight",
              N_("Softlight"))
  enum_value (GEGL_BLEND_MODE_TYPE_SCREENCOLOR,      "Screen",
              N_("Screen"))
  enum_value (GEGL_BLEND_MODE_TYPE_HSLCOLORCOLOR,      "HSLColor",
              N_("HSLColor"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAYCOLOR,      "Overlay",
              N_("Overlay"))
  enum_value (GEGL_BLEND_MODE_TYPE_LINEARLIGHTCOLOR,      "LinearLight",
              N_("LinearLight"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHTCOLOR,      "HardLight",
              N_("HardLight"))
  enum_value (GEGL_BLEND_MODE_TYPE_LCHCOLORCOLOR,      "LCHColor",
              N_("LCHColor"))
  enum_value (GEGL_BLEND_MODE_TYPE_HSVHUECOLOR,      "HSVHue",
              N_("HSVHue"))
  enum_value (GEGL_BLEND_MODE_TYPE_NOCOLOR,      "NoColor",
              N_("No Color"))
enum_end (GeglBlendModeTypegzzcolor)

/* This is the enum list of outline's base shape. Should correspond to GeglMedianBlurNeighborhood in median-blur.c */
enum_start (gegl_stroke_grow_shapeszz)
  enum_value (GEGL_stroke_GROW_SHAPE_SQUARE,  "square",  N_("Square"))
  enum_value (GEGL_stroke_GROW_SHAPE_CIRCLE,  "circle",  N_("Circle"))
  enum_value (GEGL_stroke_GROW_SHAPE_DIAMOND, "diamond", N_("Diamond"))
enum_end (GeglstrokeGrowShapeszz)

/* This is the enum list of extra outline's base shape. Should correspond to GeglMedianBlurNeighborhood in median-blur.c */
enum_start (gegl_stroke_grow_shapeszzextra)
  enum_value (GEGL_stroke_GROW_SHAPE_SQUAREextra,  "square",  N_("Square"))
  enum_value (GEGL_stroke_GROW_SHAPE_CIRCLEextra,  "circle",  N_("Circle"))
  enum_value (GEGL_stroke_GROW_SHAPE_DIAMONDextra, "diamond", N_("Diamond"))
enum_end (GeglstrokeGrowShapeszzextra)

/*This is the enum list of shiny text's blend mode switcher.*/
enum_start (gegl_blend_mode_typeshinegeffects)
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGEALTSHINE,      "GrainMergeAlt",
              N_("GrainMergeAlt"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGESHINE,      "GrainMerge",
              N_("GrainMerge"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHTSHINE,      "Hardlight",
              N_("Hardlight"))
  enum_value (GEGL_BLEND_MODE_TYPE_ADDITIONSHINE,      "Addition",
              N_("Addition"))
  enum_value (GEGL_BLEND_MODE_TYPE_REPLACESHINE, "Replace",
              N_("Multiply"))
enum_end (GeglBlendModeTypeshinegeffects)
ui_meta ("visible", "guichange {miscoptions}")

/*This is the enum list of glass text's blend mode switcher.*/
enum_start (gegl_blend_mode_type_glass)
  enum_value (GEGL_BLEND_MODE_TYPE_GLASSOVER,      "GlassOver",
              N_("GlassOver"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGEGLASS,      "GrainMerge",
              N_("GrainMerge"))
  enum_value (GEGL_BLEND_MODE_TYPE_LINEARLIGHTGLASS,      "LinearLight",
              N_("LinearLight"))
  enum_value (GEGL_BLEND_MODE_TYPE_SUBTRACTGLASS,      "Subtract",
              N_("Subtract"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAYGLASS, "Overlay",
              N_("Overlay"))
enum_end (geglblendmodetypeglass)
ui_meta ("visible", "guichange {glasstextpartner}")


   



/*This is the enum list for parts of GEGL Effects to be displayed (part of filter to be displayed)*/
enum_start (guichangeenumzz)
enum_value   (BEAVER_UI_STROKESHADOW, "strokeshadow", N_("Color Fill, Outline and Shadow"))
enum_value   (BEAVER_UI_INNERGLOWBEVEL, "innerglowbevel", N_("Bevel and Inner Glow"))
enum_value   (BEAVER_UI_IMAGEGRADIENT, "imagegradient", N_("Image file upload and Gradient"))
enum_value   (BEAVER_UI_OUTLINESPECIAL, "outlinespecial", N_("Special Options for Outline and Shadow"))
enum_value   (BEAVER_UI_MISCOPTIONS, "miscoptions", N_("Miscellaneous Text Effects"))
enum_value   (BEAVER_UI_EXTRAOSG, "extraosg", N_("Add a extra (second) Outline Shadow Glow"))
enum_value   (BEAVER_UI_GLASSTEXTPARTNER, "glasstextpartner", N_("Glass Text"))
  enum_end (guiendzz)

/*Below is sliders and other options that are displayed in GEGL Effects GUI. They are effected by the order they are in and the ui_meta association*/

property_enum(guichange, _("Part of filter to be displayed"),
    guiendzz, guichangeenumzz,
    BEAVER_UI_STROKESHADOW)
  description(_("Change the part of the GUI being displayed"))

/* Beginning of Color Overlay, Outline and Shadow GUI options*/
property_color  (optioncolor, _("Text color change (default only works on white text.)"), "#ffffff")
    description (_("Default fuctions like a color overlay on the multiply blend mode. Making the color white will make it transparent. If applied to white text it will become any color you choose. Changing the blend mode to solid will make it a normal color fill, and the other blend modes will do their thing respectively if you are using an image.   "))
  ui_meta ("visible", "guichange {strokeshadow}")


property_enum (colorblendmode, _("Blend Mode of Color Overlay"),
    GeglBlendModeTypegzzcolor, gegl_blend_mode_type_effectsgzzcolor,
    GEGL_BLEND_MODE_TYPE_MULTIPLY)
  ui_meta ("visible", "guichange {strokeshadow}")

property_double(coloropacity, _("Opacity of Color Overlay"), 0.999)
    value_range (0.0, 0.999)
  ui_steps      (0.01, 0.50)
    ui_meta     ("role", "output-extent")
  ui_meta ("visible", "guichange {strokeshadow}")
/* This option is hidden via output extent. The right click HTML notation slider on color overlay can change the opacity */

property_boolean (enableoutline, _("Enable Outline"), FALSE)
  description    (_("Disable or Enable Outline"))
  ui_meta ("visible", "guichange {strokeshadow}")

property_double (opacitystroke, _("Outline's Opacity"), 0.999)
  value_range   (0.0, 0.999)
  ui_steps      (0.01, 0.50)
  ui_meta ("visible", "guichange {strokeshadow}")


property_enum   (grow_shape, _("Outline's Base Shape"),
                 GeglstrokeGrowShapeszz, gegl_stroke_grow_shapeszz,
                 GEGL_stroke_GROW_SHAPE_CIRCLE)
  description   (_("The shape to expand or contract the outline fill in"))
  ui_meta ("visible", "guichange {strokeshadow}")

property_double (radiusstroke, _("Ability for Outline to puff out"), 0.0)
  value_range   (0.0, 12)
  ui_range      (0.0, 4)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  ui_meta ("visible", "guichange {strokeshadow}")


property_double (grow_radiusstroke, _("Outline's Size"), 12.0)
  value_range   (0.0, 100.0)
  ui_range      (0.0, 50.0)
  ui_digits     (0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the outline."))
  ui_meta ("visible", "guichange {strokeshadow}")




property_color  (colorstroke, _("Outline's Color"), "#000000")
  description   (_("The outline's color"))
    ui_meta     ("role", "color-primary")
  ui_meta ("visible", "guichange {strokeshadow}")


property_boolean (enableshadow, _("Enable Drop Shadow"), TRUE)
  description    (_("Enable Drop Shadow. This option is hidden via output extent."))
    ui_meta     ("role", "output-extent")
  ui_meta ("visible", "guichange {strokeshadow}")
/* This option is hidden via output extent. Drop Shadow is always enabled but hidden with 0 opacity. This checkbox actually disables it */

property_double (opacity, _("Shadow/Glow Opacity --ENABLE SHADOW/GLOW"), 0.0)
  value_range   (0.0, 0.999)
  ui_range      (0.0, 0.999)
  ui_steps      (0.01, 0.10)
  ui_meta ("visible", "guichange {strokeshadow}")


property_double (x, _("Shadow/Glow Horizontal Distance"), 10.0)
  description   (_("Horizontal shadow offset"))
  value_range   (-200, 200)
  ui_range      (-40.0, 40.0)
  ui_steps      (1, 10)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "x")
  ui_meta ("visible", "guichange {strokeshadow}")

property_double (y, _("Shadow/Glow Vertical Distance"), 10.0)
  description   (_("Vertical shadow offset"))
  value_range   (-200, 200)
  ui_range      (-40.0, 40.0)
  ui_steps      (1, 10)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "y")
  ui_meta ("visible", "guichange {strokeshadow}")

property_color  (color, _("Shadow/Glow Color"), "black")
  description   (_("The shadow's color (defaults to 'black')"))
  ui_meta ("visible", "guichange {strokeshadow}")

property_double (grow_radius, _("Shadow/Glow Grow size"), 0.0)
  value_range   (0.0, 100.0)
  ui_range      (0.0, 50.0)
  ui_digits     (0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the shadow before blurring; a negative value will contract the shadow instead"))
  ui_meta ("visible", "guichange {strokeshadow}")

property_double (radius, _("Shadow/Glow Blur intensity"), 12.0)
  value_range   (0.0, G_MAXDOUBLE)
  ui_range      (0.0, 90.0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  ui_meta ("visible", "guichange {strokeshadow}")

/* End of Color Overlay, Outline and Shadow GUI options*/

/* Beginning of Bevel and Inner Glow GUI Options */


property_int (depth, _("Bevel Depth"), 30)
    description (_("Depth of bevel that works with some but not all blend modes"))
    value_range (1, 100)
  ui_meta ("visible", "guichange {innerglowbevel}")

property_double (bevel1, _("Rotate Bevel Depth Angle"), 60.0)
    description (_("Elevation angle (degrees)"))
    value_range (30, 160)
    ui_meta ("unit", "degree")
  ui_meta ("visible", "guichange {innerglowbevel}")


property_double (radius1, _("Radius of Bevel"), 2.0)
  value_range (1.0, 30.0)
  ui_range (1.0, 15)
  ui_gamma (1.5)
  ui_steps      (0.01, 0.20)
  ui_meta ("visible", "guichange {innerglowbevel}")

property_double (osth, _("Bevel's unmodified edge pixel fix"), 0.100)
  value_range (0.0, 0.1)
  ui_range (0.0, 0.1)
  ui_meta ("visible", "guichange {innerglowbevel}")
    ui_meta     ("role", "output-extent")


property_double (slideupblack, _("Black Bevel and Image Bevel mode. "), 0.00)
    description (_("When in use GEGL Bevel works correctly on black Bevels when using blend modes like Grain Merge and Hardlight. All you have to do is select those blend modes for black text and then move this slider up. This same slider can also be used so bevel can apply to image file overlay's while ignoring their content."))
  value_range   (0.00, 0.999)
  ui_steps      (0.01, 0.50)
  ui_meta ("visible", "guichange {innerglowbevel}")


property_double (bevelopacity, _("Bevel's Opacity"), 0.999)
  value_range   (0.15, 0.999)
  ui_steps      (0.01, 0.50)
  ui_meta ("visible", "guichange {innerglowbevel}")

property_enum (blendmodebevel2, _("Select blend or Enable/Disable Bevel"),
    GeglBlendModeTypezz, gegl_blend_mode_type_effectszz,
    GEGL_BLEND_MODE_TYPE_BEVELOFF)
  ui_meta ("visible", "guichange {innerglowbevel}")

property_boolean (innerglow, _("Enable Inner Glow"), FALSE)
  description   (_("Whether to add an inner glow effect, which can be slow"))
  ui_meta ("visible", "guichange {innerglowbevel}")

property_enum (blendmodeinnerglow2, _("Blend Mode of Inner Glow"),
    GeglBlendModeTypeigzz, gegl_blend_mode_type_effectsigzz,
    GEGL_BLEND_MODE_TYPE_OVER)
  ui_meta ("visible", "guichange {innerglowbevel}")

property_double (innergradius, _("Inner Glow's Blur intensity"), 4.5)
  value_range   (0.0, 30.0)
  ui_range      (0.0, 30.0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("Making blur intensity very low will make an InnerStroke"))
  ui_meta ("visible", "guichange {innerglowbevel}")

property_double (innerggrow_radius, _("Inner Glow's Size"), 5)
  value_range   (1, 40.0)
  ui_range      (1, 30.0)
  ui_digits     (0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the innerglow."))
  ui_meta ("visible", "guichange {innerglowbevel}")

property_double (innergopacity, _("Inner Glow's Opacity"), 1.4)
  value_range   (0.0, 2.0)
  ui_steps      (0.01, 0.10)
  ui_meta ("visible", "guichange {innerglowbevel}")

property_color (innergvalue, _("Inner Glow's Color"), "#ff8f00")
    description (_("The color of the Inner Glow"))
  ui_meta ("visible", "guichange {innerglowbevel}")

property_double (xinnerglow, _("Inner Glow Horizontal Distance"), 0.0)
  description   (_("Horizontal shadow offset.  Anything other then zero makes inner glow an inner shadow."))
  ui_range      (-15.0, 15.0)
  value_range   (-15.0, 15.0)
  ui_steps      (1, 2)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "x")
  ui_meta ("visible", "guichange {innerglowbevel}")

property_double (yinnerglow, _("Inner Glow Vertical Distance"), 0.0)
  description   (_("Vertical shadow offset. Anything other then zero makes inner glow an inner shadow."))
  ui_range      (-15.0, 15.0)
  value_range   (-15.0, 15.0)
  ui_steps      (1, 2)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "y")
  ui_meta ("visible", "guichange {innerglowbevel}")

property_double  (fixoutline, _("Inner Glow's unmodified pixel fix"), 75)
  value_range (50, 85)
  description (_("If innerglow isn't covering a few pixels on the edge. Slide this up.'"))
  ui_meta ("visible", "guichange {innerglowbevel}")
/* End of Bevel and Inner Glow GUI Options*/

/* Beginning of Image File Overlay and Gradient GUI Options*/
property_file_path(src, _("Image file overlay upload "), "")
    description (_("Source image file path (png, jpg, raw, svg, bmp, tif, ...)"))
ui_meta ("visible", "guichange {imagegradient}")

property_enum (imageblendmode, _("Blend Mode of Image File Overlay (For layer content)"),
    GeglBlendModeTypegzzimage, gegl_blend_mode_type_effectsgzzimage,
    GEGL_BLEND_MODE_TYPE_NORMALIMAGE)
  ui_meta ("visible", "guichange {imagegradient}")



property_double(layeropacity, _("Opacity of Image File Overlay"), 0.999)
    value_range (0.0, 0.999)
  ui_steps      (0.01, 0.50)
ui_meta ("visible", "guichange {imagegradient}")


property_double (hue, _("Hue (ignores color fill and gradient)"),  0.0)
   description  (_("Hue adjustment. A color fill and gradient will be ignored by this option."))
   value_range  (-180.0, 180.0)
ui_meta ("visible", "guichange {imagegradient}")

property_double (chroma, _("Chroma (distorts color fill ignores gradient)"), 0.0)
   description  (_("Chroma adjustment. A color fill and gradient will be ignored by this option."))
   value_range  (-100.0, 100.0)
ui_meta ("visible", "guichange {imagegradient}")

property_double (lightness, _("Lightness (ignores gradient)"), 0.0)
   description  (_("Lightness adjustment. This works with color overlays but not normal gradients."))
   value_range  (-50.0, 50.0)
ui_meta ("visible", "guichange {imagegradient}")


property_boolean (gradient, _("Enable Gradient"), FALSE)
  description   (_("Whether to add Gradient overlay"))
ui_meta ("visible", "guichange {imagegradient}")


property_enum (blendmodegradient2, _("Blend Mode of Gradient"),
    GeglBlendModeTypegzz, gegl_blend_mode_type_effectsgzz,
    GEGL_BLEND_MODE_TYPE_OVER)
  ui_meta ("visible", "guichange {imagegradient}")

property_double(opacitygradient, _("Opacity of Gradient"), 0.999)
    value_range (0.0, 0.999)
  ui_steps      (0.01, 0.50)
ui_meta ("visible", "guichange {imagegradient}")


property_double (start_x, _("Gradient Horizontal1"), 659.0)
    ui_meta("unit", "pixel-coordinate")
    ui_meta("axis", "x")
  ui_steps      (0.01, 3.0)
   ui_range    (-700, 700.0)
ui_meta ("visible", "guichange {imagegradient}")


property_double (start_y, _("Gradient Vertical1"), 49.0)
    ui_meta("unit", "pixel-coordinate")
    ui_meta("axis", "y")
  ui_steps      (0.01, 3.0)
   ui_range    (-700, 700.0)
ui_meta ("visible", "guichange {imagegradient}")

property_double (end_x, _("Gradient Horizontal2"), 647.0)
    ui_meta("unit", "pixel-coordinate")
    ui_meta("axis", "x")
  ui_steps      (0.01, 3.0)
   ui_range    (-700, 700.0)
ui_meta ("visible", "guichange {imagegradient}")

property_double (end_y, _("Gradient Vertical2"), 572.0)
    ui_meta ("unit", "pixel-coordinate")
    ui_meta ("axis", "y")
   ui_range    (-700, 700.0)
  ui_steps      (0.01, 3.0)
ui_meta ("visible", "guichange {imagegradient}")

property_color (start_color, _("Gradient Start Color"), "#34ebd6")
    description (_("The color at (x1, y1)"))
ui_meta ("visible", "guichange {imagegradient}")


property_color  (end_color, _("Gradient End Color"), "#fe18f2")
    description (_("The color at (x2, y2)"))
ui_meta ("visible", "guichange {imagegradient}")


/* End of Image File Overlay and Gradient GUI Options*/

/* Beginning of Special Options for Outline and Shadow GUI Options*/
property_double (xstroke, _("Outline Horizontal Position"), 0.0)
  description   (_("Horizontal outline fill offset"))
  value_range   (-15.0, 15.0)
  ui_steps      (1, 10)
  ui_meta ("visible", "guichange {outlinespecial}")

property_double (ystroke, _("Outline Vertical Position"), 0.0)
  description   (_("Vertical outline fill offset"))
  value_range   (-15.0, 15.0)
   ui_steps      (1, 10)
  ui_meta ("visible", "guichange {outlinespecial}")

property_boolean (specialoutline, _("Enable effects on Outline"), FALSE)
  description    (_("Turn on special outline abilities"))
  ui_meta ("visible", "guichange {outlinespecial}")

property_enum (blendmodebeveloutline, _("Select blend or Enable/Disable Outline Bevel"),
    GeglBlendModeTypezzbevoutlinege, gegl_blend_mode_type_effectszzbevoutlinege,
    GEGL_BLEND_MODE_TYPE_MULTIPLYGE)
  ui_meta ("visible", "guichange {outlinespecial}")

property_int (osdepth, _("Outline Bevel Depth"), 12)
    description (_("Depth of Outline Bevel that works with some but not all blend modes"))
    value_range (1, 70)
  ui_meta ("visible", "guichange {outlinespecial}")

property_double (osbevel, _("Rotate Outline Bevel Depth Angle (90 resets)"), 80.0)
    description (_("Elevation angle (degrees)"))
    value_range (55, 125)
    ui_meta ("unit", "degree")
  ui_meta ("visible", "guichange {outlinespecial}")


property_double (osradius, _("Radius of Outline Bevel"), 3.0)
  value_range (1.0, 12.0)
  ui_range (1.0, 12)
  ui_gamma (1.5)
  ui_steps      (0.01, 0.20)
  ui_meta ("visible", "guichange {outlinespecial}")


property_double (osbevelopacity, _("Outline Bevel's Opacity"), 0.999)
  value_range   (0.15, 0.999)
  ui_steps      (0.01, 0.10)
  ui_meta ("visible", "guichange {outlinespecial}")


property_file_path(ossrc, _("Image file overlay for Outline upload "), "")
    description (_("Source image file path (png, jpg, raw, svg, bmp, tif, ...)"))
  ui_meta ("visible", "guichange {outlinespecial}")

property_double (oshue, _("Hue rotation of Outline's Image file overlay"),  0.0)
   description  (_("Hue adjustment"))
   value_range  (-180.0, 180.0)
  ui_meta ("visible", "guichange {outlinespecial}")


property_double (oslightness, _("Lightness of Outline's Image file overlay"), 0.0)
   description  (_("Lightness adjustment"))
   value_range  (-100.0, 100.0)
  ui_meta ("visible", "guichange {outlinespecial}")

property_boolean (specialshadowoptions, _("Enable/Disable special options for Shadow/Glow"), FALSE)
  description    (_("Turn on special drop shadow glow's special abilities"))
  ui_meta ("visible", "guichange {outlinespecial}")


property_file_path(shadowimage, _("Image file overlay for Shadow/Glow"), "")
    description (_("Source image file path (png, jpg, raw, svg, bmp, tif, ...)"))
  ui_meta ("visible", "guichange {outlinespecial}")


property_double (blurshadowimage, _("Blur Shadow/Glow's Image file overlay"), 0)
   description (_("Standard deviation for the XY axis"))
   value_range (0.0, 40.0)
   ui_range    (0.24, 40.0)
   ui_gamma    (3.0)
  ui_steps      (0.01, 0.50)
   ui_meta     ("unit", "pixel-distance")
  ui_meta ("visible", "guichange {outlinespecial}")

property_boolean (enableaura, _("Enable Aura mode - requires both shadow checkboxes enabled)"), FALSE)
  description    (_("This can be disabled both by unchecking this box and special options for drop shadow. Enabling this gives drop shadow glow a aura effect and seed."))
  ui_meta ("visible", "guichange {outlinespecial}")

property_double (aurasize, _("Glow Aura Intensity"), 10)
    description (_("Average diameter of each tile (in pixels)"))
    value_range (4, 15.0)
    ui_meta     ("unit", "pixel-distance")
  ui_steps      (0.01, 0.50)
  ui_meta ("visible", "guichange {outlinespecial}")

property_seed (seedaura, _("Seed of Glow Aura"), randaura)
  ui_meta ("visible", "guichange {outlinespecial}")
/* Two rand commands can't co-exist but if it is renamed to "randaura" it appears to work just fine */

/* End of Special Options for Outline and Shadow GUI Options*/
 
/* Beginning of Miscellaneous text options*/

property_boolean (enableshine, _("Enable/Disable Shiny Text"), FALSE)
  description    (_("Whether text should be shiny"))
ui_meta ("visible", "guichange {miscoptions}")


property_enum (blendmodeshine, _("Blend Mode of Shiny Text"),
    GeglBlendModeTypeshinegeffects, gegl_blend_mode_typeshinegeffects,
    GEGL_BLEND_MODE_TYPE_REPLACESHINE)
ui_meta ("visible", "guichange {miscoptions}")


property_double (opacityshine, _("Opacity of Gloss Shine"), 0.500)
    description (_("Global opacity value that is always used on top of the optional auxiliary input buffer."))
    value_range (0.100, 0.999)
    ui_range    (0.100, 0.950)
  ui_gamma (1.5)
ui_meta ("visible", "guichange {miscoptions}")

property_double (x_scaleshine, _("Horizontal Scale of Gloss Shine"), 28.00)
    description (_("Scale value for x axis"))
    value_range (15, 60.0)
    ui_range    (15, 60.0)
    ui_meta     ("unit", "pixel-distance")
    ui_meta     ("axis", "x")
ui_meta ("visible", "guichange {miscoptions}")

property_double (y_scaleshine, _("Vertical Scale of Gloss Shine"), 56.0)
    description (_("Scale value for y axis"))
    value_range (15, 150.0)
    ui_range    (15, 150.0)
    ui_meta     ("unit", "pixel-distance")
    ui_meta     ("axis", "y")
ui_meta ("visible", "guichange {miscoptions}")

property_double (complexity, _("Complexity of Gloss Shine"), 0.8)
    description (_("Complexity factor"))
    value_range (0.1, 0.8)
ui_meta ("visible", "guichange {miscoptions}")

property_double (blend_power, _("Dark to light intensity"), -0.600)
    description (_("Intensity of the Gloss Shine Effect"))
    value_range (-0.9, 0.0)
ui_meta ("visible", "guichange {miscoptions}")

property_seed (seedshine, _("Random seed of Gloss Shine"), rand)
ui_meta ("visible", "guichange {miscoptions}")

property_double (microblur, _("Very Mild Blur on Original Text "), 0)
   description (_("Applies a blur of 0.50 on the original text layer under all applied effects. This fixes rough surfaces and may enhance the bevel effect. If applied to an image it is as if you ran gaussian blur 0.50"))
   value_range (0.0, 0.5)
   ui_range    (0.0, 0.5)
  ui_steps      (0.01, 0.10)
   ui_meta     ("unit", "pixel-distance")
ui_meta ("visible", "guichange {miscoptions}")

property_int  (thinboldenable, _("Slide up or down to Enable/Disable Thin to Thick slider"), 0)
  value_range (0, 5)
  ui_range    (0, 5)
  ui_meta     ("unit", "pixel-distance")
  description (_("Sliding back to 0 will disable this effect. Thin to Thickness Slider"))
ui_meta ("visible", "guichange {miscoptions}")



property_double  (thinboldap, _("Thin to Thick text slider"), 50)
  value_range (0, 100)
  description (_("0 Makes text thin as possible. 100 makes text bold as possible. This is a median blur alpha percentile and should only be applied on text."))
ui_meta ("visible", "guichange {miscoptions}")

property_double (knockout, _("Knock out original text. (translucent text option)"), 0.0)
    description (_("Global opacity value that is always used on top of the optional auxiliary input buffer."))
    value_range (0.0, 0.999)
    ui_range    (0.0, 0.999)
  ui_steps      (0.01, 0.10)
ui_meta ("visible", "guichange {miscoptions}")

/* End of Miscellaneous text options*/

/* Beginning of Extra (second) Outline Shadow Glow GUI options*/
property_boolean (enableextraoutline, _("Enable Second Outline/Shadow/Glow"), FALSE)
  description    (_("Disable or Enable Outline"))
  ui_meta ("visible", "guichange {extraosg}")


property_double (opacitystrokeextraoutline, _("Extra Outline/Shadow/Glow's Opacity"), 0.999)
  value_range   (0.0, 0.999)
  ui_steps      (0.01, 0.50)
  ui_meta ("visible", "guichange {extraosg}")
    /* BUG SOLVED JUNE 5th 2023 -- IGNORE COMMENT-- This extra outline opacity slider is hidden via "output-extent" and the one below is to be used.
for opacity control of the second stroke. This is meant to remove a bug where GEGL Effects stacks to copies of the layer on start up.
Unfortunately to enable the extra outline the user has to Enable Extra Outline Checkbox and
slide up the opacity slider. It is what it is though inconvient. Users should be greatful that such complex
text styling is even possible in the first place. */

property_double (extrassgopacity, _("Extra Outline/Shadow/Glow's Opacity "), 0.0)
  value_range   (0.0, 0.999)
  ui_steps      (0.01, 0.50)
  ui_meta ("visible", "guichange {extraosg}")
    ui_meta     ("role", "output-extent")


property_double (xextraoutline, _("Extra Outline/Shadow/Glow Horizontal Distance"), 0.0)
  description   (_("Horizontal shadow offset"))
  value_range   (-6.0, 6.0)
  ui_range      (-6.0, 6.0)
  ui_steps      (1, 10)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "x")
  ui_meta ("visible", "guichange {extraosg}")

property_double (yextraoutline, _("Extra Outline/Shadow/Glow Vertical Distance"), 0.0)
  description   (_("Vertical shadow offset"))
  value_range   (-6.0, 6.0)
  ui_range      (-6.0, 6.0)
  ui_steps      (1, 10)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "y")
  ui_meta ("visible", "guichange {extraosg}")

property_enum   (grow_shapeextraoutline, _("Extra Outline/Shadow/Glow's Base Shape"),
                 GeglstrokeGrowShapeszzextra, gegl_stroke_grow_shapeszzextra,
                 GEGL_stroke_GROW_SHAPE_CIRCLEextra)
  description   (_("The shape to expand or contract the outline fill in"))
  ui_meta ("visible", "guichange {extraosg}")

property_double (radiusstrokeextraoutline, _("Blur to make Extra Shadow/Glow"), 0.0)
  value_range   (0.0, 50)
  ui_range      (0.0, 50.0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  ui_meta ("visible", "guichange {extraosg}")

property_double (grow_radiusstrokeextraoutline, _("Extra Outline's Size (must be past default outline)"), 19.0)
  value_range   (0.0, 100.0)
  ui_range      (0.0, 100.0)
  ui_digits     (0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the outline."))
  ui_meta ("visible", "guichange {extraosg}")

property_color  (colorstrokeextraoutline, _("Extra Outline/Shadow/Glow's Color"), "#2400ff")
  description   (_("The outline's color"))
  ui_meta ("visible", "guichange {extraosg}")

property_boolean (specialoutlineextra, _("Enable effects on Extra Outline"), FALSE)
  description    (_("Turn on special outline abilities"))
  ui_meta ("visible", "guichange {extraosg}")


property_enum (blendmodebeveloutlineextra, _("Select blend or Enable/Disable Extra Outline Bevel"),
    GeglBlendModeTypezzbevoutlinegeextra, gegl_blend_mode_type_effectszzbevoutlinegeextra,
    GEGL_BLEND_MODE_TYPE_MULTIPLYGEextra)
  ui_meta ("visible", "guichange {extraosg}")

property_int (osdepthextra, _("Extra Outline's Bevel Depth"), 12)
    description (_("Depth of Outline Bevel that works with some but not all blend modes"))
    value_range (1, 70)
  ui_meta ("visible", "guichange {extraosg}")


property_double (osbevelextra, _("Rotate Extra Outline Bevel Depth Angle (90 resets)"), 80.0)
    description (_("Elevation angle (degrees)"))
    value_range (55, 125)
    ui_meta ("unit", "degree")
  ui_meta ("visible", "guichange {extraosg}")

property_double (osradiusextra, _("Radius of Extra Outline Bevel"), 3.0)
  value_range (1.0, 12.0)
  ui_range (1.0, 12)
  ui_gamma (1.5)
  ui_steps      (0.01, 0.20)
  ui_meta ("visible", "guichange {extraosg}")



property_double (osbevelopacityextra, _("Extra Outline Bevel's Opacity"), 0.999)
  value_range   (0.15, 0.999)
  ui_steps      (0.01, 0.10)
  ui_meta ("visible", "guichange {extraosg}")



property_file_path(ossrcextra, _("Image file overlay for Extra Outline upload "), "")
    description (_("Source image file path (png, jpg, raw, svg, bmp, tif, ...)"))
  ui_meta ("visible", "guichange {extraosg}")

/* End of Extra (second) Outline Shadow Glow GUI options*/

/* Beginning of Glass Text*/

property_boolean (enableglasstext, _("Enable Glass on Text"), FALSE)
  description    (_("Enable Glass Text"))
  ui_meta ("visible", "guichange {glasstextpartner}")

property_enum (glassblendmode, _("Blend Mode of Glass on Text"),
 geglblendmodetypeglass, gegl_blend_mode_type_glass,
    GEGL_BLEND_MODE_TYPE_GLASSOVER)
  ui_meta ("visible", "guichange {glasstextpartner}")



property_double (glassazimuth, _("Azimuth"), 30.0)
    description (_("Light angle (degrees)"))
    value_range (0, 360)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")
  ui_steps      (0.5, 0.50)
  ui_meta ("visible", "guichange {glasstextpartner}")


property_int (glassdepth, _("Depth"), 20.0)
    description (_("Filter width"))
    value_range (10, 100)
  ui_meta ("visible", "guichange {glasstextpartner}")


property_double (glasselevation, _("Elevation"), 45.0)
    description (_("Elevation angle (degrees)"))
    value_range (40, 46)
    ui_meta ("unit", "degree")
  ui_steps      (0.1, 0.50)
  ui_meta ("visible", "guichange {glasstextpartner}")



property_double (glassretract, _("Retract Shine"), 3.0)
   description (_("Add a Gaussian Blur"))
   value_range (1, 3)
   ui_range    (1, 3)
   ui_gamma    (3.0)
   ui_meta     ("unit", "pixel-distance")
  ui_steps      (0.1, 0.50)
  ui_meta ("visible", "guichange {glasstextpartner}")


property_double (glassblur, _("Blur Shine"), 0.5)
   description (_("Add a very mild Gaussian Blur"))
   value_range (0.5, 1)
   ui_range    (0.5, 1)
   ui_gamma    (3.0)
  ui_steps      (0.1, 0.50)
   ui_meta     ("unit", "pixel-distance")
  ui_meta ("visible", "guichange {glasstextpartner}")


property_color (glasscolor, _("Color"), "#ffffff")
    description (_("The color to paint over the input of Glass Text"))
  ui_meta ("visible", "guichange {glasstextpartner}")


property_double (glassopacity, _("Opacity of Glass Text"), 0.999)
    description (_("Opacity slider of Glass Text"))
    value_range (0.3, 0.999)
    ui_range    (0.3, 0.999)
  ui_steps      (0.3, 0.999)
  ui_meta ("visible", "guichange {glasstextpartner}")
/* End of Glass Text*/


#else

#define GEGL_OP_META
#define GEGL_OP_NAME     gegleffectspending
#define GEGL_OP_C_SOURCE gegleffectspending.c

#include "gegl-op.h"


/* First full Node listing. This is for special GEGL Graphs that have blend mode switchers. All nodes must be listed here. They can be in any order but they are organized for convience*/
typedef struct
{
  GeglNode *input;

/*All nodes relating to bevel start here*/
  GeglNode *mbd;
  GeglNode *mbdopacity;
  GeglNode *multiplyb;
  GeglNode *nopb;
  GeglNode *multiply;
  GeglNode *subtract;
  GeglNode *addition;
  GeglNode *shinygm;
  GeglNode *colordodge;
  GeglNode *hardlight;
  GeglNode *grainextract;
  GeglNode *grainmerge;
  GeglNode *screen;
  GeglNode *beveloff;
/*All nodes relating to bevel end here*/

/*All nodes relating to innerglow start here*/
  GeglNode *innerglow;
  GeglNode *over;
  GeglNode *linearlightig;
  GeglNode *nopig;
  GeglNode *overlayig;
  GeglNode *softlightig;
  GeglNode *hslcolorig;
  GeglNode *burnig;
  GeglNode *hueig;
  GeglNode *lchcolorig;
  GeglNode *hardlightig;
  GeglNode *screenig;
  GeglNode *multiplyig;
  GeglNode *grainmergeig;
/*All nodes relating to inner glow end here*/

/*All nodes relating to outline start here*/
  GeglNode *stroke;
  GeglNode *strokebehind;
  GeglNode *nopstrokebehind;
/*All nodes relating to outline end here*/

/*All nodes relating to shadow begin here*/
  GeglNode *ds;
  GeglNode *dsbehind;
  GeglNode *nopdsbehind;
/*All nodes relating to shadow end here*/

/*All nodes relating to extra outline shadow glow begin here*/
  GeglNode *nopextrassg;
  GeglNode *behindextrassg;
  GeglNode *extrassg;
  GeglNode *extrassgopacity;
/*All nodes relating to extra outline shadow glow end here*/

/*All nodes relating to gradient start here*/
  GeglNode *gradient;
  GeglNode *atopg;
  GeglNode *nopg;
  GeglNode *overlayg;
  GeglNode *crop;
  GeglNode *additiong;
  GeglNode *softlightg;
  GeglNode *hslcolorg;
  GeglNode *screeng;
  GeglNode *multiplyg;
  GeglNode *grainmergeg;
  GeglNode *hsvhueg;
  GeglNode *linearlightg;
  GeglNode *hardlightg;
  GeglNode *burng;
  GeglNode *lchcolorg;
  GeglNode *opacitygradient;
/*All nodes relating to gradient end here*/

/*All nodes that have no relationships (shiny text,  microblur, and thin to thick text) start here*/
  GeglNode *shiny;
  GeglNode *microblur;
  GeglNode *thinbold;
  GeglNode *repairgeglgraph;
/*All nodes that have no relationships (shiny text, microblur, and thin to thick text) end here*/

/*All nodes relating to glass over text start here*/
  GeglNode *glassovertext;
  GeglNode *glassover;
  GeglNode *nopglass;
/*All nodes relating to glass over text end here*/


/*All nodes relating to knockout text start here*/
  GeglNode *xor;
  GeglNode *opacityinput;
  GeglNode *knockoutidref;
/*All nodes relating to knockout text end here*/

/*All nodes relating to image file upload start here*/
  GeglNode *atopi;
  GeglNode *image;
  GeglNode *nopimage;
  GeglNode *saturation;
  GeglNode *grainmergeimage;
  GeglNode *additionimage;
  GeglNode *overlayimage;
  GeglNode *multiplyimage;
  GeglNode *screenimage; 
  GeglNode *hslcolorimage; 
  GeglNode *softlightimage; 
  GeglNode *linearlightimage; 
  GeglNode *hardlightimage; 
  GeglNode *lchcolorimage; 
  GeglNode *hsvhueimage;
/*All nodes relating to image file upload end here*/

/*All nodes relating to color overlay start here*/
  GeglNode *mcol;
  GeglNode *nopm;
  GeglNode *grainmergecolor;
  GeglNode *additioncolor;
  GeglNode *overlaycolor;
  GeglNode *screencolor; 
  GeglNode *hslcolorcolor; 
  GeglNode *softlightcolor; 
  GeglNode *linearlightcolor; 
  GeglNode *hardlightcolor; 
  GeglNode *lchcolorcolor; 
  GeglNode *hsvhuecolor;
  GeglNode *nocolor;
  GeglNode *solidcolor;
  GeglNode *coloropacity;
  GeglNode *cropcolor;
/*All nodes relating to color overlay end here*/

  GeglNode *output;
} State;

static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  GeglNode *bevelmode;
  GeglNode *over;
  GeglNode *atopg;
  GeglNode *atopi;
  GeglNode *multiply;

/* These are the blend mode switchers 
BevelMode is Bevel, Over is Inner Glow, atopG is Gradient, atopI is inner glow and multiply is color overlay.
The two outline bevel mode switchers and shiny text blend mode switcher use their own .c files. As of June 7 2023 they are hidden operations. */

/* Everything below is the literal GEGL Graph instructions. There are technically four GEGL graphs that load depending on the checkboxes.*/

  if (!state) return;

  multiply = state->multiply; /* Blend mode switchers for color fill. May 31st 2023 GEGL Effects got the ability to change blend modes for color fill. */
  switch (o->colorblendmode) {
    case GEGL_BLEND_MODE_TYPE_MULTIPLYCOLOR: multiply = state->multiply; break;
    case GEGL_BLEND_MODE_TYPE_SOLIDCOLOR : multiply = state->solidcolor; break;
    case GEGL_BLEND_MODE_TYPE_GRAINMERGECOLOR : multiply = state->grainmergecolor; break;
    case GEGL_BLEND_MODE_TYPE_ADDITIONCOLOR: multiply = state->additioncolor; break;
    case GEGL_BLEND_MODE_TYPE_SOFTLIGHTCOLOR: multiply = state->softlightcolor; break;
    case GEGL_BLEND_MODE_TYPE_SCREENCOLOR: multiply = state->screencolor; break;
    case GEGL_BLEND_MODE_TYPE_HSLCOLORCOLOR: multiply = state->hslcolorcolor; break;
    case GEGL_BLEND_MODE_TYPE_OVERLAYCOLOR: multiply = state->overlaycolor; break;
    case GEGL_BLEND_MODE_TYPE_LINEARLIGHTCOLOR: multiply = state->linearlightcolor; break;
    case GEGL_BLEND_MODE_TYPE_HARDLIGHTCOLOR: multiply = state->hardlightcolor; break;
    case GEGL_BLEND_MODE_TYPE_LCHCOLORCOLOR: multiply = state->lchcolorcolor; break; 
    case GEGL_BLEND_MODE_TYPE_HSVHUECOLOR: multiply = state->hsvhuecolor; break;
    case GEGL_BLEND_MODE_TYPE_NOCOLOR: multiply = state->nocolor; break;

 }
/* atopi was named based on the gegl blend mode "src-atop  combined with image. Letter I hence the name "atopi*/
  atopi = state->atopi; /* Blend mode switchers for Image File Overlay "src_atop" is the default. */
  switch (o->imageblendmode) {
    case GEGL_BLEND_MODE_TYPE_NORMALIMAGE: atopi = state->atopi; break;
    case GEGL_BLEND_MODE_TYPE_GRAINMERGEIMAGE: atopi = state->grainmergeimage; break;
    case GEGL_BLEND_MODE_TYPE_ADDITIONIMAGE: atopi = state->additionimage; break;
    case GEGL_BLEND_MODE_TYPE_SOFTLIGHTIMAGE: atopi = state->softlightimage; break;
    case GEGL_BLEND_MODE_TYPE_SCREENIMAGE: atopi = state->screenimage; break;
    case GEGL_BLEND_MODE_TYPE_MULTIPLYIMAGE: atopi = state->multiplyimage; break;
    case GEGL_BLEND_MODE_TYPE_HSLCOLORIMAGE: atopi = state->hslcolorimage; break;
    case GEGL_BLEND_MODE_TYPE_OVERLAYIMAGE: atopi = state->overlayimage; break;
    case GEGL_BLEND_MODE_TYPE_LINEARLIGHTIMAGE: atopi = state->linearlightimage; break;
    case GEGL_BLEND_MODE_TYPE_HARDLIGHTIMAGE: atopi = state->hardlightimage; break;
    case GEGL_BLEND_MODE_TYPE_LCHCOLORIMAGE: atopi = state->lchcolorimage; break; 
    case GEGL_BLEND_MODE_TYPE_HSVHUEIMAGE: atopi = state->hsvhueimage; break;


 }
/* Bevel's blend mode switcher.
Bevel use to only work Multiply and Grain Merge until beaver solved a bug related to bad node connections. Bug solved in early April 2023. Bug caused the other blend modes to look bad.*/

  bevelmode = state->beveloff; 
  switch (o->blendmodebevel2) {
    case GEGL_BLEND_MODE_TYPE_MULTIPLY: bevelmode = state->multiplyb; break;
    case GEGL_BLEND_MODE_TYPE_GRAINMERGE: bevelmode = state->grainmerge; break;
    case GEGL_BLEND_MODE_TYPE_SUBTRACT: bevelmode = state->subtract; break;
    case GEGL_BLEND_MODE_TYPE_GRAINEXTRACT: bevelmode = state->grainextract; break;
    case GEGL_BLEND_MODE_TYPE_SHINYGM: bevelmode = state->shinygm; break;
    case GEGL_BLEND_MODE_TYPE_COLORDODGE: bevelmode = state->colordodge; break;
    case GEGL_BLEND_MODE_TYPE_HARDLIGHT: bevelmode = state->hardlight; break;
    case GEGL_BLEND_MODE_TYPE_SCREEN: bevelmode = state->screen; break;
    case GEGL_BLEND_MODE_TYPE_BEVELOFF: bevelmode = state->beveloff; break;

 }

  over = state->over; /* the default */
  switch (o->blendmodeinnerglow2) {
    case GEGL_BLEND_MODE_TYPE_OVER: over = state->over; break;
    case GEGL_BLEND_MODE_TYPE_GRAINMERGEIG: over = state->grainmergeig; break;
    case GEGL_BLEND_MODE_TYPE_ADDITION: over = state->addition; break;
    case GEGL_BLEND_MODE_TYPE_SOFTLIGHTIG: over = state->softlightig; break;
    case GEGL_BLEND_MODE_TYPE_SCREENIG: over = state->screenig; break;
    case GEGL_BLEND_MODE_TYPE_MULTIPLYIG: over = state->multiplyig; break;
    case GEGL_BLEND_MODE_TYPE_HSLCOLORIG: over = state->hslcolorig; break;
    case GEGL_BLEND_MODE_TYPE_OVERLAYIG: over = state->overlayig; break;
    case GEGL_BLEND_MODE_TYPE_LINEARLIGHTIG: over = state->linearlightig; break;
    case GEGL_BLEND_MODE_TYPE_HARDLIGHTIG: over = state->hardlightig; break;
    case GEGL_BLEND_MODE_TYPE_BURNIG: over = state->burnig; break; 
    case GEGL_BLEND_MODE_TYPE_LCHCOLORIG: over = state->lchcolorig; break; 
    case GEGL_BLEND_MODE_TYPE_HUEIG: over = state->hueig; break; 

  }
/* atopg was named based on the gegl blend mode "src-atop  combined with gradient G hence the name "atopg*/
  atopg = state->atopg; /* the default */
  switch (o->blendmodegradient2) {
    case GEGL_BLEND_MODE_TYPE_ATOPG: atopg = state->atopg; break;
    case GEGL_BLEND_MODE_TYPE_GRAINMERGEG: atopg = state->grainmergeg; break;
    case GEGL_BLEND_MODE_TYPE_ADDITIONG: atopg = state->additiong; break;
    case GEGL_BLEND_MODE_TYPE_SOFTLIGHTG: atopg = state->softlightg; break;
    case GEGL_BLEND_MODE_TYPE_SCREENG: atopg = state->screeng; break;
    case GEGL_BLEND_MODE_TYPE_MULTIPLYG: atopg = state->multiplyg; break;
    case GEGL_BLEND_MODE_TYPE_HSLCOLORG: atopg = state->hslcolorg; break;
    case GEGL_BLEND_MODE_TYPE_OVERLAYG: atopg = state->overlayg; break;
    case GEGL_BLEND_MODE_TYPE_LINEARLIGHTG: atopg = state->linearlightg; break;
    case GEGL_BLEND_MODE_TYPE_HSVHUEG: atopg = state->hsvhueg; break;
    case GEGL_BLEND_MODE_TYPE_HARDLIGHTG: atopg = state->hardlightg; break;
    case GEGL_BLEND_MODE_TYPE_BURNG: atopg = state->burng; break;
    case GEGL_BLEND_MODE_TYPE_LCHCOLORG: atopg = state->lchcolorg; break;

  }
       /* Second full Node listing. (contains four GEGL graphs for each checkbox) Nodes must be listed in proper orders. This is the GEGL Graph */
  if (o->innerglow)
  {
    if (o->gradient)
    {
      /* both innerglow and gradient */
         gegl_node_link_many (state->input, state->thinbold, state->microblur, state->nopimage, atopi,  multiply, state->cropcolor, state->nopglass,  state->glassover,   state->nopg, atopg,  state->crop, state->shiny, state->nopb, bevelmode, state->nopextrassg, state->knockoutidref,  state->nopig, over, state->nopstrokebehind, state->strokebehind, state->behindextrassg, state->nopdsbehind, state->dsbehind, state->xor, state->repairgeglgraph, state->output, NULL);
      /* Nodes relating to color overlay */
      gegl_node_link_many (state->mcol, state->coloropacity, NULL);
      gegl_node_connect_from (multiply, "aux", state->coloropacity, "output");
      /* Nodes relating to knockout text */
      gegl_node_link_many (state->knockoutidref, state->opacityinput, NULL);
      gegl_node_connect_from (state->xor, "aux", state->opacityinput, "output");
      /* Nodes relating to bevel of text */
      gegl_node_connect_from (bevelmode, "aux", state->mbdopacity, "output");
      gegl_node_link_many (state->nopb, state->mbd, state->mbdopacity, NULL);
      /* Nodes relating to inner glow */
      gegl_node_link_many (state->nopig, state->innerglow, NULL);
      gegl_node_connect_from (over, "aux", state->innerglow, "output");
      /* Nodes relating to gradient */
      gegl_node_link_many (state->gradient, state->opacitygradient, NULL);
      gegl_node_connect_from (atopg, "aux", state->opacitygradient, "output");
      /* Nodes relating to image file overlay */
      gegl_node_link_many ( state->image, state->saturation, NULL);
      gegl_node_connect_from (atopi, "aux", state->saturation, "output");
      /* Nodes relating to extra outline shadow glow */
      gegl_node_link_many (state->nopextrassg, state->extrassg, state->extrassgopacity, NULL);
      gegl_node_connect_from (state->behindextrassg, "aux", state->extrassgopacity, "output");
      /* Nodes relating to outline */
      gegl_node_link_many (state->nopstrokebehind, state->stroke, NULL);
      gegl_node_connect_from (state->strokebehind, "aux", state->stroke, "output");
      /* Nodes relating to shadow */
      gegl_node_link_many (state->nopdsbehind, state->ds, NULL);
      gegl_node_connect_from (state->dsbehind, "aux", state->ds, "output");
      /* Nodes relating to Glass Over Text start here */
      gegl_node_link_many (state->nopglass, state->glassovertext, NULL);
      gegl_node_connect_from (state->glassover, "aux", state->glassovertext, "output");
    }
    else
    {
      /* innerglow but no gradient */
         gegl_node_link_many (state->input, state->thinbold, state->microblur, state->nopimage, atopi, multiply, state->crop, state->nopglass, state->glassover,  state->shiny, state->nopb, bevelmode, state->nopextrassg, state->knockoutidref, state->nopig, over, state->nopstrokebehind, state->strokebehind,  state->behindextrassg, state->nopdsbehind, state->dsbehind, state->xor, state->repairgeglgraph, state->output, NULL);
      /* Nodes relating to color overlay */
      gegl_node_link_many (state->mcol, state->coloropacity, NULL);
      gegl_node_connect_from (multiply, "aux", state->coloropacity, "output");
      /* Nodes relating to knockout text */
      gegl_node_link_many (state->knockoutidref, state->opacityinput, NULL);
      gegl_node_connect_from (state->xor, "aux", state->opacityinput, "output");
      /* Nodes relating to bevel of text */
      gegl_node_link_many (state->nopb, state->mbd, state->mbdopacity,  NULL);
      gegl_node_connect_from (bevelmode, "aux", state->mbdopacity, "output");
      /* Nodes relating to inner glow */
      gegl_node_link_many (state->nopig, state->innerglow, NULL);
      gegl_node_connect_from (over, "aux", state->innerglow, "output");
      /* Nodes relating to image file overlay */
      gegl_node_link_many ( state->image, state->saturation, NULL);
      gegl_node_connect_from (atopi, "aux", state->saturation, "output");
      /* Nodes relating to extra outline shadow glow */
      gegl_node_link_many (state->nopextrassg, state->extrassg, state->extrassgopacity, NULL);
      gegl_node_connect_from (state->behindextrassg, "aux", state->extrassgopacity, "output");
      /* Nodes relating to outline */
      gegl_node_link_many (state->nopstrokebehind, state->stroke, NULL);
      gegl_node_connect_from (state->strokebehind, "aux", state->stroke, "output");
      /* Nodes relating to shadow */
      gegl_node_link_many (state->nopdsbehind, state->ds, NULL);
      gegl_node_connect_from (state->dsbehind, "aux", state->ds, "output");
      /* Nodes relating to Glass Over Text start here */
      gegl_node_link_many (state->nopglass, state->glassovertext, NULL);
      gegl_node_connect_from (state->glassover, "aux", state->glassovertext, "output");
    }
  }
  else
  {
    if (o->gradient)
    {
      /* gradient but no innerglow */
         gegl_node_link_many (state->input, state->thinbold, state->microblur,  state->nopimage, atopi,   multiply, state->cropcolor, state->nopglass,  state->glassover,  state->nopg, atopg, state->crop, state->shiny, state->nopb, bevelmode, state->nopextrassg, state->knockoutidref, state->nopstrokebehind, state->strokebehind, state->behindextrassg, state->nopdsbehind, state->dsbehind, state->xor, state->repairgeglgraph, state->output, NULL);
      /* Nodes relating to color overlay */
      gegl_node_link_many (state->mcol, state->coloropacity, NULL);
      gegl_node_connect_from (multiply, "aux", state->coloropacity, "output");
      /* Nodes relating to knockout text */
      gegl_node_link_many (state->knockoutidref, state->opacityinput, NULL);
      gegl_node_connect_from (state->xor, "aux", state->opacityinput, "output");
      /* Nodes relating to bevel of text */
      gegl_node_link_many (state->nopb, state->mbd, state->mbdopacity, NULL);
      gegl_node_connect_from (bevelmode, "aux", state->mbdopacity, "output");
      /* Nodes relating to gradient */
      gegl_node_connect_from (atopg, "aux", state->opacitygradient, "output");
      gegl_node_link_many (state->gradient, state->opacitygradient, NULL);
      /* Nodes relating to image file overlay */
      gegl_node_link_many ( state->image, state->saturation, NULL);
      gegl_node_connect_from (atopi, "aux", state->saturation, "output");
      /* Nodes relating to extra outline shadow glow */
      gegl_node_link_many (state->nopextrassg, state->extrassg, state->extrassgopacity, NULL);
      gegl_node_connect_from (state->behindextrassg, "aux", state->extrassgopacity, "output");
      /* Nodes relating to outline */
      gegl_node_link_many (state->nopstrokebehind, state->stroke, NULL);
      gegl_node_connect_from (state->strokebehind, "aux", state->stroke, "output");
      /* Nodes relating to shadow */
      gegl_node_link_many (state->nopdsbehind, state->ds, NULL);
      gegl_node_connect_from (state->dsbehind, "aux", state->ds, "output");
      /* Nodes relating to Glass Over Text start here */
      gegl_node_link_many (state->nopglass, state->glassovertext, NULL);
      gegl_node_connect_from (state->glassover, "aux", state->glassovertext, "output");



    }
    else
    {
      /* neither gradient nor innerglow */
   gegl_node_link_many (state->input, state->microblur, state->thinbold, state->nopimage, atopi,  multiply, state->crop, state->nopglass,  state->glassover,  state->shiny, state->nopb, bevelmode, state->nopextrassg, state->knockoutidref, state->nopstrokebehind, state->strokebehind, state->behindextrassg, state->nopdsbehind, state->dsbehind, state->xor, state->repairgeglgraph, state->output, NULL);
      /* Nodes relating to color overlay */
      gegl_node_link_many (state->mcol, state->coloropacity, NULL);
      gegl_node_connect_from (multiply, "aux", state->coloropacity, "output");
      /* Nodes relating to knockout text */
      gegl_node_link_many (state->knockoutidref, state->opacityinput, NULL);
      gegl_node_connect_from (state->xor, "aux", state->opacityinput, "output");
      /* Nodes relating to bevel of text */
      gegl_node_link_many (state->nopb, state->mbd, state->mbdopacity, NULL);
      gegl_node_connect_from (bevelmode, "aux", state->mbdopacity, "output");
      /* Nodes relating to image file overlay */
      gegl_node_link_many ( state->image, state->saturation, NULL);
      gegl_node_connect_from (atopi, "aux", state->saturation, "output");
      /* Nodes relating to extra outline shadow glow */
      gegl_node_link_many (state->nopextrassg, state->extrassg, state->extrassgopacity, NULL);
      gegl_node_connect_from (state->behindextrassg, "aux", state->extrassgopacity, "output");
      /* Nodes relating to outline */
      gegl_node_link_many (state->nopstrokebehind, state->stroke, NULL);
      gegl_node_connect_from (state->strokebehind, "aux", state->stroke, "output");
      /* Nodes relating to shadow */
      gegl_node_link_many (state->nopdsbehind, state->ds, NULL);
      gegl_node_connect_from (state->dsbehind, "aux", state->ds, "output");
      /* Nodes relating to Glass Over Text start here */
      gegl_node_link_many (state->nopglass, state->glassovertext, NULL);
      gegl_node_connect_from (state->glassover, "aux", state->glassovertext, "output");

    }
  }
}

 /*End of Literal GEGL Graph instructions.*/


 /*This above, (the checkboxes) is the one part of GEGL Plugin development that is complicated. As each checkbox has its if and else. Beaver wants to find a way to make this simpler. Credit goes to BarefootLiam for helping set up the checkboxes and even Pippin for explaining to Liam how to make checkboxes that reboot a GEGL Graph.*/


 /*Third full Node listing. All Nodes must be listed here, and can be listed in any order. These nodes are not organized.*/

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglNode *input, *output, *image, *mbd, *mbdopacity, *nopig, *multiplyb, *nopm, *over, *multiply, *grainextract, *hslcolorig, *overlayig, *softlightig, *screenig, *linearlightig, *multiplyig, *grainmerge, *grainmergeig, *addition, *subtract,  *nopb, *mcol, *stroke, *innerglow, *gradient, *crop, *ds,  *nopimage, *atopi, *nopg, *atopg,  *hslcolorg, *overlayg, *additiong, *softlightg, *screeng, *multiplyg, *hsvhueg, *linearlightg, *grainmergeg, *saturation, *hardlightg, *hardlightig, *burnig, *burng, *lchcolorg, *lchcolorig, *shinygm, *colordodge, *hardlight, *screen, *shiny, *microblur, *thinbold, *opacityinput, *xor, *knockoutidref, *beveloff, *extrassg, *nopextrassg, *behindextrassg, *extrassgopacity, *grainmergeimage, *additionimage, *overlayimage, *multiplyimage, *screenimage, *hslcolorimage, *softlightimage, *linearlightimage, *hardlightimage, *lchcolorimage, *hsvhueimage, *grainmergecolor, *additioncolor, *overlaycolor,  *screencolor, *hslcolorcolor, *softlightcolor, *linearlightcolor, *hardlightcolor, *lchcolorcolor, *hsvhuecolor, *nocolor, *solidcolor, *coloropacity, *strokebehind, *nopstrokebehind, *nopdsbehind, *dsbehind, *opacitygradient, *cropcolor, *glassovertext, *glassover, *nopglass, *repairgeglgraph, *hueig;


  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");

  
/* All nodes relating to outline begin here.

  zzstrokebevelimage is a hidden operation; recreation of gimp's drop shadow filter with heavy modification so it can do things like bevel outlines, aura seeds and image file overlays.
gegl effects use to use the default drop shadow until Beaver realized that the only way to give GEGL Effects styles seen in Adobe's layer effects was to abandon gegl:dropshadow and
make a enhanced version of it. */
  stroke = gegl_node_new_child (gegl,
                                  "operation", "gegl:zzstrokebevelimage",
                                  NULL);

strokebehind = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 2, "composite-mode", 0, NULL);


  nopstrokebehind = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

/* All nodes relating to outline end here. */



/* All nodes relating to shadow begin here. */


  /* zzstrokebevelimage is a hidden operation; recreation of gimp's drop shadow filter with heavy modification so it can do things like bevel outlines, aura seeds and image file overlays.
gegl effects use to use the default drop shadow until Beaver realized that the only way to give GEGL Effects styles seen in Adobe's layer effects was to abandon gegl:dropshadow and
make a enhanced version of it. */
  ds = gegl_node_new_child (gegl,
                                  "operation", "gegl:zzstrokebevelimage",
                                  NULL);

dsbehind = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 2, "composite-mode", 0, NULL);

  /* This nop acts as a ref and id for the placement of zzstrokebevelimage in the gegl graph. */
  nopdsbehind = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);


  /* All nodes relating to knock out text begin here. */

xor = gegl_node_new_child (gegl,
                                    "operation", "gimp:layer-mode", "layer-mode", 60,  "composite-mode", 0, "blend-space", 0, "composite-space", 0, NULL);

  /* This nop acts as a ref and id for the placement of translucent text in the gegl graph. */
  knockoutidref = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);
  opacityinput = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

  /* All nodes relating to knock out text end here. */



  /*All nodes relating to color overlay start here*/


  /*mcol is the color overlay. It originally stood for multiply color before it got blend mode switchers*/
  mcol = gegl_node_new_child (gegl,
                                  "operation", "gegl:color",
                                  NULL);

  /*The nop functions as a ref and ID for color overlay's placement in a GEGL Graph*/
  nopm = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);


  /*gegl:crop's are applied after gegl:color or anything gegl renders to prevent a known bug. */
  cropcolor = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop",
                                  NULL);

  /*This operation works but is hidden */
  coloropacity = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);



/* Blend modes for color overlay*/

  /*multiply is the blend mode that will need to be switchable for gegl:color-overlay*/
multiply = gegl_node_new_child (gegl,
                                    "operation", "gimp:layer-mode", "layer-mode", 30,  "composite-mode", 2, NULL);


  nocolor = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst",
                                  NULL);

solidcolor = gegl_node_new_child (gegl,
                                    "operation", "gimp:layer-mode", "layer-mode", 28,  "composite-mode", 2, NULL);

grainmergecolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 47, "composite-mode", 0, "blend-space", 3, NULL);

overlaycolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 23, "composite-mode", 0, NULL);

screencolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 31, "composite-mode", 0, NULL);

hslcolorcolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 39, "composite-mode", 0, NULL);

softlightcolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 45, "composite-mode", 0, NULL);

linearlightcolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 50, "composite-mode", 0, "blend-space", 3, NULL);

hardlightcolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 44, "composite-mode", 0, NULL);

lchcolorcolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 26, "composite-mode", 0, NULL);

hsvhuecolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 37, "composite-mode", 0, NULL);

additioncolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 33, "composite-mode", 0, NULL);

/* All nodes relating to color overlay end here*/



/* All nodes relating to the bevel start here*/

  mbd = gegl_node_new_child (gegl,
                                  "operation", "gegl:bevel",
                                  NULL);

  mbdopacity = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

  /*The nop functions as a ref and ID for bevel's placement in a GEGL Graph*/
  nopb = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

/* blend modes for bevel*/

multiplyb = gegl_node_new_child (gegl,
                                    "operation", "gimp:layer-mode", "layer-mode", 30,  "composite-mode", 0, NULL);


grainextract = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 46, "composite-mode", 0, NULL);


grainmerge = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 47, "composite-mode", 0, NULL);

subtract = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 34, "composite-mode", 0, NULL);

shinygm = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 47, "composite-space", 2, "composite-mode", 0, "blend-space", 2, NULL);

colordodge = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 42, "composite-mode", 0, "composite-space", 1, "blend-space", 1, NULL);

hardlight = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 44, "composite-mode", 0, "composite-space", 1, "blend-space", 0, NULL);

screen = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 31, "composite-mode", 0, "composite-space", 0, "blend-space", 0, NULL);

/* DST doesn't disable bevel it just makes it invisible*/
  beveloff = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst",
                                  NULL);

/* All nodes relating to bevel end here*/

/* All nodes relating to Inner Glow start here*/


  innerglow = gegl_node_new_child (gegl,
                                  "operation", "gegl:innerglow",
                                  NULL);

  /*The nop functions as a ref and ID for inner glow's placement in a GEGL Graph*/
  nopig = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);


/* Blend modes for Inner Glow are here*/

  /*over is the switch for for gegl:innerglow's blend mode switcher*/
  over = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);

grainmergeig = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 47, "composite-mode", 0, "blend-space", 3, NULL);

addition = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 33, "composite-mode", 0, NULL);

overlayig = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 23, "composite-mode", 0, NULL);

multiplyig = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 30, "composite-mode", 0, NULL);

screenig = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 31, "composite-mode", 0, NULL);

hslcolorig = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 39, "composite-mode", 0, NULL);

softlightig = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 45, "composite-mode", 0, NULL);

linearlightig = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 50, "composite-mode", 0, "blend-space", 3, NULL);

burnig = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 43, "composite-mode", 0, NULL);

hueig = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 37, "composite-mode", 0, NULL);

hardlightig = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 44, "composite-mode", 0, NULL);

lchcolorig = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 26, "composite-mode", 0, "blend-space", 2, NULL);

/* All nodes relating to Inner Glow end here*/

/* All nodes relating to Gradient's start here*/


  gradient = gegl_node_new_child (gegl,
                                  "operation", "gegl:linear-gradient",
                                  NULL);

  opacitygradient = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);


  /*To prevent a known bug gegl:crop's are used after anything gegl renders; such as a linear gradient*/
  crop = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop",
                                  NULL);



  /*The nop functions as a ref and ID for gradient's placement in a GEGL Graph*/
  nopg = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);


/* Blend modes below are for gradient*/

  /*atopg is the blend mode that is default but set to have switchable blend modes for gegl:linear-gradient*/

  atopg = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);

grainmergeg = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 47, "composite-mode", 0, "blend-space", 3, NULL);

additiong = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 33, "composite-mode", 0, NULL);

overlayg = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 23, "composite-mode", 0, NULL);

multiplyg = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 30, "composite-mode", 0, NULL);

screeng = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 31, "composite-mode", 0, NULL);

hslcolorg = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 39, "composite-mode", 0, NULL);

softlightg = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 45, "composite-mode", 0, NULL);

hsvhueg = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 37, "composite-mode", 0, NULL);

linearlightg = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 50, "composite-mode", 0, "blend-space", 3, NULL);

hardlightg = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 44, "composite-mode", 0, NULL);

burng = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 43, "composite-mode", 0, NULL);

lchcolorg = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 26, "composite-mode", 0, NULL);

/* All nodes relating to Gradient's end here*/


/* All nodes relating to Extra Outline, Shadw Glow begin here*/

  
  /* zzstrokebevelimage is a hidden operation; recreation of gimp's drop shadow filter with heavy modification so it can do things like bevel outlines, aura seeds and image file overlays.
gegl effects use to use the default drop shadow until Beaver realized that the only way to give GEGL Effects styles seen in Adobe's layer effects was to abandon gegl:dropshadow and
make a enhanced version of it. */
extrassg = gegl_node_new_child (gegl,
                                  "operation", "gegl:zzstrokebevelimage",
                                  NULL);

behindextrassg = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 2, "composite-mode", 0, NULL);

/*This nop functions as a ref and id for extra stroke shadow glows placement*/
  nopextrassg = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

/*This nop was once a function that was disabled. zzstrokebevelimags opacity caused problems for a technical reason so I had to make an extra opacity meter*/
  extrassgopacity = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

/* All nodes relating to Extra Outline, Shadw Glow end here*/

/* All nodes relating to image file overlay begin here*/

  image = gegl_node_new_child (gegl,
                                  "operation", "gegl:layer",
                                  NULL);

  /*The nop functions as a ref and ID for image file overlay's placement in a GEGL Graph*/
  nopimage = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  saturation = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);

/* Blend modes below are alternative blend modes for image file overlay.*/

  /*atopi is the blend mode that will need to be switchable for gegl:layer src=*/
  atopi = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);

grainmergeimage = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 47, "composite-mode", 0, "blend-space", 3, NULL);

overlayimage = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 23, "composite-mode", 0, NULL);

multiplyimage = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 30, "composite-mode", 0, NULL);

screenimage = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 31, "composite-mode", 0, NULL);

hslcolorimage = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 39, "composite-mode", 0, NULL);

softlightimage = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 45, "composite-mode", 0, NULL);

linearlightimage = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 50, "composite-mode", 0, "blend-space", 3, NULL);

hardlightimage = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 44, "composite-mode", 0, NULL);

lchcolorimage = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 26, "composite-mode", 0, NULL);

hsvhueimage = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 37, "composite-mode", 0, NULL);

additionimage = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 33, "composite-mode", 0, NULL);


/* All nodes for image file overlay end here*/

  /*Misc nodes (shiny text, micro blur and thin to thick text) that have no other nodes to go with them go here  */
  thinbold = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur",
                                  NULL);

  /*This is the hidden operation shiny text. Which is literally gegl:sinus on a specific setting, with a blend mode switcher and checkbox. */
  shiny = gegl_node_new_child (gegl,
                                  "operation", "gegl:shinytext",
                                  NULL);

  microblur = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
                                  NULL);


  repairgeglgraph      = gegl_node_new_child (gegl, "operation", "gegl:median-blur",
                                         "radius",       0,
                                         NULL);

 /*Repair GEGL Graph is a critical operation for Gimp's non-destructive future.
A median blur at zero radius is confirmed to make no changes to an image. 
This option resets gegl:opacity's value to prevent a known bug where
plugins like clay, glossy balloon and custom bevel glitch out when
drop shadow is applied in a gegl graph below them.*/
 
 
 

  /*Misc nodes (shiny text, micro blur and thin to thick text) that have no other nodes to go with them end here  */

  /* All nodes relating to glass on text begin here */
  glassovertext = gegl_node_new_child (gegl,
                                  "operation", "gegl:glassovertext",
                                  NULL);


  nopglass = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  /* Glass Over Text blend modes */
  glassover = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);



 /* All nodes relating to glass on text end here */

  /*Fourth full node listing. This is for GEGL operation associations. This is organize for convience but will work even if completely unorganized*/

  /*Beginning of Shadow's GUI asociations*/
  gegl_operation_meta_redirect (operation, "x", ds, "x");
  gegl_operation_meta_redirect (operation, "y", ds, "y");
  gegl_operation_meta_redirect (operation, "seedaura", ds, "seed");
  gegl_operation_meta_redirect (operation, "aurasize", ds, "tile-size");
  gegl_operation_meta_redirect (operation, "enableaura", ds, "enableaura");
  gegl_operation_meta_redirect (operation, "opacity", ds, "opacity");
  gegl_operation_meta_redirect (operation, "grow_radius", ds, "stroke");
  gegl_operation_meta_redirect (operation, "radius", ds, "blurstroke");
  gegl_operation_meta_redirect (operation, "color", ds, "color");
  gegl_operation_meta_redirect (operation, "enableshadow", ds, "enableoutline"); 
  gegl_operation_meta_redirect (operation, "shadowimage", ds, "src");
  gegl_operation_meta_redirect (operation, "specialshadowoptions", ds, "specialoutline"); 
  gegl_operation_meta_redirect (operation, "blurshadowimage", ds, "blurshadowimage");
  /*End of Shadow's GUI asociations*/

  /*Beginning of Outline's GUI asociations*/
  gegl_operation_meta_redirect (operation, "ystroke", stroke, "y");
  gegl_operation_meta_redirect (operation, "xstroke", stroke, "x");
  gegl_operation_meta_redirect (operation, "grow_shape", stroke, "grow-shape");
  gegl_operation_meta_redirect (operation, "opacitystroke", stroke, "opacity");
  gegl_operation_meta_redirect (operation, "radiusstroke", stroke, "blurstroke");
  gegl_operation_meta_redirect (operation, "grow_radiusstroke", stroke, "stroke");
  gegl_operation_meta_redirect (operation, "osdepth", stroke, "bevel2");
  gegl_operation_meta_redirect (operation, "osradius", stroke, "radius1");
  gegl_operation_meta_redirect (operation, "osbevel", stroke, "bevel1");
  gegl_operation_meta_redirect (operation, "osbevelopacity", stroke, "opacitybevel");
  gegl_operation_meta_redirect (operation, "osth", stroke, "th");
  gegl_operation_meta_redirect (operation, "ossrc", stroke, "src");
  gegl_operation_meta_redirect (operation, "specialoutline", stroke, "specialoutline");
  gegl_operation_meta_redirect (operation, "oshue", stroke, "hue");
  gegl_operation_meta_redirect (operation, "oslightness", stroke, "lightness");
  gegl_operation_meta_redirect (operation, "enableoutline", stroke, "enableoutline");
  gegl_operation_meta_redirect (operation, "colorstroke", stroke, "color");  
  gegl_operation_meta_redirect (operation, "blendmodebeveloutline", stroke, "blendmodebeveloutline"); 
  /*End of Outline's GUI asociations*/

  /*Beginning of Bevel's GUI asociations*/
  gegl_operation_meta_redirect (operation, "depth", mbd, "bevel2");
  gegl_operation_meta_redirect (operation, "radius1", mbd, "radius1");
  gegl_operation_meta_redirect (operation, "bevel1", mbd, "bevel1");
  gegl_operation_meta_redirect (operation, "slideupblack", mbd, "slideupblack");
  gegl_operation_meta_redirect (operation, "bevelopacity", mbdopacity, "value");
  gegl_operation_meta_redirect (operation, "th", mbd, "th");
  /*End of Bevel's GUI asociations*/

  /*Beginning of Image file Overlay's GUI asociations*/
  gegl_operation_meta_redirect (operation, "string", image, "string");
  gegl_operation_meta_redirect (operation, "layeropacity", image, "opacity");
  gegl_operation_meta_redirect (operation, "hue", saturation, "hue");
  gegl_operation_meta_redirect (operation, "chroma", saturation, "chroma");
  gegl_operation_meta_redirect (operation, "lightness", saturation, "lightness");
  gegl_operation_meta_redirect (operation, "src", image, "src");
  /*End of of Image file Overlay's GUI asociations*/

  /*Beginning of Inner Glow's GUI asociations*/
  gegl_operation_meta_redirect (operation, "innerggrow_radius", innerglow, "grow-radius");
  gegl_operation_meta_redirect (operation, "innergradius", innerglow, "radius");
  gegl_operation_meta_redirect (operation, "innergopacity", innerglow, "opacity");
  gegl_operation_meta_redirect (operation, "innergvalue", innerglow, "value2");
  gegl_operation_meta_redirect (operation, "fixoutline", innerglow, "fixoutline");
  gegl_operation_meta_redirect (operation, "xinnerglow", innerglow, "x");
  gegl_operation_meta_redirect (operation, "yinnerglow", innerglow, "y");
  /*End of Inner Glow's GUI asociations*/
  

  /*Beginning of Color Overlay's GUI asociations*/
  gegl_operation_meta_redirect (operation, "optioncolor", mcol, "value");
  gegl_operation_meta_redirect (operation, "coloropacity", coloropacity, "value");
  /*End of Color Overlay's GUI asociations*/

  /*Beginning of Gradient's GUI asociations*/
  gegl_operation_meta_redirect (operation, "start_x", gradient, "start-x");
  gegl_operation_meta_redirect (operation, "start_y", gradient, "start-y");
  gegl_operation_meta_redirect (operation, "end_x", gradient, "end-x");
  gegl_operation_meta_redirect (operation, "end_y", gradient, "end-y");
  gegl_operation_meta_redirect (operation, "start_color", gradient, "start-color");
  gegl_operation_meta_redirect (operation, "end_color", gradient, "end-color");
  gegl_operation_meta_redirect (operation, "opacitygradient", opacitygradient, "value"); 
  /*End of Gradient's GUI asociations*/

  /*Beginning of Shiny Text's GUI asociations*/
  gegl_operation_meta_redirect (operation, "blend_power", shiny, "blend-power");  
  gegl_operation_meta_redirect (operation, "opacityshine", shiny, "opacity");  
  gegl_operation_meta_redirect (operation, "blendmodeshine", shiny, "blendmode");  
  gegl_operation_meta_redirect (operation, "seedshine", shiny, "seed"); 
  gegl_operation_meta_redirect (operation, "complexity", shiny, "complexity"); 
  gegl_operation_meta_redirect (operation, "x_scaleshine", shiny, "x-scale"); 
  gegl_operation_meta_redirect (operation, "y_scaleshine", shiny, "y-scale"); 
  gegl_operation_meta_redirect (operation, "enableshine", shiny, "enable"); 
  /*End of Shiny Text's GUI asociations*/

  /*Beginning of Micro Blur's GUI asociations*/  
  gegl_operation_meta_redirect (operation, "microblur", microblur, "std-dev-x"); 
  gegl_operation_meta_redirect (operation, "microblur", microblur, "std-dev-y"); 
  /*End of Micro Blur's GUI asociations*/  

  /*Beginning of thin to thick text GUI asociations*/
  gegl_operation_meta_redirect (operation, "thinboldenable", thinbold, "radius"); 
  gegl_operation_meta_redirect (operation, "thinboldap", thinbold, "alpha-percentile");   
  /*End of thin to thick text GUI asociations*/  

  /*Beginning of Extra Outline Shadow Glow's GUI asociations*/
  gegl_operation_meta_redirect (operation, "enableextraoutline", extrassg, "enableoutline"); 
  gegl_operation_meta_redirect (operation, "yextraoutline", extrassg, "y");
  gegl_operation_meta_redirect (operation, "xextraoutline", extrassg, "x");
  gegl_operation_meta_redirect (operation, "opacitystrokeextraoutline", extrassg, "opacity");
  gegl_operation_meta_redirect (operation, "radiusstrokeextraoutline", extrassg, "blurstroke");
  gegl_operation_meta_redirect (operation, "grow_radiusstrokeextraoutline", extrassg, "stroke");
  gegl_operation_meta_redirect (operation, "colorstrokeextraoutline", extrassg, "color");
  gegl_operation_meta_redirect (operation, "grow_shapeextraoutline", extrassg, "grow-shape");
  gegl_operation_meta_redirect (operation, "specialoutlineextra", extrassg, "specialoutline");
  gegl_operation_meta_redirect (operation, "ossrcextra", extrassg, "src");
  gegl_operation_meta_redirect (operation, "blendmodebeveloutlineextra", extrassg, "blendmodebeveloutline");  
  gegl_operation_meta_redirect (operation, "osbevelopacityextra", extrassg, "opacitybevel");
  gegl_operation_meta_redirect (operation, "osdepthextra", extrassg, "bevel2");
  gegl_operation_meta_redirect (operation, "osradiusextra", extrassg, "radius1");
  gegl_operation_meta_redirect (operation, "osbevelextra", extrassg, "bevel1");
  gegl_operation_meta_redirect (operation, "extrassgopacity", extrassgopacity, "value");
  /*End of Extra Outline Shadow Glow's GUI asociations*/

  /*Beginning of Knock Out Text's GUI asociations*/
  gegl_operation_meta_redirect (operation, "knockout", opacityinput, "value"); 
  /*End of Knock Out Text's GUI asociations*/


  /*Beginning of Glass Over Text's GUI asociations*/
  gegl_operation_meta_redirect (operation, "enableglasstext", glassovertext, "enableglasstext"); 
  gegl_operation_meta_redirect (operation, "glassblendmode", glassovertext, "glassover"); 
  gegl_operation_meta_redirect (operation, "glassazimuth", glassovertext, "azimuth"); 
  gegl_operation_meta_redirect (operation, "glassdepth", glassovertext, "depth"); 
  gegl_operation_meta_redirect (operation, "glasselevation", glassovertext, "elevation"); 
  gegl_operation_meta_redirect (operation, "glassretract", glassovertext, "std-dev"); 
  gegl_operation_meta_redirect (operation, "glassblur", glassovertext, "blur"); 
  gegl_operation_meta_redirect (operation, "glasscolor", glassovertext, "color"); 
  gegl_operation_meta_redirect (operation, "glassopacity", glassovertext, "hyperopacity");
  /*Beginning of End of Glass Over Text's GUI asociations*/


  /* Now save points to the various gegl nodes so we can rewire them in
   * update_graph() later
   */
  State *state = g_malloc0 (sizeof (State));
  o->user_data = state;

  /*Fifth and final full node listing. This is for special GEGL graphs that have blend mode switchers. This full node listing is organized but it will work even if completely unorganized.*/
  state->input = input;

  /*All Nodes relating to Inner Glow start here*/
  state->innerglow = innerglow;
  state->over = over;
  state->nopig = nopig;
  state->hardlightig = hardlightig;
  state->hardlightg = hardlightg;
  state->atopi = atopi;
  state->burnig = burnig;
  state->overlayig = overlayig;
  state->screenig = screenig;
  state->multiplyig = multiplyig;
  state->softlightig = softlightig;
  state->hslcolorig = hslcolorig;
  state->grainmergeig = grainmergeig;
  state->linearlightig = linearlightig;
  state->lchcolorig = lchcolorig;
  state->hueig = hueig;
  /*All Nodes relating to Inner Glow end here*/

  /*All Nodes relating to Bevel start here*/
  state->multiplyb = multiplyb;
  state->colordodge = colordodge;
  state->hardlight = hardlight;
  state->screen = screen;
  state->beveloff = beveloff;
  state->mbd = mbd;
  state->multiply = multiply;
  state->addition = addition;
  state->subtract = subtract;
  state->grainmerge = grainmerge;
  state->grainextract = grainextract;
  state->shinygm = shinygm;
  state->mbdopacity = mbdopacity;
  state->nopb = nopb;
  /*All Nodes relating to Bevel end here*/

  /*All Nodes relating to Gradient start here*/
  state->atopg = atopg;
  state->nopg = nopg;
  state->gradient = gradient;
  state->opacitygradient = opacitygradient;
  state->grainmergeg = grainmergeg;
  state->linearlightg = linearlightg;
  state->overlayg = overlayg;
  state->screeng = screeng;
  state->multiplyg = multiplyg;
  state->softlightg = softlightg;
  state->hslcolorg = hslcolorg; 
  state->hsvhueg = hsvhueg;
  state->additiong = additiong;
  state->burng = burng;
  state->lchcolorg = lchcolorg;
  state->crop = crop;
  /*All Nodes relating to Gradient end here*/

  /*All Nodes relating to Image File Overlay start here*/
  state->saturation = saturation;
  state->nopimage = nopimage;
  state->image = image;
  state->grainmergeimage = grainmergeimage;
  state->additionimage = additionimage;
  state->overlayimage = overlayimage;
  state->multiplyimage = multiplyimage;
  state->screenimage = screenimage;
  state->hslcolorimage = hslcolorimage;
  state->softlightimage = softlightimage;
  state->linearlightimage = linearlightimage;
  state->hardlightimage = hardlightimage;
  state->lchcolorimage = lchcolorimage;
  state->hslcolorimage = hslcolorimage;
  state->hsvhueimage = hsvhueimage;
  /*All Nodes relating to Image File Overlay end here*/

  /*All Nodes relating to Color Overlay start here*/
  state->nopm = nopm;
  state->mcol = mcol;
  state->grainmergecolor = grainmergecolor;
  state->additioncolor = additioncolor;
  state->overlaycolor = overlaycolor;
  state->screencolor = screencolor;
  state->hslcolorcolor = hslcolorcolor;
  state->softlightcolor = softlightcolor;
  state->linearlightcolor = linearlightcolor;
  state->hardlightcolor = hardlightcolor;
  state->lchcolorcolor = lchcolorcolor;
  state->hslcolorcolor = hslcolorcolor;
  state->hsvhuecolor = hsvhuecolor;
  state->nocolor = nocolor;
  state->solidcolor = solidcolor;
  state->coloropacity = coloropacity;
  state->cropcolor = cropcolor;
  /*All Nodes relating to Color Overlay end here*/

  /*All Nodes relating to Outline start here*/
  state->stroke = stroke;
  state->nopstrokebehind = nopstrokebehind;
  state->strokebehind = strokebehind;
  /*All Nodes relating to Outline end here*/

  /*All Nodes relating to shadow start here*/
  state->ds = ds;
  state->dsbehind = dsbehind;
  state->nopdsbehind = nopdsbehind;
  /*All Nodes relating to shadow end here*/


  /*All Nodes relating extra outline shadow glow start here*/
  state->extrassg = extrassg;
  state->behindextrassg = behindextrassg;
  state->nopextrassg = nopextrassg;
  state->extrassgopacity = extrassgopacity;
  /*All Nodes relating extra outline shadow glow end here*/

  /*All Nodes relating knock out text start here*/
  state->xor = xor;
  state->knockoutidref = knockoutidref;
  state->opacityinput = opacityinput;
  /*All Nodes relating to knock out text end here*/

  /*All Nodes with no relationships (shiny text, micro blur, thin to thick slider) start here*/
  state->shiny = shiny;
  state->microblur = microblur;
  state->thinbold = thinbold;
  state->repairgeglgraph = repairgeglgraph;
  /*All Nodes with no relationships (shiny text, micro blur, thin to thick slider) end here*/

  /*All Nodes relating to glass text start here*/
  state->glassovertext = glassovertext;
  state->glassover = glassover;
  state->nopglass = nopglass;
  /*All Nodes relating to glass text end here*/

  state->output = output;
}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);
  GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);

  operation_class->attach = attach;
  operation_meta_class->update = update_graph;

  gegl_operation_class_set_keys (operation_class,
    "name",        "gegl:layereffectscontinual",
    "title",       _("GEGL Effects Continual Version"),
    "categories",  "Generic",
    "reference-hash", "continual45ed565h8500fca01b2ac",
    "description", _("GEGL text styling and speciality image outlining filter. June 10th 2023 Stable Build"
                     ""),
    NULL);
}

#endif
