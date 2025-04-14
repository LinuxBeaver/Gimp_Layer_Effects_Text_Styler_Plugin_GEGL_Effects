This is the final version of GEGL Effects ever, meant to succeed GEGL Effects CE. Though it can co-exist with all other versions of GEGL Effects on my Github. 

This revision released April 14 2025 is to make GEGL Effects as polished as possible

## Preview of GEGL Effects

![image](https://github.com/user-attachments/assets/ebe7e83b-5d9e-470b-8ba8-7146f55dd3e3)

![image](https://github.com/user-attachments/assets/9a5c2d12-ce4c-46b8-a7f2-3e44d0ec7a4e)

![image](https://github.com/user-attachments/assets/7bbfbd34-ba38-4f58-9b90-e478d023867d)

![image](https://github.com/user-attachments/assets/f48ceb4b-43fb-4f93-9abf-764cdf847a17)

![image](https://github.com/user-attachments/assets/de29a91f-822d-4b06-b732-69365ffb4afd)



## GEGL Effects  by Beaver

GEGL effects  is a GEGL Gimp plugin that does layer effects/text styling in Gimp. Allowing users to make all their favorite text styles. GEGL Effects  ships with and requires three bonus filters, and two hidden operations. You will find that gegl:bevel, gegl:innerglow and gegl:glassovertext are more useful on their own as they have more options in stand alone. 


## About GEGL Effects

This is a text styling/advance image outlining engine for GIMP similar to layer fx in Adobe Photoshop

When you download the plugin or compile the following co-plugins will ship with it as either binaries or .c files, - GEGL InnerGlow, GEGL Bevel, GEGL Glass On Text, and GEGL zzstrokebevelimage. The plugin will not work correctly without them and all normal download methods will give you all five binaries. 

## Directories to put binaries and info

.dll is for Windows and .so is for Linux

Windows `C:\Users\USERNAME\AppData\Local\gegl-0.4\plug-ins`

Windows Alt `C:\Users\AppData\Local\gegl-0.4\plug-ins`

Linux `~/.local/share/gegl-0.4/plug-ins`

Linux (Flatpak) `~/.var/app/org.gimp.GIMP/data/gegl-0.4/plug-ins`

*note - Windows users may need to create the plug-ins folder if it doesn't exist. But this may be an indication you are using an old version of GIMP 2.10.

## Location of plugin
filters>text styling>GEGL Effects
or
![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/c1803c39-c55c-4c5c-8084-fcb01f29adf2)

## Compiling and Installing

./build_plugin_linux.sh and ./build_plugin_windows.sh with mysys2 should auto compile GEGL Effects and many other plugins of mine but if you want to know what the bash script is doing read below. 

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

Gimp 2.10 users are expected to first layer to image size then open GEGL Operations and apply GEGL Effects. GIMP 2.99.19/GIMP3+ users can just type in GEGL Effects or go to filters>text styling and edit text live.

## Inner Glow Bevel and Glass Over text, the stand alone co-plugins

### Inner Glow
![image preview](/Image_previews/inner_glow.png    )

![image](https://github.com/LinuxBeaver/Gimp_Layer_Effects_Text_Styler_Plugin_GEGL_Effects/assets/78667207/ee3949f1-382e-4a28-8da4-e31bb82d5f77)

### Bevel
![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/b8db02a5-8e4c-4c5e-8068-1c4be09f9209)

### Glass on Text
![image](https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/assets/78667207/ddb15664-2e4a-4b3a-a5ff-e27d78899e74)


## Downgrading to older versions of my plugins

I strongly advise against downgrading. But if you choose to downgrade GEGL Effects or any of my plugins that has a GEGL Enum List (blend mode switchers) to an earlier version, you must go to `/config/GIMP/2.10/filters` and delete the
saved settings file associate with the plugin. You can find it by searching the plugins name,  If this is not done it will keep reporting that a file is corrupt and not allow you to make
presets.** **MULTIPLE VERSIONS OF ANY OF MY PLUGINS CANNOT CO-EXIST**.** Gimp will always select the earliest one.  The only exption to this is a builds of GEGL Effects I provided where I renamed the ENUM list to make them co-compatible. I recommend you don't downgrade unless you know what you are doing. Downgrading plugins can break .xcf files in 2.99.17+ and bring back crashes from the past.


