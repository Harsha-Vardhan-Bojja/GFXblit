#include "gfx_blit.h"


class gfx_shader_manager
{

private:
public:
    const char* vertex_shader = R"(
      attribute vec3 a_position;
      attribute vec2 a_texCoord;
      varying vec2 v_texCord;
      void main() {
          v_texCord = a_texCoord;
          gl_Position = vec4(a_position, 1.0);
      }
    )";

    /* Refer this formaula for the rotaion of P(x,y)
        x' = xcos(θ) - ysin(θ)
        y' = xsin(θ) + ysin(θ)
      Filps
        xflip = flip over x axis so result is (x, -y)
        yflip = flip over y axis so result is (-x, y)
    */
    const char* argb_to_argb_fs = R"(
      precision mediump float;
      varying vec2 v_texCord;
      uniform sampler2D tex;
      uniform int rotation;
      void main() {
          vec2 uv = v_texCord;
          if (rotation == 0) {
              uv = v_texCord;
          } else if (rotation == 1) {
              uv = vec2(v_texCord.y, 1.0 - v_texCord.x);
          } else if (rotation == 2) {
              uv = vec2(1.0 - v_texCord.x, 1.0 - v_texCord.y);
          } else if (rotation == 3) {
              uv = vec2(1.0 - v_texCord.y, v_texCord.x);
          } else if (rotation == 4) {
              uv = vec2(1.0 - v_texCord.x, v_texCord.y);
          } else if (rotation == 5) {
              uv = vec2(v_texCord.x, 1.0 - v_texCord.y);
          } else if (rotation == 6) {
              uv = vec2(1.0 - v_texCord.x, 1.0 - v_texCord.y);
          }
          gl_FragColor = texture2D(tex, uv);
      }
    )";

    const char* xrgb_to_xrgb_fs = R"(
      precision mediump float;
      varying vec2 v_texCord;
      uniform sampler2D tex;
      uniform int rotation;
      void main() {
          vec2 uv = v_texCord;
          if (rotation == 0) {
              uv = v_texCord;
          } else if (rotation == 1) {
              uv = vec2(v_texCord.y, 1.0 - v_texCord.x);
          } else if (rotation == 2) {
              uv = vec2(1.0 - v_texCord.x, 1.0 - v_texCord.y);
          } else if (rotation == 3) {
              uv = vec2(1.0 - v_texCord.y, v_texCord.x);
          } else if (rotation == 4) {
              uv = vec2(1.0 - v_texCord.x, v_texCord.y);
          } else if (rotation == 5) {
              uv = vec2(v_texCord.x, 1.0 - v_texCord.y);
          } else if (rotation == 6) {
              uv = vec2(1.0 - v_texCord.x, 1.0 - v_texCord.y);
          }
          gl_FragColor = texture2D(tex, uv);
      }
    )";

    const char* xrgb_to_argb_fs = R"(
      precision mediump float;
      varying vec2 v_texCord;
      uniform sampler2D tex;
      uniform int rotation;
      uniform float alpha_val;
      void main() {
          vec2 uv = v_texCord;
          if (rotation == 0) {
              uv = v_texCord;
          } else if (rotation == 1) {
              uv = vec2(v_texCord.y, 1.0 - v_texCord.x);
          } else if (rotation == 2) {
              uv = vec2(1.0 - v_texCord.x, 1.0 - v_texCord.y);
          } else if (rotation == 3) {
              uv = vec2(1.0 - v_texCord.y, v_texCord.x);
          } else if (rotation == 4) {
              uv = vec2(1.0 - v_texCord.x, v_texCord.y);
          } else if (rotation == 5) {
              uv = vec2(v_texCord.x, 1.0 - v_texCord.y);
          } else if (rotation == 6) {
              uv = vec2(1.0 - v_texCord.x, 1.0 - v_texCord.y);
          }
          vec4 color = texture2D(tex, uv);
          gl_FragColor = vec4(color.rgb, alpha_val);
      }
    )";
};

