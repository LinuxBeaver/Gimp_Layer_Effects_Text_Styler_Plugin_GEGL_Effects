## This is a special build of GEGL Effects that can co-exist with classical, continual, and experimental GEGL Effects. 

This build is special  because it works on adjustment layer groups in GIMP 3. Normal GEGL Effects works fine GIMP 3 without any crashing but it can't run on adjustment layer groups. So use this build for said task.

![image](https://github.com/user-attachments/assets/84d4a261-fa30-4ced-b559-0837ce688b19)

![image](https://github.com/user-attachments/assets/6ac1058f-9634-4156-a02c-37ee9a264cfc)


## GEGL Effects CE by Beaver

GEGL effects CE is a GEGL Gimp plugin that does layer effects/text styling in Gimp. Allowing users to make all their favorite text styles. GEGL Effects CE ships with and requires three bonus filters, and two hidden operations. You will find that gegl:bevel, gegl:innerglow and gegl:glassovertext are more useful on their own as they have more options in stand alone. 

## Instructions 

Build by clicking `build_os.sh` then restart Gimp and go to Gimp's GEGL operation to find "GEGL Effects Saint GEGL Edition"


## About GEGL Effects CE

In the early days this was the experimental version of GEGL Effects before becoming the default. Now this is a stable yet continual updating version of GEGL Effects that can co-exist with a designated old version of GEGL Effects (that I recommend people discard). Presets from said old stable GEGL Effects will not work with this version .  You can find old stable here. https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/tree/main

You are expected to also download GEGL InnerGlow, GEGL Bevel GEGL Glass On Text and  GEGL zzstrokebevelimage GEGL Shiny Text. The plugin will not work correctly without them.
In total you should have, gegleffectspending, glassovertext, shinytext, zzstrokebevelimage, bevel and innerglow for this filter to work.  

Example of plugin working alongside a special legacy version of GEGL Effects as seen here.

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

## Notes about using GEGL Effects on 2.10 vs GIMP 2.99/3

Gimp 2.10 users are expected to first layer to image size then open GEGL Operations and apply GEGL Effects Continual. GIMP 2.99.19/GIMP3+ users can just type in GEGL Effects or go to filters>text styling and edit text live.

## Guide on how to use GEGL Effects

This tutorial contains everything you need.

https://github.com/LinuxBeaver/Gimp_Layer_Effects_Text_Styler_Plugin_GEGL_Effects/blob/ContinualEdition/Help_Documents_for_GEGL_Effects_here/GEGL_Effects_guide_final.pdf 

## Picture gallery of GEGL Effects 

![image preview](/Image_previews/changes_GEGL_Effects_continual.png  )

![image preview](/Image_previews/changes_continual_color_dodge_bevel.png   )

![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/095a8919-05e7-4453-95ed-647e885a1997)

![image preview](/Image_previews/continual_pink.png    )

![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/69cacb6a-04e5-4539-b245-1b99857a4403)

![image preview](/Image_previews/flatpak2.png    )


## Inner Glow Bevel and Glass Over text, the stand alones

### Inner Glow
![image preview](/Image_previews/inner_glow.png    )

![image](https://github.com/LinuxBeaver/Gimp_Layer_Effects_Text_Styler_Plugin_GEGL_Effects/assets/78667207/ee3949f1-382e-4a28-8da4-e31bb82d5f77)

### Bevel
![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/b8db02a5-8e4c-4c5e-8068-1c4be09f9209)

### Glass on Text
![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/ddb15664-2e4a-4b3a-a5ff-e27d78899e74)


## Downgrading to older versions of my plugins

If you choose to downgrade GEGL Effects or any of my plugins that has a GEGL Enum List (blend mode switchers) to an earlier version, you must go to `/config/GIMP/2.10/filters` and delete the
saved settings file associate with the plugin. You can find it by searching the plugins name,  If this is not done it will keep reporting that a file is corrupt and not allow you to make
presets.** **MULTIPLE VERSIONS OF ANY OF MY PLUGINS CANNOT CO-EXIST**.** Gimp will always select the earliest one.  The only exception to this is a build of GEGL Effects I provided where I renamed the ENUM list to make them co-compatible. I recommend you don't downgrade unless you know what you are doing. Downgrading plugins can break .xcf files in 2.99.17+


