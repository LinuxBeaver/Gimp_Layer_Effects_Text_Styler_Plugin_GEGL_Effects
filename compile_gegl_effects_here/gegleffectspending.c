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

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES


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
enum_end (GeglBlendModeTypeigzz)

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


enum_start (guichangeenumzz)
enum_value   (BEAVER_UI_STROKESHADOW, "strokeshadow", N_("Color Fill, Outline and Shadow"))
enum_value   (BEAVER_UI_INNERGLOWBEVEL, "innerglowbevel", N_("Bevel and Inner Glow"))
enum_value   (BEAVER_UI_IMAGEGRADIENT, "imagegradient", N_("Image file upload and Gradient"))
enum_value   (BEAVER_UI_OUTLINESPECIAL, "outlinespecial", N_("Special Options for Outline and Shadow"))
enum_value   (BEAVER_UI_MISCOPTIONS, "miscoptions", N_("Miscellaneous Text Effects"))
enum_value   (BEAVER_UI_EXTRAOSG, "extraosg", N_("Add a extra (second) Outline Shadow Glow"))
  enum_end (guiendzz)



property_enum(guichange, _("Part of filter to be displayed"),
    guiendzz, guichangeenumzz,
    BEAVER_UI_STROKESHADOW)
  description(_("Change the part of the GUI being displayed"))


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


property_color  (optioncolor, _("Text color change (default only works on white text.)"), "#ffffff")
    description (_("Default fuctions like a color overlay on the multiply blend mode. Making the color white will make it transparent. If applied to white text it will become any color you choose. Changing the blend mode to solid will make it a normal color fill, and the other blend modes will do their thing respectively if you are using an image.   "))
  ui_meta ("visible", "guichange {strokeshadow}")


property_enum (colorblendmode, _("Blend Mode of Color Overlay"),
    GeglBlendModeTypegzzcolor, gegl_blend_mode_type_effectsgzzcolor,
    GEGL_BLEND_MODE_TYPE_MULTIPLY)
  ui_meta ("visible", "guichange {strokeshadow}")


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


property_double (xstroke, _("Outline Horizontal Position"), 0.0)
  description   (_("Horizontal outline fill offset"))
  value_range   (-15.0, 15.0)
  ui_steps      (1, 10)
  ui_meta ("visible", "guichange {outlinespecial}")


property_double (slideupblack, _("Black Bevel and Image Bevel mode. "), 0.00)
    description (_("When in use GEGL Bevel works correctly on black Bevels when using blend modes like Grain Merge and Hardlight. All you have to do is select those blend modes for black text and then move this slider up. This same slider can also be used so bevel can apply to image file overlay's while ignoring their content."))
  value_range   (0.00, 0.999)
  ui_steps      (0.01, 0.50)
  ui_meta ("visible", "guichange {innerglowbevel}")




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



property_double (bevelopacity, _("Bevel's Opacity"), 0.999)
  value_range   (0.15, 0.999)
  ui_steps      (0.01, 0.50)
  ui_meta ("visible", "guichange {innerglowbevel}")



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


property_boolean (enableshadow, _("Enable Drop Shadow"), TRUE)
  description    (_("Enable Drop Shadow. This option is hidden via output extent."))
    ui_meta     ("role", "output-extent")
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

property_boolean (enableaura, _("Enable Aura mode for Shadow Glow (Requires Enabling Special Options for Shadow Glow First)"), FALSE)
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


/* Should correspond to GeglMedianBlurNeighborhood in median-blur.c */
enum_start (gegl_stroke_grow_shapeszz)
  enum_value (GEGL_stroke_GROW_SHAPE_SQUARE,  "square",  N_("Square"))
  enum_value (GEGL_stroke_GROW_SHAPE_CIRCLE,  "circle",  N_("Circle"))
  enum_value (GEGL_stroke_GROW_SHAPE_DIAMOND, "diamond", N_("Diamond"))
enum_end (GeglstrokeGrowShapeszz)

/* Should correspond to GeglMedianBlurNeighborhood in median-blur.c */
enum_start (gegl_stroke_grow_shapeszzextra)
  enum_value (GEGL_stroke_GROW_SHAPE_SQUAREextra,  "square",  N_("Square"))
  enum_value (GEGL_stroke_GROW_SHAPE_CIRCLEextra,  "circle",  N_("Circle"))
  enum_value (GEGL_stroke_GROW_SHAPE_DIAMONDextra, "diamond", N_("Diamond"))
enum_end (GeglstrokeGrowShapeszzextra)


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

property_double (opacity, _("Shadow/Glow Opacity --ENABLE SHADOW/GLOW"), 0.0)
  value_range   (0.0, 0.999)
  ui_range      (0.0, 0.999)
  ui_steps      (0.01, 0.10)
  ui_meta ("visible", "guichange {strokeshadow}")

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

property_boolean (gradient, _("Enable Gradient"), FALSE)
  description   (_("Whether to add Gradient overlay"))
ui_meta ("visible", "guichange {imagegradient}")


property_enum (blendmodegradient2, _("Blend Mode of Gradient"),
    GeglBlendModeTypegzz, gegl_blend_mode_type_effectsgzz,
    GEGL_BLEND_MODE_TYPE_OVER)
  ui_meta ("visible", "guichange {imagegradient}")


property_double (start_x, _("Gradient Horizontal1"), 659.0)
    ui_meta("unit", "pixel-coordinate")
    ui_meta("axis", "x")
  ui_steps      (0.01, 0.10)
ui_meta ("visible", "guichange {imagegradient}")


property_double (start_y, _("Gradient Vertical1"), 49.0)
    ui_meta("unit", "pixel-coordinate")
    ui_meta("axis", "y")
  ui_steps      (0.01, 0.10)
ui_meta ("visible", "guichange {imagegradient}")

property_double (end_x, _("Gradient Horizontal2"), 647.0)
    ui_meta("unit", "pixel-coordinate")
    ui_meta("axis", "x")
  ui_steps      (0.01, 0.10)
ui_meta ("visible", "guichange {imagegradient}")

property_double (end_y, _("Gradient Vertical2"), 572.0)
    ui_meta ("unit", "pixel-coordinate")
    ui_meta ("axis", "y")
  ui_steps      (0.01, 0.10)
ui_meta ("visible", "guichange {imagegradient}")

property_color (start_color, _("Gradient Start Color"), "#34ebd6")
    description (_("The color at (x1, y1)"))
ui_meta ("visible", "guichange {imagegradient}")


property_color  (end_color, _("Gradient End Color"), "#fe18f2")
    description (_("The color at (x2, y2)"))
ui_meta ("visible", "guichange {imagegradient}")
 
property_boolean (enableshine, _("Enable/Disable Shiny Text"), FALSE)
  description    (_("Whether text should be shiny"))
ui_meta ("visible", "guichange {miscoptions}")


property_enum (blendmodeshine, _("Blend Mode of Shiny Text"),
    GeglBlendModeTypeshinegeffects, gegl_blend_mode_typeshinegeffects,
    GEGL_BLEND_MODE_TYPE_REPLACESHINE)
ui_meta ("visible", "guichange {miscoptions}")

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


#else

#define GEGL_OP_META
#define GEGL_OP_NAME     gegleffectspending
#define GEGL_OP_C_SOURCE gegleffectspending.c

#include "gegl-op.h"

 /* Typedef Structure for Bevel's blend mode switching '*/

typedef struct
{
  GeglNode *input;
  GeglNode *atopi;
  GeglNode *atopg;
  GeglNode *over;
  GeglNode *crop;
  GeglNode *mbd;
  GeglNode *mcol;
  GeglNode *nopig;
  GeglNode *nopm;
  GeglNode *nopb;
  GeglNode *nopg;
  GeglNode *linearlightig;
  GeglNode *nopimage;
  GeglNode *multiply;
  GeglNode *subtract;
  GeglNode *addition;
  GeglNode *overlayig;
  GeglNode *softlightig;
  GeglNode *hslcolorig;
  GeglNode *screenig;
  GeglNode *multiplyig;
  GeglNode *grainextract;
  GeglNode *grainmerge;
  GeglNode *grainmergeig;
  GeglNode *multiplyb;
  GeglNode *innerglow;
  GeglNode *stroke;
  GeglNode *ds;
  GeglNode *output;
  GeglNode *image;
  GeglNode *gradient;
  GeglNode *overlayg;
  GeglNode *additiong;
  GeglNode *softlightg;
  GeglNode *hslcolorg;
  GeglNode *screeng;
  GeglNode *multiplyg;
  GeglNode *grainmergeg;
  GeglNode *hsvhueg;
  GeglNode *linearlightg;
  GeglNode *saturation;
  GeglNode *hardlightg;
  GeglNode *hardlightig;
  GeglNode *burng;
  GeglNode *burnig;
  GeglNode *lchcolorg;
  GeglNode *lchcolorig;
  GeglNode *shinygm;
  GeglNode *colordodge;
  GeglNode *nopb2;
  GeglNode *hardlight;
  GeglNode *screen;
  GeglNode *beveloff;
  GeglNode *shiny;
  GeglNode *microblur;
  GeglNode *thinbold;
  GeglNode *mbdopacity;
  GeglNode *xor;
  GeglNode *opacityinput;
  GeglNode *knockoutidref;
  GeglNode *nopextrassg;
  GeglNode *behindextrassg;
  GeglNode *extrassg;
  GeglNode *extrassgopacity;
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
  GeglNode *strokebehind;
  GeglNode *dsbehind;
  GeglNode *nopstrokebehind;
  GeglNode *nopdsbehind;
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

  if (!state) return;

  multiply = state->multiply; /* Blend mode switchers for color fill. Multiply is the default. */
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


  bevelmode = state->beveloff; /* the default. Bevel looks bad with most blend modes (outside of Multiply and Grain Merge) */
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

  }

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
 
  if (o->innerglow)
  {
    if (o->gradient)
    {
      /* both innerglow and gradient */
         gegl_node_link_many (state->input, state->thinbold, state->microblur, state->nopimage, atopi,  multiply, state->cropcolor,  state->nopg, atopg,  state->crop, state->shiny, state->nopb, bevelmode, state->nopextrassg, state->knockoutidref,  state->nopig, over, state->nopstrokebehind, state->strokebehind, state->behindextrassg, state->nopdsbehind, state->dsbehind, state->xor, state->output, NULL);
      gegl_node_link_many (state->nopimage, state->image, state->saturation, NULL);
      gegl_node_link_many (state->nopig, state->innerglow, NULL);
      gegl_node_link_many (state->nopb, state->mbd, state->mbdopacity, NULL);
      gegl_node_link_many (state->mcol, state->coloropacity, NULL);
      gegl_node_link_many (state->knockoutidref, state->opacityinput, NULL);
      gegl_node_connect_from (state->xor, "aux", state->opacityinput, "output");
      gegl_node_connect_from (bevelmode, "aux", state->mbdopacity, "output");
      gegl_node_connect_from (multiply, "aux", state->coloropacity, "output");
      gegl_node_connect_from (over, "aux", state->innerglow, "output");
      gegl_node_connect_from (atopg, "aux", state->gradient, "output");
      gegl_node_connect_from (atopi, "aux", state->saturation, "output");
      gegl_node_link_many (state->nopextrassg, state->extrassg, state->extrassgopacity, NULL);
      gegl_node_connect_from (state->behindextrassg, "aux", state->extrassgopacity, "output");
      gegl_node_connect_from (state->strokebehind, "aux", state->stroke, "output");
      gegl_node_link_many (state->nopstrokebehind, state->stroke, NULL);
      gegl_node_connect_from (state->dsbehind, "aux", state->ds, "output");
      gegl_node_link_many (state->nopdsbehind, state->ds, NULL);
    }
    else
    {
      /* innerglow but no gradient */
         gegl_node_link_many (state->input, state->thinbold, state->microblur, state->nopimage, atopi, multiply, state->crop, state->shiny, state->nopb, bevelmode, state->nopextrassg, state->knockoutidref, state->nopig, over, state->nopstrokebehind, state->strokebehind,  state->behindextrassg, state->nopdsbehind, state->dsbehind, state->xor, state->output, NULL);
      gegl_node_link_many (state->nopimage, state->image, state->saturation, NULL);
      gegl_node_link_many (state->nopig, state->innerglow, NULL);
      gegl_node_link_many (state->nopb, state->mbd, state->mbdopacity,  NULL);
      gegl_node_link_many (state->mcol, state->coloropacity, NULL);
      gegl_node_link_many (state->knockoutidref, state->opacityinput, NULL);
      gegl_node_connect_from (state->xor, "aux", state->opacityinput, "output");
      gegl_node_connect_from (bevelmode, "aux", state->mbdopacity, "output");
      gegl_node_connect_from (multiply, "aux", state->coloropacity, "output");
      gegl_node_connect_from (over, "aux", state->innerglow, "output");
      gegl_node_connect_from (atopi, "aux", state->saturation, "output");
      gegl_node_link_many (state->nopextrassg, state->extrassg, state->extrassgopacity, NULL);
      gegl_node_connect_from (state->behindextrassg, "aux", state->extrassgopacity, "output");
      gegl_node_connect_from (state->strokebehind, "aux", state->stroke, "output");
      gegl_node_link_many (state->nopstrokebehind, state->stroke, NULL);
      gegl_node_connect_from (state->dsbehind, "aux", state->ds, "output");
      gegl_node_link_many (state->nopdsbehind, state->ds, NULL);
    }
  }
  else
  {
    if (o->gradient)
    {
      /* gradient but no innerglow */
         gegl_node_link_many (state->input, state->thinbold, state->microblur,  state->nopimage, atopi,   multiply, state->cropcolor, state->nopg, atopg, state->crop, state->shiny, state->nopb, bevelmode, state->nopextrassg, state->knockoutidref, state->nopstrokebehind, state->strokebehind, state->behindextrassg, state->nopdsbehind, state->dsbehind, state->xor, state->output, NULL);
      gegl_node_link_many (state->nopimage, state->image, state->saturation, NULL);
      gegl_node_link_many (state->nopb, state->mbd, state->mbdopacity, NULL);
      gegl_node_link_many (state->mcol, state->coloropacity, NULL);
      gegl_node_link_many (state->knockoutidref, state->opacityinput, NULL);
      gegl_node_connect_from (state->xor, "aux", state->opacityinput, "output");
      gegl_node_connect_from (bevelmode, "aux", state->mbdopacity, "output");
      gegl_node_connect_from (multiply, "aux", state->coloropacity, "output");
      gegl_node_connect_from (atopg, "aux", state->gradient, "output");
      gegl_node_connect_from (atopi, "aux", state->saturation, "output");
      gegl_node_link_many (state->nopextrassg, state->extrassg, state->extrassgopacity, NULL);
      gegl_node_connect_from (state->behindextrassg, "aux", state->extrassgopacity, "output");
      gegl_node_connect_from (state->strokebehind, "aux", state->stroke, "output");
      gegl_node_link_many (state->nopstrokebehind, state->stroke, NULL);
      gegl_node_connect_from (state->dsbehind, "aux", state->ds, "output");
      gegl_node_link_many (state->nopdsbehind, state->ds, NULL);
    }
    else
    {
      /* neither gradient nor innerglow */
   gegl_node_link_many (state->input, state->microblur, state->thinbold, state->nopimage, atopi,  multiply,  state->crop, state->shiny, state->nopb, bevelmode, state->nopextrassg, state->knockoutidref, state->nopstrokebehind, state->strokebehind, state->behindextrassg, state->nopdsbehind, state->dsbehind, state->xor, state->output, NULL);
      gegl_node_link_many (state->nopimage, state->image, state->saturation, NULL);
      gegl_node_link_many (state->nopb, state->mbd, state->mbdopacity, NULL);
      gegl_node_link_many (state->mcol, state->coloropacity, NULL);
      gegl_node_link_many (state->knockoutidref, state->opacityinput, NULL);
      gegl_node_connect_from (state->xor, "aux", state->opacityinput, "output");
      gegl_node_connect_from (bevelmode, "aux", state->mbdopacity, "output");
      gegl_node_connect_from (multiply, "aux", state->coloropacity, "output");
      gegl_node_connect_from (atopi, "aux", state->saturation, "output");
      gegl_node_connect_from (state->strokebehind, "aux", state->stroke, "output");
      gegl_node_link_many (state->nopstrokebehind, state->stroke, NULL);
      gegl_node_connect_from (state->dsbehind, "aux", state->ds, "output");
      gegl_node_link_many (state->nopdsbehind, state->ds, NULL);
      gegl_node_link_many (state->nopextrassg, state->extrassg, state->extrassgopacity, NULL);
      gegl_node_connect_from (state->behindextrassg, "aux", state->extrassgopacity, "output");
    }
  }
}

      /* cropcolor exist to cancel out the infinite plane bug of gegl:color in areas where crop isn't immediately after its blend mode. If the gegl:crop(')s are removed it could a scenario where drop shadows don't have blur puffiness.*/

 /* End of Typedef Structure for Bevel's blend mode switching '*/



 /*Typedef Structure for InnerGlow and Gradient Checkboxes '*/



static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglNode *input, *output, *image, *mbd, *mbdopacity, *nopig, *multiplyb, *nopm, *over, *multiply, *grainextract, *hslcolorig, *overlayig, *softlightig, *screenig, *linearlightig, *multiplyig, *grainmerge, *grainmergeig, *addition, *subtract,  *nopb, *mcol, *stroke, *innerglow, *gradient, *crop, *ds,  *nopimage, *atopi, *nopg, *atopg,  *hslcolorg, *overlayg, *additiong, *softlightg, *screeng, *multiplyg, *hsvhueg, *linearlightg, *grainmergeg, *saturation, *hardlightg, *hardlightig, *burnig, *burng, *lchcolorg, *lchcolorig, *shinygm, *colordodge, *nopb2, *hardlight, *screen, *shiny, *microblur, *thinbold, *opacityinput, *xor, *knockoutidref, *beveloff, *extrassg, *nopextrassg, *behindextrassg, *extrassgopacity, *grainmergeimage, *additionimage, *overlayimage, *multiplyimage, *screenimage, *hslcolorimage, *softlightimage, *linearlightimage, *hardlightimage, *lchcolorimage, *hsvhueimage, *grainmergecolor, *additioncolor, *overlaycolor,  *screencolor, *hslcolorcolor, *softlightcolor, *linearlightcolor, *hardlightcolor, *lchcolorcolor, *hsvhuecolor, *nocolor, *solidcolor, *coloropacity, *strokebehind, *nopstrokebehind, *nopdsbehind, *dsbehind, *cropcolor;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");

  saturation = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);

  knockoutidref = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  coloropacity = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

  /*This is Gimp's split blend mode and a good example of a Gimp exclusive blend mode inside GEGL plugins. Beaver strongly recommends using Gimp only blend modes in GEGL plugins 
even though they will NOT be compatible with other apps. Gimp blend modes have HUGE advantages over native GEGL blend modes.  '*/
xor = gegl_node_new_child (gegl,
                                    "operation", "gimp:layer-mode", "layer-mode", 60,  "composite-mode", 0, "blend-space", 0, "composite-space", 0, NULL);

  opacityinput = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);



  thinbold = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur",
                                  NULL);



  image = gegl_node_new_child (gegl,
                                  "operation", "gegl:layer",
                                  NULL);

  stroke = gegl_node_new_child (gegl,
                                  "operation", "gegl:zzstrokebevelimage",
                                  NULL);

  shiny = gegl_node_new_child (gegl,
                                  "operation", "gegl:shinytext",
                                  NULL);

  ds = gegl_node_new_child (gegl,
                                  "operation", "gegl:zzstrokebevelimage",
                                  NULL);

  innerglow = gegl_node_new_child (gegl,
                                  "operation", "gegl:innerglow",
                                  NULL);

  microblur = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
                                  NULL);


  /*atopi is the blend mode that will need to be switchable for gegl:layer src=*/


  atopi = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);

  /*atopg is the blend mode that will need to be switchable for gegl:linear-gradient*/

  atopg = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);

/*When we finally get around to adding an enum list so blend modes can change*/

  /*atopg = gegl_node_new_child (gegl,
                                 "operation", "gimp:layer-mode", "layer-mode", 23,  "composite-mode", 0, NULL);*/


  mbd = gegl_node_new_child (gegl,
                                  "operation", "gegl:bevel",
                                  NULL);

  mbdopacity = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

  cropcolor = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop",
                                  NULL);



  mcol = gegl_node_new_child (gegl,
                                  "operation", "gegl:color",
                                  NULL);


  nopm = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  nopimage = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  nopb = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  nopb2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  nopig = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  nopg = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);


  /*multiply is the blend mode that will need to be switchable for gegl:color-overlay*/

multiply = gegl_node_new_child (gegl,
                                    "operation", "gimp:layer-mode", "layer-mode", 30,  "composite-mode", 2, NULL);


  /* bevelmode is the blend mode that will need to be switchable for gegl:bevel*/


  gradient = gegl_node_new_child (gegl,
                                  "operation", "gegl:linear-gradient",
                                  NULL);

    /* 0 is Gimp blend mode (Normal) */
    /* 47 is Gimp blend mode (Grain Merge) */
    /* 23 is Gimp blend mode (Overlay) */

  /*over = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 0, "composite-mode", 0, NULL);*/

  /*over is the blend mode that will need to be switchable for gegl:innerglow*/

  over = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);
  crop = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop",
                                  NULL);

/* blend modes for bevel and cannot be reused*/

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

  beveloff = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst",
                                  NULL);


/* blend modes for bevel and cannot be reused- ends here */

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

/* Blend modes below are for gradient*/

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

hardlightig = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 44, "composite-mode", 0, NULL);

hardlightg = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 44, "composite-mode", 0, NULL);

burng = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 43, "composite-mode", 0, NULL);

burnig = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 43, "composite-mode", 0, NULL);

lchcolorg = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 26, "composite-mode", 0, NULL);

lchcolorig = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 26, "composite-mode", 0, NULL);

behindextrassg = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 2, "composite-mode", 0, NULL);

dsbehind = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 2, "composite-mode", 0, NULL);

strokebehind = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 2, "composite-mode", 0, NULL);

  nopdsbehind = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  nopstrokebehind = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);




  extrassg = gegl_node_new_child (gegl,
                                  "operation", "gegl:zzstrokebevelimage",
                                  NULL);

  nopextrassg = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  extrassgopacity = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

/* Blend modes below are alternative blend modes for image file overlay.*/

/* Look for error where multiple addition = gegl_node_new_child (gegl, exist May 29 2023*/

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

/* End of alt blend mode for image file overlay*/

/* Alt Blend modes for color overlay*/

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



/* End of Alt Blend modes for color overlay*/

  gegl_operation_meta_redirect (operation, "seedaura", ds, "seed");
  gegl_operation_meta_redirect (operation, "aurasize", ds, "tile-size");
  gegl_operation_meta_redirect (operation, "enableaura", ds, "enableaura");
  gegl_operation_meta_redirect (operation, "string", image, "string");
  gegl_operation_meta_redirect (operation, "layeropacity", image, "opacity");
  gegl_operation_meta_redirect (operation, "hue", saturation, "hue");
  gegl_operation_meta_redirect (operation, "chroma", saturation, "chroma");
  gegl_operation_meta_redirect (operation, "lightness", saturation, "lightness");
  gegl_operation_meta_redirect (operation, "x", ds, "x");
  gegl_operation_meta_redirect (operation, "y", ds, "y");
  gegl_operation_meta_redirect (operation, "ystroke", stroke, "y");
  gegl_operation_meta_redirect (operation, "xstroke", stroke, "x");
  gegl_operation_meta_redirect (operation, "opacity", ds, "opacity");
  gegl_operation_meta_redirect (operation, "grow_radius", ds, "stroke");
  gegl_operation_meta_redirect (operation, "radius", ds, "blurstroke");
  gegl_operation_meta_redirect (operation, "color", ds, "color");
  gegl_operation_meta_redirect (operation, "specialshadowoptions", ds, "specialoutline"); 
  gegl_operation_meta_redirect (operation, "enableshadow", ds, "enableoutline"); 
  gegl_operation_meta_redirect (operation, "shadowimage", ds, "src");
  gegl_operation_meta_redirect (operation, "blurshadowimage", ds, "blurshadowimage");
  gegl_operation_meta_redirect (operation, "grow_shape", stroke, "grow-shape");
  gegl_operation_meta_redirect (operation, "opacitystroke", stroke, "opacity");
  gegl_operation_meta_redirect (operation, "radiusstroke", stroke, "blurstroke");
  gegl_operation_meta_redirect (operation, "grow_radiusstroke", stroke, "stroke");
  gegl_operation_meta_redirect (operation, "colorstroke", stroke, "color");
  gegl_operation_meta_redirect (operation, "depth", mbd, "bevel2");
  gegl_operation_meta_redirect (operation, "radius1", mbd, "radius1");
  gegl_operation_meta_redirect (operation, "bevel1", mbd, "bevel1");
  gegl_operation_meta_redirect (operation, "bevelopacity", mbdopacity, "value");
  gegl_operation_meta_redirect (operation, "optioncolor", mcol, "value");
  gegl_operation_meta_redirect (operation, "src", image, "src");
  gegl_operation_meta_redirect (operation, "innerggrow_radius", innerglow, "grow-radius");
  gegl_operation_meta_redirect (operation, "innergradius", innerglow, "radius");
  gegl_operation_meta_redirect (operation, "innergopacity", innerglow, "opacity");
  gegl_operation_meta_redirect (operation, "innergvalue", innerglow, "value2");
  gegl_operation_meta_redirect (operation, "fixoutline", innerglow, "fixoutline");
  gegl_operation_meta_redirect (operation, "xinnerglow", innerglow, "x");
  gegl_operation_meta_redirect (operation, "yinnerglow", innerglow, "y");
  gegl_operation_meta_redirect (operation, "start_x", gradient, "start-x");
  gegl_operation_meta_redirect (operation, "start_y", gradient, "start-y");
  gegl_operation_meta_redirect (operation, "end_x", gradient, "end-x");
  gegl_operation_meta_redirect (operation, "end_y", gradient, "end-y");
  gegl_operation_meta_redirect (operation, "start_color", gradient, "start-color");
  gegl_operation_meta_redirect (operation, "end_color", gradient, "end-color");
  gegl_operation_meta_redirect (operation, "th", mbd, "th");
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
  gegl_operation_meta_redirect (operation, "enableextraoutline", extrassg, "enableoutline");  
  gegl_operation_meta_redirect (operation, "blendmodebeveloutline", stroke, "blendmodebeveloutline");  
  gegl_operation_meta_redirect (operation, "blend_power", shiny, "blend-power");  
  gegl_operation_meta_redirect (operation, "opacityshine", shiny, "opacity");  
  gegl_operation_meta_redirect (operation, "blendmodeshine", shiny, "blendmode");  
  gegl_operation_meta_redirect (operation, "seedshine", shiny, "seed"); 
  gegl_operation_meta_redirect (operation, "complexity", shiny, "complexity"); 
  gegl_operation_meta_redirect (operation, "x_scaleshine", shiny, "x-scale"); 
  gegl_operation_meta_redirect (operation, "y_scaleshine", shiny, "y-scale"); 
  gegl_operation_meta_redirect (operation, "enableshine", shiny, "enable"); 
  gegl_operation_meta_redirect (operation, "microblur", microblur, "std-dev-x"); 
  gegl_operation_meta_redirect (operation, "microblur", microblur, "std-dev-y"); 
  gegl_operation_meta_redirect (operation, "thinboldenable", thinbold, "radius"); 
  gegl_operation_meta_redirect (operation, "thinboldap", thinbold, "alpha-percentile"); 
  gegl_operation_meta_redirect (operation, "knockout", opacityinput, "value"); 
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
  gegl_operation_meta_redirect (operation, "coloropacity", coloropacity, "value");
  gegl_operation_meta_redirect (operation, "slideupblack", mbd, "slideupblack");


  /* Now save points to the various gegl nodes so we can rewire them in
   * update_graph() later
   */
  State *state = g_malloc0 (sizeof (State));
  o->user_data = state;

  state->input = input;
  state->atopi = atopi;
  state->overlayig = overlayig;
  state->screenig = screenig;
  state->multiplyig = multiplyig;
  state->softlightig = softlightig;
  state->hslcolorig = hslcolorig;
  state->atopg = atopg;
  state->multiply = multiply;
  state->multiplyb = multiplyb;
  state->addition = addition;
  state->subtract = subtract;
  state->grainmerge = grainmerge;
  state->grainmergeig = grainmergeig;
  state->linearlightig = linearlightig;
  state->grainextract = grainextract;
  state->nopb = nopb;
  state->nopg = nopg;
  state->nopimage = nopimage;
  state->nopm = nopm;
  state->nopig = nopig;
  state->over = over;
  state->crop = crop;
  state->mbd = mbd;
  state->mbdopacity = mbdopacity;
  state->mcol = mcol;
  state->innerglow = innerglow;
  state->stroke = stroke;
  state->ds = ds;
  state->output = output;
  state->image = image;
  state->gradient = gradient;
  state->grainmergeg = grainmergeg;
  state->linearlightg = linearlightg;
  state->overlayg = overlayg;
  state->screeng = screeng;
  state->multiplyg = multiplyg;
  state->softlightg = softlightg;
  state->hslcolorg = hslcolorg; 
  state->hsvhueg = hsvhueg;
  state->additiong = additiong;
  state->saturation = saturation;
  state->hardlightig = hardlightig;
  state->hardlightg = hardlightg;
  state->burng = burng;
  state->burnig = burnig;
  state->lchcolorg = lchcolorg;
  state->lchcolorig = lchcolorig;
  state->shinygm = shinygm;
  state->shiny = shiny;
  state->colordodge = colordodge;
  state->nopb2 = nopb2;
  state->hardlight = hardlight;
  state->screen = screen;
  state->beveloff = beveloff;
  state->microblur = microblur;
  state->thinbold = thinbold;
  state->xor = xor;
  state->knockoutidref = knockoutidref;
  state->opacityinput = opacityinput;
  state->extrassg = extrassg;
  state->behindextrassg = behindextrassg;
  state->nopextrassg = nopextrassg;
  state->extrassgopacity = extrassgopacity;
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
  state->strokebehind = strokebehind;
  state->dsbehind = dsbehind;
  state->nopdsbehind = nopdsbehind;
  state->nopstrokebehind = nopstrokebehind;
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
    "description", _("GEGL text styling and speciality image outlining filter. June 5th 2023 Stable Build"
                     ""),
    NULL);
}

#endif
