
#version 330

in VsOutFsIn {
  vec2 UV;
  vec4 particleColor;
} fs_in;

out vec4 color;

uniform sampler2D texture0;

void main(){
	color = texture( texture0, fs_in.UV ) * fs_in.particleColor;
}