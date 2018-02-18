#version 150 core
uniform sampler2D y_tex;
uniform sampler2D u_tex;
uniform sampler2D v_tex;
in vec2 v_coord;
//layout( location = 0 ) out vec4 fragcolor;
out vec4 fragcolor;

const vec3 R_cf = vec3(1.164383,  0.000000,  1.596027);
const vec3 G_cf = vec3(1.164383, -0.391762, -0.812968);
const vec3 B_cf = vec3(1.164383,  2.017232,  0.000000);
const vec3 offset = vec3(-0.0625, -0.5, -0.5);

void main() {
  float y = texture(y_tex, v_coord).r;
  float u = texture(u_tex, v_coord).r;
  float v = texture(v_tex, v_coord).r;
  vec3 yuv = vec3(y,u,v);
  yuv += offset;
  fragcolor = vec4(0.0, 0.0, 0.0, 1.0);
  fragcolor.r = dot(yuv, R_cf);
  fragcolor.g = dot(yuv, G_cf);
  fragcolor.b = dot(yuv, B_cf);
}
