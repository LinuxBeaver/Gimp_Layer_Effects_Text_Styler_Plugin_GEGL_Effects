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
 * Credit to Øvind Kolas (pippin) for major GEGL contributions
 *2022 Beaver GEGL Effects  *2023 Beaver GEGL Effects CE with new features like bevel outlines, thin/bold text mode and shines and so much more.
 * 2022 BarefootLiam (for helping give Inner Glow a disable checkbox)
 */

/*

To help future devs I will list predicted scenarios that I believe will lead to GEGL effects or any of my plugins breaking compatibility with future versions of Gimp.
--
1st example of how GEGL effects can and eventually break is if Gimp team adds new blend modes that will over ride "operation", "gimp:layer-mode", "layer-mode", #,

In Example, Gimp's Split blend mode is 60

split = gegl_node_new_child (gegl,
"operation", "gimp:layer-mode", "layer-mode", 60,
"composite-mode", 0, "blend-space", 0,  NULL);

If Gimp's team adds a new blend mode this will throw off all the gimp exclusive blend modes called by one number.

Resulting in a scenario where all my plugins will not work (by using the wrong blend mode) until the number 60 is changed to 61 or 59. (we'll have to figure it out when it happens) but I'll guess 61.
All Gimp exclusive blend modes will have to be updated when this happens. That will break a ton of my plugins but the fix should be trivial.
--
The second most common reason GEGL Effects will break is if a GEGL operation changes its properties. In example gaussian blur's values are

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

The fifth is if the namespace gegl: is blocked (for GEGL only) and the namespace lb: gets disallowed for being too short, changing it to a new namespace will fix things for sure.


Crude recreation of parts of GEGL Effects graph. This just shows how the nodes are ordered. They are many things missing and I can't possibly list all the options.
DropShadow is used in place of the hidden operation. REQUIRES lb:bevel and lb:innerglow

Remove the # to see a particular effect in action

id=1
#src-atop aux=[ ref=1 layer src=image_link_here.jpg ]
id=2
#gimp:layer-mode layer-mode=normal composite-mode=clip-to-backdrop aux=[ ref=2 color-overlay value=#0057ff  ]
#id=4 src-atop aux=[ linear-gradient start-x= start-y= end-x= end-y= star-color= end-color=  ]
crop
#id=5 multiply aux=[ ref=5 sinus color1=#ffffff color2=#000000 seed=343  complexity=0.3 ]
#id=6 multiply aux=[ ref=6 lb:bevel bevel1=49 bevel2=93  ]
#id=3 over aux=[ ref=3 glassovertext ]
#id=7 id over aux=[ ref=7 lb:innerglow   ]
#dropshadow x=0 y=0 grow-radius=12 radius=1 opacity=1 color=#ff000b
#dropshadow

*/



#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

/*This is the enum list of blend modes for Outline Bevel.  Outline Bevel is part of zzstrokebevelimage hidden operation*/
enum_start (gegl_blend_mode_type_effectszzbevoutlinege)
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLYGE,      "MultiplyGE",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGEGE,      "GrainMergeGE",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_SHINYGMGE,      "GrainMergealtGE",
              N_("Grain Merge Alt"))
  enum_value (GEGL_BLEND_MODE_TYPE_COLORDODGEGE,      "ColorDodgeGE",
              N_("Color Dodge"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHTGE,      "HardLightGE",
              N_("Hard Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_DISABLEBEVELGE,      "DisableBevelGE",
              N_("Outline Bevel Off"))
enum_end (GeglBlendModeTypezzbevoutlinege)

/*This is the enum list of blend modes for Extra Outline Bevel. Outline Bevel is part of zzstrokebevelimage hidden operation*/

enum_start (gegl_blend_mode_type_effectszzbevoutlinegeextra)
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLYGEextra,      "MultiplyGE",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGEGEextra,      "GrainMergeGE",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_SHINYGMGEextra,      "GrainMergealtGE",
              N_("Grain Merge Alt"))
  enum_value (GEGL_BLEND_MODE_TYPE_COLORDODGEGEextra,      "ColorDodgeGE",
              N_("Color Dodge"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHTGEextra,      "HardLightGE",
              N_("Hard Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_DISABLEBEVELGEextra,      "DisableBevelGE",
              N_("Outline Bevel Off"))
enum_end (GeglBlendModeTypezzbevoutlinegeextra)


/*This is the enum list of blend modes for bevel.*/
enum_start (gegl_blend_mode_type_effectszz)
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLY,      "Multiply",
              N_("Multiply Bevel"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGE,      "GrainMerge",
              N_("Grain Merge Bevel"))
  enum_value (GEGL_BLEND_MODE_TYPE_SHINYGM,      "GrainMergealt",
              N_("GrainMergeAlt Bevel"))
  enum_value (GEGL_BLEND_MODE_TYPE_SUBTRACT,      "Subtract",
              N_("Subtract Bevel"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINEXTRACT,      "GrainExtract",
              N_("Grain Extract Bevel"))
  enum_value (GEGL_BLEND_MODE_TYPE_COLORDODGE,      "ColorDodge",
              N_("Color Dodge Bevel"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHT,      "HardLight",
              N_("Hard Light Bevel"))
  enum_value (GEGL_BLEND_MODE_TYPE_SCREEN,      "Screen",
              N_("Screen Bevel"))
  enum_value (GEGL_BLEND_MODE_TYPE_BEVELOFF,      "BevelOff",
              N_("Bevel off "))
enum_end (GeglBlendModeTypezz)



/*This is the enum list of blend modes for Inner Glow.*/
enum_start (gegl_blend_mode_type_effectsigzz)
  enum_value (GEGL_BLEND_MODE_TYPE_OVER,      "Over",
              N_("Normal"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGEIG,      "GrainMerge",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_ADDITION,      "Addition",
              N_("Addition"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOFTLIGHTIG,      "Softlight",
              N_("Soft Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_SCREENIG,      "Screen",
              N_("Screen"))
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLYIG,      "Multiply",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_HSLCOLORIG,      "HSLColor",
              N_("HSL Color"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAYIG,      "Overlay",
              N_("Overlay"))
  enum_value (GEGL_BLEND_MODE_TYPE_LINEARLIGHTIG,      "LinearLight",
              N_("Linear Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHTIG,      "HardLight",
              N_("Hard Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_BURNIG,      "Burn",
              N_("Burn"))
  enum_value (GEGL_BLEND_MODE_TYPE_LCHCOLORIG,      "LCHColor",
              N_("LCh Color"))
  enum_value (GEGL_BLEND_MODE_TYPE_HUEIG,      "Hue",
              N_("HSV Hue"))
enum_end (GeglBlendModeTypeigzz)

/*This is the enum list of blend modes for Gradient.*/
enum_start (gegl_blend_mode_type_effectsgzz)
  enum_value (GEGL_BLEND_MODE_TYPE_ATOPG,      "Over",
              N_("Normal"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGEG,      "GrainMerge",
              N_("Grain Merge"))
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
              N_("Linear Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_HSVHUEG,      "HSVHue",
              N_("HSV Hue"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHTG,      "HardLight",
              N_("Hard Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_BURNG,      "Burn",
              N_("Burn"))
  enum_value (GEGL_BLEND_MODE_TYPE_LCHCOLORG,      "LCHColor",
              N_("LCh Color"))
enum_end (GeglBlendModeTypegzz)

/*This is the enum list of blend modes for Image File Overlay.*/
enum_start (gegl_blend_mode_type_effectsgzzimage)
  enum_value (GEGL_BLEND_MODE_TYPE_NORMALIMAGE,      "NormalImage",
              N_("Normal Image"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGEIMAGE,      "GrainMerge",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_ADDITIONIMAGE,      "Addition",
              N_("Addition"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOFTLIGHTIMAGE,      "Softlight",
              N_("Softlight"))
  enum_value (GEGL_BLEND_MODE_TYPE_SCREENIMAGE,      "Screen",
              N_("Screen"))
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLYIMAGE,      "Multiply",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_HSLCOLORIMAGE,      "HSLColor",
              N_("HSL Color"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAYIMAGE,      "Overlay",
              N_("Overlay"))
  enum_value (GEGL_BLEND_MODE_TYPE_LINEARLIGHTIMAGE,      "LinearLight",
              N_("Linear Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHTIMAGE,      "HardLight",
              N_("Hard Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_LCHCOLORIMAGE,      "LCHColor",
              N_("LCh Color"))
  enum_value (GEGL_BLEND_MODE_TYPE_HSVHUEIMAGE,      "HSVHue",
              N_("HSV Hue"))
enum_end (GeglBlendModeTypegzzimage)

/*This is the enum list of blend modes for Color Overlay*/
enum_start (gegl_blend_mode_type_effectsgzzcolor)
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLYCOLOR,      "Multiply",
              N_("Default Multiply; White is transparent"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOLIDCOLOR,      "SolidColor",
              N_("Solid Color"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGECOLOR,      "GrainMerge",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_ADDITIONCOLOR,      "Addition",
              N_("Addition"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOFTLIGHTCOLOR,      "Softlight",
              N_("Soft Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_SCREENCOLOR,      "Screen",
              N_("Screen"))
  enum_value (GEGL_BLEND_MODE_TYPE_HSLCOLORCOLOR,      "HSLColor",
              N_("HSL Color"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAYCOLOR,      "Overlay",
              N_("Overlay"))
  enum_value (GEGL_BLEND_MODE_TYPE_LINEARLIGHTCOLOR,      "LinearLight",
              N_("Linear Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHTCOLOR,      "HardLight",
              N_("Hard Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_LCHCOLORCOLOR,      "LCHColor",
              N_("LCh Color"))
  enum_value (GEGL_BLEND_MODE_TYPE_HSVHUECOLOR,      "HSVHue",
              N_("HSV Hue"))
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
              N_("Grain MergeAlt"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGESHINE,      "GrainMerge",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHTSHINE,      "Hardlight",
              N_("Hard Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_ADDITIONSHINE,      "Addition",
              N_("Addition"))
  enum_value (GEGL_BLEND_MODE_TYPE_REPLACESHINE, "Replace",
              N_("Multiply"))
enum_end (GeglBlendModeTypeshinegeffects)

/*This is the enum list of glass text's blend mode switcher.*/
enum_start (gegl_blend_mode_type_glass)
  enum_value (GEGL_BLEND_MODE_TYPE_GLASSOVER,      "GlassOver",
              N_("Normal"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGEGLASS,      "GrainMerge",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_LINEARLIGHTGLASS,      "LinearLight",
              N_("Linear Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_SUBTRACTGLASS,      "Subtract",
              N_("Subtract"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAYGLASS, "Overlay",
              N_("Overlay"))
enum_end (geglblendmodetypeglass)


/*This is the enum list for bevel's blend mode switcher*/
enum_start (gegl_blend_mode_typebeavbeveleffects)
  enum_value (GEGL_BEVEL_NORMAL,      "normalbevel",
              N_("Bump Bevel"))
  enum_value (GEGL_BEVEL_SHARP,      "sharpbevel",
              N_("Sharp Bevel"))
enum_end (GeglBlendModeTypebeavbeveleffects)


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

property_double(coloropacity, _("Opacity of Color Overlay"), 1.0)
    value_range (0.0, 1.0)
  ui_steps      (0.01, 0.50)
    ui_meta     ("role", "output-extent")
  ui_meta ("visible", "guichange {strokeshadow}")
/* This option is hidden via output extent. The right click HTML notation slider on color overlay can change the opacity
and it saves space in the GUI. If one day a better GUI comes along I'd like to enable this. */

property_boolean (enableoutline, _("Enable Outline"), FALSE)
  description    (_("Disable or Enable Outline"))
  ui_meta ("visible", "guichange {strokeshadow}")

property_double (opacitystroke, _("Outline's Opacity"), 1.0)
  value_range   (0.0, 1.0)
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
  ui_meta ("visible", "guichange {strokeshadow}")


property_boolean (enableshadow, _("Enable Drop Shadow"), TRUE)
  description    (_("Enable Drop Shadow. This option is hidden via output extent."))
    ui_meta     ("role", "output-extent")
  ui_meta ("visible", "guichange {strokeshadow}")
/* This option is hidden via output extent. Drop Shadow is always enabled but hidden with 0 opacity. This checkbox actually disables it.
I choose not to use this in GEGL Effects */

property_double (opacity, _("Shadow/Glow Opacity --ENABLE SHADOW/GLOW"), 0.0)
  value_range   (0.0, 1.0)
  ui_range      (0.0, 1.0)
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

property_boolean (clippolicy, _("Disable Shadow Clipping (delayed color bug trade off)"), FALSE)
  description    (_("This checkbox removes the shadow clip bug for compliance with Gimp 3's non-destructive text editing. If enabled this will triger another bug only seen after using GEGL Effects heavily, said bug appears usually after a few minutes of usage and will cause GEGL Effects to delay a selected color update until another slider is moved. It is suggested to enable this once one applies the filter. But keep it disabled while editing GEGL Effects unless you can tolerate a delayed color update."))
  ui_meta ("visible", "guichange {strokeshadow}")
    ui_meta     ("role", "output-extent")


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




property_double (slideupblack, _("Black Bevel and Image Bevel mode. "), 0.00)
    description (_("When in use Bevel works correctly on black Bevels when using blend modes like Grain Merge and Hardlight. All you have to do is select those blend modes for black text and then move this slider up. This same slider can also be used so bevel can apply to image file overlay's while ignoring their content."))
  value_range   (0.00, 1.0)
  ui_steps      (0.01, 0.50)
  ui_meta ("visible", "guichange {innerglowbevel}")



property_double (bevelopacity, _("Bevel's Opacity"), 1.0)
  value_range   (0.15, 1.0)
  ui_steps      (0.01, 0.50)
  ui_meta ("visible", "guichange {innerglowbevel}")


property_enum (typebevel, _("Type of Bevel"),
    GeglBlendModeTypebeavbeveleffects, gegl_blend_mode_typebeavbeveleffects,
    GEGL_BEVEL_NORMAL)
  description (_("Change between bump bevel and a sharp bevel style. Sharp bevel style has no radius so that slider will do nothing when sharp bevel is enabled. Black Bevel when used with sharp bevel will work on some blend modes (notably screen) but in a different way, where even its most miniscule value will enable the black bevel effect. The reason for this is because sharp bevels code is different from bump bevel. "))
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



property_double(layeropacity, _("Opacity of Image File Overlay"), 1.0)
    value_range (0.0, 1.0)
  ui_steps      (0.01, 0.50)
ui_meta ("visible", "guichange {imagegradient}")


property_double (hue, _("Hue (ignores color fill and gradient)"),  0.0)
   description  (_("Hue adjustment. A color fill and gradient will be ignored by this option."))
   value_range  (-180.0, 180.0)
ui_meta ("visible", "guichange {imagegradient}")

property_double (chroma, _("Chroma (distorts color fill ignores gradient)"), 0.0)
   description  (_("Chroma adjustment. A color fill and gradient will be ignored by this option."))
   value_range  (-100.0, 30.0)
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

property_double(opacitygradient, _("Opacity of Gradient"), 1.0)
    value_range (0.0, 1.0)
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


property_double (osbevelopacity, _("Outline Bevel's Opacity"), 1.0)
  value_range   (0.15, 1.0)
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
    value_range (0.100, 1.0)
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
    value_range (0.1, 1.0)
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
    value_range (0.0, 1.0)
    ui_range    (0.0, 1.0)
  ui_steps      (0.01, 0.10)
ui_meta ("visible", "guichange {miscoptions}")

/* End of Miscellaneous text options*/

/* Beginning of Extra (second) Outline Shadow Glow GUI options*/
property_boolean (enableextraoutline, _("Enable Second Outline/Shadow/Glow"), FALSE)
  description    (_("Disable or Enable Outline"))
  ui_meta ("visible", "guichange {extraosg}")


property_double (opacitystrokeextraoutline, _("Extra Outline/Shadow/Glow's Opacity"), 1.0)
  value_range   (0.0, 1.0)
  ui_steps      (0.01, 0.50)
  ui_meta ("visible", "guichange {extraosg}")
    /* BUG SOLVED JUNE 5th 2023 -- IGNORE COMMENT-- This extra outline opacity slider is hidden via "output-extent" and the one below is to be used.
for opacity control of the second stroke. This is meant to remove a bug where GEGL Effects stacks to copies of the layer on start up.
Unfortunately to enable the extra outline the user has to Enable Extra Outline Checkbox and
slide up the opacity slider. It is what it is though inconvient. Users should be greatful that such complex
text styling is even possible in the first place. */


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



property_double (osbevelopacityextra, _("Extra Outline Bevel's Opacity"), 1.0)
  value_range   (0.15, 1.0)
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


property_double (glassopacity, _("Opacity of Glass Text"), 1.0)
    description (_("Opacity slider of Glass Text"))
    value_range (0.3, 1.0)
    ui_range    (0.3, 1.0)
  ui_steps      (0.3, 1.0)
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

/*All nodes that have no relationships (shiny text,  microblur, and thin to thick text) start here*/
  GeglNode *microblur;
  GeglNode *thinbold;
  GeglNode *cropigb;
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
  GeglNode *imageopacity;
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
/*nothingshiny2 is a gegl:nop, the rest are dst for invisible blend mode to replace crop*/
  GeglNode *bevelmode = state->beveloff;
  GeglNode *over = state->over;
  GeglNode *atopg = state->atopg;
  GeglNode *atopi = state->atopi;
  GeglNode *multiply = state->multiply;
  GeglNode *blendchoiceshiny = state->nothingshiny1;
  GeglNode *sinusshiny = state->nothingshiny2;
  GeglNode *crop2shiny = state->nothingshiny3;


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
default: multiply = state->multiply;
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
default: atopi = state->atopi;

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
default: bevelmode = state->beveloff;
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
default: over = state->over;
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
default: atopg = state->atopg;
  }

  if (!o->enableshine) gegl_node_disconnect(blendchoiceshiny, "aux");
  if (o->enableshine)  gegl_node_connect(state->opacityshiny, "output", blendchoiceshiny, "aux");

  if (o->enableshine) sinusshiny  = state->sinusshiny;
  if (!o->enableshine) sinusshiny  = state->nothingshiny2;
  if (o->enableshine) crop2shiny  = state->crop2shiny;
  if (!o->enableshine) crop2shiny  = state->nothingshiny3;

  switch (o->blendmodeshine) {
    case GEGL_BLEND_MODE_TYPE_GRAINMERGEALTSHINE: blendchoiceshiny = state->grainmergealtshiny; break;
    case GEGL_BLEND_MODE_TYPE_GRAINMERGESHINE: blendchoiceshiny = state->grainmergeshiny; break;
    case GEGL_BLEND_MODE_TYPE_HARDLIGHTSHINE: blendchoiceshiny = state->hardlightshiny; break;
    case GEGL_BLEND_MODE_TYPE_ADDITIONSHINE: blendchoiceshiny = state->additionshiny; break;
    case GEGL_BLEND_MODE_TYPE_REPLACESHINE: blendchoiceshiny = state->replaceshiny; break;
default: blendchoiceshiny = state->replaceshiny;
}

  /*if (o->fixoutline < 50.1) = state->innerglow;*/


GValue v = G_VALUE_INIT;

  g_value_init(&v, G_TYPE_INT);
  g_value_set_int(&v,(o->fixoutline <= 50.0) ? 4 : 0);
gegl_node_set_property(state->innerglow, "mode", &v);


       /* Second full Node listing. (contains four GEGL graphs for each checkbox) Nodes must be listed in proper orders. This is the GEGL Graph */
  if (o->innerglow)
  {
    if (o->gradient)
    {
      /* both innerglow and gradient */
         gegl_node_link_many (state->input, state->thinbold, state->microblur, state->nopimage, atopi,  multiply, state->crop,  state->nopg, atopg,  state->cropcolor,  blendchoiceshiny, crop2shiny,   state->nopb, bevelmode, state->cropigb, state->nopglass,  state->glassover,   state->nopextrassg, state->knockoutidref,  state->nopig, over, state->nopstrokebehind, state->strokebehind, state->behindextrassg, state->nopdsbehind, state->dsbehind, state->xor, state->repairgeglgraph, state->output, NULL);
      /* Nodes relating to color overlay */
      gegl_node_link_many (state->mcol, state->coloropacity, NULL);
      gegl_node_connect (multiply, "aux", state->coloropacity, "output");
      /* Nodes relating to knockout text */
      gegl_node_link_many (state->knockoutidref, state->opacityinput, NULL);
      gegl_node_connect (state->xor, "aux", state->opacityinput, "output");
      /* Nodes relating to bevel of text */
      gegl_node_connect (bevelmode, "aux", state->mbdopacity, "output");
      gegl_node_link_many (state->nopb, state->mbd, state->mbdopacity, NULL);
      /* Nodes relating to inner glow */
      gegl_node_link_many (state->nopig, state->innerglow, NULL);
      gegl_node_connect (over, "aux", state->innerglow, "output");
      /* Nodes relating to gradient */
      gegl_node_link_many (state->gradient, state->opacitygradient, NULL);
      gegl_node_connect (atopg, "aux", state->opacitygradient, "output");
      /* Nodes relating to image file overlay */
      gegl_node_link_many ( state->image, state->saturation, state->imageopacity, NULL);
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
      gegl_node_connect (state->crop, "aux", state->thinbold, "output");
      gegl_node_connect (state->cropcolor, "aux", state->thinbold, "output");
      gegl_node_connect (state->crop2shiny, "aux", state->thinbold, "output");
      gegl_node_connect (state->cropigb, "aux", state->nopb, "output");
/*Nodes relating to shiny text start here*/
  gegl_node_link_many (sinusshiny, state->opacityshiny,   NULL);
  gegl_node_connect (blendchoiceshiny, "aux", state->opacityshiny, "output");
    }
    else
    {
      /* innerglow but no gradient */
         gegl_node_link_many (state->input, state->thinbold, state->microblur, state->nopimage, atopi, multiply, state->crop,   blendchoiceshiny,  crop2shiny,  state->nopb, bevelmode, state->cropigb, state->nopglass, state->glassover,  state->nopextrassg, state->knockoutidref, state->nopig, over, state->nopstrokebehind, state->strokebehind,  state->behindextrassg, state->nopdsbehind, state->dsbehind, state->xor, state->repairgeglgraph, state->output, NULL);
      /* Nodes relating to color overlay */
      gegl_node_link_many (state->mcol, state->coloropacity, NULL);
      gegl_node_connect (multiply, "aux", state->coloropacity, "output");
      /* Nodes relating to knockout text */
      gegl_node_link_many (state->knockoutidref, state->opacityinput, NULL);
      gegl_node_connect (state->xor, "aux", state->opacityinput, "output");
      /* Nodes relating to bevel of text */
      gegl_node_link_many (state->nopb, state->mbd, state->mbdopacity,  NULL);
      gegl_node_connect (bevelmode, "aux", state->mbdopacity, "output");
      /* Nodes relating to inner glow */
      gegl_node_link_many (state->nopig, state->innerglow, NULL);
      gegl_node_connect (over, "aux", state->innerglow, "output");
      /* Nodes relating to image file overlay */
      gegl_node_link_many ( state->image, state->saturation, state->imageopacity, NULL);
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
      gegl_node_connect (state->crop, "aux", state->thinbold, "output");
      gegl_node_connect (state->crop2shiny, "aux", state->thinbold, "output");
      gegl_node_connect (state->cropigb, "aux", state->nopb, "output");
/*Nodes relating to shiny text start here*/
  gegl_node_link_many (sinusshiny, state->opacityshiny,   NULL);
      gegl_node_connect (state->cropcolor, "aux", state->thinbold, "output");
  gegl_node_connect (blendchoiceshiny, "aux", state->opacityshiny, "output");
    }
  }
  else
  {
    if (o->gradient)
    {
      /* gradient but no innerglow */
         gegl_node_link_many (state->input, state->thinbold, state->microblur,  state->nopimage, atopi,   multiply, state->crop,  state->nopg, atopg, state->cropcolor,  blendchoiceshiny, crop2shiny,   state->nopb, bevelmode, state->cropigb, state->nopglass,  state->glassover,  state->nopextrassg, state->knockoutidref, state->nopstrokebehind, state->strokebehind, state->behindextrassg, state->nopdsbehind, state->dsbehind, state->xor, state->repairgeglgraph, state->output, NULL);
      /* Nodes relating to color overlay */
      gegl_node_link_many (state->mcol, state->coloropacity, NULL);
      gegl_node_connect (multiply, "aux", state->coloropacity, "output");
      /* Nodes relating to knockout text */
      gegl_node_link_many (state->knockoutidref, state->opacityinput, NULL);
      gegl_node_connect (state->xor, "aux", state->opacityinput, "output");
      /* Nodes relating to bevel of text */
      gegl_node_link_many (state->nopb, state->mbd, state->mbdopacity, NULL);
      gegl_node_connect (bevelmode, "aux", state->mbdopacity, "output");
      /* Nodes relating to gradient */
      gegl_node_connect (atopg, "aux", state->opacitygradient, "output");
      gegl_node_link_many (state->gradient, state->opacitygradient, NULL);
      /* Nodes relating to image file overlay */
      gegl_node_link_many ( state->image, state->saturation, state->imageopacity, NULL);
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
      gegl_node_connect (state->crop, "aux", state->thinbold, "output");
      gegl_node_connect (state->cropcolor, "aux", state->thinbold, "output");
      gegl_node_connect (state->crop2shiny, "aux", state->thinbold, "output");
      gegl_node_connect (state->cropigb, "aux", state->nopb, "output");
/*Nodes relating to shiny text start here*/
  gegl_node_link_many (sinusshiny, state->opacityshiny,   NULL);
  gegl_node_connect (blendchoiceshiny, "aux", state->opacityshiny, "output");
    }
    else
    {
      /* neither gradient nor innerglow */
   gegl_node_link_many (state->input, state->microblur, state->thinbold, state->nopimage, atopi,  multiply, state->crop,  blendchoiceshiny, crop2shiny,  state->nopb, bevelmode, state->cropigb, state->nopglass,  state->glassover,  state->nopextrassg, state->knockoutidref, state->nopstrokebehind, state->strokebehind, state->behindextrassg, state->nopdsbehind, state->dsbehind, state->xor, state->repairgeglgraph, state->output, NULL);
      /* Nodes relating to color overlay */
      gegl_node_link_many (state->mcol, state->coloropacity, NULL);
      gegl_node_connect (multiply, "aux", state->coloropacity, "output");
      /* Nodes relating to knockout text */
      gegl_node_link_many (state->knockoutidref, state->opacityinput, NULL);
      gegl_node_connect (state->xor, "aux", state->opacityinput, "output");
      /* Nodes relating to bevel of text */
      gegl_node_link_many (state->nopb, state->mbd, state->mbdopacity, NULL);
      gegl_node_connect (bevelmode, "aux", state->mbdopacity, "output");
      /* Nodes relating to image file overlay */
      gegl_node_link_many ( state->image, state->saturation, state->imageopacity, NULL);
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
      gegl_node_connect (state->crop, "aux", state->thinbold, "output");
      gegl_node_connect (state->cropcolor, "aux", state->thinbold, "output");
      gegl_node_connect (crop2shiny, "aux", state->thinbold, "output");
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


/* All nodes relating to outline begin here.

  zzstrokebevelimage is a hidden operation; recreation of gimp's drop shadow filter with heavy modification so it can do things like bevel outlines, aura seeds and image file overlays.
gegl effects use to use the default drop shadow until Beaver realized that the only way to give GEGL Effects styles seen in Adobe's layer effects was to abandon gegl:dropshadow and
make a enhanced version of it. */
  state->stroke = gegl_node_new_child (gegl,
                                  "operation", "lb:zzstrokebevelimage",
                                  NULL);

state->strokebehind = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 29, "composite-mode", 0, NULL);


  state->nopstrokebehind = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

/* All nodes relating to outline end here. */



/* All nodes relating to shadow begin here. */


  /* zzstrokebevelimage is a hidden operation; recreation of gimp's drop shadow filter with heavy modification so it can do things like bevel outlines, aura seeds and image file overlays.
gegl effects use to use the default drop shadow until Beaver realized that the only way to give GEGL Effects styles seen in Adobe's layer effects was to abandon gegl:dropshadow and
make a enhanced version of it. */
  state->ds = gegl_node_new_child (gegl,
                                  "operation", "lb:zzstrokebevelimage",
                                  NULL);

state->dsbehind = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 29, "composite-mode", 0, NULL);

  /* This nop acts as a ref and id for the placement of zzstrokebevelimage in the gegl graph. */
  state->nopdsbehind = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);


  /* All nodes relating to knock out text begin here. */

state->xor = gegl_node_new_child (gegl,
                                    "operation", "gimp:layer-mode", "layer-mode", 60,  "composite-mode", 0, "blend-space", 0,  NULL);

  /* This nop acts as a ref and id for the placement of translucent text in the gegl graph. */
  state->knockoutidref = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);
  state->opacityinput = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

  /* All nodes relating to knock out text end here. */



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
                                  "operation", "lb:bevel", 
                                  NULL);
 /*The Threshold Alpha setting of the bevel is being baked in so it isn't present in the GUI.*/

  state->mbdopacity = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
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
                              "operation", "gimp:layer-mode", "layer-mode", 31,  "composite-mode", 0,  "blend-space", 0,  NULL);

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


  /* zzstrokebevelimage is a hidden operation; recreation of gimp's drop shadow filter with heavy modification so it can do things like bevel outlines, aura seeds and image file overlays.
gegl effects use to use the default drop shadow until Beaver realized that the only way to give GEGL Effects styles seen in Adobe's layer effects was to abandon gegl:dropshadow and
make a enhanced version of it. */
state->extrassg = gegl_node_new_child (gegl,
                                  "operation", "lb:zzstrokebevelimage",
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

  state->saturation = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
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



  /*Misc nodes ( micro blur and thin to thick text) that have no other nodes to go with them go here  */
  state->thinbold = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "abyss-policy",     0,
                                  NULL);

 

  state->microblur = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur", "clip-extent", FALSE,  "abyss-policy", 0,
                                  NULL);

  state->cropigb = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop", 
                                  NULL);

/*makes it where inner glow doesn't clip at the legs due to a bug with bevels DST blend mode*/


  state->repairgeglgraph      = gegl_node_new_child (gegl, "operation", "gegl:median-blur", "abyss-policy",     0,
                                         "radius",       0,
                                         NULL);

 /*Repair GEGL Graph is a critical operation for Gimp's non-destructive future.
A median blur at zero radius is confirmed to make no changes to an image.
This option resets gegl:opacity's value to prevent a known bug where
plugins like clay, glossy balloon and custom bevel glitch out when
drop shadow is applied in a gegl graph below them.*/


  /*Misc nodes (micro blur and thin to thick text) that have no other nodes to go with them end here  */

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




  /*Fourth full node listing. This is for GEGL operation associations. This is organize for convience but will work even if completely unorganized*/

  /*Beginning of Shadow's GUI asociations*/
  gegl_operation_meta_redirect (operation, "x", state->ds, "x");
  gegl_operation_meta_redirect (operation, "y", state->ds, "y");
  gegl_operation_meta_redirect (operation, "seedaura", state->ds, "seed");
  gegl_operation_meta_redirect (operation, "aurasize", state->ds, "tile-size");
  gegl_operation_meta_redirect (operation, "enableaura", state->ds, "enableaura");
  gegl_operation_meta_redirect (operation, "opacity", state->ds, "opacity");
  gegl_operation_meta_redirect (operation, "grow_radius", state->ds, "stroke");
  gegl_operation_meta_redirect (operation, "radius", state->ds, "blurstroke");
  gegl_operation_meta_redirect (operation, "color", state->ds, "color");
  gegl_operation_meta_redirect (operation, "enableshadow", state->ds, "enableoutline");
  gegl_operation_meta_redirect (operation, "shadowimage", state->ds, "src");
  gegl_operation_meta_redirect (operation, "specialshadowoptions",state->ds, "specialoutline");
  gegl_operation_meta_redirect (operation, "blurshadowimage", state->ds, "blurshadowimage");
  gegl_operation_meta_redirect (operation, "clippolicy", state->ds, "clipbugpolicy");
  /*End of Shadow's GUI asociations*/

  /*Beginning of Outline's GUI asociations*/
  gegl_operation_meta_redirect (operation, "ystroke", state->stroke, "y");
  gegl_operation_meta_redirect (operation, "xstroke", state->stroke, "x");
  gegl_operation_meta_redirect (operation, "grow_shape", state->stroke, "grow-shape");
  gegl_operation_meta_redirect (operation, "opacitystroke", state->stroke, "opacity");
  gegl_operation_meta_redirect (operation, "radiusstroke", state->stroke, "blurstroke");
  gegl_operation_meta_redirect (operation, "grow_radiusstroke", state->stroke, "stroke");
  gegl_operation_meta_redirect (operation, "osdepth", state->stroke, "bevel2");
  gegl_operation_meta_redirect (operation, "osradius", state->stroke, "radius1");
  gegl_operation_meta_redirect (operation, "osbevel", state->stroke, "bevel1");
  gegl_operation_meta_redirect (operation, "osbevelopacity", state->stroke, "opacitybevel");
  gegl_operation_meta_redirect (operation, "ossrc", state->stroke, "src");
  gegl_operation_meta_redirect (operation, "specialoutline", state->stroke, "specialoutline");
  gegl_operation_meta_redirect (operation, "oshue", state->stroke, "hue");
  gegl_operation_meta_redirect (operation, "oslightness", state->stroke, "lightness");
  gegl_operation_meta_redirect (operation, "enableoutline", state->stroke, "enableoutline");
  gegl_operation_meta_redirect (operation, "colorstroke", state->stroke, "color");
  gegl_operation_meta_redirect (operation, "blendmodebeveloutline", state->stroke, "blendmodebeveloutline");
  gegl_operation_meta_redirect (operation, "clippolicy", state->stroke, "clipbugpolicy");
  /*End of Outline's GUI asociations*/

  /*Beginning of Bevel's GUI asociations*/
  gegl_operation_meta_redirect (operation, "depth", state->mbd, "bevel2");
  gegl_operation_meta_redirect (operation, "radius1", state->mbd, "radius1");
  gegl_operation_meta_redirect (operation, "bevel1", state->mbd, "bevel1");
  gegl_operation_meta_redirect (operation, "slideupblack", state->mbd, "slideupblack");
  gegl_operation_meta_redirect (operation, "bevelopacity", state->mbdopacity, "value");
  gegl_operation_meta_redirect (operation, "typebevel", state->mbd, "type");
  /*End of Bevel's GUI asociations*/

  /*Beginning of Image file Overlay's GUI asociations*/
  gegl_operation_meta_redirect (operation, "layeropacity", state->imageopacity, "value");
  gegl_operation_meta_redirect (operation, "hue", state->saturation, "hue");
  gegl_operation_meta_redirect (operation, "chroma", state->saturation, "chroma");
  gegl_operation_meta_redirect (operation, "lightness", state->saturation, "lightness");
  gegl_operation_meta_redirect (operation, "src", state->image, "src");
  /*End of of Image file Overlay's GUI asociations*/

  /*Beginning of Inner Glow's GUI asociations*/
  gegl_operation_meta_redirect (operation, "innerggrow_radius", state->innerglow, "grow-radius");
  gegl_operation_meta_redirect (operation, "innergradius", state->innerglow, "radius");
  gegl_operation_meta_redirect (operation, "innergopacity", state->innerglow, "opacity");
  gegl_operation_meta_redirect (operation, "innergvalue", state->innerglow, "value2");
  gegl_operation_meta_redirect (operation, "fixoutline", state->innerglow, "fixoutline");
  gegl_operation_meta_redirect (operation, "xinnerglow", state->innerglow, "x");
  gegl_operation_meta_redirect (operation, "yinnerglow", state->innerglow, "y");
  /*End of Inner Glow's GUI asociations*/


  /*Beginning of Color Overlay's GUI asociations*/
  gegl_operation_meta_redirect (operation, "optioncolor", state->mcol, "value");
  gegl_operation_meta_redirect (operation, "coloropacity", state->coloropacity, "value");
  /*End of Color Overlay's GUI asociations*/

  /*Beginning of Gradient's GUI asociations*/
  gegl_operation_meta_redirect (operation, "start_x", state->gradient, "start-x");
  gegl_operation_meta_redirect (operation, "start_y", state->gradient, "start-y");
  gegl_operation_meta_redirect (operation, "end_x", state->gradient, "end-x");
  gegl_operation_meta_redirect (operation, "end_y", state->gradient, "end-y");
  gegl_operation_meta_redirect (operation, "start_color", state->gradient, "start-color");
  gegl_operation_meta_redirect (operation, "end_color", state->gradient, "end-color");
  gegl_operation_meta_redirect (operation, "opacitygradient", state->opacitygradient, "value");
  /*End of Gradient's GUI asociations*/

  /*Beginning of Shiny Text's GUI asociations*/
  gegl_operation_meta_redirect (operation, "opacityshine", state->opacityshiny, "value");
  gegl_operation_meta_redirect (operation, "seedshine", state->sinusshiny, "seed");
  gegl_operation_meta_redirect (operation, "complexity", state->sinusshiny, "complexity");
  gegl_operation_meta_redirect (operation, "x_scaleshine", state->sinusshiny, "x-scale");
  gegl_operation_meta_redirect (operation, "y_scaleshine", state->sinusshiny, "y-scale");
  gegl_operation_meta_redirect (operation, "blend_power", state->sinusshiny, "blend-power");

  /*End of Shiny Text's GUI asociations*/

  /*Beginning of Micro Blur's GUI asociations*/
  gegl_operation_meta_redirect (operation, "microblur", state->microblur, "std-dev-x");
  gegl_operation_meta_redirect (operation, "microblur", state->microblur, "std-dev-y");
  /*End of Micro Blur's GUI asociations*/

  /*Beginning of thin to thick text GUI asociations*/
  gegl_operation_meta_redirect (operation, "thinboldenable", state->thinbold, "radius");
  gegl_operation_meta_redirect (operation, "thinboldap", state->thinbold, "alpha-percentile");
  /*End of thin to thick text GUI asociations*/

  /*Beginning of Extra Outline Shadow Glow's GUI asociations*/
  gegl_operation_meta_redirect (operation, "enableextraoutline", state->extrassg, "enableoutline");
  gegl_operation_meta_redirect (operation, "yextraoutline", state->extrassg, "y");
  gegl_operation_meta_redirect (operation, "xextraoutline", state->extrassg, "x");
  gegl_operation_meta_redirect (operation, "opacitystrokeextraoutline", state->extrassg, "opacity");
  gegl_operation_meta_redirect (operation, "radiusstrokeextraoutline", state->extrassg, "blurstroke");
  gegl_operation_meta_redirect (operation, "grow_radiusstrokeextraoutline", state->extrassg, "stroke");
  gegl_operation_meta_redirect (operation, "colorstrokeextraoutline", state->extrassg, "color");
  gegl_operation_meta_redirect (operation, "grow_shapeextraoutline", state->extrassg, "grow-shape");
  gegl_operation_meta_redirect (operation, "specialoutlineextra", state->extrassg, "specialoutline");
  gegl_operation_meta_redirect (operation, "ossrcextra", state->extrassg, "src");
  gegl_operation_meta_redirect (operation, "blendmodebeveloutlineextra", state->extrassg, "blendmodebeveloutline");
  gegl_operation_meta_redirect (operation, "osbevelopacityextra", state->extrassg, "opacitybevel");
  gegl_operation_meta_redirect (operation, "osdepthextra", state->extrassg, "bevel2");
  gegl_operation_meta_redirect (operation, "osradiusextra", state->extrassg, "radius1");
  gegl_operation_meta_redirect (operation, "osbevelextra", state->extrassg, "bevel1");
  gegl_operation_meta_redirect (operation, "clippolicy", state->extrassg, "clipbugpolicy");
  /*End of Extra Outline Shadow Glow's GUI asociations*/

  /*Beginning of Knock Out Text's GUI asociations*/
  gegl_operation_meta_redirect (operation, "knockout", state->opacityinput, "value");
  /*End of Knock Out Text's GUI asociations*/


  /*Beginning of Glass Over Text's GUI asociations*/
  gegl_operation_meta_redirect (operation, "enableglasstext", state->glassovertext, "enableglasstext");
  gegl_operation_meta_redirect (operation, "glassblendmode", state->glassovertext, "glassover");
  gegl_operation_meta_redirect (operation, "glassazimuth", state->glassovertext, "azimuth");
  gegl_operation_meta_redirect (operation, "glassdepth", state->glassovertext, "depth");
  gegl_operation_meta_redirect (operation, "glasselevation", state->glassovertext, "elevation");
  gegl_operation_meta_redirect (operation, "glassretract", state->glassovertext, "std-dev");
  gegl_operation_meta_redirect (operation, "glassblur", state->glassovertext, "blur");
  gegl_operation_meta_redirect (operation, "glasscolor", state->glassovertext, "color");
  gegl_operation_meta_redirect (operation, "glassopacity", state->glassovertext, "hyperopacity");
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
 /*btw, if GEGL Effects (or any of my plugins) ever breaks try changing the name space from gegl: or lb: to something else.*/
  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:layereffectscontinual",
    "title",       _("GEGL Effects Continual Edition"),
    "reference-hash", "continual45ed565h8500fca01b2ac",
    "description", _("GEGL text styling and specialty image outlining filter. May 31 2025 Stable Build"
                     ""),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("GEGL Effects CE..."),
    NULL);
}

#endif

