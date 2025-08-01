#include "gfx_blit.h"


class gfx_shader_manager
{

private:
public:
    const char* vertex_shader_source = R"(
      attribute vec2 texCord;
      varying vec2 v_texCord;
      void main() {
          v_texCord = texCord;
          gl_Position = vec4(texCord, 0.0, 1.0);
      }
    )";


    const char* fragment_shader_source = R"(
      precision mediump float;
      varying vec2 v_texCord;
      uniform sampler2D tex;
      void main() {
          gl_FragColor = texture2D(tex, v_texCord);
      }
    )";
};

