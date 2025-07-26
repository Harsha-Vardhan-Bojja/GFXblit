#ifndef GFX_BLITTER_H
#define GFX_BLITTER_H

#include "gfx_blit.h"

extern PFNEGLCREATEIMAGEKHRPROC eglCreateImageKHR;
extern PFNEGLDESTROYIMAGEKHRPROC eglDestroyImageKHR;
extern PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glEGLImageTargetTexture2DOES;
extern PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT;
class gfx_blitter
{
private:
public:

    int load_extensions();
    typedef struct egl_resource {
        struct wl_display *wayland_display;
        EGLDisplay egl_display;
        EGLContext egl_context;
        EGLConfig egl_config;
    } egl_resource_t;
    enum gfx_blitter_error {
        GFX_LOAD_EXT_SUCCESS,
        GFX_LOAD_EXT_FAIL,
    };
    gfx_blitter();
    ~gfx_blitter();
};

#endif