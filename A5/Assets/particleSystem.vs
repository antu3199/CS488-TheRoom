#version 330

in vec3 quadVertices;
in vec4 xyzs;
in vec4 color; 

out VsOutFsIn {
    out vec2 UV;
    out vec4 particleColor;
} vs_out;

uniform vec3 cameraRight_WS;
uniform vec3 cameraUp_WS;
uniform mat4 ViewProj; // Only ViewProj is needed because Model depends on camera

void main()
{
	float particleSize = xyzs.w; // because we encoded it this way.
	vec3 particleCenter_wordspace = xyzs.xyz;
	
	vec3 position_WS = 
		particleCenter_wordspace
		+ cameraRight_WS * quadVertices.x * particleSize
		+ cameraUp_WS * quadVertices.y * particleSize;

	gl_Position = ViewProj * vec4(position_WS, 1.0f);

	vs_out.UV = quadVertices.xy + vec2(0.5, 0.5);
	vs_out.particleColor = color;
}