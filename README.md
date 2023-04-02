GEGL EFFECTS by Beaver. Layer Effects Text Styling Gimp Plugin
=========

##  GEGL Effects Continual Edition (a branch with more advance features and routine updates) here
https://github.com/LinuxBeaver/GEGL-Effects---Layer-Effects-in-Gimp-using-GEGL/tree/ContinualEdition

----------------------------
GEGL effects is a GEGL plugin for Gimp that does layer effects. It may not be non-destructive but 
you can make presets and get live preview updates of your favorite text styles. 

GEGL Effects comes with two bonus filters called gegl:bevel and gegl:innerglow 
which are more useful on their own as they have more options. NOTE - It will NOT work without bevel.so and inner-glow.so


Instructions - If you choose not to use prebuilt binaries.
You must manually compile innerglow, bevel and effects and put the .so/dll files in gegl-0.4/plug-ins. Then restart Gimp and go to Gimp's GEGL operations. 

Once you get done compiling each operation go to its build direcory and get the
.so file and place it in /gegl-04/plugins. Click build_linux.sh to compile on Linux.


## Previews 

GEGL Effects blue text, black outline, black shadow, and linear light white inner glow.
![image preview](GE.png )

GEGL Effects green text, pink outline, bevel, black shadow
![image preview](effects4.png )

GEGL Bevel on its own with the multiply blend mode (Note this is a Windows XP theme for Linux)
![image preview](bevel_preview.png )

GEGL Inner Glow on its own with the Overlay blend mode
![image preview](innerglow_preview.png )

## Quick Guide 
1. Color change will only work correctly if the text is white. This is because it uses a color overlay operation on the multiply blend mode. The multiply blend mode changes colors that are white - but will NOT correctly change any other color.

2. You must slide up the opacity slider of Outline to use the outline stroke. Sliding it down again hides it as if it were never present.

2b. You must slide opacity of Shadow/Glow up to use Shadow and Glow option. Sliding it down again hides it as if it were never present.

2c. You must slide bevel depth up to use most of the bevel option. Sliding it down again hides it as if it were never present.

3. Making X and Y 0.0 on Shadow can make a glow or extra stroke depending on the blur radius. This is the same as Gimp's drop shadow filter.

4. Bevel lighting angle is on 90 by default which is not an interesting option so consider tweaking it. 

5. Inner Glow and Gradient Overlay have checkboxes to enable them. Inner Glow will slow down GEGL Effects unless you have a really powerful machine.

6. YOu can use Gimp's rectangle select around the text to speed things up. That way the filter only applies on part of the canvas instead of the entire canvas.

7. Inner Glow, Outline and Shadow can do a triple stroke effect if set to the right option but it is smarter to just use a designated filter like my custom gegl:color-trail which allows up to 5 strokes. To do something like that. 

## Instructions 
If you choose not to use release binaries you must manually compile effects, innerglow, and bevel and put the .so/dll files in gegl-0.4/plug-ins. Then restart Gimp and go to Gimp's GEGL operations.

Once you get done compiling each operation go to its build direcory and get the
.so file and place it in gegl-04/plugins. Click build_linux.sh to compile on Linux.


## OS specific location to put GEGL Filter binaries 

Windows
C:\Users\USERNAME\AppData\Local\gegl-0.4\plug-ins

or C:\Users\AppData\Local\gegl-0.4\plug-ins

*you may need to create the "plug-ins" folder if it doesn't exist.

 Linux 
 /home/(USERNAME)/.local/share/gegl-0.4/plug-ins
 
 Linux (Flatpak)
 /home/(USERNAME)/.var/app/org.gimp.GIMP/data/gegl-0.4/plug-ins


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

Have fun BTFOing photoshop users.




