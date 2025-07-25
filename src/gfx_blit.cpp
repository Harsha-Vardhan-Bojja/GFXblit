#include "gfx_blit.h"
#include "gfx_blitter.h"
#include <iostream>
#include <stdio.h>
using namespace std;

extern "C" int gfx_blit(gfx_blit_image_t *src, gfx_blit_image_t *dst)
{ 
  gfx_blitter test;
  cout << "Executed the gfx_blit public api" << endl;
  return 0;
}
