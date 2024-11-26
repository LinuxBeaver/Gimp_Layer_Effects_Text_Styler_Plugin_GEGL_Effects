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
 * 
 *2022 Beaver GEGL Effects  *2023 Beaver GEGL Effects CE with new features like bevel outlines, thin/bold text mode and shines and so much more.
 * 2022 BarefootLiam (for helping give Inner Glow a disable checkbox)
 *2024 Beaver, a special build of GEGL Effects for GIMP 2.99.19 that doesn't crash because it has no gegl crop
 */

/*

To help future devs I will list predicted scenarios that I believe will lead to GEGL effects or any of my plugins breaking compatibility with future versions of Gimp.
--
1st example of how GEGL effects can and eventually break is if Gimp team adds new blend modes that will over ride "operation", "gimp:layer-mode", "layer-mode", #,

In Example, Gimp's Split blend mode is 60

split = gegl_node_new_child (gegl,
"operation", "gimp:layer-mode", "layer-mode", 60,
"composite-mode", 0, "blend-space", 0, "composite-space", 0, NULL);

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
then the title and or gegl operation of name of one mf my plugins; example "gegl:bevel" would have to change to "depth" everywhere in my codebase
where it is used.

The fifth is if the namespace gegl: is blocked (for GEGL only) and the namespace lb: gets disallowed for being too short, changing it to a new namespace will fix things for sure.


Crude recreation of parts of GEGL Effects graph. This just shows how the nodes are ordered. They are many things missing and I can't possibly list all the options.
DropShadow is used in place of the hidden operation. REQUIRES lb:bevel and lb:innerglow

Remove the # to see a particular effect in action

color-overlay color=transparent
id=1
#src-atop aux=[ ref=1 layer src=image_link_here.jpg ]
id=2
#gimp:layer-mode layer-mode=normal composite-mode=clip-to-backdrop aux=[ ref=2 color-overlay value=#0057ff  ]
#id=6 multiply aux=[ ref=6 lb:bevel elevation=49 depth=93  ]
#id=3 over aux=[ ref=3 glassovertext ]
#id=7 id over aux=[ ref=7 lb:innerglow   ]
#dropshadow x=0 y=0 grow-radius=12 radius=1 opacity=1 color=#ff000b
#dropshadow

*/



#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

/*This is the enum list of blend modes for Outline Bevel.  Outline Bevel is part of zzstrokebevel hidden operation*/
enum_start (gegl_blend_mode_type_effectszzbevoutlinegecrop299)
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHT1, "Hardlight",
              N_("HardLight"))
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLY1,      "Multiply",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_COLORDODGE1,      "ColorDodge",
              N_("ColorDodge"))
  enum_value (GEGL_BLEND_MODE_TYPE_PLUS1,      "Plus",
              N_("Plus"))
  enum_value (GEGL_BLEND_MODE_TYPE_DARKEN1,      "Darken",
              N_("Darken"))
  enum_value (GEGL_BLEND_MODE_TYPE_LIGHTEN1,      "Lighten",
              N_("Lighten"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAY1,      "Overlay",
              N_("Overlay"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGE1,      "GrainMerge",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOFTLIGHT1,      "Softlight",
              N_("Soft Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_ADDITION1,      "Addition",
              N_("Addition"))
enum_end (GeglBlendModeTypezzbevoutlinegecrop299)

/*This is the enum list of blend modes for Extra Outline Bevel. Outline Bevel is part of zzstrokebevel hidden operation*/

enum_start (gegl_blend_mode_type_effectszzbevoutlinegeextracrop299)
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHT2, "Hardlight",
              N_("HardLight"))
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLY2,      "Multiply",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_COLORDODGE2,      "ColorDodge",
              N_("ColorDodge"))
  enum_value (GEGL_BLEND_MODE_TYPE_PLUS2,      "Plus",
              N_("Plus"))
  enum_value (GEGL_BLEND_MODE_TYPE_DARKEN2,      "Darken",
              N_("Darken"))
  enum_value (GEGL_BLEND_MODE_TYPE_LIGHTEN2,      "Lighten",
              N_("Lighten"))
  enum_value (GEGL_BLEND_MODE_TYPE_OVERLAY2,      "Overlay",
              N_("Overlay"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGE2,      "GrainMerge",
              N_("Grain Merge"))
  enum_value (GEGL_BLEND_MODE_TYPE_SOFTLIGHT2,      "Softlight",
              N_("Soft Light"))
  enum_value (GEGL_BLEND_MODE_TYPE_ADDITION2,      "Addition",
              N_("Addition"))
enum_end (GeglBlendModeTypezzbevoutlinegeextracrop299)


/*This is the enum list of blend modes for bevel.*/
enum_start (gegl_blend_mode_type_effectszzcrop299)
  enum_value (GEGL_BLEND_MODE_TYPE_BEVELOFF,      "BevelOff",
              N_("Bevel off "))
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
enum_end (GeglBlendModeTypezzcrop299)



/*This is the enum list of blend modes for Inner Glow.*/
enum_start (gegl_blend_mode_type_effectsigzzcrop299)
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
enum_end (GeglBlendModeTypeigzzcrop299)





/* This is the enum list of outline's base shape. Should correspond to GeglMedianBlurNeighborhood in median-blur.c */
enum_start (gegl_stroke_grow_shapeszzcrop299)
  enum_value (GEGL_stroke_GROW_SHAPE_SQUARE,  "square",  N_("Square"))
  enum_value (GEGL_stroke_GROW_SHAPE_CIRCLE,  "circle",  N_("Circle"))
  enum_value (GEGL_stroke_GROW_SHAPE_DIAMOND, "diamond", N_("Diamond"))
enum_end (GeglstrokeGrowShapeszzcrop299)

/* This is the enum list of extra outline's base shape. Should correspond to GeglMedianBlurNeighborhood in median-blur.c */
enum_start (gegl_stroke_grow_shapeszzextracrop299)
  enum_value (GEGL_stroke_GROW_SHAPE_SQUAREextra,  "square",  N_("Square"))
  enum_value (GEGL_stroke_GROW_SHAPE_CIRCLEextra,  "circle",  N_("Circle"))
  enum_value (GEGL_stroke_GROW_SHAPE_DIAMONDextra, "diamond", N_("Diamond"))
enum_end (GeglstrokeGrowShapeszzextracrop299)



/*This is the enum list of glass text's blend mode switcher.
enum_start (gegl_blend_mode_type_glasscrop)
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
enum_end (geglblendmodetypeglasscrop)
*/

/*This is the enum list for parts of GEGL Effects to be displayed (part of filter to be displayed)*/
enum_start (guichangeenumzzcrop299)
enum_value   (BEAVER_UI_STROKESHADOW, "strokeshadow", N_("Color Fill, Outline and Shadow"))
enum_value   (BEAVER_UI_INNERGLOWBEVEL, "innerglowbevel", N_("Bevel and Inner Glow"))
enum_value   (BEAVER_UI_OUTLINESPECIAL, "outlinespecial", N_("Special Options for Outline and Shadow"))
enum_value   (BEAVER_UI_EXTRAOSG, "extraosg", N_("Add a extra (second) Outline Shadow Glow"))
enum_value   (BEAVER_UI_GLASSTEXTPARTNER, "glasstextpartner", N_("Glass Text"))
enum_value   (BEAVER_UI_MISC, "misc", N_("Misc Stuff"))
  enum_end (guiendzzcrop299)

/*Below is sliders and other options that are displayed in GEGL Effects GUI. They are effected by the order they are in and the ui_meta association*/

property_enum(guichange, _("Part of filter to be displayed"),
    guiendzzcrop299, guichangeenumzzcrop299,
    BEAVER_UI_STROKESHADOW)
  description(_("Change the part of the GUI being displayed"))

property_boolean (enablecolor, _("Enable Color"), FALSE)
  description    (_("Disable or Enable the color"))
  ui_meta ("visible", "guichange {strokeshadow}")


/* Beginning of Color Overlay, Outline and Shadow GUI options*/
property_color  (optioncolor, _("Text color change"), "White")
    description (_("Default fuctions like a color overlay on white but you can give it any color you want"))
  ui_meta ("visible", "guichange {strokeshadow}")



property_boolean (enableoutline, _("Enable Outline"), FALSE)
  description    (_("Disable or Enable Outline"))
  ui_meta ("visible", "guichange {strokeshadow}")

property_double (opacitystroke, _("Outline's Opacity"), 2.0)
  value_range   (0.0, 2.0)
  ui_steps      (0.01, 0.50)
  ui_meta ("visible", "guichange {strokeshadow}")


property_enum   (grow_shape, _("Outline's Base Shape"),
                 GeglstrokeGrowShapeszzcrop299, gegl_stroke_grow_shapeszzcrop299,
                 GEGL_stroke_GROW_SHAPE_CIRCLE)
  description   (_("The shape to expand or contract the outline fill in"))
  ui_meta ("visible", "guichange {strokeshadow}")

property_double (radiusstroke, _("Ability for Outline to puff out"), 0.0)
  value_range   (0.0, 12.0)
  ui_range      (0.0, 3.0)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  ui_meta ("visible", "guichange {strokeshadow}")


property_int (grow_radiusstroke, _("Outline's Size"), 12)
  value_range   (0, 100)
  ui_range      (0, 50)
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

/* This option is hidden via output extent. Drop Shadow is always enabled but hidden with 0 opacity. This checkbox actually disables it.
I choose not to use this in GEGL Effects */

property_double (opacity, _("Shadow/Glow Opacity --ENABLE SHADOW/GLOW"), 0.0)
  value_range   (0.0, 1.0)
  ui_range      (0.0, 1.0)
  ui_steps      (0.01, 0.10)
  ui_meta ("visible", "guichange {strokeshadow}")


property_double (x, _("Shadow/Glow Horizontal Distance"), 10.0)
  description   (_("Horizontal shadow offset"))
  value_range   (-200.0, 200.0)
  ui_range      (-40.0, 40.0)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "x")
  ui_meta ("visible", "guichange {strokeshadow}")

property_double (y, _("Shadow/Glow Vertical Distance"), 10.0)
  description   (_("Vertical shadow offset"))
  value_range   (-200.0, 200.0)
  ui_range      (-40.0, 40.0)
  ui_steps      (1, 10)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "y")
  ui_meta ("visible", "guichange {strokeshadow}")

property_color  (color, _("Shadow/Glow Color"), "black")
  description   (_("The shadow's color (defaults to 'black')"))
  ui_meta ("visible", "guichange {strokeshadow}")

property_int (grow_radius, _("Shadow/Glow Grow size"), 0)
  value_range   (0, 100)
  ui_range      (0, 50)
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

property_double (elevation, _("Rotate Bevel Depth Angle"), 60.0)
    description (_("Elevation angle (degrees)"))
    value_range (30, 160)
    ui_meta ("unit", "degree")
  ui_meta ("visible", "guichange {innerglowbevel}")


property_double (gaus, _("Radius of Bevel"), 2.0)
  value_range (1.0, 30.0)
  ui_range (1.0, 15.0)
  ui_gamma (1.5)
  ui_steps      (0.01, 0.20)
  ui_meta ("visible", "guichange {innerglowbevel}")




property_double (slideupblack, _("Black Bevel mode. "), 0.0)
    description (_("When in use GEGL Bevel works correctly on black Bevels when using blend modes like Grain Merge and Hardlight. All you have to do is select those blend modes for black text and then move this slider up. This same slider can also be used so bevel can apply to image file overlay's while ignoring their content."))
  value_range   (0.00, 1.0)
  ui_steps      (0.01, 0.50)
  ui_meta ("visible", "guichange {innerglowbevel}")


property_double (bevelopacity, _("Bevel's Opacity"), 1.0)
  value_range   (0.15, 1.0)
  ui_steps      (0.01, 0.50)
  ui_meta ("visible", "guichange {innerglowbevel}")


property_enum (typebevel, _("Type of Bevel"),
    GeglBlendModeTypebeavbeveleffectscrop299, gegl_blend_mode_typebeavbeveleffectscrop299,
    GEGL_BEVEL_NORMAL)
  description (_("Change between normal bevel and a sharp bevel style. Sharp bevel style has no radius so that slider will do nothing when sharp bevel is enabled. Black Bevel when used with sharp bevel will work on some blend modes (notably screen) but in a different way, where even its most miniscule value will enable the black bevel effect. The reason for this is because sharp bevels code is different from normal bevel. "))
  ui_meta ("visible", "guichange {innerglowbevel}")

enum_start (gegl_blend_mode_typebeavbeveleffectscrop299)
  enum_value (GEGL_BEVEL_NORMAL,      "normalbevel",
              N_("Normal Bevel"))
  enum_value (GEGL_BEVEL_SHARP,      "sharpbevel",
              N_("Sharp Bevel"))
enum_end (GeglBlendModeTypebeavbeveleffectscrop299)

property_enum (blendmodebevel2, _("Select blend or Enable/Disable Bevel"),
    GeglBlendModeTypezzcrop299, gegl_blend_mode_type_effectszzcrop299,
    GEGL_BLEND_MODE_TYPE_BEVELOFF)
  ui_meta ("visible", "guichange {innerglowbevel}")

property_boolean (innerglow, _("Enable Inner Glow"), FALSE)
  description   (_("Whether to add an inner glow effect"))
  ui_meta ("visible", "guichange {innerglowbevel}")

property_enum (blendmodeinnerglow2, _("Blend Mode of Inner Glow"),
    GeglBlendModeTypeigzzcrop299, gegl_blend_mode_type_effectsigzzcrop299,
    GEGL_BLEND_MODE_TYPE_OVER)
  ui_meta ("visible", "guichange {innerglowbevel}")

property_double (innergradius, _("Inner Glow's Blur intensity"), 4.5)
  value_range   (0.0, 30.0)
  ui_range      (0.0, 30.0)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("Making blur intensity very low will make an InnerStroke"))
  ui_meta ("visible", "guichange {innerglowbevel}")

property_int (innerggrow_radius, _("Inner Glow's Size"), 5)
  value_range   (1, 40)
  ui_range      (1, 30)
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
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "y")
  ui_meta ("visible", "guichange {innerglowbevel}")


/* End of Bevel and Inner Glow GUI Options*/



/* Beginning of Special Options for Outline and Shadow GUI Options*/
property_double (xstroke, _("Outline Horizontal Position"), 0.0)
  description   (_("Horizontal outline fill offset"))
  value_range   (-15.0, 15.0)
  ui_steps      (1, 10)
  ui_meta ("visible", "guichange {outlinespecial}")

property_double (ystroke, _("Outline Vertical Position"), 0.0)
  description   (_("Vertical outline fill offset"))
  value_range   (-15.0, 15.0)
  ui_meta ("visible", "guichange {outlinespecial}")

property_boolean (specialoutline, _("Enable Bevel on Outline"), FALSE)
  description    (_("Turn on special outline abilities"))
  ui_meta ("visible", "guichange {outlinespecial}")

property_enum (blendmodebeveloutline, _("Select blend of Bevel"),
    GeglBlendModeTypezzbevoutlinegecrop299, gegl_blend_mode_type_effectszzbevoutlinegecrop299,
    GEGL_BLEND_MODE_TYPE_MULTIPLY1)
  ui_meta ("visible", "guichange {outlinespecial}")

property_int (osdepth, _("Outline Bevel Depth"), 9)
    description (_("Depth of Outline Bevel that works with some but not all blend modes"))
    value_range (1, 40)
  ui_meta ("visible", "guichange {outlinespecial}")

property_double (osbevel, _("Rotate Outline Bevel Depth Angle (90 resets)"), 80.0)
    description (_("Elevation angle (degrees)"))
    value_range (55, 125)
    ui_meta ("unit", "degree")
  ui_meta ("visible", "guichange {outlinespecial}")


property_double (osradius, _("Radius of Outline Bevel"), 5.0)
  value_range (1.0, 10.0)
  ui_range (1.0, 10.0)
  ui_gamma (1.5)
  ui_meta ("visible", "guichange {outlinespecial}")


property_double (microblur, _("Very Mild Blur on Original Text "), 0.0)
   description (_("Applies a blur of 0.50 on the original text layer under all applied effects. This fixes rough surfaces and may enhance the bevel effect. If applied to an image it is as if you ran gaussian blur 0.50"))
   value_range (0.0, 0.5)
   ui_range    (0.0, 0.5)
   ui_meta     ("unit", "pixel-distance")
ui_meta ("visible", "guichange {misc}")

property_int  (thinboldenable, _("Slide up or down to Enable/Disable Thin to Thick slider"), 0)
  value_range (0, 5)
  ui_range    (0, 5)
  ui_meta     ("unit", "pixel-distance")
  description (_("Sliding back to 0 will disable this effect. Thin to Thickness Slider"))
ui_meta ("visible", "guichange {misc}")



property_double  (thinboldap, _("Thin to Thick text slider"), 50.0)
  value_range (0, 100)
  description (_("0 Makes text thin as possible. 100 makes text bold as possible. This is a median blur alpha percentile and should only be applied on text."))
ui_meta ("visible", "guichange {misc}")

property_double (knockout, _("Knock out original text. (translucent text option)"), 0.0)
    description (_("Global opacity value that is always used on top of the optional auxiliary input buffer."))
    value_range (0.0, 1.0)
    ui_range    (0.0, 1.0)
  ui_steps      (0.01, 0.10)
ui_meta ("visible", "guichange {misc}")

/* End of Miscellaneous text options*/

/* Beginning of Extra (second) Outline Shadow Glow GUI options*/
property_boolean (enableextraoutline, _("Enable Second Outline/Shadow/Glow"), FALSE)
  description    (_("Disable or Enable Outline"))
  ui_meta ("visible", "guichange {extraosg}")


property_double (opacitystrokeextraoutline, _("Extra Outline/Shadow/Glow's Opacity"), 2.0)
  value_range   (0.0, 2.0)
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
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "x")
  ui_meta ("visible", "guichange {extraosg}")

property_double (yextraoutline, _("Extra Outline/Shadow/Glow Vertical Distance"), 0.0)
  description   (_("Vertical shadow offset"))
  value_range   (-6.0, 6.0)
  ui_range      (-6.0, 6.0)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "y")
  ui_meta ("visible", "guichange {extraosg}")

property_enum   (grow_shapeextraoutline, _("Extra Outline/Shadow/Glow's Base Shape"),
                 GeglstrokeGrowShapeszzextracrop299, gegl_stroke_grow_shapeszzextracrop299,
                 GEGL_stroke_GROW_SHAPE_CIRCLEextra)
  description   (_("The shape to expand or contract the outline fill in"))
  ui_meta ("visible", "guichange {extraosg}")

property_double (radiusstrokeextraoutline, _("Blur to make Extra Shadow/Glow"), 0.0)
  value_range   (0.0, 12.0)
  ui_range      (0.0, 6.0)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  ui_meta ("visible", "guichange {extraosg}")

property_int (grow_radiusstrokeextraoutline, _("Extra Outline's Size (must be past default outline)"), 19)
  value_range   (0, 100)
  ui_range      (0, 100)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the outline."))
  ui_meta ("visible", "guichange {extraosg}")

property_color  (colorstrokeextraoutline, _("Extra Outline/Shadow/Glow's Color"), "#2400ff")
  description   (_("The outline's color"))
  ui_meta ("visible", "guichange {extraosg}")

property_boolean (specialoutlineextra, _("Enable Bevel on Extra Outline"), FALSE)
  description    (_("Turn on special outline abilities"))
  ui_meta ("visible", "guichange {extraosg}")


property_enum (blendmodebeveloutlineextra, _("Select blend of Outline Bevel"),
    GeglBlendModeTypezzbevoutlinegeextracrop299, gegl_blend_mode_type_effectszzbevoutlinegeextracrop299,
    GEGL_BLEND_MODE_TYPE_MULTIPLY2)
  ui_meta ("visible", "guichange {extraosg}")

property_int (osdepthextra, _("Extra Outline's Bevel Depth"), 22)
    description (_("Depth of Outline Bevel that works with some but not all blend modes"))
    value_range (1, 40)
  ui_meta ("visible", "guichange {extraosg}")


property_double (osbevelextra, _("Rotate Extra Outline Bevel Depth Angle (90 resets)"), 80.0)
    description (_("Elevation angle (degrees)"))
    value_range (55, 125)
    ui_meta ("unit", "degree")
  ui_meta ("visible", "guichange {extraosg}")

property_double (osradiusextra, _("Radius of Extra Outline Bevel"), 5.0)
  value_range (1.0, 10.0)
  ui_range (1.0, 10.0)
  ui_gamma (1.5)
  ui_steps      (0.01, 0.20)
  ui_meta ("visible", "guichange {extraosg}")





/* End of Extra (second) Outline Shadow Glow GUI options*/

/* Beginning of Glass Text*/

property_boolean (enableglasstext, _("Enable Glass on Text"), FALSE)
  description    (_("Enable Glass Text"))
  ui_meta ("visible", "guichange {glasstextpartner}")


property_double (glassazimuth, _("Azimuth"), 30.0)
    description (_("Light angle (degrees)"))
    value_range (0, 360)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")
  ui_meta ("visible", "guichange {glasstextpartner}")


property_int (glassdepth, _("Depth"), 20.0)
    description (_("Filter width"))
    value_range (10, 100)
  ui_meta ("visible", "guichange {glasstextpartner}")


property_double (glasselevation, _("Elevation"), 45.0)
    description (_("Elevation angle (degrees)"))
    value_range (40, 46)
    ui_meta ("unit", "degree")
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
   value_range (0.5, 1.0)
   ui_range    (0.5, 1.0)
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
  ui_meta ("visible", "guichange {glasstextpartner}")
/* End of Glass Text*/


#else

#define GEGL_OP_META
#define GEGL_OP_NAME     gegleffectsrework
#define GEGL_OP_C_SOURCE gegleffectsrework.c

#include "gegl-op.h"


/* First full Node listing. This is for special GEGL Graphs that have blend mode switchers. All nodes must be listed here. They can be in any order but they are organized for convience*/
typedef struct
{
  GeglNode *input;

/*All nodes relating to color overlay here*/
  GeglNode *mcol2;
  GeglNode *nopcolor;
/*All nodes relating to bevel start here*/
  GeglNode *thebevel;
  GeglNode *thebevelsharp;
  GeglNode *thebevelopacity;
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
  GeglNode *dstinglow;
  GeglNode *nopinglow;
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
  GeglNode *strokenop;
  GeglNode *strokebevel;
  GeglNode *strokebevelnop;
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
  GeglNode *behindextrassgdst;
  GeglNode *extrassg;
  GeglNode *extrassgnop;
/*All nodes relating to extra outline shadow glow end here*/

/*All nodes that have no relationships (shiny text,  microblur, and thin to thick text) start here*/
  GeglNode *microblur;
  GeglNode *thinbold;
  GeglNode *repairgeglgraph;
/*All nodes that have no relationships (shiny text, microblur, and thin to thick text) end here*/

/*All nodes relating to glass over text start here*/
  GeglNode *glassovertext;
  GeglNode *glassover;
  GeglNode *glassovertextoff;
  GeglNode *glassoveroff;
  GeglNode *nopglass;
/*All nodes relating to glass over text end here*/


/*All nodes relating to knockout text start here*/
  GeglNode *xor;
  GeglNode *opacityinput;
  GeglNode *knockoutidref;
/*All nodes relating to knockout text end here*/


/*more ungrouped nodes*/

  GeglNode *strokebehinddst;
  GeglNode *extrassgbevelnop;
  GeglNode *extrassgbevel;




  GeglNode *output;
} State;

static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  GeglNode *bevelmode = state->beveloff; 
  GeglNode *over = state->over; 
  GeglNode *mcol = state->nopcolor; 
  GeglNode *innerglow = state->dstinglow; 
  GeglNode *glassover = state->glassoveroff; 
  GeglNode *glassovertext = state->glassovertextoff; 
  GeglNode *strokex = state->strokenop; 
  GeglNode *strokebehindx = state->strokebehinddst; 
  GeglNode *extrassgx = state->extrassgnop; 
  GeglNode *strokebevelx = state->strokebevelnop; 
  GeglNode *extrassgbevelx = state->extrassgbevelnop; 
  GeglNode *behindextrassgx = state->behindextrassgdst; 


  if (!state) return;

  bevelmode = state->beveloff; 
  switch (o->blendmodebevel2) {
    case GEGL_BLEND_MODE_TYPE_BEVELOFF: bevelmode = state->beveloff; break;
    case GEGL_BLEND_MODE_TYPE_MULTIPLY: bevelmode = state->multiplyb; break;
    case GEGL_BLEND_MODE_TYPE_GRAINMERGE: bevelmode = state->grainmerge; break;
    case GEGL_BLEND_MODE_TYPE_SUBTRACT: bevelmode = state->subtract; break;
    case GEGL_BLEND_MODE_TYPE_GRAINEXTRACT: bevelmode = state->grainextract; break;
    case GEGL_BLEND_MODE_TYPE_SHINYGM: bevelmode = state->shinygm; break;
    case GEGL_BLEND_MODE_TYPE_COLORDODGE: bevelmode = state->colordodge; break;
    case GEGL_BLEND_MODE_TYPE_HARDLIGHT: bevelmode = state->hardlight; break;
    case GEGL_BLEND_MODE_TYPE_SCREEN: bevelmode = state->screen; break;
    default: bevelmode = state->beveloff;
 }

  over = state->over; 
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


  if (o->enablecolor) mcol = state->mcol2;
  if (!o->enablecolor) mcol = state->nopcolor;

  if (o->innerglow) innerglow = state->innerglow;
  if (!o->innerglow) innerglow = state->nopinglow;

  if (o->innerglow) over = over;
  if (!o->innerglow) over = state->dstinglow;

  if (o->enableglasstext) glassover = state->glassover;
  if (!o->enableglasstext) glassover = state->glassoveroff;
  if (o->enableglasstext) glassovertext = state->glassovertext;
  if (!o->enableglasstext) glassovertext = state->glassovertextoff;

  if (o->enableoutline) strokex = state->stroke;
  if (!o->enableoutline) strokex = state->strokenop;
  if (o->enableoutline) strokebehindx = state->strokebehind;
  if (!o->enableoutline) strokebehindx = state->strokebehinddst;

  if (o->enableextraoutline) extrassgx = state->extrassg;
  if (!o->enableextraoutline) extrassgx = state->extrassgnop;
  if (o->enableextraoutline) behindextrassgx = state->behindextrassg;
  if (!o->enableextraoutline) behindextrassgx = state->behindextrassgdst;

  if (o->specialoutline) strokebevelx = state->strokebevel;
  if (o->specialoutlineextra) extrassgbevelx = state->extrassgbevel;

  if (!o->specialoutline) strokebevelx = state->strokebevelnop;
  if (!o->specialoutlineextra) extrassgbevelx = state->extrassgbevelnop;


  GeglNode *thebevel = state->thebevel; 
    switch (o->typebevel) {
    case GEGL_BEVEL_NORMAL: thebevel = state->thebevel; break;
    case GEGL_BEVEL_SHARP: thebevel = state->thebevelsharp; break;
    default: thebevel = state->thebevel;

}

         gegl_node_link_many (state->input, state->thinbold, state->microblur, mcol, state->nopb, bevelmode, state->nopglass, glassover,  state->nopextrassg, state->knockoutidref, state->nopig, over, state->nopstrokebehind, strokebehindx, behindextrassgx, state->ds, state->xor, state->repairgeglgraph, state->output, NULL);
      /* Nodes relating to knockout text */
      gegl_node_link_many (state->knockoutidref, state->opacityinput, NULL);
      gegl_node_connect (state->xor, "aux", state->opacityinput, "output");
      /* Nodes relating to bevel of text */
      gegl_node_link_many (state->nopb, thebevel, state->thebevelopacity,  NULL);
      gegl_node_connect (bevelmode, "aux", state->thebevelopacity, "output");
      /* Nodes relating to inner glow */
      gegl_node_link_many (state->nopig, innerglow, NULL);
      gegl_node_connect (over, "aux", innerglow, "output");
      /* Nodes relating to extra outline shadow glow */
      gegl_node_link_many (state->nopextrassg, extrassgx, extrassgbevelx, NULL);
      gegl_node_connect (behindextrassgx, "aux", extrassgbevelx, "output");
      /* Nodes relating to outline */
      gegl_node_link_many (state->nopstrokebehind, strokex, strokebevelx, NULL);
      gegl_node_connect (strokebehindx, "aux", strokebevelx, "output");
      /* Nodes relating to Glass Over Text start here */
      gegl_node_link_many (state->nopglass,  glassovertext, NULL);
      gegl_node_connect (glassover, "aux", glassovertext, "output");

  }

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglNode *input, *output,  *thebevel, *thebevelopacity, *mcol2, *thebevelsharp, *nopig, *multiplyb, *over, *nopcolor, *grainextract, *dstinglow, *nopinglow,  *hslcolorig, *overlayig, *softlightig, *screenig, *linearlightig, *multiplyig, *grainmerge, *grainmergeig, *addition, *subtract,  *nopb,  *stroke, *innerglow,  *ds,  *hardlightig, *burnig, *lchcolorig, *shinygm, *colordodge, *hardlight, *screen,  *microblur, *thinbold, *opacityinput, *xor, *knockoutidref, *beveloff, *extrassg, *extrassgbevel, *nopextrassg, *behindextrassg,  *strokebehind, *nopstrokebehind, *strokebevel, *strokebevelnop, *extrassgbevelnop, *behindextrassgdst, *strokebehinddst, *strokenop, *extrassgnop, *nopdsbehind, *dsbehind, *glassovertext, *glassover, *glassovertextoff, *glassoveroff, *nopglass, *repairgeglgraph, *hueig;




  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");


/* All nodes relating to outline begin here.

  zzstrokebevelis a hidden operation; recreation of gimp's drop shadow filter with heavy modification so it can do things like bevel outlines, aura seeds 
gegl effects use to use the default drop shadow until Beaver realized that the only way to give GEGL Effects styles seen in Adobe's layer effects was to abandon gegl:dropshadow and
make a enhanced version of it. */
  stroke = gegl_node_new_child (gegl,
                                  "operation", "lb:ssg",
                                  NULL);

  strokebevel = gegl_node_new_child (gegl,
                                  "operation", "lb:custom-bevel",
                                  NULL);


strokebehind = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 29, "composite-mode", 0, NULL);


  nopstrokebehind = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

/* All nodes relating to outline end here. */



/* All nodes relating to shadow begin here. */


  /* zzstrokebevel is a hidden operation; recreation of gimp's drop shadow filter with heavy modification so it can do things like bevel outlines, aura seeds.
gegl effects use to use the default drop shadow until Beaver realized that the only way to give GEGL Effects styles seen in Adobe's layer effects was to abandon gegl:dropshadow and
make a enhanced version of it. */
  ds = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow",
                                  NULL);

dsbehind = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 29, "composite-mode", 0, NULL);

  /* This nop acts as a ref and id for the placement of zzstrokebevel in the gegl graph. */
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



/* Blend modes for color overlay*/

  /*multiply is the blend mode that will need to be switchable for gegl:color-overlay*/

  mcol2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);


  nopcolor = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);


/* All nodes relating to color overlay end here*/



/* All nodes relating to the bevel start here*/

  thebevel = gegl_node_new_child (gegl,
                                  "operation", "lb:bevel", 
                                  NULL);

  thebevelsharp = gegl_node_new_child (gegl,
                                  "operation", "lb:bevel", "type", 1,  
                                  NULL);
 /*The Threshold Alpha setting of the bevel is being baked in so it isn't present in the GUI.*/

  thebevelopacity = gegl_node_new_child (gegl,
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
                                  "operation", "lb:innerglow", "mode", 4, 
                                  NULL);

  dstinglow = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst",
                                  NULL);

  nopinglow = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
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
                              "operation", "gimp:layer-mode", "layer-mode", 45, "composite-mode", 0,  NULL);

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



/* All nodes relating to Extra Outline, Shadw Glow begin here*/


extrassg = gegl_node_new_child (gegl,
                                  "operation", "lb:ssg",
                                  NULL);

extrassgbevel = gegl_node_new_child (gegl,
                                  "operation", "lb:custom-bevel",
                                  NULL);

behindextrassg = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 29, "composite-mode", 0, NULL);

/*This nop functions as a ref and id for extra stroke shadow glows placement*/
  nopextrassg = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

/* All nodes relating to Extra Outline, Shadw Glow end here*/



  /*Misc nodes (shiny text, micro blur and thin to thick text) that have no other nodes to go with them go here  */
  thinbold = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "abyss-policy",     0,
                                  NULL);


  microblur = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur", "clip-extent", FALSE,  "abyss-policy", 0,
                                  NULL);


  repairgeglgraph      = gegl_node_new_child (gegl, "operation", "gegl:median-blur", "abyss-policy",     0,
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
                                  "operation", "lb:glassovertext",
                                  NULL);


  nopglass = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  /* Glass Over Text blend modes */
  glassover = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);

  glassoveroff = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst",
                                  NULL);


  glassovertextoff = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

/*more nops dsts I'm not lazy to organize'*/
strokebevelnop = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

extrassgnop = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

extrassgbevelnop = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

behindextrassgdst = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst",
                                  NULL);

strokebehinddst = gegl_node_new_child (gegl,
                                  "operation", "gegl:dst",
                                  NULL);


strokenop = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

 /* All nodes relating to glass on text end here */

  /*Fourth full node listing. This is for GEGL operation associations. This is organize for convience but will work even if completely unorganized*/

  /*Beginning of Shadow's GUI asociations*/
  gegl_operation_meta_redirect (operation, "x", ds, "x");
  gegl_operation_meta_redirect (operation, "y", ds, "y");
  gegl_operation_meta_redirect (operation, "opacity", ds, "opacity");
  gegl_operation_meta_redirect (operation, "grow_radius", ds, "grow-radius");
  gegl_operation_meta_redirect (operation, "radius", ds, "radius");
  gegl_operation_meta_redirect (operation, "color", ds, "color");
  /*End of Shadow's GUI asociations*/

  /*Beginning of Outline's GUI asociations*/
  gegl_operation_meta_redirect (operation, "ystroke", stroke, "y");
  gegl_operation_meta_redirect (operation, "xstroke", stroke, "x");
  gegl_operation_meta_redirect (operation, "grow_shape", stroke, "grow-shape");
  gegl_operation_meta_redirect (operation, "opacitystroke", stroke, "opacityssg");
  gegl_operation_meta_redirect (operation, "radiusstroke", stroke, "blurstroke");
  gegl_operation_meta_redirect (operation, "grow_radiusstroke", stroke, "stroke");
  gegl_operation_meta_redirect (operation, "osdepth", strokebevel, "depth");
  gegl_operation_meta_redirect (operation, "osradius", strokebevel, "gaus");
  gegl_operation_meta_redirect (operation, "osbevel", strokebevel, "elevation");
  gegl_operation_meta_redirect (operation, "colorstroke", stroke, "colorssg");
  gegl_operation_meta_redirect (operation, "blendmodebeveloutline", strokebevel, "blendmode");
  /*End of Outline's GUI asociations*/

  /*Beginning of Bevel's GUI asociations*/
  gegl_operation_meta_redirect (operation, "depth", thebevel, "bevel2");
  gegl_operation_meta_redirect (operation, "depth", thebevelsharp, "bevel2");
  gegl_operation_meta_redirect (operation, "gaus", thebevel, "radius1");
  gegl_operation_meta_redirect (operation, "elevation", thebevelsharp, "bevel1");
  gegl_operation_meta_redirect (operation, "elevation", thebevel, "bevel1");
  gegl_operation_meta_redirect (operation, "slideupblack", thebevel, "slideupblack");
  gegl_operation_meta_redirect (operation, "bevelopacity", thebevelopacity, "value");
  /*End of Bevel's GUI asociations*/

  /*Beginning of Inner Glow's GUI asociations*/
  gegl_operation_meta_redirect (operation, "innerggrow_radius", innerglow, "grow-radius");
  gegl_operation_meta_redirect (operation, "innergradius", innerglow, "radius");
  gegl_operation_meta_redirect (operation, "innergopacity", innerglow, "opacity");
  gegl_operation_meta_redirect (operation, "innergvalue", innerglow, "value2");
  gegl_operation_meta_redirect (operation, "xinnerglow", innerglow, "x");
  gegl_operation_meta_redirect (operation, "yinnerglow", innerglow, "y");
  /*End of Inner Glow's GUI asociations*/


  /*Beginning of Color Overlay's GUI asociations*/
  gegl_operation_meta_redirect (operation, "optioncolor", mcol2, "value");
  /*End of Color Overlay's GUI asociations*/

  /*Beginning of Micro Blur's GUI asociations*/
  gegl_operation_meta_redirect (operation, "microblur", microblur, "std-dev-x");
  gegl_operation_meta_redirect (operation, "microblur", microblur, "std-dev-y");
  /*End of Micro Blur's GUI asociations*/

  /*Beginning of thin to thick text GUI asociations*/
  gegl_operation_meta_redirect (operation, "thinboldenable", thinbold, "radius");
  gegl_operation_meta_redirect (operation, "thinboldap", thinbold, "alpha-percentile");
  /*End of thin to thick text GUI asociations*/

  /*Beginning of Extra Outline Shadow Glow's GUI asociations*/

  gegl_operation_meta_redirect (operation, "yextraoutline", extrassg, "y");
  gegl_operation_meta_redirect (operation, "xextraoutline", extrassg, "x");
  gegl_operation_meta_redirect (operation, "opacitystrokeextraoutline", extrassg, "opacityssg");
  gegl_operation_meta_redirect (operation, "radiusstrokeextraoutline", extrassg, "blurstroke");
  gegl_operation_meta_redirect (operation, "grow_radiusstrokeextraoutline", extrassg, "stroke");
  gegl_operation_meta_redirect (operation, "colorstrokeextraoutline", extrassg, "colorssg");
  gegl_operation_meta_redirect (operation, "grow_shapeextraoutline", extrassg, "grow-shape");
  gegl_operation_meta_redirect (operation, "blendmodebeveloutlineextra", extrassgbevel, "blendmode");
  gegl_operation_meta_redirect (operation, "osdepthextra", extrassgbevel, "depth");
  gegl_operation_meta_redirect (operation, "osradiusextra", extrassgbevel, "gaus");
  gegl_operation_meta_redirect (operation, "osbevelextra", extrassgbevel, "elevation");
  /*End of Extra Outline Shadow Glow's GUI asociations*/


  /*Beginning of Knock Out Text's GUI asociations*/
  gegl_operation_meta_redirect (operation, "knockout", opacityinput, "value");
  /*End of Knock Out Text's GUI asociations*/


  /*Beginning of Glass Over Text's GUI asociations*/
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

/*More nops/dsts that I don't give a F about organizing'*/

  state->strokebevelnop = strokebevelnop;     
  state->extrassgnop = extrassgnop;   
  state->extrassgbevelnop = extrassgbevelnop;   
  state->behindextrassgdst = behindextrassgdst;   
  state->strokebehinddst = strokebehinddst;
  state->strokenop = strokenop;

  /*All Nodes relating to Inner Glow start here*/
  state->innerglow = innerglow;
  state->over = over;
  state->dstinglow = dstinglow;
  state->nopinglow = nopinglow;
  state->nopig = nopig;
  state->hardlightig = hardlightig;
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
  state->thebevelsharp = thebevelsharp;
  state->colordodge = colordodge;
  state->hardlight = hardlight;
  state->screen = screen;
  state->beveloff = beveloff;
  state->thebevel = thebevel;
  state->addition = addition;
  state->subtract = subtract;
  state->grainmerge = grainmerge;
  state->grainextract = grainextract;
  state->shinygm = shinygm;
  state->thebevelopacity = thebevelopacity;
  state->nopb = nopb;
  /*All Nodes relating to Bevel end here*/

  /*All Nodes relating to Color Overlay start here*/
  state->mcol2 = mcol2;
  state->nopcolor = nopcolor;
  /*All Nodes relating to Color Overlay end here*/

  /*All Nodes relating to Outline start here*/
  state->stroke = stroke;
  state->strokebevel = strokebevel;
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
  state->extrassgbevel = extrassgbevel;
  state->behindextrassg = behindextrassg;
  state->nopextrassg = nopextrassg;
  /*All Nodes relating extra outline shadow glow end here*/

  /*All Nodes relating knock out text start here*/
  state->xor = xor;
  state->knockoutidref = knockoutidref;
  state->opacityinput = opacityinput;
  /*All Nodes relating to knock out text end here*/

  /*All Nodes with no relationships (shiny text, micro blur, thin to thick slider) start here*/
  state->microblur = microblur;
  state->thinbold = thinbold;
  state->repairgeglgraph = repairgeglgraph;
  /*All Nodes with no relationships (shiny text, micro blur, thin to thick slider) end here*/

  /*All Nodes relating to glass text start here*/
  state->glassovertext = glassovertext;
  state->glassover = glassover;
  state->glassovertextoff = glassovertextoff;
  state->glassoveroff = glassoveroff;
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
 /*btw, if GEGL Effects (or any of my plugins) ever breaks try changing the name space from gegl: or lb: to something else.*/
  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:layereffects2",
    "title",       _("GEGL Effects (SaintGEGL Edition)"),
    "reference-hash", "ommanipadmehum",
    "description", _("GEGL text styling and specialty image outlining filter"),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("GEGL Effects (SaintGEGL Edition)..."),
    NULL);
}

#endif
