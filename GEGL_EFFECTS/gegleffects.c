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
 *2022 Beaver GEGL Effects
 * 2022 BarefootLiam (for helping give Inner Glow a disable checkbox)
 */

#include "config.h"
#include <glib/gi18n-lib.h>





#ifdef GEGL_PROPERTIES

enum_start (gegl_blend_mode_type_effects)
  enum_value (GEGL_BLEND_MODE_TYPE_MULTIPLY,      "Multiply",
              N_("Multiply"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINMERGE,      "GrainMerge",
              N_("GrainMerge"))
  enum_value (GEGL_BLEND_MODE_TYPE_SUBTRACT,      "Subtract",
              N_("Subtract"))
  enum_value (GEGL_BLEND_MODE_TYPE_GRAINEXTRACT,      "GrainExtract",
              N_("GrainExtract"))
enum_end (GeglBlendModeType)




enum_start (gegl_blend_mode_type_effectsig)
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
enum_end (GeglBlendModeTypeig)

enum_start (gegl_blend_mode_type_effectsg)
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
              N_("Hardlight"))
  enum_value (GEGL_BLEND_MODE_TYPE_BURNG,      "Burn",
              N_("Burn"))
  enum_value (GEGL_BLEND_MODE_TYPE_LCHCOLORG,      "LCHColor",
              N_("LCHColor"))
enum_end (GeglBlendModeTypeg)





enum_start (guichangeenum)
enum_value   (BEAVER_UI_STROKESHADOW, "strokeshadow", N_("Outline and Shadow"))
enum_value   (BEAVER_UI_INNERGLOWBEVEL, "innerglowbevel", N_("Bevel and Inner Glow"))
enum_value   (BEAVER_UI_IMAGEGRADIENT, "imagegradient", N_("Image file upload and Gradient"))
  enum_end (guiend)



property_enum(guichange, _("Part of filter to be displayed"),
    guiend, guichangeenum,
    BEAVER_UI_STROKESHADOW)
  description(_("Change the GUI option"))

property_enum (blendmodebevel2, _("Blend Mode of Bevel's emboss'"),
    GeglBlendModeType, gegl_blend_mode_type_effects,
    GEGL_BLEND_MODE_TYPE_MULTIPLY)
  ui_meta ("visible", "guichange {innerglowbevel}")



property_file_path(src, _("Optional image file overlay upload "), "")
    description (_("Source image file path (png, jpg, raw, svg, bmp, tif, ...)"))
ui_meta ("visible", "guichange {imagegradient}")

property_double(layeropacity, _("Opacity of Image File Overlay"), 1.0)
    value_range (0.0, 1.0)
ui_meta ("visible", "guichange {imagegradient}")


property_color  (optioncolor, _("Optional Color Overlay"), "#ffffff")
  ui_meta ("visible", "guichange {strokeshadow}")



property_int (depth, _("Bevel Depth --ENABLE BEVEL"), 1)
    description (_("Filter width"))
    value_range (1, 70)
  ui_meta ("visible", "guichange {innerglowbevel}")

property_double (bevel1, _("Rotate Bevel Lighting (90 resets)"), 90.0)
    description (_("Elevation angle (degrees)"))
    value_range (55, 125)
    ui_meta ("unit", "degree")
  ui_meta ("visible", "guichange {innerglowbevel}")


property_double (radius1, _("Radius of Bevel"), 7.0)
  value_range (1.0, 12.0)
  ui_range (1.0, 12)
  ui_gamma (1.5)
  ui_meta ("visible", "guichange {innerglowbevel}")

property_double (opacitystroke, _("Outline's Opacity --ENABLE OUTLINE"), 0.0)
  value_range   (0.0, 1.0)
  ui_steps      (0.01, 0.10)
  ui_meta ("visible", "guichange {strokeshadow}")


/* Should correspond to GeglMedianBlurNeighborhood in median-blur.c */
enum_start (gegl_stroke_grow_shapes)
  enum_value (GEGL_stroke_GROW_SHAPE_SQUARE,  "square",  N_("Square"))
  enum_value (GEGL_stroke_GROW_SHAPE_CIRCLE,  "circle",  N_("Circle"))
  enum_value (GEGL_stroke_GROW_SHAPE_DIAMOND, "diamond", N_("Diamond"))
enum_end (GeglstrokeGrowShapes)


property_double (xstroke, _("Outline X Position"), 0.0)
  description   (_("Horizontal shadow offset"))
  ui_range      (-10.0, 10.0)
  ui_steps      (1, 10)
  ui_meta ("visible", "guichange {strokeshadow}")



property_double (ystroke, _("Outline Y Position"), 0.0)
  description   (_("Vertical shadow offset"))
  ui_range      (-10.0, 10.0)
  ui_steps      (1, 10)
  ui_meta ("visible", "guichange {strokeshadow}")





property_enum   (grow_shape, _("Grow shape"),
                 GeglstrokeGrowShapes, gegl_stroke_grow_shapes,
                 GEGL_stroke_GROW_SHAPE_CIRCLE)
  description   (_("The shape to expand or contract the stroke in"))
  ui_meta ("visible", "guichange {strokeshadow}")

property_double (radiusstroke, _("Outline's Blur radius"), 0.0)
  value_range   (0.0, 3)
  ui_range      (0.0, 3.0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  ui_meta ("visible", "guichange {strokeshadow}")


property_double (grow_radiusstroke, _("Outline's Grow radius"), 12.0)
  value_range   (0.0, 100.0)
  ui_range      (0.0, 50.0)
  ui_digits     (0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the stroke before blurring; a negative value will contract the stroke instead"))
  ui_meta ("visible", "guichange {strokeshadow}")




property_color  (colorstroke, _("Outline's Color"), "#000000")
    /* TRANSLATORS: the string 'black' should not be translated */
  description   (_("The shadow's color (defaults to 'black')"))
  ui_meta ("visible", "guichange {strokeshadow}")

property_double (opacity, _("Shadow/Glow Opacity --ENABLE SHADOW/GLOW"), 0.0)
  value_range   (0.0, 1.0)
  ui_steps      (0.01, 0.10)
  ui_meta ("visible", "guichange {strokeshadow}")

property_double (hue, _("Hue"),  0.0)
   description  (_("Hue adjustment"))
   value_range  (-180.0, 180.0)
ui_meta ("visible", "guichange {imagegradient}")

property_double (chroma, _("Chroma"), 0.0)
   description  (_("Chroma adjustment"))
   value_range  (-100.0, 100.0)
ui_meta ("visible", "guichange {imagegradient}")

property_double (lightness, _("Lightness"), 0.0)
   description  (_("Lightness adjustment"))
   value_range  (-100.0, 100.0)
ui_meta ("visible", "guichange {imagegradient}")



property_double (x, _("Shadow/Glow X"), 10.0)
  description   (_("Horizontal shadow offset"))
  ui_range      (-40.0, 40.0)
  ui_steps      (1, 10)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "x")
  ui_meta ("visible", "guichange {strokeshadow}")

property_double (y, _("Shadow/Glow Y"), 10.0)
  description   (_("Vertical shadow offset"))
  ui_range      (-40.0, 40.0)
  ui_steps      (1, 10)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "y")
  ui_meta ("visible", "guichange {strokeshadow}")


property_color  (color, _("Shadow/Glow Color"), "black")
    /* TRANSLATORS: the string 'black' should not be translated */
  description   (_("The shadow's color (defaults to 'black')"))
  ui_meta ("visible", "guichange {strokeshadow}")

/* It does make sense to sometimes have opacities > 1 (see GEGL logo
 * for example)
 */




property_double (grow_radius, _("Shadow/Glow Grow radius"), 0.0)
  value_range   (0.0, 100.0)
  ui_range      (0.0, 50.0)
  ui_digits     (0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the shadow before blurring; a negative value will contract the shadow instead"))
  ui_meta ("visible", "guichange {strokeshadow}")

property_double (radius, _("Shadow/Glow Blur radius"), 12.0)
  value_range   (0.0, G_MAXDOUBLE)
  ui_range      (0.0, 300.0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  ui_meta ("visible", "guichange {strokeshadow}")


property_boolean (innerglow, _("Enable Inner Glow"), FALSE)
  description   (_("Whether to add an inner glow effect, which can be slow"))
  ui_meta ("visible", "guichange {innerglowbevel}")

property_enum (blendmodeinnerglow2, _("Blend Mode of Inner Glow"),
    GeglBlendModeTypeig, gegl_blend_mode_type_effectsig,
    GEGL_BLEND_MODE_TYPE_OVER)
  ui_meta ("visible", "guichange {innerglowbevel}")


property_double (innergradius, _("Inner Glow's Blur radius"), 4.5)
  value_range   (0.0, 30.0)
  ui_range      (0.0, 30.0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  ui_meta ("visible", "guichange {innerglowbevel}")


property_double (innerggrow_radius, _("Inner Glow's Grow radius"), 5)
  value_range   (1, 30.0)
  ui_range      (1, 30.0)
  ui_digits     (0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the shadow before blurring; a negative value will contract the shadow instead"))
  ui_meta ("visible", "guichange {innerglowbevel}")

property_double (innergopacity, _("Inner Glow's opacity"), 1.4)
  value_range   (0.0, 2.0)
  ui_steps      (0.01, 0.10)
  ui_meta ("visible", "guichange {innerglowbevel}")



property_color (innergvalue, _("Inner Glow's Color"), "#ff8f00")
    description (_("The color to paint over the input"))
  ui_meta ("visible", "guichange {innerglowbevel}")


property_double  (fixoutline, _("Inner Glow's unmodified pixel fix"), 75)
  value_range (50, 85)
  description (_("Neighborhood alpha percentile"))
  ui_meta ("visible", "guichange {innerglowbevel}")



property_boolean (gradient, _("Enable Gradient"), FALSE)
  description   (_("Whether to add Gradient overlay"))
ui_meta ("visible", "guichange {imagegradient}")

property_enum (blendmodegradient2, _("Blend Mode of Gradient"),
    GeglBlendModeTypeg, gegl_blend_mode_type_effectsg,
    GEGL_BLEND_MODE_TYPE_OVER)
  ui_meta ("visible", "guichange {imagegradient}")


property_double (start_x, _("Gradient X1"), 659.0)
    ui_meta("unit", "pixel-coordinate")
    ui_meta("axis", "x")
ui_meta ("visible", "guichange {imagegradient}")


property_double (start_y, _("Gradient Y1"), 49.0)
    ui_meta("unit", "pixel-coordinate")
    ui_meta("axis", "y")
ui_meta ("visible", "guichange {imagegradient}")

property_double (end_x, _("Gradient X2"), 647.0)
    ui_meta("unit", "pixel-coordinate")
    ui_meta("axis", "x")
ui_meta ("visible", "guichange {imagegradient}")

property_double (end_y, _("Gradient Y2"), 572.0)
    ui_meta ("unit", "pixel-coordinate")
    ui_meta ("axis", "y")
ui_meta ("visible", "guichange {imagegradient}")

property_color (start_color, _("Gradient Start Color"), "#34ebd6")
    description (_("The color at (x1, y1)"))
ui_meta ("visible", "guichange {imagegradient}")


property_color  (end_color, _("Gradient End Color"), "#fe18f2")
    description (_("The color at (x2, y2)"))
ui_meta ("visible", "guichange {imagegradient}")




#else

#define GEGL_OP_META
#define GEGL_OP_NAME     gegleffects
#define GEGL_OP_C_SOURCE gegleffects.c

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
} State;

static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  GeglNode *multiplyb;
  GeglNode *over;
  GeglNode *atopg;

  if (!state) return;

  multiplyb = state->multiply; /* the default. Bevel looks bad with most blend modes (outside of Multiply and Grain Merge) */
  switch (o->blendmodebevel2) {
    case GEGL_BLEND_MODE_TYPE_MULTIPLY: multiplyb = state->multiplyb; break;
    case GEGL_BLEND_MODE_TYPE_GRAINMERGE: multiplyb = state->grainmerge; break;
    case GEGL_BLEND_MODE_TYPE_SUBTRACT: multiplyb = state->subtract; break;
    case GEGL_BLEND_MODE_TYPE_GRAINEXTRACT: multiplyb = state->grainextract; break;

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
         gegl_node_link_many (state->input, state->nopimage, state->atopi, state->nopg, atopg,  state->crop, state->nopb, multiplyb, state->nopm, state->multiply,  state->nopig, over, state->stroke, state->ds, state->output, NULL);
      gegl_node_link_many (state->nopimage, state->image, state->saturation, NULL);
      gegl_node_link_many (state->nopig, state->innerglow, NULL);
      gegl_node_link_many (state->nopb, state->mbd, NULL);
      gegl_node_link_many (state->nopm, state->mcol, NULL);
      gegl_node_connect (multiplyb, "aux", state->mbd, "output");
      gegl_node_connect (state->multiply, "aux", state->mcol, "output");
      gegl_node_connect (over, "aux", state->innerglow, "output");
      gegl_node_connect (atopg, "aux", state->gradient, "output");
      gegl_node_connect (state->atopi, "aux", state->saturation, "output");
    }
    else
    {
      /* innerglow but no gradient */
         gegl_node_link_many (state->input, state->nopimage, state->atopi,  state->crop, state->nopb, multiplyb, state->nopm, state->multiply,  state->nopig, over, state->stroke, state->ds, state->output, NULL);
      gegl_node_link_many (state->nopimage, state->image, state->saturation, NULL);
      gegl_node_link_many (state->nopig, state->innerglow, NULL);
      gegl_node_link_many (state->nopb, state->mbd, NULL);
      gegl_node_link_many (state->nopm, state->mcol, NULL);
      gegl_node_connect (multiplyb, "aux", state->mbd, "output");
      gegl_node_connect (state->multiply, "aux", state->mcol, "output");
      gegl_node_connect (over, "aux", state->innerglow, "output");
      gegl_node_connect (state->atopi, "aux", state->saturation, "output");
    }
  }
  else
  {
    if (o->gradient)
    {
      /* gradient but no innerglow */
         gegl_node_link_many (state->input, state->nopimage, state->atopi, state->nopg, atopg,  state->crop, state->nopb, multiplyb, state->nopm, state->multiply, state->stroke, state->ds, state->output, NULL);
      gegl_node_link_many (state->nopimage, state->image, state->saturation, NULL);
      gegl_node_link_many (state->nopb, state->mbd, NULL);
      gegl_node_link_many (state->nopm, state->mcol, NULL);
      gegl_node_connect (multiplyb, "aux", state->mbd, "output");
      gegl_node_connect (state->multiply, "aux", state->mcol, "output");
      gegl_node_connect (atopg, "aux", state->gradient, "output");
      gegl_node_connect (state->atopi, "aux", state->saturation, "output");
    }
    else
    {
      /* neither gradient nor innerglow */
   gegl_node_link_many (state->input, state->nopimage, state->atopi, state->crop, state->nopb, multiplyb, state->nopm, state->multiply, state->stroke, state->ds, state->output, NULL);
      gegl_node_link_many (state->nopimage, state->image, state->saturation, NULL);
      gegl_node_link_many (state->nopb, state->mbd, NULL);
      gegl_node_link_many (state->nopm, state->mcol, NULL);
      gegl_node_connect (multiplyb, "aux", state->mbd, "output");
      gegl_node_connect (state->multiply, "aux", state->mcol, "output");
      gegl_node_connect (state->atopi, "aux", state->saturation, "output");
    }
  }
}


 /* End of Typedef Structure for Bevel's blend mode switching '*/



 /*Typedef Structure for InnerGlow and Gradient Checkboxes '*/



static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);
  GeglNode *input, *output, *image, *mbd, *nopig, *multiplyb, *nopm, *over, *multiply, *grainextract, *hslcolorig, *overlayig, *softlightig, *screenig, *linearlightig, *multiplyig, *grainmerge, *grainmergeig, *addition, *subtract,  *nopb, *mcol, *stroke, *innerglow, *gradient, *crop, *ds,  *nopimage, *atopi, *nopg, *atopg,  *hslcolorg, *overlayg, *additiong, *softlightg, *screeng, *multiplyg, *hsvhueg, *linearlightg, *grainmergeg, *saturation, *hardlightg, *hardlightig, *burnig, *lchcolorg, *lchcolorig, *burng;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");

  saturation = gegl_node_new_child (gegl,
                                  "operation", "gegl:hue-chroma",
                                  NULL);


  image = gegl_node_new_child (gegl,
                                  "operation", "gegl:layer",
                                  NULL);

  stroke = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow",
                                  NULL);

  ds = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow",
                                  NULL);

  innerglow = gegl_node_new_child (gegl,
                                  "operation", "lb:innerglow",
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
                                  "operation", "lb:bevel", "th", 0.100,
                                  NULL);

  mcol = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
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

  nopig = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

  nopg = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);


  /*multiply is the blend mode that will need to be switchable for gegl:color-overlay*/

  multiply = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);

  /*multiplyb is the blend mode that will need to be switchable for gegl:blendablebevel*/

  multiplyb = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);

  gradient = gegl_node_new_child (gegl,
                                  "operation", "gegl:linear-gradient",
                                  NULL);

    /* 0 is Gimp blend mode (Normal) */
    /* 47 is Gimp blend mode (Grain Merge) */
    /* 23 is Gimp blend mode (Overlay) */

  /*over = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 0, "composite-mode", 0, NULL);*/

  /*over is the blend mode that will need to be switchable for gegl:innergloweffects*/

  over = gegl_node_new_child (gegl,
                                  "operation", "gegl:src-atop",
                                  NULL);
  crop = gegl_node_new_child (gegl,
                                  "operation", "gegl:crop",
                                  NULL);

/* grainextract, grainmerge, and subtract are for bevel and cannot be reused*/

grainextract = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 46, "composite-mode", 0, NULL);


grainmerge = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 47, "composite-mode", 0, NULL);

subtract = gegl_node_new_child (gegl,
                              "operation", "gimp:layer-mode", "layer-mode", 34, "composite-mode", 0, NULL);
/* grainextract, grainmerge, and subtract are for bevel and cannot be reused - ends here */

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


  gegl_node_link_many (input, nopimage, atopi, nopg, atopg, crop, nopb, multiplyb, nopm, multiply, nopig, over, stroke, ds, output, NULL);
  gegl_node_link_many (nopimage, image, NULL);
 gegl_node_link_many (nopig, innerglow, NULL);
 gegl_node_link_many (nopb, mbd, NULL);
 gegl_node_link_many (nopm, mcol, NULL);
  gegl_node_connect (multiplyb, "aux", mbd, "output");
  gegl_node_connect (multiply, "aux", mcol, "output");
  gegl_node_connect (over, "aux", innerglow, "output");
  gegl_node_connect (atopi, "aux", image, "output");
  gegl_node_connect (atopg, "aux", gradient, "output");


  gegl_operation_meta_redirect (operation, "layeropacity", image, "opacity");
  gegl_operation_meta_redirect (operation, "hue", saturation, "hue");
  gegl_operation_meta_redirect (operation, "chroma", saturation, "chroma");
  gegl_operation_meta_redirect (operation, "lightness", saturation, "lightness");
  gegl_operation_meta_redirect (operation, "x", ds, "x");
  gegl_operation_meta_redirect (operation, "y", ds, "y");
  gegl_operation_meta_redirect (operation, "ystroke", stroke, "y");
  gegl_operation_meta_redirect (operation, "xstroke", stroke, "x");
  gegl_operation_meta_redirect (operation, "opacity", ds, "opacity");
  gegl_operation_meta_redirect (operation, "grow_radius", ds, "grow-radius");
  gegl_operation_meta_redirect (operation, "radius", ds, "radius");
  gegl_operation_meta_redirect (operation, "color", ds, "color");
  gegl_operation_meta_redirect (operation, "grow_shape", stroke, "grow-shape");
  gegl_operation_meta_redirect (operation, "opacitystroke", stroke, "opacity");
  gegl_operation_meta_redirect (operation, "radiusstroke", stroke, "radius");
  gegl_operation_meta_redirect (operation, "grow_radiusstroke", stroke, "grow-radius");
  gegl_operation_meta_redirect (operation, "colorstroke", stroke, "color");
  gegl_operation_meta_redirect (operation, "depth", mbd, "bevel2");
  gegl_operation_meta_redirect (operation, "radius1", mbd, "radius1");
  gegl_operation_meta_redirect (operation, "bevel1", mbd, "bevel1");
  gegl_operation_meta_redirect (operation, "optioncolor", mcol, "value");
  gegl_operation_meta_redirect (operation, "src", image, "src");
  gegl_operation_meta_redirect (operation, "innerggrow_radius", innerglow, "grow-radius");
  gegl_operation_meta_redirect (operation, "innergradius", innerglow, "radius");
  gegl_operation_meta_redirect (operation, "innergopacity", innerglow, "opacity");
  gegl_operation_meta_redirect (operation, "innergvalue", innerglow, "value2");
  gegl_operation_meta_redirect (operation, "fixoutline", innerglow, "fixoutline");
  gegl_operation_meta_redirect (operation, "start_x", gradient, "start-x");
  gegl_operation_meta_redirect (operation, "start_y", gradient, "start-y");
  gegl_operation_meta_redirect (operation, "end_x", gradient, "end-x");
  gegl_operation_meta_redirect (operation, "end_y", gradient, "end-y");
  gegl_operation_meta_redirect (operation, "start_color", gradient, "start-color");
  gegl_operation_meta_redirect (operation, "end_color", gradient, "end-color");


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
    "name",        "lb:layereffects",
    "title",       _("GEGL Effects Classic"),
    "categories",  "Generic",
    "reference-hash", "45ed565h8500fca01b2ac",
    "description", _("GEGL text styling and speciality image outlining filter from early 2023 before major overhauls. Text recoloring only works if the color is white.  "
                     ""),
    NULL);
}

#endif
