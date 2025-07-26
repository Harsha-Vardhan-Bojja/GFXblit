#include "gfx_blitter.h"

PFNEGLCREATEIMAGEKHRPROC eglCreateImageKHR = nullptr;
PFNEGLDESTROYIMAGEKHRPROC eglDestroyImageKHR = nullptr;
PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glEGLImageTargetTexture2DOES = nullptr;
PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT = nullptr;

gfx_blitter::gfx_blitter()
{
    cout << "gfx_bliiter Constructer called" << endl;
}

gfx_blitter::~gfx_blitter()
{
    cout << "gfx_bliiter Deconstructor called" << endl;
}

int gfx_blitter::load_extensions()
{
    eglGetPlatformDisplayEXT = (PFNEGLGETPLATFORMDISPLAYEXTPROC)
        eglGetProcAddress("eglGetPlatformDisplayEXT");
    eglCreateImageKHR = (PFNEGLCREATEIMAGEKHRPROC)
        eglGetProcAddress("eglCreateImageKHR");
    eglDestroyImageKHR = (PFNEGLDESTROYIMAGEKHRPROC)
        eglGetProcAddress("eglDestroyImageKHR");
    glEGLImageTargetTexture2DOES = (PFNGLEGLIMAGETARGETTEXTURE2DOESPROC)
        eglGetProcAddress("glEGLImageTargetTexture2DOES");

    if(!eglGetPlatformDisplayEXT || !eglCreateImageKHR ||
        !eglDestroyImageKHR || !glEGLImageTargetTexture2DOES) {
            cout << "[ERROR]: Failed to load the extensions" << endl;
            return gfx_blitter::GFX_LOAD_EXT_FAIL;
    }
    else {
        return gfx_blitter::GFX_LOAD_EXT_SUCCESS;
    }
}

void gfx_blitter::find_operation_type(gfx_blit_image_t *src, gfx_blit_image_t *dst)
{

    if ((src->width != dst->width) || (src->height != dst->height)) {
        type_of_operation |= GFX_SCALE;
    }
    if (src->format != dst->format) {
        type_of_operation |= GFX_CSC;
    }
    if (src->rotation != dst->rotation) {
        type_of_operation |= GFX_ROTATION;
    }
}