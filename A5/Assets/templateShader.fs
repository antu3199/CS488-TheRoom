

#version 330

out vec4 fragColour;

in VsOutFsIn {
  vec3 position_M;
} fs_in;

void main() {
    vec3 noiseCol = vec3(1.0, 0.0, 0.0);
    fragColour = vec4(noiseCol, 1.0);
}
