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
 * 2022 Beaver (GEGL rock text)
 */

/*
GEGL Graph recreation of Rock Text so users can test without installing.
This may not be 100% accurate but it is close enough.

#rock text
median-blur radius=10 alpha-percentile=70
noise-spread amount-x=22 amount-y=42
gaussian-blur std-dev-y=2.5 std-dev-x=2.5
color-overlay value=#ff8100
id=1
src-atop aux=[ ref=1 layer ]
gimp:layer-mode layer-mode=multiply aux=[ ref=1 emboss depth=12 elevation=9 azimuth=4 ]
gimp:threshold-alpha
unsharp-mask scale=1.2

Yes, Rock Text and Rock Text 2 should co-exist and they are different filters.
 */


#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

property_enum(guichange, _("Part of filter to be displayed:"),
    guiendrockfun, guichangerockfun,
    ROCKTEXT_SHOW_DEFAULT)
  description(_("Change the GUI option"))


enum_start (guichangerockfun)
enum_value   (ROCKTEXT_SHOW_DEFAULT, "default", N_("Basic Sliders"))
enum_value   (ROCKTEXT_SHOW_LEGACY, "legacy", N_("Legacy Sliders for technical users"))
  enum_end (guiendrockfun)


property_enum (rockblend, _("Blend mode of rock emboss"),
    againnicherock, nicherock,
    multiply)
   description  (_("Blend mode of the rock's emboss"))


enum_start (nicherock)
  enum_value (multiply,      "multiply",
              N_("Multiply"))
  enum_value (hardlight,      "hardlight",
              N_("Hard Light"))
enum_end (againnicherock)


property_int  (size, _("Internal Median Blur Radius"), 0)
  value_range (0, 10)
  ui_range    (0, 10)
  ui_meta     ("unit", "pixel-distance")
  description (_("Neighborhood radius, a negative value will calculate with inverted percentiles"))
ui_meta ("visible", "guichange {legacy}")

property_double  (alpha_percentile, _("Internal Median Blur Alpha percentile"), 28)
  value_range (0, 100)
  description (_("Neighborhood alpha percentile"))
ui_meta ("visible", "guichange {legacy}")

property_int    (amountx, _("Rockification Horizontal"), 33)
    description (_("Internal horizontal spread amount that makes the bevel rocky"))
    value_range (0, 112)
    ui_meta     ("unit", "pixel-distance")
    ui_meta     ("axis", "x")
  ui_range    (0.0, 70.0)

property_int    (amounty, _("Rockification Vertical"), 33)
    description (_("Internal vertical spread amount that makes the bevel rocky"))
    value_range (0, 112)
    ui_meta     ("unit", "pixel-distance")
    ui_meta     ("axis", "y")
  ui_range    (0.0, 70.0)

property_seed (seed, _("Random seed"), rand)
    description(_("Seed of the Rock Text"))


property_double (gaussian, _("Internal Gaussian Blur"), 1.8)
   description (_("Gaussian Blur to expand the rock text"))
   value_range (0.24, 4.0)
   ui_range    (0.24, 4.0)
   ui_gamma    (3.0)
   ui_meta     ("unit", "pixel-distance")
   ui_meta     ("axis", "x")
ui_meta ("visible", "guichange {legacy}")

property_int  (shift, _("Horizontal Shift"), 1)
    description(_("Apply an internal shift filter inside the rock text"))
    value_range (0, 10)
    ui_meta    ("unit", "pixel-distance")
ui_meta ("visible", "guichange {legacy}")

property_seed (seed2, _("Random seed"), rand2)
    description(_("Another Seed of the rock text"))
ui_meta ("visible", "guichange {legacy}")

property_int  (size2, _("Crumbled mini rocks effect "), 2)
  value_range (0, 10)
  ui_range    (0, 10)
  ui_meta     ("unit", "pixel-distance")
  description (_("Crumble effect, lower values means more crumbling mini rocks. Higher means a more orderly rock."))


property_double (azimuth, _("Light Rotation"), 180.0)
    description (_("Light angle (degrees) of the internal emboss"))
    value_range (0, 360)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")

property_double (elevation, _("Elevation"), 80.0)
    description (_("Elevation angle (degrees) of the internal emboss"))
    value_range (0, 180)
    ui_meta ("unit", "degree")

property_int (depth, _("Depth"), 20)
    description (_("Depth of the internal emboss"))
    value_range (20, 100)
    ui_meta ("unit", "degree")

property_double (grains, _("Grains over rock"), 0.8)
  value_range   (0.0, 1.0)


property_color  (mvalue, _("Color Overide"), "#9a7143")
    description (_("Color of the rock text"))

property_file_path(src, _("Image overlay (use white text)"), "")
    description (_("Image file overlays work proper on white colored text. Source image file paths accepted are (png, jpg, raw, svg, bmp, tif, ...)"))
ui_meta ("visible", "guichange {legacy}")

property_double (x, _("X outline"), 1.0)
  description   (_("Horizontal shadow offset"))
  ui_range      (-6.0, 6.0)
  ui_steps      (1, 10)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "x")
ui_meta ("visible", "guichange {legacy}")

property_double (y, _("Y outline"), 1.0)
  description   (_("Vertical shadow offset"))
  ui_range      (-6.0, 6.0)
  ui_steps      (1, 10)
  ui_meta       ("unit", "pixel-distance")
  ui_meta       ("axis", "y")
ui_meta ("visible", "guichange {legacy}")

property_double (radius, _("Outline Blur radius"), 0.5)
  value_range   (0.0, G_MAXDOUBLE)
  ui_range      (0.0, 1.0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
ui_meta ("visible", "guichange {legacy}")

property_double (grow_radius, _("Outline Grow radius"), 0)
  value_range   (-5.0, 5.0)
  ui_range      (-5.0, 5.0)
  ui_digits     (0)
  ui_steps      (1, 5)
  ui_gamma      (1.5)
  ui_meta       ("unit", "pixel-distance")
  description (_("The distance to expand the shadow before blurring; a negative value will contract the shadow instead"))
ui_meta ("visible", "guichange {legacy}")

property_color  (color, _("Outline Color"), "black")
    /* TRANSLATORS: the string 'black' should not be translated */
  description   (_("The shadow's color (defaults to 'black')"))
ui_meta ("visible", "guichange {legacy}")

/* It does make sense to sometimes have opacities > 1 (see GEGL logo
 * for example)
 */
property_double (opacity, _("Outline Opacity"), 0.0)
  value_range   (0.0, 1.0)
  ui_steps      (0.01, 0.10)
ui_meta ("visible", "guichange {legacy}")

property_double (exposure, _("Darkness to light"), 0.5)
    description (_("Exposure's darkness to light"))
    ui_range    (-2.0, 2.0)
ui_meta ("visible", "guichange {legacy}")



#else

#define GEGL_OP_META
#define GEGL_OP_NAME     rock_text
#define GEGL_OP_C_SOURCE rock-text.c

#include "gegl-op.h"

typedef struct
{
  GeglNode *input;
  GeglNode *output;
  GeglNode *color;
  GeglNode *median;
  GeglNode *median2;
  GeglNode *gaussian;
  GeglNode *noise;
  GeglNode *exposure;
  GeglNode *alpha;
  GeglNode *shift;
  GeglNode *emboss;
  GeglNode *normal;
  GeglNode *graph;
  GeglNode *opacity;
  GeglNode *edgesmooth;
  GeglNode *smooth;
  GeglNode *multiply;
  GeglNode *mcol;
  GeglNode *hardlight;
  GeglNode *grainmerge;
  GeglNode *colordodge;
  GeglNode *nop;
  GeglNode *nop2;
  GeglNode *outline;
  GeglNode *coloroverlay;
  GeglNode *imagefileupload;
  GeglNode *image;
  GeglNode *idref;
}State;


static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);

  State *state = o->user_data = g_malloc0 (sizeof (State));


 GeglColor *rock_hidden_color = gegl_color_new ("#ffffff");

      state->input    = gegl_node_get_input_proxy (gegl, "input");
      state->output   = gegl_node_get_output_proxy (gegl, "output");


     state->color    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay", "value", rock_hidden_color,
                                  NULL);

      state->median    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur",  "abyss-policy", 0,
                                  NULL);


       state->median2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "abyss-policy", 0,
                                  NULL);


       state->gaussian    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur", "clip-extent", 0, "abyss-policy", 0,
                                  NULL);

       state->noise    = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-spread",
                                  NULL);

       state->exposure    = gegl_node_new_child (gegl,
                                  "operation", "gegl:exposure",
                                  NULL);

       state->normal    = gegl_node_new_child (gegl,
                                  "operation", "gegl:over",
                                  NULL);
#define mysyntax \
" id=1 gimp:layer-mode layer-mode=hsv-value  opacity=1 aux=[  ref=1 noise-hsv holdness=7 hue-distance=0 saturation-distance=0 value-distance=0.90 ]   "\


      state->graph    = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", mysyntax, 
                                  NULL);


      state->opacity    = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);




      state->edgesmooth    = gegl_node_new_child (gegl,
                                  "operation", "lb:edgesmooth", "alpha-percentile2", 67.0, "gaus", 0.75,  "value", 2.2, "abyss-policy", 0, 
                                  NULL);



      state->alpha    = gegl_node_new_child (gegl,
                                  "operation", "gimp:threshold-alpha",
                                  NULL);

      state->shift   = gegl_node_new_child (gegl,
                                  "operation", "gegl:shift",
                                  NULL);

     state->emboss    = gegl_node_new_child (gegl,
                                  "operation", "gegl:emboss",
                                  NULL);


     state->mcol    = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);

     state->image    = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);

     state->nop    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

     state->multiply    = gegl_node_new_child (gegl,
                                  "operation", "gegl:multiply",
                                  NULL);

     state->hardlight    = gegl_node_new_child (gegl,
                                  "operation", "gegl:hard-light",
                                  NULL);

     state->colordodge    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-dodge",
                                  NULL);

     state->grainmerge    = gegl_node_new_child (gegl,
                                  "operation", "gimp:layer-mode", "layer-mode", 47, "composite-mode", 0,
                                  NULL);


     state->smooth    = gegl_node_new_child (gegl,
                                  "operation", "gegl:mean-curvature-blur", "iterations", 1,
                                  NULL);


     state->nop2    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

     state->idref    = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);


     state->imagefileupload    = gegl_node_new_child (gegl,
                                  "operation", "port:load",
                                  NULL);

    state->coloroverlay    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);

      state->outline    = gegl_node_new_child (gegl,
                                  "operation", "gegl:dropshadow",
                                  NULL);


  gegl_operation_meta_redirect (operation, "size", state->median, "radius");
  gegl_operation_meta_redirect (operation, "shift", state->shift, "shift");
  gegl_operation_meta_redirect (operation, "seed2", state->shift, "seed");
  gegl_operation_meta_redirect (operation, "amountx", state->noise, "amount-x");
  gegl_operation_meta_redirect (operation, "amounty", state->noise, "amount-y");
  gegl_operation_meta_redirect (operation, "seed", state->noise, "seed");
  gegl_operation_meta_redirect (operation, "size2", state->median2, "radius");
  gegl_operation_meta_redirect (operation, "gaussian", state->gaussian, "std-dev-x");
  gegl_operation_meta_redirect (operation, "gaussian", state->gaussian, "std-dev-y");
  gegl_operation_meta_redirect (operation, "azimuth", state->emboss, "azimuth");
  gegl_operation_meta_redirect (operation, "elevation", state->emboss, "elevation");
  gegl_operation_meta_redirect (operation, "depth", state->emboss, "depth");
  gegl_operation_meta_redirect (operation, "alpha-percentile", state->median, "alpha-percentile");
  gegl_operation_meta_redirect (operation, "radius", state->outline, "radius");
  gegl_operation_meta_redirect (operation, "opacity", state->outline, "opacity");
  gegl_operation_meta_redirect (operation, "x", state->outline, "x");
  gegl_operation_meta_redirect (operation, "y", state->outline, "y");
  gegl_operation_meta_redirect (operation, "grow_radius", state->outline, "grow-radius");
  gegl_operation_meta_redirect (operation, "color", state->outline, "color");
  gegl_operation_meta_redirect (operation, "exposure", state->exposure, "exposure");
  gegl_operation_meta_redirect (operation, "mvalue", state->coloroverlay, "value");
  gegl_operation_meta_redirect (operation, "src", state->imagefileupload, "src");
  gegl_operation_meta_redirect (operation, "grains", state->opacity, "value");


}


static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;

switch (o->rockblend) {
        break;
    case multiply:
  gegl_node_link_many (state->input, state->color, state->median, state->noise, state->gaussian, state->shift, state->median2, state->idref, state->multiply, state->alpha, state->image, state->outline, state->nop, state->mcol,  state->nop2, state->normal, state->smooth,  state->exposure, state->edgesmooth, state->output, NULL);
  gegl_node_connect (state->image, "aux", state->imagefileupload, "output");
  gegl_node_connect (state->mcol, "aux", state->coloroverlay, "output");
  gegl_node_connect (state->normal, "aux", state->opacity, "output");
  gegl_node_link_many (state->nop2, state->graph, state->opacity, NULL);
  gegl_node_link_many (state->nop, state->coloroverlay, NULL);
  gegl_node_connect (state->multiply, "aux", state->emboss, "output");
  gegl_node_link_many (state->idref, state->emboss, NULL);
        break;
    case hardlight:
  gegl_node_link_many (state->input, state->color, state->median, state->noise, state->gaussian, state->shift, state->median2, state->idref, state->hardlight, state->alpha, state->image, state->outline, state->nop, state->mcol,  state->nop2, state->normal, state->smooth,  state->exposure, state->edgesmooth, state->output, NULL);
  gegl_node_connect (state->image, "aux", state->imagefileupload, "output");
  gegl_node_connect (state->mcol, "aux", state->coloroverlay, "output");
  gegl_node_connect (state->normal, "aux", state->opacity, "output");
  gegl_node_link_many (state->nop2, state->graph, state->opacity, NULL);
  gegl_node_link_many (state->nop, state->coloroverlay, NULL);
  gegl_node_connect (state->hardlight, "aux", state->emboss, "output");
  gegl_node_link_many (state->idref, state->emboss, NULL);
        break;
    }
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
    "name",        "lb:rock-text",
    "title",       _("Rock Text"),
    "reference-hash", "30do6a1h24f10jcjf25sb2ac",
    "description", _("Make rock shaped text "
                     ""),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Rock Text (original)..."),
    NULL);
}

#endif
