If you want this plugin in Gimp officially show support here.
https://gitlab.gnome.org/GNOME/gimp/-/issues/9327

Ready to use Binaries for Windows and Linux are avalible in the "Releases" Section. 
https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/releases/tag/Continual_July22_2023

After putting the binaries in the instructed place restart Gimp and go to GEGL Operations and select the plugins from the list. 

![image preview](/Image_previews/changes_GEGL_Effects_continual.png  )

## GEGL Effects CE by Beaver

GEGL effects CE is a GEGL Gimp plugin that does layer effects in Gimp. It may not be non-destructive yet but you can make presets and get live preview updates of your favorite text styles. GEGL Effects CE ships with and requires three bonus filters, and two hidden operations. You will find that gegl:bevel, gegl:innerglow and gegl:glassovertext are more useful on their own as they have more options in stand alone. 

## Instructions 

If you choose not to use prebuilt binaries. You must compile innerglow, glassovertext, zzstrokebevelimage, shinytext, bevel and gegleffects and put the .so/dll files in gegl-0.4/plug-ins. Then restart Gimp and go to Gimp's GEGL operations. You can effortlessly compile just by clicking on the sh file respectively for Windows (mysys2) or Linux


## About CE

This is a stable yet continual updating version of GEGL Effects that can co-exist with a designated old version of GEGL Effects (that i recommend people avoid). Presets from said old stable GEGL Effects will not work with this version .  You can find old stable here. https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/tree/main


You are expected to also download GEGL InnerGlow, GEGL Bevel GEGL Glass On Text and  GEGL zzstrokebevelimage GEGL Shiny Text. The plugin will not work correctly without them.
In total you should have, gegleffectspending, glassovertext, shinytext, zzstrokebevelimage, bevel and innerglow for this filter to work.  

This plugin will work alongside a special legacy version of GEGL Effects as seen here.

![image preview](coexisting_plugins.jpg  )



## Directories to put binaries and info

.dll is for Windows and .so is for Linux

Windows `C:\Users\USERNAME\AppData\Local\gegl-0.4\plug-ins`

Windows Alt `C:\Users\AppData\Local\gegl-0.4\plug-ins`

Linux `/home/(USERNAME)/.local/share/gegl-0.4/plug-ins`

Linux (Flatpak) `/home/(USERNAME)/.var/app/org.gimp.GIMP/data/gegl-0.4/plug-ins`


*note - Windows users may need to create the plug-ins folder if it doesn't exist.




## Compiling and Installing

### Linux

To compile and install you will need the GEGL header files (`libgegl-dev` on
Debian based distributions or `gegl` on Arch Linux) and meson (`meson` on
most distributions).

```bash
meson setup --buildtype=release build
ninja -C build

```

If you have an older version of gegl you may need to copy to `~/.local/share/gegl-0.3/plug-ins`
instead (on Ubuntu 18.04 for example).

### Windows

The easiest way to compile this project on Windows is by using msys2.  Download
and install it from here: https://www.msys2.org/

Open a msys2 terminal with `C:\msys64\mingw64.exe`.  Run the following to
install required build dependencies:

```bash
pacman --noconfirm -S base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-meson mingw-w64-x86_64-gegl
```

Then build the same way you would on Linux:

```bash
meson setup --buildtype=release build
ninja -C build
```

## Quick Guide 
0. This is meant to be applied on text layers or raster text/image. It does live previews but does NOT actively apply when typing text.  Lots of people are let down by this but that is just the way it is. In the future when Gimp gets non-destructive editing and a GEGL text plugin then it will in theory be possible. 

1. Color change will only work correctly if the text is white. This is because it uses a color overlay operation on the multiply blend mode. The multiply blend mode changes colors that are white - but will NOT correctly change any other color. (THIS IS STILL COMPLETELY RELEVANT BUT WE NOW HAVE A SOLID COLOR OPTION FOR COLOR OVERLAY)

2. You must enable the checkbox to use the outline but it also has an opacity slider that hides it.

2b. You must slide opacity of Shadow/Glow up to use Shadow and Glow option. Sliding it down again hides it as if it were never present.

2c. You must slide bevel depth up to use most of the bevel option. Sliding it down again hides it as if it were never present. (THIS IS NOT RELEVANT ANYMORE)

3. Making X and Y 0.0 on Shadow can make a glow or extra stroke depending on the blur radius. This is the same as Gimp's drop shadow filter.

4. Bevel lighting angle is on 90 by default which is not an interesting option so consider tweaking it. (THIS IS NOT RELEVANT ANYMORE)

5. Inner Glow and Gradient Overlay have checkboxes to enable them. Inner Glow will slow down GEGL Effects unless you have a really powerful machine.

6. You can use Gimp's rectangle select around the text to speed things up. That way the filter only applies on part of the canvas instead of the entire canvas.

7. Inner Glow, Outline and Shadow can do a triple stroke effect if set to the right option but it is smarter to just use a designated filter like my custom gegl:color-trail which allows up to 5 strokes. To do something like that. (WE NOW HAVE AN EXTRA OUTLINE TO DO FOUR OUTLINES)



### Here are some more pics just to show off various text styles made with the Gimp Plugin GEGL Effects 
----------------
  CE can do everything the normal version can do and more.

![image preview](/Image_previews/generated_text_style.png  )

![image preview](/Image_previews/changes_continual_color_dodge_bevel.png   )

![image preview](/Image_previews/changes_bevel_outline_and_.image_overlay.png   )

![image preview](/Image_previews/continual_pink.png    )

![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/69cacb6a-04e5-4539-b245-1b99857a4403)


![image preview](/Image_previews/flatpak2.png    )

![image preview](/Image_previews/example_preset.png    )



### Inner Glow Bevel and Glass Over text, the stand alones
![image preview](/Image_previews/inner_glow.png    )

![image preview](/Image_previews/innerglow_promo.png    )


![image preview](/Image_previews/bevel.png    )

![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/ddb15664-2e4a-4b3a-a5ff-e27d78899e74)

### Downgrading to older versions of my plugins

If you choose to downgrade GEGL Effects or any of my plugins that has a GEGL Enum List (blend mode switchers) to an earlier version, you must go to `/config/GIMP/2.10/filters` and delete the
saved settings file associate with the plugin. You can find it by searching the plugins name,  If this is not done it will keep reporting that a file is corrupt and not allow you to make
presets.** **MULTIPLE VERSIONS OF ANY OF MY PLUGINS CANNOT CO-EXIST**.** Gimp will always select the earliest one.  The only exception to this is a build of GEGL Effects I provided where I renamed the ENUM list to make them co-compatible. I recommend you don't downgrade unless you know what you are doing.


