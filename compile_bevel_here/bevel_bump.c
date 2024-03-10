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
median-blur radius=3 alpha-percentile=100 id=0 src aux=[ ref=0 median-blur radius=0 distance-transform  color-to-alpha opacity-threshold=0.1 color=black emboss depth=4
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

property_double (radius1special, _("Radius Normal Bevel"), 2.0)
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



property_int (radius2cove, _("Radius Box Bevel"), 11)
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

property_double (bevel1special2, _("Depth Angle"), 140.0)
    description (_("Elevation angle (degrees)"))
    value_range (20, 140)
    ui_meta ("unit", "degree")
  ui_steps      (0.1, 1.50)
ui_meta ("visible", "type {stackedembossbevel}" )


property_int (bevel2special, _("Depth"), 7)
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

property_double (azimuthspecial, _("Rotate Lighting"), 40.0)
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
  value_range   (0.00, 0.999)
  ui_steps      (0.1, 0.50)
  ui_steps      (0.01, 0.50)


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
  GeglNode *normallayer;
  GeglNode *normallayer2;
  GeglNode *slideupblack;
  GeglNode *fix;
  GeglNode *dt;
  GeglNode *c2a;
  GeglNode *nop;
  GeglNode *levels;
  GeglNode *threeembosses;
  GeglNode *fourembosses;
  GeglNode *fiveembosses;
  GeglNode *replace;
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
  GeglNode *input, *output, *boxblur, *blur, *mcb, *startmedian, *lowersharpopacity,  *invert, *threeembosses, *fourembosses, *fiveembosses, *normallayer2, *hyperopacity, *emgsb, *blursb, *blurcove, *boxblurcove, *emb, *th, *levels, *embcovemove, *embcovefix, *dt, *median, *median2, *replace, *bilateral, *nop, *c2a, *fix, *whitecolor, *normallayer, *slideupblack;
  GeglColor *white_color = gegl_color_new ("#ffffff");

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");

   blur = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",  "clip-extent", FALSE, "abyss-policy", 0,
                                  NULL);

   blurcove = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur", "clip-extent", FALSE, "abyss-policy", 0,
                                  NULL);


   blursb = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur", "std-dev-x", 1.0, "std-dev-y", 1.0,  "clip-extent", FALSE, "abyss-policy", 0,
                                  NULL);


   boxblur = gegl_node_new_child (gegl,
                                  "operation", "gegl:box-blur",
                                  NULL);

   boxblurcove = gegl_node_new_child (gegl,
                                  "operation", "gegl:box-blur",
                                  NULL);


 emb   = gegl_node_new_child (gegl,
                                  "operation", "gegl:emboss",
                                  NULL);


 embcovemove   = gegl_node_new_child (gegl,
                                  "operation", "gegl:emboss",
                                  NULL);

 embcovefix   = gegl_node_new_child (gegl,
                                  "operation", "gegl:emboss", "depth", 20, "elevation", 90.0, 
                                  NULL);

 emgsb   = gegl_node_new_child (gegl,
                                  "operation", "gegl:emboss", "depth", 90,
                                  NULL);


  th = gegl_node_new_child (gegl,
                                  "operation", "gimp:threshold-alpha",
                                  NULL);


  whitecolor    = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                   "value", white_color, NULL);


normallayer = gegl_node_new_child (gegl,
                                    "operation", "gimp:layer-mode", "layer-mode", 28,  "composite-mode", 2, NULL);


normallayer2 = gegl_node_new_child (gegl,
                                    "operation", "gimp:layer-mode", "layer-mode", 28,  "composite-mode", 0, NULL);



 slideupblack   = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

 hyperopacity   = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity",
                                  NULL);

 lowersharpopacity   = gegl_node_new_child (gegl,
                                  "operation", "gegl:opacity", "value", 0.75,
                                  NULL);


 fix   = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

#define three \
" emboss depth=3 elevation=90 gaussian-blur abyss-policy=none clip-extent=false std-dev-x=0.5  std-dev-y=0.5 emboss depth=3 elevation=90 gaussian-blur abyss-policy=none  clip-extent=false std-dev-x=0.5  std-dev-y=0.5 emboss depth=3 elevation=90 "\

 threeembosses   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", three,
                                  NULL);

#define four \
" emboss depth=3 elevation=90 gaussian-blur abyss-policy=none clip-extent=false  std-dev-x=0.5  std-dev-y=0.5 emboss depth=3 elevation=60 gaussian-blur abyss-policy=none clip-extent=false  std-dev-x=0.5  std-dev-y=0.5 emboss depth=3 elevation=90 gaussian-blur abyss-policy=none clip-extent=false  std-dev-x=0.5  std-dev-y=0.5 emboss depth=3 elevation=90 "\

 fourembosses   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", four,
                                  NULL);

#define five \
" emboss depth=3 elevation=60 gaussian-blur  clip-extent=false std-dev-x=0.5  std-dev-y=0.5 emboss depth=3 elevation=60 gaussian-blur abyss-policy=none clip-extent=false  std-dev-x=0.5  std-dev-y=0.5 emboss depth=3 elevation=60 gaussian-blur abyss-policy=none clip-extent=false std-dev-x=0.5  std-dev-y=0.5 emboss depth=3 elevation=60 gaussian-blur abyss-policy=none clip-extent=false  std-dev-x=0.5  std-dev-y=0.5 emboss depth=3 elevation=60 gaussian-blur abyss-policy=none clip-extent=false  std-dev-x=0.5  std-dev-y=0.5  "\

 fiveembosses   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", five,
                                  NULL);

 nop   = gegl_node_new_child (gegl,
                                  "operation", "gegl:nop",
                                  NULL);

 invert   = gegl_node_new_child (gegl,
                                  "operation", "gegl:invert",
                                  NULL);


replace = gegl_node_new_child (gegl, "operation", "gegl:src", NULL);

 levels   = gegl_node_new_child (gegl,
                                  "operation", "gegl:levels", "in-high", 1.3, "in-low", 0.33, 
                                  NULL);

 dt   = gegl_node_new_child (gegl,
                                  "operation", "gegl:distance-transform", "metric", 2,
                                  NULL);

 bilateral   = gegl_node_new_child (gegl,
                                  "operation", "gegl:bilateral-filter", "edge-preservation", 0.0,
                                  NULL);


 c2a   = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl", "string", EMBEDDEDGRAPH,
                                  NULL);

 median   = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 3, "alpha-percentile", 100.0, "abyss-policy",     GEGL_ABYSS_NONE,
                                  NULL);

 median2   = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 0, "abyss-policy",     GEGL_ABYSS_NONE,
                                  NULL);

   mcb = gegl_node_new_child (gegl,
                                  "operation", "gegl:mean-curvature-blur", "iterations", 2,
                                  NULL);

 startmedian   = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur", "radius", 2, "alpha-percentile", 100.0, "neighborhood", 0, "abyss-policy",     GEGL_ABYSS_NONE,
                                  NULL);



  gegl_operation_meta_redirect (operation, "radius1", blur, "std-dev-x");
  gegl_operation_meta_redirect (operation, "radius1", blur, "std-dev-y");
  gegl_operation_meta_redirect (operation, "radius1special", blurcove, "std-dev-x");
  gegl_operation_meta_redirect (operation, "radius1special", blurcove, "std-dev-y");
  gegl_operation_meta_redirect (operation, "bevel1", emb, "elevation");
  gegl_operation_meta_redirect (operation, "bevel2", emb, "depth");
  gegl_operation_meta_redirect (operation, "azimuth", emb, "azimuth");
  gegl_operation_meta_redirect (operation, "azimuthspecial", embcovemove, "azimuth");
  gegl_operation_meta_redirect (operation, "bevel1special", embcovemove, "elevation");
  gegl_operation_meta_redirect (operation, "bevel2special", embcovemove, "depth");
  gegl_operation_meta_redirect (operation, "bevel1special2", emgsb, "elevation");
  gegl_operation_meta_redirect (operation, "radius2", boxblur, "radius");
  gegl_operation_meta_redirect (operation, "radius2cove", boxblurcove, "radius");
  gegl_operation_meta_redirect (operation, "th", th, "value");
  gegl_operation_meta_redirect (operation, "slideupblack", slideupblack, "value");
  gegl_operation_meta_redirect (operation, "invert", normallayer2, "opacity");
  gegl_operation_meta_redirect (operation, "smooth", bilateral, "blur-radius");
  gegl_operation_meta_redirect (operation, "metric", dt, "metric");
  gegl_operation_meta_redirect (operation, "hyperopacity", hyperopacity, "value");



 /* Now save points to the various gegl nodes so we can rewire them in
   * update_graph() later
   */
  State *state = g_malloc0 (sizeof (State));
  state->input = input;
  state->blur = blur;
  state->mcb = mcb;
  state->startmedian = startmedian;
  state->blurcove = blurcove;
  state->boxblur = boxblur;
  state->boxblurcove = boxblurcove;
  state->emb = emb;
  state->invert = invert;
  state->hyperopacity = hyperopacity;
  state->embcovefix = embcovefix;
  state->embcovemove = embcovemove;
  state->emgsb = emgsb;
  state->th = th;
  state->whitecolor = whitecolor;
  state->normallayer = normallayer;
  state->normallayer2 = normallayer2;
  state->slideupblack = slideupblack;
  state->fix = fix;
  state->median = median;
  state->blursb = blursb;
  state->median2 = median2;
  state->levels = levels;
  state->nop = nop;
  state->replace = replace;
  state->bilateral = bilateral;
  state->c2a = c2a;
  state->dt = dt;
  state->threeembosses = threeembosses;
  state->fourembosses = fourembosses;
  state->fiveembosses = fiveembosses;
  state->lowersharpopacity = lowersharpopacity;
  state->output = output;
  o->user_data = state;
}

static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;


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
            gegl_node_link_many (state->input, state->normallayer, state->blur, state->boxblur,  state->emb, state->th, state->fix, state->output, NULL);
    gegl_node_link_many (state->input, state->whitecolor, state->slideupblack,  NULL);
    gegl_node_connect (state->normallayer, "aux", state->slideupblack, "output");
        break;
    case GEGL_BEVEL_SHARP:
    gegl_node_link_many (state->input, state->normallayer, state->median, state->nop, state->replace, state->levels, state->bilateral, state->lowersharpopacity, state->output, NULL);
    gegl_node_link_many (state->nop, state->median2,  state->dt, state->c2a, state->emb,  NULL);
    gegl_node_connect (state->replace, "aux", state->emb, "output"); 
    gegl_node_link_many (state->input, state->whitecolor, state->slideupblack,  NULL);
    gegl_node_connect (state->normallayer, "aux", state->slideupblack, "output");
        break;
    case GEGL_BEVEL_EMBOSS_MODE:
    gegl_node_link_many (state->input, state->normallayer, state->blur, state->boxblur,  state->emb, state->fix, state->output, NULL);
    gegl_node_link_many (state->input, state->whitecolor, state->slideupblack,  NULL);
    gegl_node_connect (state->normallayer, "aux", state->slideupblack, "output");
        break;
    case GEGL_BEVEL_COVE_DEEP_MODE:
    gegl_node_link_many (state->input, state->normallayer, state->blurcove, state->boxblurcove,  state->embcovefix,  state->embcovemove, state->fix, state->output, NULL);
    gegl_node_link_many (state->input, state->whitecolor, state->slideupblack,  NULL);
    gegl_node_connect (state->normallayer, "aux", state->slideupblack, "output");
        break;
    case GEGL_BEVEL_EMBOSS_STACK_MODE:
    gegl_node_link_many (state->input, state->startmedian, state->normallayer, state->blursb, state->boxblurcove,  state->emgsb, state->nop, state->invert, embosschoice, state->hyperopacity, state->mcb, state->output, NULL);
    gegl_node_link_many (state->nop, state->invert,  NULL);
    gegl_node_link_many (state->input, state->whitecolor, state->slideupblack,  NULL);
    gegl_node_connect (state->normallayer, "aux", state->slideupblack, "output");
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
    "description", _("User is expected to use blending options with this plugin. Works best with blend modes multiply, hardlight and grain merge with varying opacities. Emboss mode requires non-GEGL Gimp layer blend modes."
                     ""),
    "gimp:menu-path", "<Image>/Filters/Text Styling",
    "gimp:menu-label", _("Bevel (to blend)..."),
    NULL);
}

#endif
