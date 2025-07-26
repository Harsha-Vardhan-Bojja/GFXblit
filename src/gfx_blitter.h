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
    void find_operation_type(gfx_blit_image_t *src, gfx_blit_image_t *dst);
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

    enum gfx_operation {
        GFX_NONE = 0x0,
        GFX_ROTATION = 0x1,
        GFX_SCALE = 0x2,
        GFX_CSC = 0x4,
    };

    int type_of_operation = GFX_NONE;
    gfx_blitter();
    ~gfx_blitter();
};

#endif