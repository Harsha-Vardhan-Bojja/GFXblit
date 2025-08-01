#ifndef GFX_BLITTER_H
#define GFX_BLITTER_H

#include "gfx_blit.h"
#include "gfx_shader.h"

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
    int create_program();
    int compile_shader(GLuint shader_type, const char *shader_source);
    void decide_shaders(const char **vertex_shader, const char **fragment_shader);
    typedef struct egl_resource {
        struct wl_display *wayland_display;
        EGLDisplay egl_display;
        EGLContext egl_context;
        EGLConfig egl_config;
    } egl_resource_t;

    enum gfx_blitter_error {
        GFX_COMPILE_FAIL = -3,
        GFX_PROGRAM_LINK_FAIL = -2,
        GFX_CREATE_PROGRAM_FAIL = -1,
        GFX_LOAD_EXT_SUCCESS,
        GFX_LOAD_EXT_FAIL,
        GFX_DECIDE_SHADER_FAIL,
    };

    typedef struct gfx_pipeline {
        GLuint program;
        GLuint textures[2];
        GLuint fbo;
        GLuint vbo;
    } gfx_pipeline_t;

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