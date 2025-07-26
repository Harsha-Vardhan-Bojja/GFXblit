#include "gfx_blit.h"
#include <stdio.h>
#include <stdlib.h>
int main () {
  gfx_blit_image_t *src = malloc(sizeof(gfx_blit_image_t));
  gfx_blit_image_t *dst = malloc(sizeof(gfx_blit_image_t));
  src->width = 512;
  src->height = 512;
  src->format = GFX_FORMAT_RGB888;
  src->rotation = GFX_ROTATION_0;

  dst->width = 512;
  dst->height = 514;
  dst->format = GFX_FORMAT_ARGB8888;
  dst->rotation = GFX_ROTATION_90;

  printf("Executing the test\n");
  gfx_blit(src,dst);
}