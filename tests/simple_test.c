#include "gfx_blit.h"
#include <stdio.h>

int main () {
  gfx_blit_image_t *src = NULL;
  gfx_blit_image_t *dst = NULL;
  printf("Executing the test\n");
  gfx_blit(src,dst);
}