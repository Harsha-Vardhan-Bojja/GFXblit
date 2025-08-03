// This is the main application file that uses gfx_blit.h to perform a blit.
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

    // Use GBM_BO_USE_LINEAR for mapping to a CPU address
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
    img->stride[0] = stride; // Store the stride for the first plane
    return ptr;
}

static void fill_image(gfx_blit_image_t *img, uint32_t width, uint32_t height, void* ptr) {
    // Fill with a simple color pattern: a green gradient
    uint32_t *pixels = (uint32_t *)ptr;
    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            uint8_t green_val = (y * 255) / height;
            pixels[y * (img->stride[0] / 4) + x] = 0xFF000000 | (green_val << 8); // ARGB
        }
    }
}

static void write_to_file(const char* filename, gfx_blit_image_t* img, void* ptr) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        perror("Failed to open output file");
        return;
    }

    uint32_t *pixels = (uint32_t *)ptr;
    for (uint32_t y = 0; y < img->height; ++y) {
        fwrite(&pixels[y * (img->stride[0] / 4)], img->width, 4, fp);
    }
    fclose(fp);
    printf("Successfully wrote output to %s\n", filename);
}

int main() {
    // Open a render node
    drm_fd = open("/dev/dri/card1", O_RDWR | O_CLOEXEC);
    if (drm_fd < 0) {
        perror("Failed to open render node /dev/dri/card0. Make sure to use the correct path.");
        return 1;
    }

    // Create a GBM device
    gbm_dev = gbm_create_device(drm_fd);
    if (!gbm_dev) {
        perror("Failed to create GBM device");
        close(drm_fd);
        return 1;
    }

    // Allocate memory for image structs
    gfx_blit_image_t *src = (gfx_blit_image_t*)malloc(sizeof(gfx_blit_image_t));
    gfx_blit_image_t *dst = (gfx_blit_image_t*)malloc(sizeof(gfx_blit_image_t));
    memset(src, 0, sizeof(gfx_blit_image_t));
    memset(dst, 0, sizeof(gfx_blit_image_t));


    // Create source image and fill with data
    void *src_ptr = create_image(src, 512, 512, GFX_FORMAT_ARGB8888);
    if (!src_ptr) {
        fprintf(stderr, "Failed to allocate source GBM image\n");
        return 1;
    }
    fill_image(src, src->width, src->height, src_ptr);

    // Create destination image
    void *dst_ptr = create_image(dst, 512, 512, GFX_FORMAT_ARGB8888);
    if (!dst_ptr) {
        fprintf(stderr, "Failed to allocate destination GBM image\n");
        gbm_bo_unmap(src->bo, NULL);
        gbm_bo_destroy(src->bo);
        return 1;
    }

    src->rotation = GFX_ROTATION_0;
    dst->rotation = GFX_ROTATION_0;

    // Call the blitting function
    printf("Executing the test\n");
    int status = gfx_blit(src, dst);
    if (status != 0) {
        fprintf(stderr, "gfx_blit failed with status %d\n", status);
    }

    // Unmap the source buffer
    //gbm_bo_unmap(src->bo, NULL);

    // Write the output to a file
    write_to_file("output.argb", dst, dst_ptr);

    // Clean up
    gbm_bo_unmap(dst->bo, NULL);
    gbm_bo_destroy(src->bo);
    gbm_bo_destroy(dst->bo);
    free(src);
    free(dst);

    gbm_device_destroy(gbm_dev);
    close(drm_fd);
    return 0;
}