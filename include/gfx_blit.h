#ifndef GFX_BLIT_H
#define GFX_BLIT_H

#include "gfx_common_header.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum gfx_rotation {
    GFX_ROTATION_0,
    GFX_ROTATION_90,
    GFX_ROTATION_180,
    GFX_ROTATION_270,
    GFX_ROTATION_X_FLIP,
    GFX_ROTATION_Y_FLIP,
    GFX_ROTATION_XY_FLIP,
} gfx_rotation_t;

typedef enum gfx_format {
    GFX_FORMAT_ARGB8888,
    GFX_FORMAT_RGBA8888,
    GFX_FORMAT_RGB888,
    GFX_FORMAT_XRGB8888,
    GFX_FORMAT_NV12,
} gfx_format_t;

typedef struct gfx_blit_image {
    uint32_t height;
    uint32_t width;
    int fd;
    uint32_t stride[2];
    uint32_t offset[2];
    gfx_format_t format;
    gfx_rotation_t rotation;
    struct gbm_bo *bo;
    struct gbm_device *device;
    bool status;
} gfx_blit_image_t;

/**
 * Main blit interface: performs GPU-based scaling, rotation, format conversion.
 *
 * @param src Pointer to source image info.
 * @param dst Pointer to destination image info.
 * @return GFX_BLIT_SUCCESS on success, or GFX_BLIT_ERROR on failure.
 */
int gfx_blit(gfx_blit_image_t *src, gfx_blit_image_t *dst);

#ifdef __cplusplus
}
#endif
#endif