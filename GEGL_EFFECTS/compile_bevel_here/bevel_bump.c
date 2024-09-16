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
 * 2022 Beaver (GEGL Bevel)
 */

/*GEGL Bevel is a stand alone plugin but it is also part of GEGL Effects. The stand alone version does more then the GEGL Effects implementation of it. */


/*
Graph here to test without installing.

"normal bevel"
id=1 gimp:layer-mode layer-mode=normal composite-mode=clip-to-backdrop aux=[color value=#ffffff  opacity value=0.1 ]
gaussian-blur std-dev-x=2 std-dev-y=2
emboss azimuth=44 depth=9
gimp:threshold-alpha value=0.03

"alt emboss graph"
id=1 gimp:layer-mode layer-mode=normal composite-mode=clip-to-backdrop aux=[color value=#ffffff  opacity value=0.1 ]
gaussian-blur std-dev-x=2 std-dev-y=2
emboss azimuth=44 depth=9


"alt sharp bevel graph"
median-blur radius=3 alpha-percentile=100 id=0  aux=[ ref=0 median-blur radius=0 distance-transform  color-to-alpha opacity-threshold=0.1 color=black emboss depth=4
]
levels in-high=1.3 in-low=0.33
bilateral-filter blur-radius=3


"cove bevel graph"
gaussian-blur std-dev-x=6 std-dev-y=6
emboss depth=90 elevation=90
emboss depth=2

opacity value=1.4


"stacked emboss bevel graph"
median-blur radius=2 neighborhood=square alpha-percentile=100
id=1 gimp:layer-mode layer-mode=normal composite-mode=clip-to-backdrop aux=[color value=#ffffff  opacity value=0.1 ] median-blur radius=4 neighborhood=square alpha-percentile=100
gaussian-blur std-dev-x=7 std-dev-y=7
 box-blur  radius=11
emboss depth=90 elevation=90
invert
emboss depth=3 elevation=60 gaussian-blur std-dev-x=0.5  std-dev-y=0.5 emboss depth=3 elevation=60 gaussian-blur std-dev-x=0.5  std-dev-y=0.5 emboss depth=3 elevation=60 emboss depth=3 elevation=60
opacity value=2
mean-curvature-blur iterations=2
*/


#define EMBEDDEDGRAPH \
" color-to-alpha opacity-threshold=0.1 color=black "\

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

property_enum (type, _("Type of Bevel"),
    GeglBlendModeTypebeavbevel, gegl_blend_mode_typebeavbevel,
    GEGL_BEVEL_NORMAL)
    description (_("Types of bevels present. Sharp Bevel mode requires the user to change the depth angle to less then 70 or above 115 for it to work proper."))

property_double (radius1, _("Radius Normal Bevel"), 7.0)
  value_range (0.5, 40.0)
  ui_range (1.0, 9.0)
  ui_gamma (1.5)
  ui_steps      (0.1, 0.50)
ui_meta ("visible", "!type {sharpbevel, stackedembossbevel, covebevel}" )
    description (_("Make a normal bevel bump map using Gaussian Blur"))

property_double (radius1special, _("Radius Bevel (cove)"), 2.0)
  value_range (1.0, 8.0)
  ui_range (1.0, 8.0)
  ui_gamma (1.5)
  ui_steps      (0.1, 0.50)
ui_meta ("visible", "type {covebevel}" )
    description (_("Make a normal bevel bump map using Gaussian Blur"))

property_int (radius2, _("Radius Box Bevel"), 0)
    description (_("Make a box bevel bump map using Box Blur - this bevel is sharper looking but it is not the same as 'sharp bevel mode'. Box Blur 0 is the default setting which disables this effect by default. Both normal bevel and box bevel sliders can be used together to hybridize the bevel. "))
   value_range (0, 10.0)
   ui_range    (0, 9.0)
   ui_gamma   (1.5)
ui_meta ("visible", "!type {sharpbevel, stackedembossbevel, covebevel}" )



property_int (radius2cove, _("Radius Box Bevel (cove stacked)"), 11)
    description (_("Make a box bevel bump map using Box Blur - this bevel is sharper but it is not the same as --sharp bevel mode--. Box Blur 0 is the default setting which disables this effect by default. Both normal bevel and sharp bevel sliders can be used together to hybridiz the bevel. "))
   value_range (10.0, 16.0)
   ui_range    (10.0, 16.0)
   ui_gamma   (1.5)
ui_meta ("visible", "type {covebevel, stackedembossbevel}" )


property_double (bevel1, _("Depth Angle"), 45.0)
    description (_("Elevation angle (degrees)"))
    value_range (0, 180)
    ui_meta ("unit", "degree")
  ui_steps      (0.1, 1.50)
ui_meta ("visible", "!type {covebevel, stackedembossbevel}" )


property_int (bevel2, _("Depth"), 40)
    description (_("Emboss depth -Brings out depth and detail of the bevel"))
    value_range (1, 100)
ui_meta ("visible", "!type {covebevel, stackedembossbevel}" )

property_double (bevel1special, _("Depth Angle"), 60.0)
    description (_("Elevation angle (degrees)"))
    value_range (50, 90)
    ui_meta ("unit", "degree")
  ui_steps      (0.1, 1.50)
ui_meta ("visible", "type {covebevel}" )

property_double (bevel1special2, _("Depth Angle (stacked)"), 140.0)
    description (_("Elevation angle (degrees)"))
    value_range (20, 140)
    ui_meta ("unit", "degree")
  ui_steps      (0.1, 1.50)
ui_meta ("visible", "type {stackedembossbevel}" )


property_int (bevel2special, _("Depth (cove)"), 7)
    description (_("Emboss depth -Brings out depth and detail of the bevel"))
    value_range (1, 20)
  ui_steps      (0.1, 0.50)
  ui_steps      (0.1, 0.50)
ui_meta ("visible", "type {covebevel}" )


property_double (th, _("Bevel's coverage threshold."), 0.100)
    description (_("Internal Threshold Alpha. Lower covers more and higher covers less."))
  value_range (0.0, 1.0)
  ui_range (0.0, 0.5)
  ui_steps      (0.1, 0.50)
ui_meta ("visible", "type {normalbevel}" )


property_double (azimuth, _("Rotate Lighting"), 40.0)
    description (_("Light angle (degrees)"))
    value_range (0, 350)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")
  ui_steps      (0.1, 0.50)
ui_meta ("visible", "!type {covebevel, stackedembossbevel}" )

property_double (azimuthspecial, _("Rotate Lighting (cove)"), 40.0)
    description (_("Light angle (degrees)"))
    value_range (0, 350)
    ui_meta ("unit", "degree")
    ui_meta ("direction", "ccw")
  ui_steps      (0.1, 2.00)
ui_meta ("visible", "type {covebevel}" )

property_double (hyperopacity, _("Hyper Opacity of Stacked Embosses"), 1.5)
    description (_("This"))
  value_range   (1.5, 3.0)
  ui_steps      (0.01, 0.50)
ui_meta ("visible", "type {stackedembossbevel}" )



property_double (slideupblack, _("Black Bevel/Image Bevel mode."), 0.00)
    description (_("This slider allows GEGL bevel to works on black Bevels; but the user must still manually select blend modes like Grain Merge and Hardlight that are known to work with very dark Bevels. This also allows bevel to be applied on image file overlays without conforming to an image's details."))
  value_range   (0.00, 1.0)
  ui_steps      (0.1, 0.50)



property_enum (metric, _("Distance Map Setting"),
               GeglDistanceMetric, gegl_distance_metric, GEGL_DISTANCE_METRIC_CHEBYSHEV)
    description (_("Distance Map has three settings that alter the structure of the sharp bevel. Chebyshev is the default; due to it being the best. But try the other two. "))
ui_meta ("visible", "type {sharpbevel}" )

property_double (smooth, _("Smooth Roughness "), 2.5)
  description(_("Bilateral blur smoothes roughness  of the sharp bevel but is resource intensive at high values."))
  value_range   (0.0, 10.0)
  ui_range      (0.0, 10.0)
  ui_gamma      (1.5)
  ui_steps      (0.1, 0.50)
ui_meta ("visible", "type {sharpbevel}" )



enum_start (gegl_blend_mode_typebeavbevel)
  enum_value (GEGL_BEVEL_NORMAL,      "normalbevel",
              N_("Normal Bevel"))
  enum_value (GEGL_BEVEL_SHARP,      "sharpbevel",
              N_("Sharp Bevel"))
  enum_value (GEGL_BEVEL_EMBOSS_MODE,      "embossmode",
              N_("Emboss Bevel (requires Gimp Layers)"))
  enum_value (GEGL_BEVEL_COVE_DEEP_MODE,      "covebevel",
              N_("Cove Bevel"))
  enum_value (GEGL_BEVEL_EMBOSS_STACK_MODE,      "stackedembossbevel",
              N_("Emboss Stack Bevel"))
enum_end (GeglBlendModeTypebeavbevel)



property_enum (embossamount, _("Amount of Stacked Embosses on the Bevel:"),
    stackemboss, stack_emboss_fun,
    FIVE_EMBOSSES)
    description (_("The amount of extra embosses present in 'stacked embosses' bevel mode. Five is default, three is minimum."))
ui_meta ("visible", "type {stackedembossbevel}" )

enum_start (stack_emboss_fun)
  enum_value (THREE_EMBOSSES,      "threeembosses",
              N_("Three  Stacked Embosses"))
  enum_value (FOUR_EMBOSSES,      "fourembosses",
              N_("Four Stacked Embosses"))
  enum_value (FIVE_EMBOSSES,      "fiveembosses",
              N_("Five Stacked Embosses (absurd effect)"))
enum_end (stackemboss)

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     bevel_bump
#define GEGL_OP_C_SOURCE bevel_bump.c

#include "gegl-op.h"


typedef struct
{
  GeglNode *input;
  GeglNode *blur;
  GeglNode *mcb;
  GeglNode *startmedian;
  GeglNode *blurcove;
  GeglNode *boxblur;
  GeglNode *boxblurcove;
  GeglNode *emb;
  GeglNode *emgsb;
  GeglNode *blursb;
  GeglNode *embcovefix;
  GeglNode *embcovemove;
  GeglNode *th;
  GeglNode *whitecolor;
  GeglNode *slideupblack;
  GeglNode *fix;
  GeglNode *dt;
  GeglNode *c2a;
  GeglNode *levels;
  GeglNode *threeembosses;
  GeglNode *fourembosses;
  GeglNode *fiveembosses;
  GeglNode *invert;
  GeglNode *median;
  GeglNode *lowersharpopacity;
  GeglNode *hyperopacity;
  GeglNode *median2;
  GeglNode *bilateral;
  GeglNode *output;
} State;


static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = g_malloc0 (sizeof (State));
  o->user_data = state;


  state->input    = gegl_node_get_input_proxy (gegl, "input");
    state->output   = gegl_node_get_output_proxy (gegl, "output");

     state->blur = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",  "clip-extent", FALSE, "abyss-policy", 0,
                                  NULL);

     state->blurcove = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur", "clip-extent", FALSE, "abyss-policy", 0,
                                  NULL);


     state->blursb = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur", "std-dev-x", 1.0, "std-dev-y", 1.0,  "clip-extent", FALSE, "abyss-policy", 0,
                                  NULL);


     state->boxblur = gegl_node_new_child (gegl,
                                  "operation", "gegl:box-blur",
                                  NULL);

     state->boxblurcove = gegl_node_new_child (gegl,
                                  "operation", "gegl:box-blur",
                                  NULL);


   state->emb   = gegl_node_new_child (gegl,
                                  "operation", "gegl:emboss",
                                  NULL);


   state->embcovemove   = gegl_node_new_child (gegl,
                                  "operation", "gegl:emboss",
                                  NULL);

   state->embcovefix   = gegl_node_new_child (gegl,
                                  "operation", "gegl:emboss", "depth", 20, "elevation", 90.0,
                                  NULL);

   state->emgsb   = gegl_node_new_child (gegl,
                                  "operation", "gegl:emboss", "depth", 90,
                                  NULL);


    state->th = gegl_node_new_child (gegl,
                                  "operation", "gimp:threshold-alpha",
                                  NULL);

   state->slideupblack   = gegl_node_new_child (gegl,
                                  "operation", "gegl:levels",
                                  NULL);

   state->hyperopacity   = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

   state->lowersharpopacity   = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity", "value", 0.75,
                                  NULL);


   state->fix  = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 0, "abyss-policy",     GEGL_ABYSS_NONE,
                                  NULL);

#define three \
" emboss depth=3 elevation=90 gaussian-blur abyss-policy=none clip-extent=false std-dev-x=0.5  std-dev-y=0.5 emboss depth=3 elevation=90 gaussian-blur abyss-policy=none  clip-extent=false std-dev-x=0.5  std-dev-y=0.5 emboss depth=3 elevation=90 "\

   state->threeembosses   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", three,
                                  NULL);

#define four \
" emboss depth=3 elevation=90 gaussian-blur abyss-policy=none clip-extent=false  std-dev-x=0.5  std-dev-y=0.5 emboss depth=3 elevation=60 gaussian-blur abyss-policy=none clip-extent=false  std-dev-x=0.5  std-dev-y=0.5 emboss depth=3 elevation=90 gaussian-blur abyss-policy=none clip-extent=false  std-dev-x=0.5  std-dev-y=0.5 emboss depth=3 elevation=90 "\

   state->fourembosses   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", four,
                                  NULL);

#define five \
" emboss depth=3 elevation=60 gaussian-blur  clip-extent=false std-dev-x=0.5  std-dev-y=0.5 emboss depth=3 elevation=60 gaussian-blur abyss-policy=none clip-extent=false  std-dev-x=0.5  std-dev-y=0.5 emboss depth=3 elevation=60 gaussian-blur abyss-policy=none clip-extent=false std-dev-x=0.5  std-dev-y=0.5 emboss depth=3 elevation=60 gaussian-blur abyss-policy=none clip-extent=false  std-dev-x=0.5  std-dev-y=0.5 emboss depth=3 elevation=60 gaussian-blur abyss-policy=none clip-extent=false  std-dev-x=0.5  std-dev-y=0.5  "\

   state->fiveembosses   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", five,
                                  NULL);


   state->invert   = gegl_node_new_child (gegl,
                                  "operation", "gegl:invert",
                                  NULL);


 state->levels   = gegl_node_new_child (gegl,
                                  "operation", "gegl:levels", "in-high", 1.3, "in-low", 0.33,
                                  NULL);

   state->dt   = gegl_node_new_child (gegl,
                                  "operation", "gegl:distance-transform", "metric", 2,
                                  NULL);

   state->bilateral   = gegl_node_new_child (gegl,
                                  "operation", "gegl:bilateral-filter", "edge-preservation", 0.0,
                                  NULL);


   state->c2a   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", EMBEDDEDGRAPH,
                                  NULL);

   state->median   = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 3, "alpha-percentile", 100.0, "abyss-policy",     GEGL_ABYSS_NONE,
                                  NULL);

   state->median2   = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 0, "abyss-policy",     GEGL_ABYSS_NONE,
                                  NULL);

     state->mcb = gegl_node_new_child (gegl,
                                  "operation", "gegl:mean-curvature-blur", "iterations", 2,
                                  NULL);

   state->startmedian   = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 2, "alpha-percentile", 100.0, "neighborhood", 0, "abyss-policy",     GEGL_ABYSS_NONE,
                                  NULL);


  gegl_operation_meta_redirect (operation, "radius1", state->blur, "std-dev-x");
  gegl_operation_meta_redirect (operation, "radius1", state->blur, "std-dev-y");
  gegl_operation_meta_redirect (operation, "radius1special", state->blurcove, "std-dev-x");
  gegl_operation_meta_redirect (operation, "radius1special", state->blurcove, "std-dev-y");
  gegl_operation_meta_redirect (operation, "bevel1", state->emb, "elevation");
  gegl_operation_meta_redirect (operation, "bevel2", state->emb, "depth");
  gegl_operation_meta_redirect (operation, "azimuth", state->emb, "azimuth");
  gegl_operation_meta_redirect (operation, "azimuthspecial", state->embcovemove, "azimuth");
  gegl_operation_meta_redirect (operation, "bevel1special", state->embcovemove, "elevation");
  gegl_operation_meta_redirect (operation, "bevel2special", state->embcovemove, "depth");
  gegl_operation_meta_redirect (operation, "bevel1special2", state->emgsb, "elevation");
  gegl_operation_meta_redirect (operation, "radius2", state->boxblur, "radius");
  gegl_operation_meta_redirect (operation, "radius2cove", state->boxblurcove, "radius");
  gegl_operation_meta_redirect (operation, "th", state->th, "value");
  gegl_operation_meta_redirect (operation, "slideupblack", state->slideupblack, "out-low");
  gegl_operation_meta_redirect (operation, "smooth", state->bilateral, "blur-radius");
  gegl_operation_meta_redirect (operation, "metric", state->dt, "metric");
  gegl_operation_meta_redirect (operation, "hyperopacity", state->hyperopacity, "value");

}


static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;

  GeglNode *embosschoice = state->fiveembosses; /* the default */
  switch (o->embossamount) {
    case THREE_EMBOSSES: embosschoice = state->threeembosses; break;
    case FOUR_EMBOSSES: embosschoice = state->fourembosses; break;
    case FIVE_EMBOSSES: embosschoice = state->fiveembosses; break;
default: embosschoice = state->fiveembosses;

}

switch (o->type) {
        break;
    case GEGL_BEVEL_NORMAL:
            gegl_node_link_many (state->input, state->slideupblack, state->blur, state->boxblur,  state->emb, state->th, state->fix, state->output, NULL);
        break;
    case GEGL_BEVEL_SHARP:
    gegl_node_link_many (state->input, state->slideupblack, state->median, state->dt, state->c2a, state->emb, state->levels, state->bilateral, state->lowersharpopacity, state->output, NULL);
        break;
    case GEGL_BEVEL_EMBOSS_MODE:
    gegl_node_link_many (state->input, state->slideupblack, state->blur, state->boxblur,  state->emb, state->fix, state->output, NULL);
        break;
    case GEGL_BEVEL_COVE_DEEP_MODE:
    gegl_node_link_many (state->input, state->slideupblack, state->blurcove, state->boxblurcove,  state->embcovefix,  state->embcovemove, state->fix, state->output, NULL);
        break;
    case GEGL_BEVEL_EMBOSS_STACK_MODE:
    gegl_node_link_many (state->input, state->startmedian, state->slideupblack, state->blursb, state->boxblurcove,  state->emgsb,  state->invert, embosschoice, state->hyperopacity, state->mcb, state->output, NULL);
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
    "name",        "lb:bevel",
    "title",       _("Bevel (to blend)"),
    "reference-hash", "45ed5656a28a512570f0f25sb2ac",
    "description", _("User should use blending options with this plugin. Works best with blend modes multiply, hardlight and grain merge with varying opacities. Emboss mode requires non-GEGL Gimp layer blend modes."
                     ""),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Bevel (to blend)..."),
    NULL);
}

#endif
