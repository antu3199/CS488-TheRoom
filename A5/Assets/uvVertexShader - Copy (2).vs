#version 330

// Model-Space coordinates
in vec3 position;
in vec3 normal;
in vec2 uv;
in vec3 tangent;
in vec3 bitangent;


struct LightSource {
    vec3 position;
    vec3 color;
    float intensity;
};
uniform LightSource light;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Perspective;

// Remember, this is transpose(inverse(ModelView)).  Normals should be
// transformed using this matrix instead of the ModelView matrix.
uniform mat3 NormalMatrix;


out VsOutFsIn {
	vec3 position_ES; // eye space
  vec3 position_WS;
  vec2 uv;
  vec3 normal;
  vec3 bitangent;
  vec3 tangent;
	LightSource light;
} vs_out;


void main() {
	vec4 pos4 = vec4(position, 1.0);
  vs_out.position_ES = vec3(View * Model * pos4);
  vs_out.position_WS = vec3(Model * pos4);
  vec2 uv_out = uv;
  uv_out.y = 1.0f - uv_out.y;
  vs_out.uv = uv_out;
  
  vs_out.normal = normalize(NormalMatrix * normal);
  vs_out.tangent = normalize(NormalMatrix * tangent);
  vs_out.bitangent = normalize(NormalMatrix * bitangent);
  vs_out.light = light;


  gl_Position = Perspective * View * Model * pos4;
}
