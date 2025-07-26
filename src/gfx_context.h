#ifndef GFX_CONTEXT_H
#define GFX_CONTEXT_H


#include "gfx_blit.h"
#include "gfx_blitter.h"

class gfx_context
{
private:

public:
  int gfx_egl_context_create(gfx_blitter::egl_resource *egl_res);
  int get_wayaland_disp(struct wl_display **wl_dspy);
  void print_sys_info(EGLDisplay display);
  enum wayland_error {
    WAYLAND_DSPY_SUCCESS,
    WAYLAND_DSPY_FAIL,
  };
  enum gfx_context_error {
    GFX_EGL_CONTEXT_SUCCESS,
    GFX_DISPLAY_FAILED,
    GFX_INITIALIZE_FAILED,
    GFX_CHOOSE_CONFIG_FAILED,
    GFX_CREATE_CONTEXT_FAILED,
    GFX_MAKE_CURRENT_CONTEXT_FAILED,
  };
};

#endif



