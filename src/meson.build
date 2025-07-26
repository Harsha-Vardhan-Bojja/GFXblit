# src/meson.build

srcs = [
  'gfx_blit.cpp',
  'gfx_blitter.cpp',
  'gfx_context.cpp',
]

# Required system dependencies
libdrm_dep       = dependency('libdrm', required: true)
gbm_dep          = dependency('gbm', required: true)
egl_dep          = dependency('egl', required: true)
gles2_dep        = dependency('glesv2', required: true)
opengl_dep       = dependency('gl', required: false)     # Optional
wayland_client   = dependency('wayland-client', required: false)
wayland_server   = dependency('wayland-server', required: false)
wayland_egl      = dependency('wayland-egl', required: false)

all_deps = [
  libdrm_dep,
  gbm_dep,
  egl_dep,
  gles2_dep,
]

# Add optional deps if found
if opengl_dep.found()
  all_deps += opengl_dep
endif

if wayland_client.found()
  all_deps += wayland_client
endif

if wayland_server.found()
  all_deps += wayland_server
endif

if wayland_egl.found()
  all_deps += wayland_egl
endif

# Include directories
incs = include_directories('.', '../include')

# Shared Library
libgfxblit_shared = shared_library(
  'gfxblit',
  sources: srcs,
  include_directories: incs,
  dependencies: all_deps,
  install: true
)

# Static Library
libgfxblit_static = static_library(
  'gfxblit',
  sources: srcs,
  include_directories: incs,
  dependencies: all_deps,
  install: true
)

# Exportable dependency
gfxblit_dep = declare_dependency(
  include_directories: incs,
  link_with: libgfxblit_shared,
  dependencies: all_deps
)
