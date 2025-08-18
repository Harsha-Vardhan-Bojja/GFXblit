# tests/meson.build

test_exe = executable('test_argb_to_argb',
  'test_argb_to_argb.c',
  include_directories: include_directories('..'),
  dependencies: [gfxblit_dep],
  install: false
)

test_exe = executable('test_xrgb_to_xrgb',
  'test_xrgb_to_xrgb.c',
  include_directories: include_directories('..'),
  dependencies: [gfxblit_dep],
  install: false
)

test_exe = executable('test_xrgb_to_argb',
  'test_xrgb_to_argb.c',
  include_directories: include_directories('..'),
  dependencies: [gfxblit_dep],
  install: false
)

test_exe = executable('test_argb_to_xrgb',
  'test_argb_to_xrgb.c',
  include_directories: include_directories('..'),
  dependencies: [gfxblit_dep],
  install: false
)

test('gfxblit tests', test_exe)