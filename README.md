GEGL EFFECTS by Beaver. The GEGL filter you've been waiting for!
=========

A custom GEGL filter that does layer effects. It may not be non-destructive but 
you can make presets of your favorite text styles.




![image preview](GE.png )
![image preview](bevel_preview.png )

## Guide 
Color change will only work correctly if the text is white.

Setting the opacity of outline to 0 removes the stroke outline entirely and the shadow will take over.

Setting the opacity of shadow/glow to 0 removes the shadow glow.

Bevel Bumpmap is disabled by default but sliding the opacity meter up enables it.

Inner Glow and Gradient Overlay have checkboxes to enable them. Inner Glow will slow down GEGL Effects unless you have a really powerful machine.

Image file overlay will not work if a gradient is activate.

Use Gimp's rectangle select around the text to speed things up. That way the filter only applies on part of the canvas instead of the entire canvas.

## Instructions 
You must manually compile mcol, mbd, zzwhite, it, innerglow, and bevel and put the .so files in gegl-0.4/plug-ins. Then restart Gimp and go to Gimp's GEGL operations.

Once you get done compiling each operation go to its build direcory and get the
.so file and place it in gegl-04/plugins. Click build_linux.sh to compile on Linux.



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




