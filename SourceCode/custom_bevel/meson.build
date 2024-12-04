project('cbevel', 'c',
  version : '0.1',
  license : 'GPL-3.0-or-later')

# These arguments are only used to build the shared library
# not the executables that use the library.
lib_args = ['-DBUILDING_GEGLACTIONLINES']

gegl = dependency('gegl-0.3', required : false)
if not gegl.found()
    gegl = dependency('gegl-0.4')
endif
shlib = shared_library('cbevel', 'cbevel.c', 'config.h',
  c_args : lib_args,
  dependencies : gegl,
  name_prefix : '',
)

# Make this library usable as a Meson subproject.
stroke_dep = declare_dependency(
  include_directories: include_directories('.'),
  link_with : shlib)

