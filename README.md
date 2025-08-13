## This is a special build of GEGL Effects that can co-exist with classical, continual, and experimental and my personal lb:effects  

![image](https://github.com/user-attachments/assets/84d4a261-fa30-4ced-b559-0837ce688b19)

![image](https://github.com/user-attachments/assets/6ac1058f-9634-4156-a02c-37ee9a264cfc)

## GEGL Effects Saint GEGL Edition by Beaver

GEGL effects Saint GEGl Edition  is a GEGL GIMP plugin that does layer styles/text styling in GIMP like Adobe's layer fx. Allowing users to make an infisimal amount of text styling effects in GIMP.  Saint GEGL edition is a special branch that ships with and requires five additional filters (Stroke Shadow Glow, Custom Bevel, Glass on Text, Bevel, and Inner Glow) and one hidden operations (Port Load). SSG, Bevel, Inner Glow and Glass Over Text are more useful on their own as they have more options in stand alone. 

## About GEGL Effects Saint GEGL

This is a text styling/advance image outlining engine for GIMP similar to layer fx in Adobe Photoshop

When you download the plugin or compile the following co-plugins will ship with it as either binaries and or .c files, - InnerGlow , Bevel, Glass On Text, GEGL zzstrokebevelimage and port load. The plugin will not work correctly without them and all normal download methods will give you all seven binaries.  You should have- lb:effects, lb:ssg, lb:custom-bevel, lb:bevel, lb:innerglow lb:glassovertext,  port:load


## About GEGL Effects Saint GEGL Edition

its literally a special build of GEGL Effects that tries not to depend on the node gegl:crop, that is the theory behind it. It also uses existing plugin SSG and Custom Bevel that's why I don't talk about it much. Back in early 2024 during the early GIMP 2.99.19 days it servced as a version of my layer fx engine that didn't clip on text and disable on layer groups but now after updates my main layer fx engine works fine making Saint GEGL edition a tinker toy. I keep quite about this branch of GEGL Effects as for most users its just additional bloat. Though if you like it cool!

![image preview](coexisting_plugins.jpg  )


## Directories to put binaries and info

.dll is for Windows and .so is for Linux

**Windows** `C:\Users\USERNAME\AppData\Local\gegl-0.4\plug-ins`

**Windows Alt** `C:\Users\AppData\Local\gegl-0.4\plug-ins`

**Linux** `/.local/share/gegl-0.4/plug-ins`

**Linux (Flatpak)** `~.var/app/org.gimp.GIMP/data/gegl-0.4/plug-ins`

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

GIMP 2.10 users are expected to first layer to image size then open GEGL Operations and apply GEGL Effects Continual. GIMP 2.99.19/GIMP3+ users can just type in GEGL Effects or go to filters>text styling and edit text live.

## Guide on how to use GEGL Effects

This tutorial contains everything you need.

https://github.com/LinuxBeaver/Gimp_Layer_Effects_Text_Styler_Plugin_GEGL_Effects/blob/ContinualEdition/Help_Documents_for_GEGL_Effects_here/GEGL_Effects_guide_final.pdf 

## Inner Glow Bevel and Glass Over text, Custom Bevel and SSG the stand alones that ship with it

### Inner Glow
![image preview](/Image_previews/inner_glow.png    )

![image](https://github.com/LinuxBeaver/Gimp_Layer_Effects_Text_Styler_Plugin_GEGL_Effects/assets/78667207/ee3949f1-382e-4a28-8da4-e31bb82d5f77)

### Bevel
![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/b8db02a5-8e4c-4c5e-8068-1c4be09f9209)

### Glass on Text
![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/ddb15664-2e4a-4b3a-a5ff-e27d78899e74)

### SSG and Custom Bevel info here

https://github.com/LinuxBeaver/GEGL-GIMP-PLUGIN_stroke_shadow_glow/

https://github.com/LinuxBeaver/GEGL-GIMP-PLUGIN_custom_bevel/

## Downgrading to older versions of my plugins

If you choose to downgrade GEGL Effects or any of my plugins that has a GEGL Enum List (blend mode switchers) to an earlier version, you must go to `/config/GIMP/2.10/filters` and delete the
saved settings file associate with the plugin. You can find it by searching the plugins name,  If this is not done it will keep reporting that a file is corrupt and not allow you to make
presets.** **MULTIPLE VERSIONS OF ANY OF MY PLUGINS CANNOT CO-EXIST**.** Gimp will always select the earliest one.  The only exception to this is a build of GEGL Effects I provided where I renamed the ENUM list to make them co-compatible. I recommend you don't downgrade unless you know what you are doing. Downgrading plugins can break .xcf files in 2.99.17+


