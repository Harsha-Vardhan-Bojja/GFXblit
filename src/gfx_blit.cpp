#include "gfx_blitter.h"
#include "gfx_context.h"

extern "C" int gfx_blit(gfx_blit_image_t *src, gfx_blit_image_t *dst)
{ 
    gfx_blitter gfx_blitter_t;
    gfx_context gfx_context_t;
    gfx_blitter::egl_resource egl_res = {0};
    gfx_blitter::gfx_pipeline_t gfx_pipe_res = {0};
    int ret_status = gfx_blitter_t.load_extensions();
    if(ret_status == gfx_blitter_t.GFX_LOAD_EXT_FAIL) {
        cout << "[INFO]: Failed to load extensions" << endl;
        return -1;
    }
    else {
        cout << "[INFO]: Loaded the extensions" << endl;
    }

    ret_status = gfx_context_t.get_wayaland_disp(&egl_res.wayland_display);
    if(ret_status == gfx_context_t.WAYLAND_DSPY_FAIL) {
        cout << "[INFO]: Failed to get wayland get display" << endl;
        return -1;
    }
    else {
        cout  << "[INFO]: Connected to wayland display" << endl;
    }

    ret_status = gfx_context_t.gfx_egl_context_create(&egl_res);
    if(ret_status != gfx_context_t.GFX_EGL_CONTEXT_SUCCESS) {
        cout << "[INFO]: Failed to setup egl context creation" << endl;
        return -1;
    }
    else {
        cout  << "[INFO]: EGL context creation success" << endl;
    }

    gfx_blitter_t.find_operation_type(src, dst);
    gfx_pipe_res.program = gfx_blitter_t.create_program(src->format, dst->format);
    if(gfx_pipe_res.program <= 0) {
        cout << "[INFO]: Failed to creete the program" << endl;
    }

    gfx_pipe_res.vbo = gfx_blitter_t.create_gl_buffer();
    if(gfx_pipe_res.vbo == 0) {
        printf("[ERROR]: Failed create the gl buffer\n");
        return -1;
    }
    else {
        printf("[DEBUG]: VBO = %d\n", gfx_pipe_res.vbo);
    }

    EGLImageKHR src_image = gfx_blitter_t.create_egl_image(src->bo, egl_res.egl_display, "INPUT ");
    if(src_image == EGL_NO_IMAGE_KHR || src_image == NULL) {
        printf("[ERROR]: Failed to create the src egl_image\n");
        return -1;
    }
    EGLImageKHR dst_image = gfx_blitter_t.create_egl_image(dst->bo, egl_res.egl_display, "OUTPUT");
    if(src_image == EGL_NO_IMAGE_KHR || src_image == NULL) {
        printf("[ERROR]: Failed to create the dst egl_image\n");
        return -1;
    }

    gfx_pipe_res.textures[0] = gfx_blitter_t.create_texture(src_image, egl_res.egl_display);
    printf("[INFO]: Src texture ID = %d\n", gfx_pipe_res.textures[0]);

    gfx_pipe_res.textures[1] = gfx_blitter_t.create_texture(dst_image, egl_res.egl_display);
    printf("[INFO]: dst texture ID = %d\n", gfx_pipe_res.textures[1]);

    gfx_pipe_res.fbo = gfx_blitter_t.create_fbo(gfx_pipe_res.textures[1], dst->width, dst->height);
    if(gfx_pipe_res.fbo  > 0) {
        printf("[INFO]: FBO ID = %d\n", gfx_pipe_res.fbo);
    }
    else {
        return -1;
    }

    ret_status = gfx_blitter_t.render(gfx_pipe_res, dst->width, dst->height);
    if(ret_status == gfx_blitter_t.GFX_RENDER_SUCCESS){
        printf("[INFO]: Rendering got success\n");
    }
    else {
        printf("[ERROR]: Rendering got failed\n");
        return -1;
    }

    cout << "Executed the gfx_blit public api" << endl;
    return 0;
}
