#version 330

// Model-Space coordinates
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;


struct LightSource {
    vec3 position;
    vec3 color;
    float intensity;
};

struct Light {
    vec3 position_WS;
    vec3 position_ES;
    vec3 position_TS;
    vec3 color;
    float intensity;
};

uniform LightSource light[4];

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Perspective;

// Remember, this is transpose(inverse(ModelView)).  Normals should be
// transformed using this matrix instead of the ModelView matrix.
uniform mat3 NormalMatrix;
uniform bool testing;


out VsOutFsIn {
  vec2 UV;
  vec3 position_WS;
  vec3 position_ES;
  vec3 position_TS;
  vec3 normal_TS;
  Light lights[4];
} vs_out;


Light createLight(LightSource s) {
  Light l;
  l.position_WS = s.position;
  l.color = s.color;
  l.intensity = s.intensity;
  return l;
};

mat3 TBN;
uniform int numLights;

void passLightToFrag(int i) {
  if (i >= numLights) {
    return;
  }

  Light l = createLight(light[i]);
  l.position_ES = ( View * vec4(l.position_WS,1)).xyz + vs_out.position_ES;
  l.position_TS = TBN * l.position_ES;
  vs_out.lights[i] = l;
}


void main() {
	gl_Position = Perspective * View * Model * vec4(position,1);
	
	vs_out.position_WS = (Model * vec4(position,1)).xyz;
	vs_out.position_ES = -( View * Model * vec4(position,1)).xyz;

  vec2 uvOut = uv;
  uvOut.y = -uvOut.y;
	vs_out.UV = uvOut;

	vec3 vertexTangent_cameraspace = NormalMatrix * tangent;
	vec3 vertexBitangent_cameraspace = NormalMatrix * bitangent;
	vec3 vertexNormal_cameraspace = NormalMatrix * normal;
	
	TBN = transpose(mat3(
		vertexTangent_cameraspace,
		vertexBitangent_cameraspace,
		vertexNormal_cameraspace	
	));
	vs_out.position_TS =  TBN * vs_out.position_ES;
  vs_out.normal_TS = normalize( TBN * NormalMatrix * normal );
  

  passLightToFrag(0);
  passLightToFrag(1);
  passLightToFrag(2);
  passLightToFrag(3);
}
