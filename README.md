## Downloadable binaries for Windows and Linux is here 
[https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/releases/tag/Continual_July22_2023](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/releases)

### Image Previews
![image](https://github.com/LinuxBeaver/Gimp_Layer_Effects_Text_Styler_Plugin_GEGL_Effects/assets/78667207/034f250e-fda5-4e65-95be-27bf94801a36)


![image preview](/Image_previews/changes_GEGL_Effects_continual.png  )

![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/c6874a65-d761-4994-96a4-2e46b996b07a)


## GEGL Effects CE by Beaver

GEGL effects CE is a GEGL Gimp plugin that does layer effects in Gimp. It may not be non-destructive yet but you can make presets and get live preview updates of your favorite text styles. GEGL Effects CE ships with and requires three bonus filters, and two hidden operations. You will find that gegl:bevel, gegl:innerglow and gegl:glassovertext are more useful on their own as they have more options in stand alone. 

## Instructions 

If you choose not to use prebuilt binaries. You must compile innerglow, glassovertext, zzstrokebevelimage, shinytext, bevel and gegleffects and put the .so/dll files in gegl-0.4/plug-ins. Then restart Gimp and go to Gimp's GEGL operations. You can effortlessly compile just by clicking on the sh file respectively for Windows (mysys2) or Linux


## About CE

This is a stable yet continual updating version of GEGL Effects that can co-exist with a designated old version of GEGL Effects (that I recommend people avoid). Presets from said old stable GEGL Effects will not work with this version .  You can find old stable here. https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/tree/main


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

![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/c1803c39-c55c-4c5c-8084-fcb01f29adf2)



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
**0.** This is meant to be applied on text layers or raster text/image. It does live previews but does NOT actively apply when typing text.  Lots of people are let down by this but that is just the way it is. Though in CMYK Student's build of Gimp it does live preview edit non-destructively. 

**1.** Please right click a text layer and select "layer to image size" before applying this filter. This way it prevents clipping as discussed here. Consider making a back up text layer before applying "layer to image size".  
https://github.com/LinuxBeaver/LinuxBeaver/issues/8

**2.** In default color change will only work correctly if the text is white. This is because it uses a color overlay operation on the 'multiply' blend mode. The multiply blend mode changes colors that are white - but will NOT correctly change any other color. During the Summer of 2023 GEGL Effects got an update that gave it the ability to change blend modes to "Solid Color" to override any color regardless if it is white.

**3.** You must enable the checkbox to use the outline but it also has an opacity slider that hides it when it reaches 0%.

**3b.** You must slide opacity of Shadow/Glow up to use Shadow and Glow option. Sliding it down again to 0% opacity hides it as if it were never present.

**3c.** To use Bevel enable it from the "select blend or enable/disable bevel" from the drop down list.

**4.** Making X and Y 0.0 on Shadow can make a glow or extra stroke depending on the blur radius. This is the same as Gimp's drop shadow filter. Glow, Shadow and Stroke in Adobe are technically the same thing and my plugins make that even more obvious.

**5.** Bevel has many blend modes that do many different things, so try them out. It also has a black/image bevel mode that allows a bevel even if the text color is dark or text has an image file overlay. The Black bevel slider is meant for dark colored text and image uploads over text.

**6.** Inner Glow, Gradient Overlay, Bevel on outline and other things have checkboxes to enable them because if I just made them invisible they would make GEGL Effects more resource intensive.

**7.** You can use Gimp's rectangle select around the text to speed things up. That way the filter only applies on part of the canvas instead of the entire canvas. Though beware the shadow might clip.




### Here are some more pics just to show off various text styles made with the Gimp Plugin GEGL Effects 

![image preview](/Image_previews/generated_text_style.png  )

![image preview](/Image_previews/changes_continual_color_dodge_bevel.png   )

![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/095a8919-05e7-4453-95ed-647e885a1997)


![image preview](/Image_previews/continual_pink.png    )

![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/69cacb6a-04e5-4539-b245-1b99857a4403)


![image preview](/Image_previews/flatpak2.png    )

![image preview](/Image_previews/example_preset.png    )



### Inner Glow Bevel and Glass Over text, the stand alones
![image preview](/Image_previews/inner_glow.png    )

![image](https://github.com/LinuxBeaver/Gimp_Layer_Effects_Text_Styler_Plugin_GEGL_Effects/assets/78667207/ee3949f1-382e-4a28-8da4-e31bb82d5f77)


![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/b8db02a5-8e4c-4c5e-8068-1c4be09f9209)


![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/ddb15664-2e4a-4b3a-a5ff-e27d78899e74)

## Downgrading to older versions of my plugins

If you choose to downgrade GEGL Effects or any of my plugins that has a GEGL Enum List (blend mode switchers) to an earlier version, you must go to `/config/GIMP/2.10/filters` and delete the
saved settings file associate with the plugin. You can find it by searching the plugins name,  If this is not done it will keep reporting that a file is corrupt and not allow you to make
presets.** **MULTIPLE VERSIONS OF ANY OF MY PLUGINS CANNOT CO-EXIST**.** Gimp will always select the earliest one.  The only exception to this is a build of GEGL Effects I provided where I renamed the ENUM list to make them co-compatible. I recommend you don't downgrade unless you know what you are doing.

## Experimental branch that is stable (but incompatible with this build)

GEGL Effects CE has an experimental branch here that includes many major text styling plugins of mine. It is stable to use but I feel uneasy to make it the default branch as it has a ton of new features. If you want to try it go here. This branch will over ride default GEGL Effects CE and cannot co-exist with it. So it is either normal GEGL Effects CE or the experimental branch of CE not both. 

https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/tree/GEGL-Effects-Continual-Experimental-update

![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/08f27b9c-7282-4d68-9332-f11ab32950e6)


