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
	vec3 position_ES; // Eye-space position
  vec3 position_WS;
	vec3 normal_ES;   // Eye-space normal
  vec2 uv_ES;
  vec3 tangentLightPos;
  vec3 tangentViewPos;
	LightSource light;
} vs_out;


void main() {
	vec4 pos4 = vec4(position, 1.0);
  mat4 ModelView = View * Model;
  mat3 mv = mat3(ModelView);

  vec3 normal_cameraSpace =  normalize(NormalMatrix * normal);
  vec3 tangent_cameraSpace = normalize(NormalMatrix *tangent);
  vec3 bitangent_cameraSpace = normalize(NormalMatrix *bitangent);

  mat3 TBN = transpose(mat3(
          tangent_cameraSpace,
          bitangent_cameraSpace,
          normal_cameraSpace
      ));
  
  
	//-- Convert position and normal to Eye-Space:
	vs_out.position_ES = (ModelView * pos4).xyz;
  vs_out.position_WS = (Model * pos4).xyz;

  vs_out.tangentLightPos = TBN * light.position;
  vs_out.tangentViewPos =  (ModelView * vec4(TBN * position, 1.0)).xyz;

	vs_out.normal_ES = normalize(NormalMatrix * normal);
  vec2 uv_out = uv;
  uv_out.y = 1.0f - uv_out.y;
  vs_out.uv_ES = uv_out;

	vs_out.light = light;

	gl_Position = Perspective * ModelView * vec4(position, 1.0);
}
