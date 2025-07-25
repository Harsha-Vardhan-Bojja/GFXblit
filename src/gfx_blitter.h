#ifndef GFX_BLITTER_H
#define GFX_BLITTER_H

#include "gfx_blit.h"

class gfx_blitter
{
private:
    typedef struct egl_resource {
        struct wl_display *wayland_display;
        EGLDisplay egl_display;
        EGLContext egl_context;
        EGLConfig egl_config;
    } egl_resource_t;
public:
    gfx_blitter();
    ~gfx_blitter();
};

#endif