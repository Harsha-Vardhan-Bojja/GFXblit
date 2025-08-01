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

void gfx_blitter::decide_shaders(const char **vtx_shader,const char **frag_shader)
{
    gfx_shader_manager shader_manager;
    *vtx_shader = shader_manager.vertex_shader_source;
    *frag_shader = shader_manager.fragment_shader_source;
}

int gfx_blitter::compile_shader(GLuint shader_type,const char *shader_source)
{
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);

    GLint stauts;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &stauts);
    if (!stauts) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("[DEBUG]: Shader compilation failed: %s\n", infoLog);
        return GFX_COMPILE_FAIL;
    }
    return shader;
}

int gfx_blitter::create_program()
{
    const char *vertex_shader = "NONE";
    const char *fragment_shader = "NONE";
    decide_shaders(&vertex_shader, &fragment_shader);
    if(!strcmp(vertex_shader, "NONE") || !strcmp(fragment_shader,"NONE")) {
        printf("[ERROR]: Failed to choose the shaders\n");
        printf("[ERROR]: vertex_shader = %s & fragment_shader = %s\n", vertex_shader, fragment_shader);
        return GFX_DECIDE_SHADER_FAIL;
    }

    GLint vs_id = compile_shader(GL_VERTEX_SHADER, vertex_shader);
    GLint fs_id = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);
    if(vs_id == GFX_COMPILE_FAIL || fs_id == GFX_COMPILE_FAIL) {
        return GFX_CREATE_PROGRAM_FAIL;
    }
    else {
        printf("[DEBUG]: Vertex_shader ID = %d & Fragment_shader_ID = %d\n", vs_id, fs_id);
    }

    GLuint program_id;
    program_id = glCreateProgram();
    glAttachShader(program_id, vs_id);
    glAttachShader(program_id, fs_id);
    glLinkProgram(program_id);

    GLint status = 0;
    glGetProgramiv(program_id, GL_LINK_STATUS, &status);
    if(!status) {
        char infoLog[512];
        glGetProgramInfoLog(program_id, 512, NULL, infoLog);
        printf("[DEBUG]: Program linking failed: %s\n", infoLog);
        return GFX_PROGRAM_LINK_FAIL;
    }
    else {
        printf("[DEBUG]: Program ID: %d\n", program_id);
        return program_id;
    }
}