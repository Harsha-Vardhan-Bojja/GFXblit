#ifndef GFX_BLITTER_H
#define GFX_BLITTER_H

#include "gfx_blit.h"
#include "gfx_shader.h"

extern PFNEGLCREATEIMAGEKHRPROC eglCreateImageKHR;
extern PFNEGLDESTROYIMAGEKHRPROC eglDestroyImageKHR;
extern PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glEGLImageTargetTexture2DOES;
extern PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT;
class gfx_blitter : public gfx_shader_manager
{
private:
public:
    typedef struct egl_resource {
        struct wl_display *wayland_display;
        EGLDisplay egl_display;
        EGLContext egl_context;
        EGLConfig egl_config;
    } egl_resource_t;

    enum gfx_blitter_error {
        GFX_FBO_GEN_FAIL = -6,
        GFX_FRAMEBUFFER_INCOMPLETE = -5,
        GFX_GET_ATTRI_FAIL = -4,
        GFX_COMPILE_FAIL = -3,
        GFX_PROGRAM_LINK_FAIL = -2,
        GFX_CREATE_PROGRAM_FAIL = -1,
        GFX_LOAD_EXT_SUCCESS,
        GFX_LOAD_EXT_FAIL,
        GFX_GET_ATTRI_SUCCESS,
        GFX_DECIDE_SHADER_FAIL,
        GFX_RENDER_SUCCESS,
    };

    typedef struct gfx_pipeline {
        GLuint program;
        GLuint textures[2];
        GLuint fbo;
        GLuint vbo;
        GLint posAttrib;
        GLint texAttrib;
        GLint rotation_idx;
    } gfx_pipeline_t;

    enum gfx_operation {
        GFX_NONE = 0x0,
        GFX_ROTATION = 0x1,
        GFX_SCALE = 0x2,
        GFX_CSC = 0x4,
    };

    typedef struct shader_table {
        gfx_format_t src_fmt;
        gfx_format_t dst_fmt;
        const char *v_shader;
        const char *f_shader;
    } shader_table_t;

    shader_table_t shader_map[50] = {
        {GFX_FORMAT_ARGB8888, GFX_FORMAT_RGBA8888, "vertex_shader", "argb_to_rgba_fs"},
        {GFX_FORMAT_ARGB8888, GFX_FORMAT_RGB888,   "vertex_shader", "argb_to_rgb_fs"},
        {GFX_FORMAT_ARGB8888, GFX_FORMAT_XRGB8888, "vertex_shader", "argb_to_xrgb_fs"},
        {GFX_FORMAT_ARGB8888, GFX_FORMAT_NV12,     "vertex_shader", "argb_to_nv12_fs"},
        {GFX_FORMAT_ARGB8888, GFX_FORMAT_ARGB8888, vertex_shader,    argb_to_argb_fs},
        {GFX_FORMAT_RGBA8888, GFX_FORMAT_ARGB8888, "vertex_shader", "rgba_to_argb_fs"},
        {GFX_FORMAT_RGBA8888, GFX_FORMAT_RGB888,   "vertex_shader", "rgba_to_rgb_fs"},
        {GFX_FORMAT_RGBA8888, GFX_FORMAT_XRGB8888, "vertex_shader", "rgba_to_xrgb_fs"},
        {GFX_FORMAT_RGBA8888, GFX_FORMAT_NV12,     "vertex_shader", "rgba_to_nv12_fs"},
        {GFX_FORMAT_RGBA8888, GFX_FORMAT_RGBA8888, "vertex_shader", "rgba_to_rgba_fs"},
        {GFX_FORMAT_RGB888,   GFX_FORMAT_ARGB8888, "vertex_shader", "rgb_to_argb_fs"},
        {GFX_FORMAT_RGB888,   GFX_FORMAT_RGBA8888, "vertex_shader", "rgb_to_rgba_fs"},
        {GFX_FORMAT_RGB888,   GFX_FORMAT_XRGB8888, "vertex_shader", "rgb_to_xrgb_fs"},
        {GFX_FORMAT_RGB888,   GFX_FORMAT_NV12,     "vertex_shader", "rgb_to_nv12_fs"},
        {GFX_FORMAT_RGB888,  GFX_FORMAT_RGB888,    "vertex_shader", "rgb_to_rgb_fs"},
        {GFX_FORMAT_XRGB8888, GFX_FORMAT_ARGB8888, "vertex_shader", "xrgb_to_argb_fs"},
        {GFX_FORMAT_XRGB8888, GFX_FORMAT_RGBA8888, "vertex_shader", "xrgb_to_rgba_fs"},
        {GFX_FORMAT_XRGB8888, GFX_FORMAT_RGB888,   "vertex_shader", "xrgb_to_rgb_fs"},
        {GFX_FORMAT_XRGB8888, GFX_FORMAT_NV12,     "vertex_shader", "xrgb_to_nv12_fs"},
        {GFX_FORMAT_XRGB8888, GFX_FORMAT_XRGB8888, "vertex_shader", "xrgb_to_xrgb_fs"},
        {GFX_FORMAT_NV12,     GFX_FORMAT_ARGB8888, "vertex_shader", "nv12_to_argb_fs"},
        {GFX_FORMAT_NV12,     GFX_FORMAT_RGBA8888, "vertex_shader", "nv12_to_rgba_fs"},
        {GFX_FORMAT_NV12,     GFX_FORMAT_RGB888,   "vertex_shader", "nv12_to_rgb_fs"},
        {GFX_FORMAT_NV12,     GFX_FORMAT_XRGB8888, "vertex_shader", "nv12_to_xrgb_fs"},
        {GFX_FORMAT_NV12,     GFX_FORMAT_NV12,     "vertex_shader", "nv12_to_nv12_fs"},
    };

    int type_of_operation = GFX_NONE;

    float vertices[20] = {
        // Position         // Texcoords
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, // Bottom-left
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f, // Bottom-right
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f, // Top-left
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f  // Top-right
    };

    void find_operation_type(gfx_blit_image_t *src, gfx_blit_image_t *dst);
    int load_extensions();
    int create_program(gfx_format_t src_fmt, gfx_format_t dst_fmt);
    int compile_shader(GLuint shader_type, const char *shader_source);
    void decide_shaders(const char **vertex_shader, const char **fragment_shader, gfx_format_t src_fmt, gfx_format_t dst_fmt);
    EGLImageKHR create_egl_image(struct gbm_bo *bo, EGLDisplay display, const char *buffer_type);
    int create_fbo(GLuint texture_id, uint32_t width, uint32_t height);
    int create_gl_buffer();
    int create_texture(EGLImageKHR image, EGLDisplay display);
    int setup_gl_ver_Attr(GLint *pos_attri, GLint *tex_attri,GLint *rotation_idx, GLuint program, gfx_rotation_t rot_value);
    int render(gfx_pipeline_t gfx_pipe_res, uint32_t width, uint32_t height, gfx_rotation_t rot_value);

    gfx_blitter();
    ~gfx_blitter();
};

#endif