#version 150 core
uniform mat4 u_pm;
uniform vec4 draw_pos;

const vec2 verts[4] = vec2[] (
  vec2(-0.5,  0.5),
  vec2(-0.5, -0.5),
  vec2( 0.5,  0.5),
  vec2( 0.5, -0.5)
);

const vec2 texcoords[4] = vec2[] (
  vec2(0.0, 1.0),
  vec2(0.0, 0.0),
  vec2(1.0, 1.0),
  vec2(1.0, 0.0)
);

out vec2 v_coord;

void main() {
   vec2 vert = verts[gl_VertexID];
   vec4 p = vec4((0.5 * draw_pos.z) + draw_pos.x + (vert.x * draw_pos.z),
                 (0.5 * draw_pos.w) + draw_pos.y + (vert.y * draw_pos.w),
                 0, 1);
   gl_Position = u_pm * p;
   v_coord = texcoords[gl_VertexID];
}
