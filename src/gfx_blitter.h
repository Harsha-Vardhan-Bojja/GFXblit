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
    EGLImageKHR create_egl_image(struct gbm_bo *bo, EGLDisplay display, const char *buffer_type);
    int create_gl_buffer();
    int create_texture(EGLImageKHR image);
    int setup_gl_ver_Attr(GLint *pos_attri, GLint *tex_attri, GLuint program);
    typedef struct egl_resource {
        struct wl_display *wayland_display;
        EGLDisplay egl_display;
        EGLContext egl_context;
        EGLConfig egl_config;
    } egl_resource_t;

    enum gfx_blitter_error {
        GFX_GET_ATTRI_FAIL = -4,
        GFX_COMPILE_FAIL = -3,
        GFX_PROGRAM_LINK_FAIL = -2,
        GFX_CREATE_PROGRAM_FAIL = -1,
        GFX_LOAD_EXT_SUCCESS,
        GFX_LOAD_EXT_FAIL,
        GFX_GET_ATTRI_SUCCESS,
        GFX_DECIDE_SHADER_FAIL,
    };

    typedef struct gfx_pipeline {
        GLuint program;
        GLuint textures[2];
        GLuint fbo;
        GLuint vbo;
        GLint posAttrib;
        GLint texAttrib;
    } gfx_pipeline_t;

    enum gfx_operation {
        GFX_NONE = 0x0,
        GFX_ROTATION = 0x1,
        GFX_SCALE = 0x2,
        GFX_CSC = 0x4,
    };

    int type_of_operation = GFX_NONE;

    float vertices[20] = {
        // Position         // Texcoords
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, // Bottom-left
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f, // Bottom-right
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f, // Top-left
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f  // Top-right
    };

    gfx_blitter();
    ~gfx_blitter();
};

#endif