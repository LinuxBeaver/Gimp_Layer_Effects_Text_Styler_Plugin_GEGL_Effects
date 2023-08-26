**THIS IS A MODIFIED BRANCH OF GEGL EFFECTS CE it cannot co-exist with default GEGL Effects CE.** 

It is the same as normal GEGL Effects CE with the exception of an 8th tab in its menu drop down list that includes a list of all major text styling plugins of mine that can be used inside GEGL Effects. So it requires a lot more plugins then just bevel, inner glow and glass over text. This version of GEGL Effects needs to overwrite normal GEGL Effects CE, they cannot co-exist. This branch is assumed stable. 

To example things simple. GEGL Effects got an update that allows it to use most of my other individual text styling plugins. You can run plugins of mine like "Glossy Balloon" "Custom Bevel" and "Sharp Bevel " inside GEGL Effects.  After putting the binaries in the instructed place restart Gimp and go to GEGL Operations and select the plugins from the list. If you use Gimp 2.99 you will find it in the main menu under filters>text styling.

If you already have any or most of these binaries overide all of them with the latest version.

**Preconfigured Windows Binaries can be found here** (over write anything that you already have)

https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/blob/GEGL-Effects-Continual-Experimental-update/GEGL_Effects_aug_26_2023_Windows_Binaries.zip



**Preconfigured Linux Binaries can be found here** (over write anything that you already have)

https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/blob/GEGL-Effects-Continual-Experimental-update/GEGL_Effects_aug_26_2023_Linux_Binaries.zip


**Building From Source is easy** 

Just click the build_everything for your OS. On Windows you will need MySys2. If correctly done a folder called "LinuxBinaries" or "WindowsBinaries" will be generated with all 19 binaries needed for this special branch of GEGL Effects CE.

## Image previews of this special branch of GEGL Effects using my major text stylers.
![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/ad8df18b-a116-4984-b18c-9c0124328e44)
![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/45a13adb-d4b7-4af8-9d8f-5c334db37453)
![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/8fbde507-b0c7-4d8b-b79f-0adb6b721f77)
![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/9a28c803-459a-4ef5-b999-c90b6745d7b9)
![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/fbe71ca3-536e-46bb-a4da-af65c105a1d3)
![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/10e87a77-622c-433f-b3bf-d1fe299fbc77)
![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/636834c7-0932-4259-a880-151eb38a0e51)
![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/be1f7993-7594-4728-a68e-9bb9bd2d51e2)


## EVERYTHING BELOW IS WHAT YOU WOULD FIND ON THE PAGE OF GEGL EFFECTS CE AND IS STILL RELEVANT.

![image preview](/Image_previews/changes_GEGL_Effects_continual.png  )

## GEGL Effects CE by Beaver

GEGL effects CE is a GEGL Gimp plugin that does layer effects in Gimp. It may not be non-destructive yet but you can make presets and get live preview updates of your favorite text styles. The many stand alone filters that GEGL Effects ships with do a lot more on their own then in GEGL Effects.

## Instructions 

If you choose not to use prebuilt binaries. You must compile everything and this can be easily done by clicking on build_everything_Linux or build_everything_windows using mysys2.
https://www.msys2.org/


## About CE

This is a stable yet continual updating version of GEGL Effects that can co-exist with a designated old version of GEGL Effects (that I recommend people avoid). Presets from said old stable GEGL Effects will not work with this version .  You can find old stable here. https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/tree/main


You are expected to also download all 19 binaries in the zip folder for this plugin to work.

This plugin will work alongside a special legacy version of GEGL Effects as seen here, but it will **NOT** co-exist with normal continual edition

![image preview](coexisting_plugins.jpg  )


## Directories to put binaries and info

.dll is for Windows and .so is for Linux

Windows `C:\Users\USERNAME\AppData\Local\gegl-0.4\plug-ins`

Windows Alt `C:\Users\AppData\Local\gegl-0.4\plug-ins`

Linux `/home/(USERNAME)/.local/share/gegl-0.4/plug-ins`

Linux (Flatpak) `/home/(USERNAME)/.var/app/org.gimp.GIMP/data/gegl-0.4/plug-ins`


*note - Windows users may need to create the plug-ins folder if it doesn't exist.

![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/c1803c39-c55c-4c5c-8084-fcb01f29adf2)



## Compiling and Installing (EVEN MORE MANUALLY BY NOT USING THE CLICK TO INSTALLL)

To compile manually without my script, visit each indiviudal filters folder and run these bash commands.

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

2c. To use Bevel enable it from the "select blend or enable/disable bevel" from the drop down list.

3. Making X and Y 0.0 on Shadow can make a glow or extra stroke depending on the blur radius. This is the same as Gimp's drop shadow filter.

4. Bevel has many blend modes that do many different things and a black/image bevel mode if the text is dark or has an image file overlay.

5. Inner Glow and Gradient Overlay have checkboxes to enable them.

6. You can use Gimp's rectangle select around the text to speed things up. That way the filter only applies on part of the canvas instead of the entire canvas.

7. Inner Glow, Outline and Shadow can do a triple stroke effect if set to the right option but it is smarter to just use a designated filter like my custom gegl:color-trail which allows up to 5 strokes. To do something like that. (WE NOW HAVE AN EXTRA OUTLINE TO DO FOUR OUTLINES)

### Here are some more pics just to show off various text styles made with the Gimp Plugin GEGL Effects 
----------------
  CE can do everything the normal version can do and more.

![image preview](/Image_previews/generated_text_style.png  )

![image preview](/Image_previews/changes_continual_color_dodge_bevel.png   )

![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/095a8919-05e7-4453-95ed-647e885a1997)


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


