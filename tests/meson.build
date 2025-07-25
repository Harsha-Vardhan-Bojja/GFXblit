# tests/meson.build

test_exe = executable('simple_test',
  'simple_test.c',
  include_directories: include_directories('..'),
  dependencies: [gfxblit_dep],
  install: false
)

test('gfxblit tests', test_exe)