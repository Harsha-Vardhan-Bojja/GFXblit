#include "gfx_blit.h"
#include <gbm.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static int drm_fd = -1;
static struct gbm_device *gbm_dev = NULL;

static void* create_image(gfx_blit_image_t *img, uint32_t width, uint32_t height, gfx_format_t fmt) {
    uint32_t gbm_fmt;
    switch (fmt) {
        case GFX_FORMAT_RGB888:   gbm_fmt = GBM_FORMAT_RGB888; break;
        case GFX_FORMAT_ARGB8888: gbm_fmt = GBM_FORMAT_ARGB8888; break;
        case GFX_FORMAT_XRGB8888: gbm_fmt = GBM_FORMAT_XRGB8888; break;
        default: fprintf(stderr, "Unsupported format\n"); return NULL;
    }

    struct gbm_bo *bo = gbm_bo_create(gbm_dev, width, height, gbm_fmt, GBM_BO_USE_LINEAR | GBM_BO_USE_RENDERING | GBM_BO_USE_SCANOUT);
    if (!bo) {
        perror("gbm_bo_create failed");
        return NULL;
    }

    uint32_t stride;
    void *map_data = NULL;
    void *ptr = gbm_bo_map(bo, 0, 0, width, height, GBM_BO_TRANSFER_READ_WRITE, &stride, &map_data);
    if (!ptr) {
        perror("gbm_bo_map failed");
        gbm_bo_destroy(bo);
        return NULL;
    }

    img->width = width;
    img->height = height;
    img->format = fmt;
    img->bo = bo;
    img->stride[0] = stride;
    return ptr;
}

int main() {
    drm_fd = open("/dev/dri/card1", O_RDWR | O_CLOEXEC);
    if (drm_fd < 0) {
        perror("Failed to open render node");
        return 1;
    }

    gbm_dev = gbm_create_device(drm_fd);
    if (!gbm_dev) {
        perror("Failed to create GBM device");
        close(drm_fd);
        return 1;
    }

    gfx_blit_image_t *src = malloc(sizeof(gfx_blit_image_t));
    gfx_blit_image_t *dst = malloc(sizeof(gfx_blit_image_t));

    void *src_ptr = create_image(src, 512, 512, GFX_FORMAT_XRGB8888);
    void *dst_ptr = create_image(dst, 512, 512, GFX_FORMAT_ARGB8888);

    if (!src_ptr || !dst_ptr) {
        fprintf(stderr, "Failed to allocate GBM images\n");
        return 1;
    }


    src->rotation = GFX_ROTATION_0;
    dst->rotation = GFX_ROTATION_90;

    printf("Executing the test\n");
    int status = gfx_blit(src, dst);
    if (status != 0) {
        fprintf(stderr, "gfx_blit failed with status %d\n", status);
    }

    free(src);
    free(dst);

    gbm_device_destroy(gbm_dev);
    close(drm_fd);
    return 0;
}