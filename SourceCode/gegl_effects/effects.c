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
 *2022-2025 Beaver GEGL Effects  
 *2022 BarefootLiam (for helping give Inner Glow a disable checkbox)

Recreation of Graph 

id=1
#src-atop aux=[ ref=1 layer src=image_link_here.jpg ]
id=2
gimp:layer-mode layer-mode=normal composite-mode=clip-to-backdrop aux=[ ref=2 color-overlay value=#0057ff  ]
id=4 src-atop aux=[ linear-gradient start-x= start-y= end-x= end-y= start-color= end-color=  ]
crop aux=[ ref=4 ]
#id=5 multiply aux=[ ref=5 sinus color1=#ffffff color2=#000000 seed=343  complexity=0.3 ]
#id=6 multiply aux=[ ref=6 lb:bevel bevel1=49 bevel2=93  ]
id=3 over aux=[ ref=3 lb:glassovertext ]
id=7 over aux=[ ref=7 lb:innerglow   ]
dropshadow x=0 y=0 grow-radius=12 radius=1 opacity=1 color=#ff000b
dropshadow

*/

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

enum_start (gegl_blend_mode_type_effectszzbevoutlinege_sally)
  enum_value (GEGL_BLEND_MODE_TYPE_DISABLEBEVELGE,      "nobevel",
              N_("Outline Bevel Off"))
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLYGE,      "multiply",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGEGE,      "grainmerge",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_SHINYGMGE,      "grainmergealt",
              N_("Grain Merge Alt"))
  enum_value (GEGL_BLEND_MODE_TYPE_COLORDODGEGE,      "colordodge",
              N_("Color Dodge"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHTGE,      "hardlight",
              N_("Hard Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_LIGHTENONLYGE,      "lightenonly",
              N_("Lighten Only"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINEXTRACTGE,      "grainextract",
              N_("Grain Extract"))
enum_end (GeglBlendModeTypezzbevoutlinege_sally)

enum_start (gegl_blend_mode_type_effectszzbevoutlinegeextra_sally)
  enum_value (GEGL_BLEND_MODE_TYPE_DISABLEBEVELGEextra,      "nobevel",
              N_("Outline Bevel Off"))
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLYGEextra,      "multiply",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGEGEextra,      "grainmerge",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_SHINYGMGEextra,      "grainmergealt",
              N_("Grain Merge Alt"))
  enum_value (GEGL_BLEND_MODE_TYPE_COLORDODGEGEextra,      "colordodge",
              N_("Color Dodge"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHTGEextra,      "hardlight",
              N_("Hard Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_LIGHTENONLYGEextra,      "lightenonly",
              N_("Lighten Only"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINEXTRACTGEextra,      "grainextract",
              N_("Grain Extract"))
enum_end (GeglBlendModeTypezzbevoutlinegeextra_sally)

/*This is the enum list of blend modes for bevel.*/
enum_start (gegl_blend_mode_type_effectszz_sally)
  enum_value (GEGL_BLEND_MODE_TYPE_BEVELOFF,      "nobevel",
              N_("Bevel off "))
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLY,      "multiply",
              N_("Multiply Bevel"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGE,      "grainmerge",
              N_("Grain Merge Bevel"))
  enum_value (GEGL_BLEND_MODE_TYPE_SHINYGM,      "grainmergealt",
              N_("GrainMergeAlt Bevel"))
  enum_value (GEGL_BLEND_MODE_TYPE_SUBTRACT,      "subtract",
              N_("Subtract Bevel"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINEXTRACT,      "grainextract",
              N_("Grain Extract Bevel"))
  enum_value (GEGL_BLEND_MODE_TYPE_COLORDODGE,      "colordodge",
              N_("Color Dodge Bevel"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHT,      "hardlight",
              N_("Hard Light Bevel"))
  enum_value (GEGL_BLEND_MODE_TYPE_SCREEN,      "screen",
              N_("Screen Bevel"))
  enum_value (GEGL_BLEND_MODE_TYPE_LIGHTENONLY,      "lightenonly",
              N_("Lighten Only Bevel"))
enum_end (GeglBlendModeTypezz_sally)

/*This is the enum list of blend modes for Inner Glow.*/
enum_start (gegl_blend_mode_type_effectsigzz_sally)
  enum_value (GEGL_BLEND_MODE_TYPE_OVER,      "normal",
              N_("Normal"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGEIG,      "grainmerge",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_ADDITION,      "addition",
              N_("Addition"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOFTLIGHTIG,      "softlight",
              N_("Soft Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_SCREENIG,      "screen",
              N_("Screen"))
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLYIG,      "multiply",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_HSLCOLORIG,      "hslcolor",
              N_("HSL Color"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAYIG,      "overlay",
              N_("Overlay"))
  enum_value (GEGL_BLEND_MODE_TYPE_LINEARLIGHTIG,      "linearlight",
              N_("Linear Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHTIG,      "HardLight",
              N_("Hard Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_BURNIG,      "burn",
              N_("Burn"))
  enum_value (GEGL_BLEND_MODE_TYPE_LCHCOLORIG,      "lchcolor",
              N_("LCh Color"))
  enum_value (GEGL_BLEND_MODE_TYPE_HUEIG,      "hsvhue",
              N_("HSV Hue"))
enum_end (GeglBlendModeTypeigzz_sally)

/*This is the enum list of blend modes for Gradient.*/
enum_start (gegl_blend_mode_type_effectsgzz_sally)
  enum_value (GEGL_BLEND_MODE_TYPE_ATOPG,      "normal",
              N_("Normal"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGEG,      "grainmerge",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_ADDITIONG,      "addition",
              N_("Addition"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOFTLIGHTG,      "softlight",
              N_("Softlight"))
  enum_value (GEGL_BLEND_MODE_TYPE_SCREENG,      "screen",
              N_("Screen"))
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLYG,      "multiply",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_HSLCOLORG,      "hslcolor",
              N_("HSLColor"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAYG,      "overlay",
              N_("Overlay"))
  enum_value (GEGL_BLEND_MODE_TYPE_LINEARLIGHTG,      "linearLight",
              N_("Linear Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_HSVHUEG,      "hsvhue",
              N_("HSV Hue"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHTG,      "hardlight",
              N_("Hard Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_BURNG,      "burn",
              N_("Burn"))
  enum_value (GEGL_BLEND_MODE_TYPE_LCHCOLORG,      "lchcolor",
              N_("LCh Color"))
enum_end (GeglBlendModeTypegzz_sally)

/*This is the enum list of blend modes for Image File Overlay.*/
enum_start (gegl_blend_mode_type_effectsgzzimage_sally)
  enum_value (GEGL_BLEND_MODE_TYPE_NORMALIMAGE,      "normal",
              N_("Normal Image"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGEIMAGE,      "grainmerge",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_ADDITIONIMAGE,      "addition",
              N_("Addition"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOFTLIGHTIMAGE,      "softlight",
              N_("Softlight"))
  enum_value (GEGL_BLEND_MODE_TYPE_SCREENIMAGE,      "screen",
              N_("Screen"))
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLYIMAGE,      "multiply",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_HSLCOLORIMAGE,      "hslcolor",
              N_("HSL Color"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAYIMAGE,      "overlay",
              N_("Overlay"))
  enum_value (GEGL_BLEND_MODE_TYPE_LINEARLIGHTIMAGE,      "linearlight",
              N_("Linear Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHTIMAGE,      "hardLight",
              N_("Hard Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_LCHCOLORIMAGE,      "lchcolor",
              N_("LCh Color"))
  enum_value (GEGL_BLEND_MODE_TYPE_HSVHUEIMAGE,      "hsvhue",
              N_("HSV Hue"))
enum_end (GeglBlendModeTypegzzimage_sally)

/*This is the enum list of blend modes for Color Overlay*/
enum_start (gegl_blend_mode_type_effectsgzzcolor_sally)
  enum_value (GEGL_BLEND_MODE_TYPE_NOCOLOR,      "none",
              N_("No Color"))
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLYCOLOR,      "multiply",
              N_("Default Multiply; White is transparent"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOLIDCOLOR,      "solidcolor",
              N_("Solid Color"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGECOLOR,      "grainmerge",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_ADDITIONCOLOR,      "addition",
              N_("Addition"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOFTLIGHTCOLOR,      "softlight",
              N_("Soft Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_SCREENCOLOR,      "screen",
              N_("Screen"))
  enum_value (GEGL_BLEND_MODE_TYPE_HSLCOLORCOLOR,      "hslcolor",
              N_("HSL Color"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAYCOLOR,      "overlay",
              N_("Overlay"))
  enum_value (GEGL_BLEND_MODE_TYPE_LINEARLIGHTCOLOR,      "linearlight",
              N_("Linear Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHTCOLOR,      "hardlight",
              N_("Hard Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_LCHCOLORCOLOR,      "lchcolor",
              N_("LCh Color"))
  enum_value (GEGL_BLEND_MODE_TYPE_HSVHUECOLOR,      "hsvhue",
              N_("HSV Hue"))
enum_end (GeglBlendModeTypegzzcolor_sally)

/* This is the enum list of outline's base shape. Should correspond to GeglMedianBlurNeighborhood in median-blur.c */
enum_start (gegl_stroke_grow_shapeszz_sally)
  enum_value (GEGL_stroke_GROW_SHAPE_SQUARE,  "square",  N_("Square"))
  enum_value (GEGL_stroke_GROW_SHAPE_CIRCLE,  "circle",  N_("Circle"))
  enum_value (GEGL_stroke_GROW_SHAPE_DIAMOND, "diamond", N_("Diamond"))
enum_end (GeglstrokeGrowShapeszz_sally)

/* This is the enum list of extra outline's base shape. Should correspond to GeglMedianBlurNeighborhood in median-blur.c */
enum_start (gegl_stroke_grow_shapeszzextra_sally)
  enum_value (GEGL_stroke_GROW_SHAPE_SQUAREextra,  "square",  N_("Square"))
  enum_value (GEGL_stroke_GROW_SHAPE_CIRCLEextra,  "circle",  N_("Circle"))
  enum_value (GEGL_stroke_GROW_SHAPE_DIAMONDextra, "diamond", N_("Diamond"))
enum_end (GeglstrokeGrowShapeszzextra_sally)

/*This is the enum list of shiny text's blend mode switcher.*/
enum_start (gegl_blend_mode_typeshinegeffects_sally)
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGEALTSHINE,      "grainmergealt",
              N_("Grain Merge Alt"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGESHINE,      "grainmerge",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHTSHINE,      "hardlight",
              N_("Hard Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_ADDITIONSHINE,      "addition",
              N_("Addition"))
  enum_value (GEGL_BLEND_MODE_TYPE_REPLACESHINE, "multiply",
              N_("Multiply"))
enum_end (GeglBlendModeTypeshinegeffects_sally)

/*This is the enum list of glass text's blend mode switcher.*/
enum_start (gegl_blend_mode_type_glass_sally)
  enum_value (GEGL_BLEND_MODE_TYPE_GLASSOVER,      "normal",
              N_("Normal"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGEGLASS,      "grainmerge",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_LINEARLIGHTGLASS,      "linearlight",
              N_("Linear Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_SUBTRACTGLASS,      "subtract",
              N_("Subtract"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAYGLASS, "overlay",
              N_("Overlay"))
enum_end (geglblendmodetypeglass_sally)

/*This is the enum list for bevel's blend mode switcher*/
enum_start (gegl_blend_mode_typebeavbeveleffects_sally)
  enum_value (GEGL_BEVEL_NORMAL,      "normalbevel",
              N_("Bump Bevel"))
  enum_value (GEGL_BEVEL_SHARP,      "sharpbevel",
              N_("Sharp Bevel"))
  enum_value (GEGL_BEVEL_SHARPREVISE,      "sharpbevel2",
              N_("Sharp Bevel Alt"))
  enum_value (GEGL_BEVEL_SHARPREVISEFLATSURFACE,      "sharpbevelflat",
              N_("Sharp Bevel Alt Flat Surface"))
  enum_value (GEGL_BEVEL_SHARPREVISEINSIDE,      "sharpbevelinside",
              N_("Sharp Bevel Alt Inside"))
enum_end (GeglBlendModeTypebeavbeveleffects_sally)


/*This is the enum list for parts of GEGL Effects to be displayed (part of filter to be displayed)*/
enum_start (guichangeenumzz_sally)
enum_value   (BEAVER_UI_STROKESHADOW, "strokeshadow", N_("Color Fill, Outline and Shadow"))
enum_value   (BEAVER_UI_INNERGLOWBEVEL, "innerglowbevel", N_("Bevel and Inner Glow"))
enum_value   (BEAVER_UI_IMAGEGRADIENT, "imagegradient", N_("Fill Image and Gradient"))
enum_value   (BEAVER_UI_OUTLINESPECIAL, "outlinespecial", N_("Special Options for Outline and Shadow"))
enum_value   (BEAVER_UI_EXTRAOSG, "extraosg", N_("Secondary Outline"))
enum_value   (BEAVER_UI_GLASSTEXTSHINE, "glassshine", N_("Glass Text and Shine"))
  enum_end (guiendzz_sally)

/*Below is sliders and other options that are displayed in GEGL Effects GUI. They are effected by the order they are in and the ui_meta association*/

property_enum(guichange, _("Part of filter to be displayed:"),
    guiendzz_sally, guichangeenumzz_sally,
    BEAVER_UI_STROKESHADOW)
  description(_("Change the part of the GUI being displayed"))

/* Beginning of Color Overlay, Outline and Shadow GUI options*/
property_color  (fill_color, _("Text color change (default only works on white text)"), "#ffffff")
    description (_("Default fuctions like a color overlay on the multiply blend mode. Making the color white will make it transparent. If applied to white text it will become any color you choose. Changing the blend mode to solid will make it a normal color fill, and the other blend modes will do their thing respectively if you are using an image.   "))
  ui_meta ("visible", "guichange {strokeshadow}")


property_enum (fill_color_blend_mode, _("Blend mode of color overlay"),
    GeglBlendModeTypegzzcolor_sally, gegl_blend_mode_type_effectsgzzcolor_sally,
    GEGL_BLEND_MODE_TYPE_MULTIPLY)
  ui_meta ("visible", "guichange {strokeshadow}")

property_double(fill_color_opacity, _("Opacity of color overlay"), 1.0)
    value_range (0.0, 1.0)
  ui_steps      (0.01, 0.50)
  ui_meta     ("role", "output-extent")
  ui_meta ("visible", "guichange {strokeshadow}")

property_double (fill_transparent, _("Make text fill transparent"), 0.0)
    description (_("Make original text fill transparent"))
    value_range (0.0, 1.0)
    ui_range    (0.0, 1.0)
  ui_steps      (0.01, 0.10)
  ui_meta ("visible", "guichange {strokeshadow}")

property_boolean (enable_outline, _("Enable outline"), FALSE)
  description    (_("Disable or Enable Outline"))
  ui_meta ("visible", "guichange {strokeshadow}")

property_double (outline_opacity, _("Outline opacity"), 1.0)
  value_range   (0.0, 1.0)
  ui_steps      (0.01, 0.50)
  ui_meta ("visible", "guichange {strokeshadow}")
  ui_meta     ("sensitive", " enable_outline")

property_double (outline_x, _("Outline horizontal distance"), 0.0)
  description   (_("Horizontal outline fill offset"))
  value_range   (-15.0, 15.0)
  ui_steps      (1, 10)
  ui_meta ("visible", "guichange {strokeshadow}")
  ui_meta     ("sensitive", " enable_outline")

property_double (outline_y, _("Outline vertical distance"), 0.0)
  description   (_("Vertical outline fill offset"))
  value_range   (-15.0, 15.0)
   ui_steps      (1, 10)
  ui_meta ("visible", "guichange {strokeshadow}")
  ui_meta     ("sensitive", " enable_outline")


property_enum   (outline_grow_shape, _("Outline base shape"),
                 GeglstrokeGrowShapeszz_sally, gegl_stroke_grow_shapeszz_sally,
                 GEGL_stroke_GROW_SHAPE_CIRCLE)
  description   (_("The shape to expand or contract the outline fill in"))
  ui_meta ("visible", "guichange {strokeshadow}")
  ui_meta     ("sensitive", " enable_outline")

property_double (outline_radius, _("Ability for outline to puff out"), 0.0)
  value_range   (0.0, 12)
  ui_range      (0.0, 4)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  ui_meta ("visible", "guichange {strokeshadow}")
  ui_meta     ("sensitive", " enable_outline")


property_double (outline_grow_radius, _("Outline's size"), 12.0)
  value_range   (0.0, 100.0)
  ui_range      (0.0, 50.0)
  ui_digits     (0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the outline."))
  ui_meta ("visible", "guichange {strokeshadow}")
  ui_meta     ("sensitive", " enable_outline")


property_color  (outline_color, _("Outline's color"), "#000000")
  description   (_("The outline's color"))
  ui_meta ("visible", "guichange {strokeshadow}")
  ui_meta     ("sensitive", " enable_outline")



property_boolean (enable_shadow, _("Enable drop shadow"), TRUE)
  description    (_("Enable Drop Shadow. This option is hidden via output extent."))
    ui_meta     ("role", "output-extent")
  ui_meta ("visible", "guichange {strokeshadow}")
/* This option is hidden via output extent. Drop Shadow is always enabled but hidden with 0 opacity. This checkbox actually disables it.
I choose not to use this in GEGL Effects */

property_double (shadow_opacity, _("Shadow opacity"), 0.0)
  value_range   (0.0, 1.0)
  ui_range      (0.0, 1.0)
  ui_steps      (0.01, 0.10)
  ui_meta ("visible", "guichange {strokeshadow}")


property_double (shadow_x, _("Shadow horizontal distance"), 10.0)
  description   (_("Horizontal shadow offset"))
  value_range   (-200, 200)
  ui_range      (-40.0, 40.0)
  ui_steps      (1, 10)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "x")
  ui_meta ("visible", "guichange {strokeshadow}")

property_double (shadow_y, _("Shadow vertical distance"), 10.0)
  description   (_("Vertical shadow offset"))
  value_range   (-200, 200)
  ui_range      (-40.0, 40.0)
  ui_steps      (1, 10)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "y")
  ui_meta ("visible", "guichange {strokeshadow}")

property_color  (shadow_color, _("Shadow color"), "black")
  description   (_("The shadow's color (defaults to 'black')"))
  ui_meta ("visible", "guichange {strokeshadow}")

property_double (shadow_grow_radius, _("Shadow grow size"), 0.0)
  value_range   (0.0, 100.0)
  ui_range      (0.0, 50.0)
  ui_digits     (0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the shadow before blurring; a negative value will contract the shadow instead"))
  ui_meta ("visible", "guichange {strokeshadow}")

property_double (shadow_radius, _("Shadow blur intensity"), 12.0)
  value_range   (0.0, G_MAXDOUBLE)
  ui_range      (0.0, 90.0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  ui_meta ("visible", "guichange {strokeshadow}")

/* End of Color Overlay, Outline and Shadow GUI options*/

/* Beginning of Bevel and Inner Glow GUI Options */

property_double (bevel_azimuth, _("Bevel azimuth"), 30.0)
    description (_("Light angle (degrees)"))
    value_range (0, 360)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")
  ui_steps      (0.5, 0.50)
  ui_meta ("visible", "guichange {innerglowbevel}")

property_double (bevel_elevation, _("Bevel elevation"), 60.0)
    description (_("Elevation angle (degrees)"))
    value_range (30, 160)
    ui_meta ("unit", "degree")
  ui_meta ("visible", "guichange {innerglowbevel}")
  ui_steps      (0.01, 0.50)

property_int (bevel_depth, _("Bevel depth"), 30)
    description (_("Emboss depth of bevel"))
    value_range (1, 100)
  ui_meta ("visible", "guichange {innerglowbevel}")

property_double (bevel_radius, _("Bevel radius"), 4.5)
  value_range (1.0, 30.0)
  ui_range (1.0, 15)
  ui_gamma (1.5)
  ui_steps      (0.01, 0.50)
  ui_meta ("visible", "guichange {innerglowbevel}")

property_double (bevel_black, _("Black bevel and image bevel mode"), 0.00)
    description (_("When in use Bevel works correctly on black Bevels when using blend modes like Grain Merge and Hardlight. All you have to do is select those blend modes for black text and then move this slider up. This same slider can also be used so bevel can apply to image file overlay's while ignoring their content."))
  value_range   (0.00, 1.0)
  ui_steps      (0.01, 0.50)
  ui_meta ("visible", "guichange {innerglowbevel}")

property_double (bevel_opacity, _("Bevel opacity"), 1.0)
  value_range   (0.0, 1.0)
  ui_steps      (0.01, 0.50)
  ui_meta ("visible", "guichange {innerglowbevel}")

property_enum (bevel_type, _("Type of bevel"),
    GeglBlendModeTypebeavbeveleffects_sally, gegl_blend_mode_typebeavbeveleffects_sally,
    GEGL_BEVEL_NORMAL)
  description (_("Change between bump bevel and a sharp bevel style. Sharp bevel style has no radius so that slider will do nothing when sharp bevel is enabled. Black Bevel when used with sharp bevel will work on some blend modes (notably screen) but in a different way, where even its most miniscule value will enable the black bevel effect. The reason for this is because sharp bevels code is different from bump bevel. "))
  ui_meta ("visible", "guichange {innerglowbevel}")

property_enum (enable_bevel_and_blend_mode, _("Select blend or enable/disable bevel"),
    GeglBlendModeTypezz_sally, gegl_blend_mode_type_effectszz_sally,
    GEGL_BLEND_MODE_TYPE_BEVELOFF)
  ui_meta ("visible", "guichange {innerglowbevel}")

property_boolean (enable_inner_glow, _("Enable inner glow"), FALSE)
  description   (_("Whether to add an inner glow effect, which can be slow"))
  ui_meta ("visible", "guichange {innerglowbevel}")

property_enum (inner_glow_blend_mode, _("Blend mode of inner glow"),
    GeglBlendModeTypeigzz_sally, gegl_blend_mode_type_effectsigzz_sally,
    GEGL_BLEND_MODE_TYPE_OVER)
  ui_meta ("visible", "guichange {innerglowbevel}")
  ui_meta     ("sensitive", " enable_inner_glow")


property_double (inner_glow_radius, _("Inner glow's blur intensity"), 4.5)
  value_range   (0.0, 30.0)
  ui_range      (0.0, 30.0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("Making blur intensity very low will make an Inner stroke"))
  ui_meta ("visible", "guichange {innerglowbevel}")
  ui_meta     ("sensitive", " enable_inner_glow")

property_double (inner_glow_grow_radius, _("Inner glow's size"), 5)
  value_range   (1, 40.0)
  ui_range      (1, 30.0)
  ui_digits     (0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the innerglow."))
  ui_meta ("visible", "guichange {innerglowbevel}")
  ui_meta     ("sensitive", " enable_inner_glow")

property_double (inner_glow_opacity, _("Inner glow's opacity"), 1.4)
  value_range   (0.0, 2.0)
  ui_steps      (0.01, 0.10)
  ui_meta ("visible", "guichange {innerglowbevel}")
  ui_meta     ("sensitive", " enable_inner_glow")

property_color (inner_glow_color, _("Inner glow color"), "#ff8f00")
    description (_("The color of the Inner Glow"))
  ui_meta ("visible", "guichange {innerglowbevel}")
  ui_meta     ("sensitive", " enable_inner_glow")

property_double (inner_glow_x, _("Inner glow horizontal distance"), 0.0)
  description   (_("Horizontal shadow offset. Anything other then zero makes inner glow an inner shadow."))
  ui_range      (-15.0, 15.0)
  value_range   (-15.0, 15.0)
  ui_steps      (1, 2)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "x")
  ui_meta ("visible", "guichange {innerglowbevel}")
  ui_meta     ("sensitive", " enable_inner_glow")

property_double (inner_glow_y, _("Inner glow vertical distance"), 0.0)
  description   (_("Vertical shadow offset. Anything other then zero makes inner glow an inner shadow."))
  ui_range      (-15.0, 15.0)
  value_range   (-15.0, 15.0)
  ui_steps      (1, 2)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "y")
  ui_meta ("visible", "guichange {innerglowbevel}")
  ui_meta     ("sensitive", " enable_inner_glow")

property_double  (inner_glow_reach, _("Inner glow's unmodified pixel fix"), 75)
  value_range (50, 85)
  description (_("If innerglow isn't covering a few pixels on the edge. Slide this up.'"))
  ui_meta ("visible", "guichange {innerglowbevel}")
  ui_meta     ("sensitive", " enable_inner_glow")
/* End of Bevel and Inner Glow GUI Options*/

/* Beginning of Image File Overlay and Gradient GUI Options*/
property_file_path(image, _("Image file overlay upload "), "")
    description (_("Source image file path (png, jpg, raw, svg, bmp, tif, ...)"))
ui_meta ("visible", "guichange {imagegradient}")

property_enum (image_blend_mode, _("Blend Mode of image file overlay (for layer content)"),
    GeglBlendModeTypegzzimage_sally, gegl_blend_mode_type_effectsgzzimage_sally,
    GEGL_BLEND_MODE_TYPE_NORMALIMAGE)
  ui_meta ("visible", "guichange {imagegradient}")

property_double(image_opacity, _("Opacity of image file overlay"), 1.0)
    value_range (0.0, 1.0)
  ui_steps      (0.01, 0.50)
ui_meta ("visible", "guichange {imagegradient}")


property_double (hue, _("Hue"),  0.0)
   description  (_("Hue adjustment. A color fill and gradient will be ignored by this option"))
   value_range  (-180.0, 180.0)
ui_meta ("visible", "guichange {imagegradient}")

property_double (saturation, _("Saturation"), 1.0)
   description  (_("Saturation adjustment. distorts color fill, ignores gradient"))
   value_range  (0.0, 3.0)
ui_meta ("visible", "guichange {imagegradient}")

property_double (lightness, _("Lightness"), 0.0)
   description  (_("Lightness adjustment. This works with color overlays but not normal gradients"))
   value_range  (-50.0, 50.0)
ui_meta ("visible", "guichange {imagegradient}")

property_boolean (enable_gradient, _("Enable Gradient"), FALSE)
  description   (_("Whether to add Gradient overlay"))
ui_meta ("visible", "guichange {imagegradient}")

property_enum (gradient_blend_mode, _("Blend Mode of gradient"),
    GeglBlendModeTypegzz_sally, gegl_blend_mode_type_effectsgzz_sally,
    GEGL_BLEND_MODE_TYPE_OVER)
  ui_meta ("visible", "guichange {imagegradient}")
  ui_meta     ("sensitive", " enable_gradient")

property_double(gradient_opacity, _("Opacity of gradient"), 1.0)
    value_range (0.0, 1.0)
  ui_steps      (0.01, 0.50)
ui_meta ("visible", "guichange {imagegradient}")
  ui_meta     ("sensitive", " enable_gradient")


property_double (gradient_start_x, _("Gradient horizontal 1"), 256.0)
    ui_meta("unit", "pixel-coordinate")
    ui_meta("axis", "x")
  ui_steps      (0.01, 3.0)
   ui_range    (-700, 700.0)
ui_meta ("visible", "guichange {imagegradient}")
  ui_meta     ("sensitive", " enable_gradient")


property_double (gradient_start_y, _("Gradient vertical 1"), 256.0)
    ui_meta("unit", "pixel-coordinate")
    ui_meta("axis", "y")
  ui_steps      (0.01, 3.0)
   ui_range    (-700, 700.0)
ui_meta ("visible", "guichange {imagegradient}")
  ui_meta     ("sensitive", " enable_gradient")

property_double (gradient_end_x, _("Gradient horizontal 2"), 256.0)
    ui_meta("unit", "pixel-coordinate")
    ui_meta("axis", "x")
  ui_steps      (0.01, 3.0)
   ui_range    (-700, 700.0)
ui_meta ("visible", "guichange {imagegradient}")
  ui_meta     ("sensitive", " enable_gradient")

property_double (gradient_end_y, _("Gradient vertical 2"), 135.0)
    ui_meta ("unit", "pixel-coordinate")
    ui_meta ("axis", "y")
   ui_range    (-700, 700.0)
  ui_steps      (0.01, 3.0)
ui_meta ("visible", "guichange {imagegradient}")
  ui_meta     ("sensitive", " enable_gradient")

property_color  (gradient_color_1, _("Gradient color 1"), "#fe18f2")
    description (_("The color at (x2, y2)"))
ui_meta ("visible", "guichange {imagegradient}")
  ui_meta     ("sensitive", " enable_gradient")

property_color (gradient_color_2, _("Gradient color 2"), "#34ebd6")
    description (_("The color at (x1, y1)"))
ui_meta ("visible", "guichange {imagegradient}")
  ui_meta     ("sensitive", " enable_gradient")

/* End of Image File Overlay and Gradient GUI Options*/

/* Beginning of Special Options for Outline and Shadow GUI Options*/


property_boolean (enable_os, _("Enable effects on outline"), FALSE)
  description    (_("Turn on special outline abilities"))
  ui_meta ("visible", "guichange {outlinespecial}")
  ui_meta     ("sensitive", " enable_outline")


property_enum (enable_os_bevel_and_blend_mode, _("Select blend or enable/disable outline bevel"),
    GeglBlendModeTypezzbevoutlinege_sally, gegl_blend_mode_type_effectszzbevoutlinege_sally,
    GEGL_BLEND_MODE_TYPE_MULTIPLYGE)
  ui_meta ("visible", "guichange {outlinespecial}")
  ui_meta     ("sensitive", " enable_outline")
  ui_meta     ("sensitive", " enable_os")


property_double (os_azimuth, _("Outline bevel azimuth"), 30.0)
    description (_("Light angle (degrees)"))
    value_range (0, 360)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")
  ui_meta ("visible", "guichange {outlinespecial}")
  ui_steps      (0.5, 0.50)
  ui_meta     ("sensitive", " enable_outline")
  ui_meta     ("sensitive", " enable_os")

property_double (os_elevation, _("Outline bevel elevation"), 80.0)
    description (_("Elevation angle (degrees)"))
    value_range (55, 125)
    ui_meta ("unit", "degree")
  ui_meta ("visible", "guichange {outlinespecial}")
  ui_meta     ("sensitive", " enable_outline")
  ui_meta     ("sensitive", " enable_os")

property_int (os_depth, _("Outline bevel depth"), 12)
    description (_("Depth of Outline Bevel that works with some but not all blend modes"))
    value_range (1, 70)
  ui_meta ("visible", "guichange {outlinespecial}")
  ui_meta     ("sensitive", " enable_outline")
  ui_meta     ("sensitive", " enable_os")


property_double (os_radius, _("Outline bevel radius"), 3.0)
  value_range (1.0, 12.0)
  ui_range (1.0, 12)
  ui_gamma (1.5)
  ui_steps      (0.01, 0.20)
  ui_meta ("visible", "guichange {outlinespecial}")
  ui_meta     ("sensitive", " enable_outline")
  ui_meta     ("sensitive", " enable_os")


property_double (os_opacity, _("Outline bevel opacity"), 1.0)
  value_range   (0.15, 1.0)
  ui_steps      (0.01, 0.10)
  ui_meta ("visible", "guichange {outlinespecial}")
  ui_meta     ("sensitive", " enable_outline")
  ui_meta     ("sensitive", " enable_os")


property_file_path(os_image, _("Image file overlay for outline upload"), "")
    description (_("Source image file path (png, jpg, raw, svg, bmp, tif, ...)"))
  ui_meta     ("sensitive", " enable_outline")
  ui_meta     ("sensitive", " enable_os")


  ui_meta ("visible", "guichange {outlinespecial}")

property_double (os_hue, _("Hue rotation of outline's image file overlay"),  0.0)
   description  (_("Hue adjustment"))
   value_range  (-180.0, 180.0)
  ui_meta ("visible", "guichange {outlinespecial}")
  ui_meta     ("sensitive", " enable_outline")
  ui_meta     ("sensitive", " enable_os")



property_double (os_lightness, _("Lightness of outline's image file overlay"), 0.0)
   description  (_("Lightness adjustment"))
   value_range  (-100.0, 100.0)
  ui_meta ("visible", "guichange {outlinespecial}")
  ui_meta     ("sensitive", " enable_outline")
  ui_meta     ("sensitive", " enable_os")


property_boolean (enable_shadow_special, _("Enable special options for shadow/glow"), FALSE)
  description    (_("Turn on special drop shadow glow's special abilities"))
  ui_meta ("visible", "guichange {outlinespecial}")


property_file_path(shadow_image, _("Image file overlay for shadow/glow"), "")
    description (_("Source image file path (png, jpg, raw, svg, bmp, tif, ...)"))
  ui_meta ("visible", "guichange {outlinespecial}")
  ui_meta     ("sensitive", " enable_shadow_special")

property_double (shadow_blur_image, _("Blur shadow/glow's image file overlay"), 0)
   description (_("Blur the image file upload you overlayed on the dropshadow"))
   value_range (0.0, 40.0)
   ui_range    (0.24, 40.0)
   ui_gamma    (3.0)
  ui_steps      (0.01, 0.50)
   ui_meta     ("unit", "pixel-distance")
  ui_meta ("visible", "guichange {outlinespecial}")
  ui_meta     ("sensitive", " enable_shadow_special")

property_boolean (enable_aura, _("Enable aura mode - requires both shadow checkboxes enabled)"), FALSE)
  description    (_("This can be disabled both by unchecking this box and special options for drop shadow. Enabling this gives drop shadow glow a aura effect and seed."))
  ui_meta ("visible", "guichange {outlinespecial}")
  ui_meta     ("sensitive", " enable_shadow_special")

property_double (aura_size, _("Glow aura intensity"), 10)
    description (_("Average diameter of each tile (in pixels)"))
    value_range (4, 15.0)
    ui_meta     ("unit", "pixel-distance")
  ui_steps      (0.01, 0.50)
  ui_meta ("visible", "guichange {outlinespecial}")
  ui_meta     ("sensitive", " enable_shadow_special")
  ui_meta     ("sensitive", " enable_aura")

property_seed (aura_seed, _("Seed of glow aura"), randaura)
  ui_meta ("visible", "guichange {outlinespecial}")
  ui_meta     ("sensitive", " enable_shadow_special")
  ui_meta     ("sensitive", " enable_aura")
/* Two rand commands can't co-exist but if it is renamed to "randaura" it appears to work just fine */

/* End of Special Options for Outline and Shadow GUI Options*/

/* Beginning of Extra (second) Outline Shadow Glow GUI options*/
property_boolean (enable_outline_extra, _("Enable Second Outline/Shadow/Glow"), FALSE)
  description    (_("Disable or Enable Outline"))
  ui_meta ("visible", "guichange {extraosg}")

property_double (outline_extra_opacity, _("Extra outline/shadow/glow's opacity"), 1.0)
  value_range   (0.0, 1.0)
  ui_steps      (0.01, 0.50)
  ui_meta ("visible", "guichange {extraosg}")
  ui_meta     ("sensitive", " enable_outline_extra")

property_double (outline_extra_x, _("Extra outline/shadow/glow horizontal distance"), 0.0)
  description   (_("Horizontal shadow offset"))
  value_range   (-40.0, 40.0)
  ui_range      (-15.0, 15.0)
  ui_steps      (1, 10)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "x")
  ui_meta ("visible", "guichange {extraosg}")
  ui_meta     ("sensitive", " enable_outline_extra")
 
property_double (outline_extra_y, _("Extra outline/shadow/glow vertical distance"), 0.0)
  description   (_("Vertical shadow offset"))
  value_range   (-40.0, 40.0)
  ui_range      (-15.0, 15.0)
  ui_steps      (1, 10)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "y")
  ui_meta ("visible", "guichange {extraosg}")
  ui_meta     ("sensitive", " enable_outline_extra")

property_enum   (outline_extra_grow_shape, _("Extra outline/shadow/glow's base shape"),
                 GeglstrokeGrowShapeszzextra_sally, gegl_stroke_grow_shapeszzextra_sally,
                 GEGL_stroke_GROW_SHAPE_CIRCLEextra)
  description   (_("The shape to expand or contract the outline fill in"))
  ui_meta ("visible", "guichange {extraosg}")
  ui_meta     ("sensitive", " enable_outline_extra")

property_double (outline_extra_blur_radius, _("Blur to make Extra shadow/glow"), 0.0)
  value_range   (0.0, 50)
  ui_range      (0.0, 50.0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  ui_meta ("visible", "guichange {extraosg}")
  ui_meta     ("sensitive", " enable_outline_extra")

property_double (outline_extra_grow_radius, _("Extra outline's size (must be past default outline)"), 19.0)
  value_range   (0.0, 100.0)
  ui_range      (0.0, 100.0)
  ui_digits     (0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the outline."))
  ui_meta ("visible", "guichange {extraosg}")
  ui_meta     ("sensitive", " enable_outline_extra")

property_color  (outline_extra_color, _("Extra outline/shadow/glow's color"), "#00a1ff")
  description   (_("The outline's color"))
  ui_meta ("visible", "guichange {extraosg}")
  ui_meta     ("sensitive", " enable_outline_extra")

property_boolean (enable_ose, _("Enable effects on extra outline"), FALSE)
  description    (_("Turn on special outline abilities"))
  ui_meta ("visible", "guichange {extraosg}")
  ui_meta     ("sensitive", " enable_outline_extra")

property_enum (enable_ose_bevel_and_blend_mode, _("Select blend or enable/disable extra outline bevel"),
    GeglBlendModeTypezzbevoutlinegeextra_sally, gegl_blend_mode_type_effectszzbevoutlinegeextra_sally,
    GEGL_BLEND_MODE_TYPE_MULTIPLYGEextra)
  ui_meta ("visible", "guichange {extraosg}")
  ui_meta     ("sensitive", " enable_outline_extra")
  ui_meta     ("sensitive", " enable_ose")

property_double (ose_azimuth, _("Extra outline bevel azimuth"), 30.0)
    description (_("Light angle (degrees)"))
    value_range (0, 360)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")
  ui_meta ("visible", "guichange {extraosg}")
  ui_steps      (0.5, 0.50)
  ui_meta     ("sensitive", " enable_outline_extra")
  ui_meta     ("sensitive", " enable_ose")

property_double (ose_elevation, _("Extra outline bevel elevation"), 80.0)
    description (_("Elevation angle (degrees)"))
    value_range (55, 125)
    ui_meta ("unit", "degree")
  ui_meta ("visible", "guichange {extraosg}")
  ui_meta     ("sensitive", " enable_outline_extra")
  ui_meta     ("sensitive", " enable_ose")


property_int (ose_depth, _("Extra outline bevel depth"), 12)
    description (_("Depth of Outline Bevel that works with some but not all blend modes"))
    value_range (1, 70)
  ui_meta ("visible", "guichange {extraosg}")
  ui_meta     ("sensitive", " enable_outline_extra")
  ui_meta     ("sensitive", " enable_ose")




property_double (ose_radius, _("Radius of extra outline bevel"), 1.5)
  value_range (1.0, 12.0)
  ui_range (1.0, 12)
  ui_gamma (1.5)
  ui_steps      (0.01, 0.20)
  ui_meta ("visible", "guichange {extraosg}")
  ui_meta     ("sensitive", " enable_outline_extra")
  ui_meta     ("sensitive", " enable_ose")

property_double (ose_bevel_opacity, _("Extra outline bevel opacity"), 1.0)
  value_range   (0.15, 1.0)
  ui_steps      (0.01, 0.10)
  ui_meta ("visible", "guichange {extraosg}")
  ui_meta     ("sensitive", " enable_outline_extra")
  ui_meta     ("sensitive", " enable_ose")


property_file_path(ose_image, _("Image file overlay for extra outline upload "), "")
    description (_("Source image file path (png, jpg, raw, svg, bmp, tif, ...)"))
  ui_meta ("visible", "guichange {extraosg}")
  ui_meta     ("sensitive", " enable_outline_extra")
  ui_meta     ("sensitive", " enable_ose")


property_double (ose_hue, _("Hue rotation of outline's image file overlay"),  0.0)
   description  (_("Hue adjustment"))
   value_range  (-180.0, 180.0)
  ui_meta ("visible", "guichange {extraosg}")
  ui_meta     ("sensitive", " enable_outline_extra")
  ui_meta     ("sensitive", " enable_ose")


property_double (ose_lightness, _("Lightness of outline's image file overlay"), 0.0)
   description  (_("Lightness adjustment"))
   value_range  (-100.0, 100.0)
  ui_meta ("visible", "guichange {extraosg}")
  ui_meta     ("sensitive", " enable_outline_extra")
  ui_meta     ("sensitive", " enable_ose")

/* End of Extra (second) Outline Shadow Glow GUI options*/

/* Beginning of Glass Text*/

property_boolean (enable_glass, _("Enable glass on text"), FALSE)
  description    (_("Enable Glass Text"))
  ui_meta ("visible", "guichange {glassshine}")

property_enum (glass_blend_mode, _("Blend mode of glass"),
 geglblendmodetypeglass_sally, gegl_blend_mode_type_glass_sally,
    GEGL_BLEND_MODE_TYPE_GLASSOVER)
  ui_meta ("visible", "guichange {glassshine}")
  ui_meta     ("sensitive", " enable_glass")


property_double (glass_azimuth, _("Glass azimuth rotation"), 30.0)
    description (_("Light angle (degrees)"))
    value_range (0, 360)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")
  ui_steps      (0.5, 0.50)
  ui_meta ("visible", "guichange {glassshine}")
  ui_meta     ("sensitive", " enable_glass")


property_int (glass_depth, _("Glass depth"), 20.0)
    description (_("Filter width"))
    value_range (10, 100)
  ui_meta ("visible", "guichange {glassshine}")
  ui_meta     ("sensitive", " enable_glass")


property_double (glass_elevation, _("Glass elevation"), 45.0)
    description (_("Elevation angle (degrees)"))
    value_range (40, 46)
    ui_meta ("unit", "degree")
  ui_steps      (0.1, 0.50)
  ui_meta ("visible", "guichange {glassshine}")
  ui_meta     ("sensitive", " enable_glass")



property_double (glass_retract, _("Retract glass"), 3.0)
   description (_("Retract the glass shine"))
   value_range (1, 3)
   ui_range    (1, 3)
   ui_gamma    (3.0)
   ui_meta     ("unit", "pixel-distance")
  ui_steps      (0.1, 0.50)
  ui_meta ("visible", "guichange {glassshine}")
  ui_meta     ("sensitive", " enable_glass")

property_double (glass_blur, _("Blur glass"), 0.5)
   description (_("Add a mild gaussian blur to the glass"))
   value_range (0.5, 1)
   ui_range    (0.5, 1)
   ui_gamma    (3.0)
  ui_steps      (0.1, 0.50)
   ui_meta     ("unit", "pixel-distance")
  ui_meta ("visible", "guichange {glassshine}")
  ui_meta     ("sensitive", " enable_glass")

property_color (glass_color, _("Color of glass text"), "#ffffff")
    description (_("The color to paint over the glass"))
  ui_meta ("visible", "guichange {glassshine}")
  ui_meta     ("sensitive", " enable_glass")

property_double (glass_opacity, _("Opacity of glass text"), 1.0)
    description (_("Opacity slider of Glass Text"))
    value_range (0.3, 1.0)
    ui_range    (0.3, 1.0)
  ui_steps      (0.3, 1.0)
  ui_meta ("visible", "guichange {glassshine}")
  ui_meta     ("sensitive", " enable_glass")
/* End of Glass Text*/

/*Beginning of shine text*/
property_boolean (enable_shine, _("Enable shiny text"), FALSE)
  description    (_("Whether text should be shiny"))
  ui_meta ("visible", "guichange {glassshine}")

property_enum (shine_blend_mode, _("Blend mode of shiny text"),
    GeglBlendModeTypeshinegeffects_sally, gegl_blend_mode_typeshinegeffects_sally,
    GEGL_BLEND_MODE_TYPE_REPLACESHINE)
  ui_meta ("visible", "guichange {glassshine}")
  ui_meta     ("sensitive", " enable_shine")


property_double (shine_opacity, _("Opacity of shine"), 0.500)
    description (_("Opacity value of the shine"))
    value_range (0.100, 1.0)
    ui_range    (0.100, 0.950)
  ui_gamma (1.5)
  ui_meta ("visible", "guichange {glassshine}")
  ui_meta     ("sensitive", " enable_shine")


property_double (shine_x_scale, _("Horizontal scale of shine"), 28.00)
    description (_("Scale value for x axis"))
    value_range (15, 60.0)
    ui_range    (15, 60.0)
    ui_meta     ("unit", "pixel-distance")
    ui_meta     ("axis", "x")
  ui_meta ("visible", "guichange {glassshine}")
  ui_meta     ("sensitive", " enable_shine")


property_double (shine_y_scale, _("Vertical scale of shine"), 56.0)
    description (_("Scale value for y axis"))
    value_range (15, 150.0)
    ui_range    (15, 150.0)
    ui_meta     ("unit", "pixel-distance")
    ui_meta     ("axis", "y")
  ui_meta ("visible", "guichange {glassshine}")
  ui_meta     ("sensitive", " enable_shine")


property_double (shine_complexity, _("Complexity of gloss shine"), 0.8)
    description (_("Complexity factor"))
    value_range (0.1, 1.0)
  ui_meta ("visible", "guichange {glassshine}")
  ui_meta     ("sensitive", " enable_shine")


property_double (shine_blend_power, _("Shine's dark to light intensity"), -0.600)
    description (_("Intensity of the Gloss Shine Effect"))
    value_range (-0.9, 0.0)
  ui_meta ("visible", "guichange {glassshine}")
  ui_meta     ("sensitive", " enable_shine")

property_seed (shine_seed, _("Random seed of gloss shine"), rand)
  ui_meta ("visible", "guichange {glassshine}")
  ui_meta     ("sensitive", " enable_shine")


/*End of shine text*/

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     effects
#define GEGL_OP_C_SOURCE effects.c

#include "gegl-op.h"


/* First full Node listing. This is for special GEGL Graphs that have blend mode switchers. All nodes must be listed here. They can be in any order but they are organized for convience*/
typedef struct
{
  GeglNode *input;

/*All nodes relating to bevel start here*/
  GeglNode *mbd;
  GeglNode *mbd2;
  GeglNode *mbd3;
  GeglNode *mbd4;
  GeglNode *mbd5;
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
  GeglNode *lightenonly;
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

/*All nodes relating to shiny text start here*/
  GeglNode *inputshiny;
  GeglNode *opacityshiny;
  GeglNode *crop2shiny;
  GeglNode *nop0shiny;
  GeglNode *nopshiny;
  GeglNode *blendshiny;
  GeglNode *sinusshiny;
  GeglNode *additionshiny;
  GeglNode *hardlightshiny;
  GeglNode *grainmergeshiny;
  GeglNode *grainmergealtshiny;
  GeglNode *replaceshiny;
  GeglNode *nothingshiny1;
  GeglNode *nothingshiny2;
  GeglNode *nothingshiny3;
/*all nodes relating to shiny text end here*/

/*All nodes that have no relationships*/
 GeglNode *repairgeglgraph;
 GeglNode *cropigb;
/*Repair GEGL graph is a median blur at 0 that resets the graph
This auxed crop is meant to fix a bug with the dst blend mode that inner glow users
bug only happens when bevel and inner glow interact. The puzzle on why 
inner glow clips legs has been solved. 

 happens with inner glow and bevel 
* */
/*All nodes that have no relationships  end here*/

/*All nodes relating to glass over text start here*/
  GeglNode *glassovertext;
  GeglNode *glassover;
  GeglNode *nopglass;
/*All nodes relating to glass over text end here*/

/*All nodes relating to image file upload start here*/
  GeglNode *atopi;
  GeglNode *image;
  GeglNode *imageopacity;
  GeglNode *nopimage;
  GeglNode *huelight;
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
  GeglNode *xor;
  GeglNode *opacityinput;
  GeglNode *knockoutidref;
  GeglNode *medianko;
/*All nodes relating to color overlay end here*/


  GeglNode *output;
} State;

static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
/*nothingshiny2 is a gegl:nop, the rest are dst for invisible blend mode to replace crop*/
  GeglNode *bevelmode = state->beveloff;
  GeglNode *over = state->over;
  GeglNode *atopg = state->atopg;
  GeglNode *atopi = state->atopi;
  GeglNode *multiply = state->multiply;
  GeglNode *blendchoiceshiny = state->nothingshiny1;
  GeglNode *sinusshiny = state->nothingshiny2;
  GeglNode *crop2shiny = state->nothingshiny3;
  GeglNode *beveltype = state->mbd;

/* These are the blend mode switchers
BevelMode is Bevel, Over is Inner Glow, atopG is Gradient, atopI is inner glow and multiply is color overlay.
The two outline bevel mode switchers and shiny text blend mode switcher use their own .c files.

Everything below is the literal GEGL Graph instructions. There are technically four GEGL graphs that load depending on the checkboxes.*/

  if (!state) return;

  multiply = state->multiply;

  switch (o->fill_color_blend_mode) {
    case GEGL_BLEND_MODE_TYPE_NOCOLOR: multiply = state->nocolor; break;
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
default: multiply = state->multiply;
 }

beveltype = state->mbd;
  switch (o->bevel_type) {
    case GEGL_BEVEL_NORMAL: beveltype = state->mbd; break;
    case GEGL_BEVEL_SHARP: beveltype = state->mbd2; break;
    case GEGL_BEVEL_SHARPREVISE: beveltype = state->mbd3; break;
    case GEGL_BEVEL_SHARPREVISEFLATSURFACE: beveltype = state->mbd4; break;
    case GEGL_BEVEL_SHARPREVISEINSIDE: beveltype = state->mbd5; break;
default: beveltype = state->mbd;
  }

/* atopi was named based on the gegl blend mode "src-atop  combined with image. Letter I hence the name "atopi*/
  atopi = state->atopi; /* Blend mode switchers for Image File Overlay "src_atop" is the default. */
  switch (o->image_blend_mode) {
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
default: atopi = state->atopi;

 }
  bevelmode = state->beveloff;
  switch (o->enable_bevel_and_blend_mode) {
    case GEGL_BLEND_MODE_TYPE_BEVELOFF: bevelmode = state->beveloff; break;
    case GEGL_BLEND_MODE_TYPE_MULTIPLY: bevelmode = state->multiplyb; break;
    case GEGL_BLEND_MODE_TYPE_GRAINMERGE: bevelmode = state->grainmerge; break;
    case GEGL_BLEND_MODE_TYPE_SUBTRACT: bevelmode = state->subtract; break;
    case GEGL_BLEND_MODE_TYPE_GRAINEXTRACT: bevelmode = state->grainextract; break;
    case GEGL_BLEND_MODE_TYPE_SHINYGM: bevelmode = state->shinygm; break;
    case GEGL_BLEND_MODE_TYPE_COLORDODGE: bevelmode = state->colordodge; break;
    case GEGL_BLEND_MODE_TYPE_HARDLIGHT: bevelmode = state->hardlight; break;
    case GEGL_BLEND_MODE_TYPE_SCREEN: bevelmode = state->screen; break;
    case GEGL_BLEND_MODE_TYPE_LIGHTENONLY: bevelmode = state->lightenonly; break;
default: bevelmode = state->beveloff;
 }

  over = state->over; 
  switch (o->inner_glow_blend_mode) {
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
default: over = state->over;
  }
/* atopg was named based on the gegl blend mode "src-atop  combined with gradient G hence the name "atopg*/
  atopg = state->atopg; /* the default */
  switch (o->gradient_blend_mode) {
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
default: atopg = state->atopg;
  }

  if (!o->enable_shine) gegl_node_disconnect(blendchoiceshiny, "aux");
  if (o->enable_shine)  gegl_node_connect(state->opacityshiny, "output", blendchoiceshiny, "aux");

  if (o->enable_shine) sinusshiny  = state->sinusshiny;
  if (!o->enable_shine) sinusshiny  = state->nothingshiny2;
  if (o->enable_shine) crop2shiny  = state->crop2shiny;
  if (!o->enable_shine) crop2shiny  = state->nothingshiny3;

  switch (o->shine_blend_mode) {
    case GEGL_BLEND_MODE_TYPE_GRAINMERGEALTSHINE: blendchoiceshiny = state->grainmergealtshiny; break;
    case GEGL_BLEND_MODE_TYPE_GRAINMERGESHINE: blendchoiceshiny = state->grainmergeshiny; break;
    case GEGL_BLEND_MODE_TYPE_HARDLIGHTSHINE: blendchoiceshiny = state->hardlightshiny; break;
    case GEGL_BLEND_MODE_TYPE_ADDITIONSHINE: blendchoiceshiny = state->additionshiny; break;
    case GEGL_BLEND_MODE_TYPE_REPLACESHINE: blendchoiceshiny = state->replaceshiny; break;
default: blendchoiceshiny = state->replaceshiny;
}

GValue v = G_VALUE_INIT;

  g_value_init(&v, G_TYPE_INT);
  g_value_set_int(&v,(o->inner_glow_reach <= 50.0) ? 4 : 0);
gegl_node_set_property(state->innerglow, "mode", &v);

       /* Location of the GEGL Graph itself*/
  if (o->enable_inner_glow)
  {
    if (o->enable_gradient)
    {
      /* both innerglow and gradient */
         gegl_node_link_many (state->input, state->nopimage, atopi,  multiply, state->crop,  state->nopg, atopg,  state->cropcolor,  blendchoiceshiny, crop2shiny,   state->nopb, bevelmode, state->cropigb, state->nopglass,  state->glassover,   state->nopextrassg, state->knockoutidref,  state->nopig, over, state->nopstrokebehind, state->strokebehind, state->behindextrassg, state->nopdsbehind, state->dsbehind, state->xor, state->repairgeglgraph, state->output, NULL);
      /* Nodes relating to color overlay */
      gegl_node_link_many (state->mcol, state->coloropacity, NULL);
      gegl_node_connect (multiply, "aux", state->coloropacity, "output");
      /* Nodes relating to knockout text */
      gegl_node_link_many (state->knockoutidref, state->medianko, state->opacityinput, NULL);
      gegl_node_connect (state->xor, "aux", state->opacityinput, "output");
      /* Nodes relating to bevel of text */
      gegl_node_connect (bevelmode, "aux", state->mbdopacity, "output");
      gegl_node_link_many (state->nopb, beveltype, state->mbdopacity, NULL);
      /* Nodes relating to inner glow */
      gegl_node_link_many (state->nopig, state->innerglow, NULL);
      gegl_node_connect (over, "aux", state->innerglow, "output");
      /* Nodes relating to gradient */
      gegl_node_link_many (state->gradient, state->opacitygradient, NULL);
      gegl_node_connect (atopg, "aux", state->opacitygradient, "output");
      /* Nodes relating to image file overlay */
      gegl_node_link_many ( state->image, state->huelight, state->saturation, state->imageopacity, NULL);
      gegl_node_connect (atopi, "aux", state->imageopacity, "output");
      /* Nodes relating to extra outline shadow glow */
      gegl_node_link_many (state->nopextrassg, state->extrassg, NULL);
      gegl_node_connect (state->behindextrassg, "aux", state->extrassg, "output");
      /* Nodes relating to outline */
      gegl_node_link_many (state->nopstrokebehind, state->stroke, NULL);
      gegl_node_connect (state->strokebehind, "aux", state->stroke, "output");
      /* Nodes relating to shadow */
      gegl_node_link_many (state->nopdsbehind, state->ds, NULL);
      gegl_node_connect (state->dsbehind, "aux", state->ds, "output");
      /* Nodes relating to Glass Over Text start here */
      gegl_node_link_many (state->nopglass, state->glassovertext, NULL);
      gegl_node_connect (state->glassover, "aux", state->glassovertext, "output");
/*gegl crop to prevent new clip bug*/
      gegl_node_connect (state->crop, "aux", state->input, "output");
      gegl_node_connect (state->cropcolor, "aux", state->input, "output");
      gegl_node_connect (state->crop2shiny, "aux", state->input, "output");
      gegl_node_connect (state->cropigb, "aux", state->nopb, "output");
/*Nodes relating to shiny text start here*/
  gegl_node_link_many (sinusshiny, state->opacityshiny,   NULL);
  gegl_node_connect (blendchoiceshiny, "aux", state->opacityshiny, "output");
    }
    else
    {
      /* innerglow but no gradient */
         gegl_node_link_many (state->input,  state->nopimage, atopi, multiply, state->crop,   blendchoiceshiny,  crop2shiny,  state->nopb, bevelmode,  state->cropigb, state->nopglass, state->glassover,  state->nopextrassg, state->knockoutidref, state->nopig, over, state->nopstrokebehind, state->strokebehind,  state->behindextrassg, state->nopdsbehind, state->dsbehind, state->xor, state->repairgeglgraph, state->output, NULL);
      /* Nodes relating to color overlay */
      gegl_node_link_many (state->mcol, state->coloropacity, NULL);
      gegl_node_connect (multiply, "aux", state->coloropacity, "output");
      /* Nodes relating to knockout text */
      gegl_node_link_many (state->knockoutidref, state->medianko, state->opacityinput, NULL);
      gegl_node_connect (state->xor, "aux", state->opacityinput, "output");
      /* Nodes relating to bevel of text */
      gegl_node_link_many (state->nopb, beveltype, state->mbdopacity,  NULL);
      gegl_node_connect (bevelmode, "aux", state->mbdopacity, "output");
      /* Nodes relating to inner glow */
      gegl_node_link_many (state->nopig, state->innerglow, NULL);
      gegl_node_connect (over, "aux", state->innerglow, "output");
      /* Nodes relating to image file overlay */
      gegl_node_link_many ( state->image, state->huelight, state->saturation, state->imageopacity, NULL);
      gegl_node_connect (atopi, "aux", state->imageopacity, "output");
      /* Nodes relating to extra outline shadow glow */
      gegl_node_link_many (state->nopextrassg, state->extrassg, NULL);
      gegl_node_connect (state->behindextrassg, "aux", state->extrassg, "output");
      /* Nodes relating to outline */
      gegl_node_link_many (state->nopstrokebehind, state->stroke, NULL);
      gegl_node_connect (state->strokebehind, "aux", state->stroke, "output");
      /* Nodes relating to shadow */
      gegl_node_link_many (state->nopdsbehind, state->ds, NULL);
      gegl_node_connect (state->dsbehind, "aux", state->ds, "output");
      /* Nodes relating to Glass Over Text start here */
      gegl_node_link_many (state->nopglass, state->glassovertext, NULL);
      gegl_node_connect (state->glassover, "aux", state->glassovertext, "output");
/*gegl crop to prevent new clip bug*/
      gegl_node_connect (state->crop, "aux", state->input, "output");
      gegl_node_connect (state->crop2shiny, "aux", state->input, "output");
      gegl_node_connect (state->cropigb, "aux", state->nopb, "output");
/*Nodes relating to shiny text start here*/
  gegl_node_link_many (sinusshiny, state->opacityshiny,   NULL);
      gegl_node_connect (state->cropcolor, "aux", state->input, "output");
  gegl_node_connect (blendchoiceshiny, "aux", state->opacityshiny, "output");
    }
  }
  else
  {
    if (o->enable_gradient)
    {
      /* gradient but no innerglow */
         gegl_node_link_many (state->input, state->nopimage, atopi,   multiply, state->crop,  state->nopg, atopg, state->cropcolor,  blendchoiceshiny, crop2shiny,   state->nopb, bevelmode,  state->cropigb, state->nopglass,  state->glassover,  state->nopextrassg, state->knockoutidref, state->nopstrokebehind, state->strokebehind, state->behindextrassg, state->nopdsbehind, state->dsbehind, state->xor, state->repairgeglgraph, state->output, NULL);
      /* Nodes relating to color overlay */
      gegl_node_link_many (state->mcol, state->coloropacity, NULL);
      gegl_node_connect (multiply, "aux", state->coloropacity, "output");
      /* Nodes relating to knockout text */
      gegl_node_link_many (state->knockoutidref, state->medianko, state->opacityinput, NULL);
      gegl_node_connect (state->xor, "aux", state->opacityinput, "output");
      /* Nodes relating to bevel of text */
      gegl_node_link_many (state->nopb, beveltype, state->mbdopacity, NULL);
      gegl_node_connect (bevelmode, "aux", state->mbdopacity, "output");
      /* Nodes relating to gradient */
      gegl_node_connect (atopg, "aux", state->opacitygradient, "output");
      gegl_node_link_many (state->gradient, state->opacitygradient, NULL);
      /* Nodes relating to image file overlay */
      gegl_node_link_many ( state->image, state->huelight, state->saturation, state->imageopacity, NULL);
      gegl_node_connect (atopi, "aux", state->imageopacity, "output");
      /* Nodes relating to extra outline shadow glow */
      gegl_node_link_many (state->nopextrassg, state->extrassg, NULL);
      gegl_node_connect (state->behindextrassg, "aux", state->extrassg, "output");
      /* Nodes relating to outline */
      gegl_node_link_many (state->nopstrokebehind, state->stroke, NULL);
      gegl_node_connect (state->strokebehind, "aux", state->stroke, "output");
      /* Nodes relating to shadow */
      gegl_node_link_many (state->nopdsbehind, state->ds, NULL);
      gegl_node_connect (state->dsbehind, "aux", state->ds, "output");
      /* Nodes relating to Glass Over Text start here */
      gegl_node_link_many (state->nopglass, state->glassovertext, NULL);
      gegl_node_connect (state->glassover, "aux", state->glassovertext, "output");
/*gegl crop to prevent new clip bug*/
      gegl_node_connect (state->crop, "aux", state->input, "output");
      gegl_node_connect (state->cropcolor, "aux", state->input, "output");
      gegl_node_connect (state->crop2shiny, "aux", state->input, "output");
      gegl_node_connect (state->cropigb, "aux", state->nopb, "output");
/*Nodes relating to shiny text start here*/
  gegl_node_link_many (sinusshiny, state->opacityshiny,   NULL);
  gegl_node_connect (blendchoiceshiny, "aux", state->opacityshiny, "output");
    }
    else
    {
      /* neither gradient nor innerglow */
   gegl_node_link_many (state->input,  state->nopimage, atopi,  multiply, state->crop,  blendchoiceshiny, crop2shiny,  state->nopb, bevelmode, state->cropigb, state->nopglass,  state->glassover,  state->nopextrassg, state->knockoutidref, state->nopstrokebehind, state->strokebehind, state->behindextrassg, state->nopdsbehind, state->dsbehind, state->xor, state->repairgeglgraph, state->output, NULL);
      /* Nodes relating to color overlay */
      gegl_node_link_many (state->mcol, state->coloropacity, NULL);
      gegl_node_connect (multiply, "aux", state->coloropacity, "output");
      /* Nodes relating to knockout text */
      gegl_node_link_many (state->knockoutidref, state->medianko, state->opacityinput, NULL);
      gegl_node_connect (state->xor, "aux", state->opacityinput, "output");
      /* Nodes relating to bevel of text */
      gegl_node_link_many (state->nopb, beveltype, state->mbdopacity, NULL);
      gegl_node_connect (bevelmode, "aux", state->mbdopacity, "output");
      /* Nodes relating to image file overlay */
      gegl_node_link_many ( state->image, state->huelight, state->saturation, state->imageopacity, NULL);
      gegl_node_connect (atopi, "aux", state->imageopacity, "output");
      /* Nodes relating to extra outline shadow glow */
      gegl_node_link_many (state->nopextrassg, state->extrassg,  NULL);
      gegl_node_connect (state->behindextrassg, "aux", state->extrassg, "output");
      /* Nodes relating to outline */
      gegl_node_link_many (state->nopstrokebehind, state->stroke, NULL);
      gegl_node_connect (state->strokebehind, "aux", state->stroke, "output");
      /* Nodes relating to shadow */
      gegl_node_link_many (state->nopdsbehind, state->ds, NULL);
      gegl_node_connect (state->dsbehind, "aux", state->ds, "output");
      /* Nodes relating to Glass Over Text start here */
      gegl_node_link_many (state->nopglass, state->glassovertext, NULL);
      gegl_node_connect (state->glassover, "aux", state->glassovertext, "output");
/*gegl crop to prevent new clip bug*/
      gegl_node_connect (state->crop, "aux", state->input, "output");
      gegl_node_connect (state->cropcolor, "aux", state->input, "output");
      gegl_node_connect (crop2shiny, "aux", state->input, "output");
      gegl_node_connect (state->cropigb, "aux", state->nopb, "output");
/*Nodes relating to shiny text start here*/
  gegl_node_link_many (sinusshiny, state->opacityshiny,   NULL);
  gegl_node_connect (blendchoiceshiny, "aux", state->opacityshiny, "output");
    }
  }
}

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);

  GeglColor *sinuscolor1 = gegl_color_new ("#ffffff");
  GeglColor *sinuscolor2 = gegl_color_new ("#000000");

State *state = g_malloc0 (sizeof (State));
  o->user_data = state;
  state->input    = gegl_node_get_input_proxy (gegl, "input");
  state->output   = gegl_node_get_output_proxy (gegl, "output");


/* All nodes relating to outline begin here. */
  state->stroke = gegl_node_new_child (gegl,
                                  "operation", "lb:osga", "clipbugpolicy", FALSE, "behind", FALSE,
                                  NULL);

state->strokebehind = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 29, "composite-mode", 0, NULL);


  state->nopstrokebehind = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

/* All nodes relating to outline end here. */



/* All nodes relating to shadow begin here. */



  state->ds = gegl_node_new_child (gegl,
                                  "operation", "lb:osga", "clipbugpolicy", FALSE,  "behind", FALSE, "blendmodebeveloutline", 0, 
                                  NULL);

state->dsbehind = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 29, "composite-mode", 0, NULL);


  state->nopdsbehind = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  /*All nodes relating to color overlay start here*/


  /*mcol is the color overlay. It originally stood for multiply color before it got blend mode switchers*/
  state->mcol = gegl_node_new_child (gegl,
                                  "operation", "gegl:color",
                                  NULL);

  /*The nop functions as a ref and ID for color overlay's placement in a GEGL Graph*/
  state->nopm = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);


  /*gegl:dst's are applied after gegl:color or anything gegl renders to prevent a known bug. */
  state->cropcolor = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop",
                                  NULL);

  /*This operation works but is hidden */
  state->coloropacity = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);


state->xor = gegl_node_new_child (gegl,
                                    "operation", "gimp:layer-mode", "layer-mode", 60,  "composite-mode", 0, "blend-space", 0, NULL);

  /* This nop acts as a ref and id for the placement of translucent text in the gegl graph. */
  state->knockoutidref = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);
  state->opacityinput = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

#define kostring \
" opacity value=4   gaussian-blur std-dev-x=1.5 std-dev-y=1.5 abyss-policy=none clip-extent=false median-blur radius=0 abyss-policy=none "\


  state->medianko = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", kostring,  
                                  NULL);

/* Blend modes for color overlay*/

  /*multiply is the blend mode that will need to be switchable for gegl:color-overlay*/
state->multiply = gegl_node_new_child (gegl,
                                    "operation", "gimp:layer-mode", "layer-mode", 30,  "composite-mode", 2, NULL);


  state->nocolor = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst",
                                  NULL);

state->solidcolor = gegl_node_new_child (gegl,
                                    "operation", "gimp:layer-mode", "layer-mode", 28,  "composite-mode", 2, NULL);

state->grainmergecolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 47, "composite-mode", 0, "blend-space", 3, NULL);

state->overlaycolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 23, "composite-mode", 0, NULL);

state->screencolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 31, "composite-mode", 0, NULL);

state->hslcolorcolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 39, "composite-mode", 0, NULL);

state->softlightcolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 45, "composite-mode", 0, NULL);

state->linearlightcolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 50, "composite-mode", 0, "blend-space", 3, NULL);

state->hardlightcolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 44, "composite-mode", 0, NULL);

state->lchcolorcolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 26, "composite-mode", 0, NULL);

state->hsvhuecolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 37, "composite-mode", 0, NULL);

state->additioncolor = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 33, "composite-mode", 0, NULL);

/* All nodes relating to color overlay end here*/



/* All nodes relating to the bevel start here*/

  state->mbd = gegl_node_new_child (gegl,
                                  "operation", "lb:bevel", "smooth", 1.0,
                                  NULL);

  state->mbd2 = gegl_node_new_child (gegl,
                                  "operation", "lb:bevel", "type", 1, "smooth", 1.0, 
                                  NULL);

  state->mbd3 = gegl_node_new_child (gegl,
                                  "operation", "lb:bevel", "type", 5, "metric", 0,  
                                  NULL);

  state->mbd4 = gegl_node_new_child (gegl,
                                  "operation", "lb:bevel", "type", 5, "metric", 0,  "flatsurface", 2.0, 
                                  NULL);

  state->mbd5 = gegl_node_new_child (gegl,
                                  "operation", "lb:bevel", "type", 5, "metric", 0, "inside", 5.0, 
                                  NULL);

 /*The Threshold Alpha setting of the bevel is being baked in so it isn't present in the GUI.*/

  state->mbdopacity = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity", "value", 0.0,
                                  NULL);

  /*The nop functions as a ref and ID for bevel's placement in a GEGL Graph*/
  state->nopb = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

/* blend modes for bevel*/

state->multiplyb = gegl_node_new_child (gegl,
                                    "operation", "gimp:layer-mode", "layer-mode", 30,  "composite-mode", 0, NULL);


state->grainextract = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 46, "composite-mode", 0, NULL);


state->grainmerge = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 47, "composite-mode", 0, NULL);

state->subtract = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 34, "composite-mode", 0, NULL);

state->shinygm = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 47, "composite-space", 2, "composite-mode", 0, "blend-space", 2, NULL);

state->colordodge = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 42, "composite-mode", 0, "composite-space", 1, "blend-space", 1, NULL);

state->hardlight = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 44, "composite-mode", 0, "composite-space", 1, "blend-space", 0, NULL);

state->screen = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 31, "composite-mode", 0,  "blend-space", 0, NULL);

state->lightenonly = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 36, "composite-mode", 0, NULL);


/* DST doesn't disable bevel it just makes it invisible*/
  state->beveloff = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst",
                                  NULL);

/* All nodes relating to bevel end here*/

/* All nodes relating to Inner Glow start here*/


  state->innerglow = gegl_node_new_child (gegl,
                                  "operation", "lb:innerglow",
                                  NULL);

  /*The nop functions as a ref and ID for inner glow's placement in a GEGL Graph*/
  state->nopig = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);
/* Blend modes for Inner Glow are here*/

  /*over is the switch for for gegl:innerglow's blend mode switcher*/
  state->over = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);

state->grainmergeig = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 47, "composite-mode", 0, "blend-space", 3, NULL);

state->addition = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 33, "composite-mode", 0, NULL);

state->overlayig = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 23, "composite-mode", 0, NULL);

state->multiplyig = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 30, "composite-mode", 0, NULL);

state->screenig = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 31, "composite-mode", 0, NULL);

state->hslcolorig = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 39, "composite-mode", 0, NULL);

state->softlightig = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 45, "composite-mode", 0,  NULL);

state->linearlightig = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 50, "composite-mode", 0, "blend-space", 3, NULL);

state->burnig = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 43, "composite-mode", 0, NULL);

state->hueig = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 37, "composite-mode", 0, NULL);

state->hardlightig = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 44, "composite-mode", 0, NULL);

state->lchcolorig = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 26, "composite-mode", 0, "blend-space", 2, NULL);

/* All nodes relating to Inner Glow end here*/

/* All nodes relating to Gradient's start here*/


  state->gradient = gegl_node_new_child (gegl,
                                  "operation", "gegl:linear-gradient",
                                  NULL);

  state->opacitygradient = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);


  /*To prevent a known bug gegl:dst's are used after anything gegl renders; such as a linear gradient*/
  state->crop = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop",
                                  NULL);



  /*The nop functions as a ref and ID for gradient's placement in a GEGL Graph*/
  state->nopg = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);


/* Blend modes below are for gradient*/

  /*atopg is the blend mode that is default but set to have switchable blend modes for gegl:linear-gradient*/

  state->atopg = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);

state->grainmergeg = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 47, "composite-mode", 0, "blend-space", 3, NULL);

state->additiong = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 33, "composite-mode", 0, NULL);

state->overlayg = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 23, "composite-mode", 0, NULL);

state->multiplyg = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 30, "composite-mode", 0, NULL);

state->screeng = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 31, "composite-mode", 0, NULL);

state->hslcolorg = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 39, "composite-mode", 0, NULL);

state->softlightg = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 45, "composite-mode", 0, NULL);

state->hsvhueg = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 37, "composite-mode", 0, NULL);

state->linearlightg = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 50, "composite-mode", 0, "blend-space", 3, NULL);

state->hardlightg = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 44, "composite-mode", 0, NULL);

state->burng = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 43, "composite-mode", 0, NULL);

state->lchcolorg = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 26, "composite-mode", 0, NULL);

/* All nodes relating to Gradient's end here*/


/* All nodes relating to Extra Outline, Shadw Glow begin here*/


state->extrassg = gegl_node_new_child (gegl,
                                  "operation", "lb:osga", "clipbugpolicy", FALSE,  "behind", FALSE,
                                  NULL);

state->behindextrassg = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 29, "composite-mode", 0, NULL);

/*This nop functions as a ref and id for extra stroke shadow glows placement*/
  state->nopextrassg = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

/* All nodes relating to Extra Outline, Shadw Glow end here*/

/* All nodes relating to image file overlay begin here*/

  state->image = gegl_node_new_child (gegl,
                                  "operation", "port:load",
                                  NULL);

  /*The nop functions as a ref and ID for image file overlay's placement in a GEGL Graph*/
  state->nopimage = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  state->huelight = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);

  state->saturation = gegl_node_new_child (gegl,
                                  "operation", "gegl:saturation",
                                  NULL);

/* Blend modes below are alternative blend modes for image file overlay.*/

  /*atopi is the blend mode that will need to be switchable for port:load src=*/
  state->atopi = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);

  state->imageopacity = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

 state->grainmergeimage = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 47, "composite-mode", 0, "blend-space", 3, NULL);

state->overlayimage = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 23, "composite-mode", 0, NULL);

state->multiplyimage = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 30, "composite-mode", 0, NULL);

state->screenimage = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 31, "composite-mode", 0, NULL);

state->hslcolorimage = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 39, "composite-mode", 0, NULL);

state->softlightimage = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 45, "composite-mode", 0, NULL);

state->linearlightimage = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 50, "composite-mode", 0, "blend-space", 3, NULL);

state->hardlightimage = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 44, "composite-mode", 0, NULL);

state->lchcolorimage = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 26, "composite-mode", 0, NULL);

state->hsvhueimage = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 37, "composite-mode", 0, NULL);

state->additionimage = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 33, "composite-mode", 0, NULL);
/* All nodes for image file overlay end here*/

/*All nodes for shiny text start here*/
 /*This is the hidden operation shiny text. Which is literally gegl:sinus on a specific setting, with a blend mode switcher and checkbox. */

  state->inputshiny = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  state->sinusshiny    = gegl_node_new_child (gegl,
                                  "operation", "gegl:sinus", "color1", sinuscolor1, "color2", sinuscolor2, "blend-mode", 0,
                                  NULL);

  state->opacityshiny = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);


    state->nopshiny    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

    state->crop2shiny    = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop",
                                  NULL);

    state->nop0shiny    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

    state->nothingshiny1    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst",
                                  NULL);

    state->nothingshiny2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

    state->nothingshiny3    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst",
                                  NULL);

  state->replaceshiny = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 30, "composite-space", 2, "composite-mode", 0, "blend-space", 2, NULL);

  state->hardlightshiny = gegl_node_new_child (gegl,
                                  "operation", "gimp:layer-mode", "layer-mode", 44, "composite-mode", 0, "blend-space", 0, NULL);


  state->additionshiny = gegl_node_new_child (gegl,
                                  "operation", "gimp:layer-mode", "layer-mode", 33, "composite-mode", 0, NULL);

  state->grainmergeshiny = gegl_node_new_child (gegl,
                                  "operation", "gimp:layer-mode", "layer-mode", 47, "composite-mode", 0, NULL);

  state->grainmergealtshiny = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 47, "composite-space", 2, "composite-mode", 0, "blend-space", 2, NULL);

  state->blendshiny    = gegl_node_new_child (gegl,
                                  "operation", "gegl:plus",
                                  NULL);

/*All nodes for shiny text end here*/


  /*Misc nodes  */

  state->repairgeglgraph      = gegl_node_new_child (gegl, "operation", "gegl:median-blur", "abyss-policy",     0,
                                         "radius",       0,
                                         NULL);

  state->cropigb    = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop",
                                  NULL);

  /*Misc nodes end here  */

  /* All nodes relating to glass on text begin here */
  state->glassovertext = gegl_node_new_child (gegl,
                                  "operation", "lb:glassovertext",
                                  NULL);


  state->nopglass = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  /* Glass Over Text blend modes */
  state->glassover = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);

  /*Node list mapping property names, operations and original operation property names*/

  /*Beginning of Shadow's GUI asociations*/
  gegl_operation_meta_redirect (operation, "shadow_x", state->ds, "x");
  gegl_operation_meta_redirect (operation, "shadow_y", state->ds, "y");
  gegl_operation_meta_redirect (operation, "aura_seed", state->ds, "seed");
  gegl_operation_meta_redirect (operation, "aura_size", state->ds, "tile-size");
  gegl_operation_meta_redirect (operation, "enable_aura", state->ds, "enableaura");
  gegl_operation_meta_redirect (operation, "shadow_opacity", state->ds, "opacity");
  gegl_operation_meta_redirect (operation, "shadow_grow_radius", state->ds, "stroke");
  gegl_operation_meta_redirect (operation, "shadow_radius", state->ds, "blurstroke");
  gegl_operation_meta_redirect (operation, "shadow_color", state->ds, "color");
  gegl_operation_meta_redirect (operation, "enable_shadow", state->ds, "enableoutline");
  gegl_operation_meta_redirect (operation, "shadow_image", state->ds, "src");
  gegl_operation_meta_redirect (operation, "enable_shadow_special",state->ds, "enablespecialoutline");
  gegl_operation_meta_redirect (operation, "shadow_blur_image", state->ds, "blurshadowimage");
  /*End of Shadow's GUI asociations*/

  /*Beginning of Outline's GUI asociations*/
  gegl_operation_meta_redirect (operation, "outline_y", state->stroke, "y");
  gegl_operation_meta_redirect (operation, "outline_x", state->stroke, "x");
  gegl_operation_meta_redirect (operation, "outline_grow_shape", state->stroke, "grow-shape");
  gegl_operation_meta_redirect (operation, "outline_opacity", state->stroke, "opacity");
  gegl_operation_meta_redirect (operation, "outline_radius", state->stroke, "blurstroke");
  gegl_operation_meta_redirect (operation, "outline_grow_radius", state->stroke, "stroke");
  gegl_operation_meta_redirect (operation, "os_depth", state->stroke, "depth");
  gegl_operation_meta_redirect (operation, "os_radius", state->stroke, "radius");
  gegl_operation_meta_redirect (operation, "os_elevation", state->stroke, "elevation");
  gegl_operation_meta_redirect (operation, "os_azimuth", state->stroke, "azimuth");
  gegl_operation_meta_redirect (operation, "os_opacity", state->stroke, "opacitybevel");
  gegl_operation_meta_redirect (operation, "os_image", state->stroke, "src");
  gegl_operation_meta_redirect (operation, "enable_os", state->stroke, "enablespecialoutline");
  gegl_operation_meta_redirect (operation, "os_hue", state->stroke, "hue");
  gegl_operation_meta_redirect (operation, "os_lightness", state->stroke, "lightness");
  gegl_operation_meta_redirect (operation, "enable_outline", state->stroke, "enableoutline");
  gegl_operation_meta_redirect (operation, "outline_color", state->stroke, "color");
  gegl_operation_meta_redirect (operation, "enable_os_bevel_and_blend_mode", state->stroke, "blendmodebeveloutline");
  /*End of Outline's GUI asociations*/

  /*Beginning of Bevel's GUI asociations*/
  gegl_operation_meta_redirect (operation, "bevel_depth", state->mbd, "bevel2");
  gegl_operation_meta_redirect (operation, "bevel_radius", state->mbd, "radius1");
  gegl_operation_meta_redirect (operation, "bevel_elevation", state->mbd, "bevel1");
  gegl_operation_meta_redirect (operation, "bevel_azimuth", state->mbd, "azimuth");
  gegl_operation_meta_redirect (operation, "bevel_black", state->mbd, "slideupblack");

  gegl_operation_meta_redirect (operation, "bevel_depth", state->mbd2, "bevel2");
  gegl_operation_meta_redirect (operation, "bevel_radius", state->mbd2, "radius1");
  gegl_operation_meta_redirect (operation, "bevel_elevation", state->mbd2, "bevel1");
  gegl_operation_meta_redirect (operation, "bevel_azimuth", state->mbd2, "azimuth");
  gegl_operation_meta_redirect (operation, "bevel_black", state->mbd2, "slideupblack");

  gegl_operation_meta_redirect (operation, "bevel_depth", state->mbd3, "bevel2");
  gegl_operation_meta_redirect (operation, "bevel_radius", state->mbd3, "radius1");
  gegl_operation_meta_redirect (operation, "bevel_elevation", state->mbd3, "bevel1");
  gegl_operation_meta_redirect (operation, "bevel_azimuth", state->mbd3, "azimuth");
  gegl_operation_meta_redirect (operation, "bevel_black", state->mbd3, "slideupblack");

  gegl_operation_meta_redirect (operation, "bevel_depth", state->mbd4, "bevel2");
  gegl_operation_meta_redirect (operation, "bevel_radius", state->mbd4, "radius1");
  gegl_operation_meta_redirect (operation, "bevel_elevation", state->mbd4, "bevel1");
  gegl_operation_meta_redirect (operation, "bevel_azimuth", state->mbd4, "azimuth");
  gegl_operation_meta_redirect (operation, "bevel_black", state->mbd4, "slideupblack");

  gegl_operation_meta_redirect (operation, "bevel_depth", state->mbd5, "bevel2");
  gegl_operation_meta_redirect (operation, "bevel_radius", state->mbd5, "radius1");
  gegl_operation_meta_redirect (operation, "bevel_elevation", state->mbd5, "bevel1");
  gegl_operation_meta_redirect (operation, "bevel_azimuth", state->mbd5, "azimuth");
  gegl_operation_meta_redirect (operation, "bevel_black", state->mbd5, "slideupblack");


  gegl_operation_meta_redirect (operation, "bevel_opacity", state->mbdopacity, "value");
  /*End of Bevel's GUI asociations*/

  /*Beginning of Image file Overlay's GUI asociations*/
  gegl_operation_meta_redirect (operation, "image_opacity", state->imageopacity, "value");
  gegl_operation_meta_redirect (operation, "hue", state->huelight, "hue");
  gegl_operation_meta_redirect (operation, "saturation", state->saturation, "scale");
  gegl_operation_meta_redirect (operation, "lightness", state->huelight, "lightness");
  gegl_operation_meta_redirect (operation, "image", state->image, "src");
  /*End of of Image file Overlay's GUI asociations*/

  /*Beginning of Inner Glow's GUI asociations*/
  gegl_operation_meta_redirect (operation, "inner_glow_grow_radius", state->innerglow, "grow-radius");
  gegl_operation_meta_redirect (operation, "inner_glow_radius", state->innerglow, "radius");
  gegl_operation_meta_redirect (operation, "inner_glow_opacity", state->innerglow, "opacity");
  gegl_operation_meta_redirect (operation, "inner_glow_color", state->innerglow, "value2");
  gegl_operation_meta_redirect (operation, "inner_glow_reach", state->innerglow, "fixoutline");
  gegl_operation_meta_redirect (operation, "inner_glow_x", state->innerglow, "x");
  gegl_operation_meta_redirect (operation, "inner_glow_y", state->innerglow, "y");
  /*End of Inner Glow's GUI asociations*/


  /*Beginning of Color Overlay's GUI asociations*/
  gegl_operation_meta_redirect (operation, "fill_color", state->mcol, "value");
  gegl_operation_meta_redirect (operation, "fill_color_opacity", state->coloropacity, "value");
  gegl_operation_meta_redirect (operation, "fill_transparent", state->opacityinput, "value");
  /*End of Color Overlay's GUI asociations*/

  /*Beginning of Gradient's GUI asociations*/
  gegl_operation_meta_redirect (operation, "gradient_start_x", state->gradient, "start-x");
  gegl_operation_meta_redirect (operation, "gradient_start_y", state->gradient, "start-y");
  gegl_operation_meta_redirect (operation, "gradient_end_x", state->gradient, "end-x");
  gegl_operation_meta_redirect (operation, "gradient_end_y", state->gradient, "end-y");
  gegl_operation_meta_redirect (operation, "gradient_color_2", state->gradient, "start-color");
  gegl_operation_meta_redirect (operation, "gradient_color_1", state->gradient, "end-color");
  gegl_operation_meta_redirect (operation, "gradient_opacity", state->opacitygradient, "value");
  /*End of Gradient's GUI asociations*/

  /*Beginning of Shiny Text's GUI asociations*/
  gegl_operation_meta_redirect (operation, "shine_opacity", state->opacityshiny, "value");
  gegl_operation_meta_redirect (operation, "shine_seed", state->sinusshiny, "seed");
  gegl_operation_meta_redirect (operation, "shine_complexity", state->sinusshiny, "complexity");
  gegl_operation_meta_redirect (operation, "shine_x_scale", state->sinusshiny, "x-scale");
  gegl_operation_meta_redirect (operation, "shine_y_scale", state->sinusshiny, "y-scale");
  gegl_operation_meta_redirect (operation, "shine_blend_power", state->sinusshiny, "blend-power");
  /*End of Shiny Text's GUI asociations*/

  /*Beginning of Extra Outline Shadow Glow's GUI asociations*/
  gegl_operation_meta_redirect (operation, "enable_outline_extra", state->extrassg, "enableoutline");
  gegl_operation_meta_redirect (operation, "outline_extra_y", state->extrassg, "y");
  gegl_operation_meta_redirect (operation, "outline_extra_x", state->extrassg, "x");
  gegl_operation_meta_redirect (operation, "outline_extra_opacity", state->extrassg, "opacity");
  gegl_operation_meta_redirect (operation, "outline_extra_blur_radius", state->extrassg, "blurstroke");
  gegl_operation_meta_redirect (operation, "outline_extra_grow_radius", state->extrassg, "stroke");
  gegl_operation_meta_redirect (operation, "outline_extra_color", state->extrassg, "color");
  gegl_operation_meta_redirect (operation, "outline_extra_grow_shape", state->extrassg, "grow-shape");
  gegl_operation_meta_redirect (operation, "enable_ose", state->extrassg, "enablespecialoutline");
  gegl_operation_meta_redirect (operation, "ose_image", state->extrassg, "src");
  gegl_operation_meta_redirect (operation, "ose_hue", state->extrassg, "hue");
  gegl_operation_meta_redirect (operation, "ose_lightness", state->extrassg, "lightness");
  gegl_operation_meta_redirect (operation, "enable_ose_bevel_and_blend_mode", state->extrassg, "blendmodebeveloutline");
  gegl_operation_meta_redirect (operation, "ose_bevel_opacity", state->extrassg, "opacitybevel");
  gegl_operation_meta_redirect (operation, "ose_depth", state->extrassg, "depth");
  gegl_operation_meta_redirect (operation, "ose_radius", state->extrassg, "radius");
  gegl_operation_meta_redirect (operation, "ose_elevation", state->extrassg, "elevation");
  gegl_operation_meta_redirect (operation, "ose_azimuth", state->extrassg, "azimuth");
  /*End of Extra Outline Shadow Glow's GUI asociations*/

  /*Beginning of Glass Over Text's GUI asociations*/
  gegl_operation_meta_redirect (operation, "enable_glass", state->glassovertext, "enableglasstext");
  gegl_operation_meta_redirect (operation, "glass_blend_mode", state->glassovertext, "glassover");
  gegl_operation_meta_redirect (operation, "glass_azimuth", state->glassovertext, "azimuth");
  gegl_operation_meta_redirect (operation, "glass_depth", state->glassovertext, "depth");
  gegl_operation_meta_redirect (operation, "glass_elevation", state->glassovertext, "elevation");
  gegl_operation_meta_redirect (operation, "glass_retract", state->glassovertext, "std-dev");
  gegl_operation_meta_redirect (operation, "glass_blur", state->glassovertext, "blur");
  gegl_operation_meta_redirect (operation, "glass_color", state->glassovertext, "color");
  gegl_operation_meta_redirect (operation, "glass_opacity", state->glassovertext, "hyperopacity");
  /*Beginning of End of Glass Over Text's GUI asociations*/

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
    "name",        "lb:effects",
    "title",       _("GEGL Effects"),
    "reference-hash", "finalfairygeglswilliscomplete",
    "description", _("GEGL text styling and specialty image outlining filter."
                     ""),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("GEGL Effects..."),
    NULL);
}

#endif
