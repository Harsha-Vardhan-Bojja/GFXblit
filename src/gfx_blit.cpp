#include "gfx_blitter.h"

extern "C" int gfx_blit(gfx_blit_image_t *src, gfx_blit_image_t *dst)
{ 
  gfx_blitter test;
  cout << "Executed the gfx_blit public api" << endl;
  return 0;
}
