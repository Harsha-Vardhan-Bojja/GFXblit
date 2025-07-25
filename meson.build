project('gfxblit', ['cpp', 'c'],
  version: '0.1',
  default_options: ['cpp_std=c++17']
)

# Include path for headers
inc = include_directories('include')

# Subdir with source files
subdir('src')
subdir('tests')

# Install public headers
install_headers('include/gfx_blit.h', 'include/gfx_common_header.h', subdir: 'gfxblit')
