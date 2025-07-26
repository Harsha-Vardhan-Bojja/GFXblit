#include "gfx_blitter.h"
#include "gfx_context.h"

extern "C" int gfx_blit(gfx_blit_image_t *src, gfx_blit_image_t *dst)
{ 
    gfx_blitter gfx_blitter_t;
    gfx_context gfx_context_t;
    gfx_blitter::egl_resource egl_res = {0};

    int ret_status = gfx_blitter_t.load_extensions();
    if(ret_status == gfx_blitter_t.GFX_LOAD_EXT_FAIL) {
        cout << "[INFO]: Failed to load extensions" << endl;
    }
    else {
        cout << "[INFO]: Loaded the extensions" << endl;
    }

    ret_status = gfx_context_t.get_wayaland_disp(&egl_res.wayland_display);
    if(ret_status == gfx_context_t.WAYLAND_DSPY_FAIL) {
        cout << "[INFO]: Failed to get wayland get display" << endl;
    }
    else {
        cout  << "[INFO]: Connected to wayland display" << endl;
    }

    ret_status = gfx_context_t.gfx_egl_context_create(&egl_res);
    if(ret_status != gfx_context_t.GFX_EGL_CONTEXT_SUCCESS) {
        cout << "[INFO]: Failed to setup egl context creation" << endl;
    }
    else {
        cout  << "[INFO]: EGL context creation success" << endl;
    }

    gfx_blitter_t.find_operation_type(src, dst);

    cout << "Executed the gfx_blit public api" << endl;
    return 0;
}
