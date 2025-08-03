#include "gfx_blit.h"


class gfx_shader_manager
{

private:
public:
    const char* vertex_shader_source = R"(
      attribute vec3 a_position;
      attribute vec2 a_texCoord;
      varying vec2 v_texCord;
      void main() {
          v_texCord = a_texCoord;
          gl_Position = vec4(a_position, 1.0);
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

