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

void gfx_blitter::decide_shaders(const char **vtx_shader,const char **frag_shader, gfx_format_t src_fmt, gfx_format_t dst_fmt)
{
    int n = sizeof(shader_map)/sizeof(shader_table_t);
    for(int i = 0; i < n; i++){
        if(src_fmt == shader_map[i].src_fmt && dst_fmt == shader_map[i].dst_fmt) {
            *vtx_shader = shader_map[i].v_shader;
            *frag_shader = shader_map[i].f_shader;
            break;
        }
    }
    printf("[DEBUG]: vertex_shader = %s & fragment_shader = %s\n", *vtx_shader, *frag_shader);
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

int gfx_blitter::create_program(gfx_format_t src_fmt, gfx_format_t dst_fmt)
{
    const char *vertex_shader = "NONE";
    const char *fragment_shader = "NONE";
    decide_shaders(&vertex_shader, &fragment_shader, src_fmt, dst_fmt);
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

EGLImageKHR gfx_blitter::create_egl_image(struct gbm_bo *bo, EGLDisplay display, const char *type)
{
    EGLint width = gbm_bo_get_width(bo);
    EGLint height = gbm_bo_get_height(bo);
    EGLint stride = gbm_bo_get_stride(bo);
    EGLint fd = gbm_bo_get_fd(bo);
    EGLint bpp = gbm_bo_get_bpp(bo);
    EGLint offset = gbm_bo_get_offset(bo,0);
    EGLint format = gbm_bo_get_format(bo);
    if(fd <= 0) {
        printf("[ERROR]: Bcz of fd is < 1(%d)\n", fd);
        return NULL;
    }
    else {
        printf("[DEBUG]: %s: Width = %d, Height = %d, Stride = %d, Format = 0x%x, Offset = %d, fd = %d, bpp = %d\n",
                type ,width, height, stride, format, offset, fd, bpp);
    }

    EGLint attrs[] = {
        EGL_WIDTH, width,
        EGL_HEIGHT, height,
        EGL_LINUX_DRM_FOURCC_EXT, format,
        EGL_DMA_BUF_PLANE0_FD_EXT, fd,
        EGL_DMA_BUF_PLANE0_OFFSET_EXT, offset,
        EGL_DMA_BUF_PLANE0_PITCH_EXT, stride,
        EGL_NONE
    };

    EGLImageKHR image = eglCreateImageKHR(display, EGL_NO_CONTEXT, EGL_LINUX_DMA_BUF_EXT, NULL, attrs);
    if(image == EGL_NO_IMAGE_KHR) {
        printf("[ERROR]: Failed to create the EGLimageKHR");
        return EGL_NO_IMAGE_KHR;
    }

    return image;
}

int gfx_blitter::create_texture(EGLImageKHR image, EGLDisplay display)
{
    GLuint texture = 0;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, image);
    eglDestroyImageKHR(display, image);
    return texture;
}

int gfx_blitter::create_gl_buffer()
{
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    return vbo;
}

int gfx_blitter::setup_gl_ver_Attr(GLint *pos_attri, GLint *tex_attri, GLint *rotation_idx, GLuint program, gfx_rotation rot_value)
{
    *pos_attri = -1;
    *tex_attri = -1;
    *rotation_idx = -1;
    *pos_attri = glGetAttribLocation(program, "a_position");
    *tex_attri = glGetAttribLocation(program, "a_texCoord");
    *rotation_idx = glGetUniformLocation(program, "rotation");
    if(*pos_attri == -1 || *tex_attri == -1 || *rotation_idx == -1) {
        printf("[ERROR]: Failed to get the Attributes location\n");
        printf("[ERROR]: pos_attri = %d, tex_attri = %d, roation_idx = %d\n", *pos_attri, *tex_attri, *rotation_idx);
        return GFX_GET_ATTRI_FAIL;
    }

    glEnableVertexAttribArray(*pos_attri);
    glVertexAttribPointer(*pos_attri, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void *)0);
    glEnableVertexAttribArray(*tex_attri);
    glVertexAttribPointer(*tex_attri, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void *)(3 * sizeof(float)));
    glUniform1i(*rotation_idx, rot_value);
    return GFX_GET_ATTRI_SUCCESS;
}

int gfx_blitter::create_fbo(GLuint texture_id, uint32_t width, uint32_t height)
{
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER,fbo);
    if(fbo <  1) {
        printf("[ERROR]: Failed to get the FBO (%d)\n", fbo);
        return GFX_FBO_GEN_FAIL;
    }
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE) {
        printf("[ERROR]: Framebuffer is not complete! Status: 0x%x\n", status);
        return GFX_FRAMEBUFFER_INCOMPLETE;
    }

    return fbo;
}

int gfx_blitter::render(gfx_pipeline_t gfx_pipe_res, uint32_t width, uint32_t height, gfx_rotation_t rot_value)
{
    int ret_status;
    glBindFramebuffer(GL_FRAMEBUFFER, gfx_pipe_res.fbo);

    glViewport(0, 0, width, height);
    glUseProgram(gfx_pipe_res.program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gfx_pipe_res.textures[0]);
    glUniform1i(glGetUniformLocation(gfx_pipe_res.program, "tex"), 0);
    glBindBuffer(GL_ARRAY_BUFFER, gfx_pipe_res.vbo);

    glClearColor(0.0f, 0.0f, 0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ret_status = setup_gl_ver_Attr(&gfx_pipe_res.posAttrib, &gfx_pipe_res.texAttrib, &gfx_pipe_res.rotation_idx, gfx_pipe_res.program, rot_value);
    if(ret_status == GFX_GET_ATTRI_SUCCESS) {
        printf("[INFO]: posAttrib Loc = %d, texAttrib Loc = %d, rotation_idx = %d\n", gfx_pipe_res.posAttrib, gfx_pipe_res.texAttrib, gfx_pipe_res.rotation_idx);
    }
    else {
        return GFX_GET_ATTRI_FAIL;
    }

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glFinish();

    glDisableVertexAttribArray(gfx_pipe_res.posAttrib);
    glDisableVertexAttribArray(gfx_pipe_res.texAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return GFX_RENDER_SUCCESS;
}