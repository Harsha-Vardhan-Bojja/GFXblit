#include "gfx_context.h"

int gfx_context::get_wayaland_disp(struct wl_display **wl_dspy)
{
    *wl_dspy = wl_display_connect(NULL);
    if(*wl_dspy == NULL) {
        return WAYLAND_DSPY_FAIL;
    }
    return WAYLAND_DSPY_SUCCESS;
}

int gfx_context::gfx_egl_context_create(gfx_blitter::egl_resource *egl_res)
{
    EGLint major, minor;
    EGLint numconfigs;
    bool initialized = false;

    // Try Wayland platform
    egl_res->egl_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_WAYLAND_KHR, egl_res->wayland_display, NULL);
    if (egl_res->egl_display != EGL_NO_DISPLAY && eglInitialize(egl_res->egl_display, &major, &minor)) {
        cout << "[INFO]: Initialized EGL with Wayland display (EGL_PLATFORM_WAYLAND_KHR)\n";
        initialized = true;
    }

    // Try surfaceless platform
    if (!initialized) {
        egl_res->egl_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_SURFACELESS_MESA, EGL_DEFAULT_DISPLAY, NULL);
        if (egl_res->egl_display != EGL_NO_DISPLAY && eglInitialize(egl_res->egl_display, &major, &minor)) {
            cout << "[INFO]: Initialized EGL with surfaceless platform (EGL_PLATFORM_SURFACELESS_MESA)\n";
            initialized = true;
        }
    }

    // Try default display
    if (!initialized) {
        egl_res->egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (egl_res->egl_display != EGL_NO_DISPLAY && eglInitialize(egl_res->egl_display, &major, &minor)) {
            cout << "[INFO]: Initialized EGL with default display (eglGetDisplay)\n";
            initialized = true;
        }
    }

    // All attempts failed
    if (!initialized) {
        cout << "[ERROR]: Failed to initialize EGL with any backend\n";
        return GFX_INITIALIZE_FAILED;
    }

    const EGLint config_attri[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE,
    };

    if (!eglChooseConfig(egl_res->egl_display, config_attri, &egl_res->egl_config, 1, &numconfigs)) {
        cout << "[ERROR]: Failed to choose configs" << endl;
        return GFX_CHOOSE_CONFIG_FAILED;
    }

    const EGLint context_attri[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    egl_res->egl_context = eglCreateContext(egl_res->egl_display, egl_res->egl_config, EGL_NO_CONTEXT, context_attri);
    if (!egl_res->egl_context) {
        cout << "[ERROR]: Failed to create context" << endl;
        return GFX_CREATE_CONTEXT_FAILED;
    }

    if (!eglMakeCurrent(egl_res->egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, egl_res->egl_context)) {
        cout << "[ERROR]: Failed to make context as current" << endl;
        return GFX_MAKE_CURRENT_CONTEXT_FAILED;
    }

    print_sys_info(egl_res->egl_display);

    return GFX_EGL_CONTEXT_SUCCESS;
}

void gfx_context::print_sys_info(EGLDisplay display) {
    cout << "================= EGL Information =================\n";
    cout << "EGL Vendor:     " << eglQueryString(display, EGL_VENDOR) << endl;
    cout << "EGL Version:    " << eglQueryString(display, EGL_VERSION) << endl;
    cout << "EGL Extensions: " << eglQueryString(display, EGL_EXTENSIONS) << endl;
    cout << "===================================================\n";

    cout << "\n============= OpenGL ES Information ==============\n";
    cout << "GL Vendor:      " << glGetString(GL_VENDOR) << endl;
    cout << "GL Renderer:    " << glGetString(GL_RENDERER) << endl;
    cout << "GL Version:     " << glGetString(GL_VERSION) << endl;
    cout << "GL Extensions:  " << glGetString(GL_EXTENSIONS) << endl;
    cout << "==================================================\n";
}