

#version 330
layout (location = 0) in vec3 position;

uniform mat4 ModelView;
uniform mat4 Perspective;

out VsOutFsIn {
  vec3 position_M;
} vs_out;

void main() {
  vs_out.position_M = position;
  gl_Position = Perspective * ModelView * vec4(position, 1.0);
}