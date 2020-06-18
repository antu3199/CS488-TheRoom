#version 330

uniform bool picking;
uniform bool testing;

struct LightSource {
    vec3 position;
    vec3 color;
    float intensity;
};

in VsOutFsIn {
	vec3 position_ES; // Eye-space position
  vec3 position_WS;
	vec3 normal_ES;   // Eye-space normal
  vec2 uv_ES;
  vec3 tangentLightPos;
  vec3 tangentViewPos;
	LightSource light;
} fs_in;

out vec4 fragColour;

struct Material {
    vec3 kd;
    vec3 ks;
    float shininess;
};
uniform Material material;

// Ambient light intensity for each RGB component.
uniform vec3 ambientIntensity;

// Texture stuff
uniform sampler2D texture0;
uniform sampler2D normalMap;

uniform bool hasTextureMap;
uniform bool hasNormalMap;

vec3 phongModel(vec3 fragPosition, vec3 fragNormal, vec3 lightPos) {
	LightSource light = fs_in.light;
    // Direction from fragment to light source.
    vec3 l = normalize(lightPos - fragPosition);

    // Direction from fragment to viewer (origin - fragPosition).
    vec3 v = normalize(-fragPosition);

    float n_dot_l = max(dot(fragNormal, l), 0.0);

    vec3 diffuse;
    vec3 texColour = hasTextureMap ? vec3(texture(texture0, fs_in.uv_ES)) : vec3(1.0);
    diffuse = material.kd * texColour;

    vec3 specular = vec3(0.0);

    if (n_dot_l > 0.0) {
      // Halfway vector.
      vec3 h = normalize(v + l);
      float n_dot_h = max(dot(fragNormal, h), 0.0);
      //specular = material.ks * pow(max(0.0, dot(v, reflect(-l, fragNormal))), material.shininess);
      specular = material.ks * pow(n_dot_h, material.shininess);
    }

    float distanceToLight = length(lightPos - fs_in.position_WS);
    float attenuation = 1.0 / (1.0 + pow(distanceToLight, 2));
    
    return ambientIntensity + attenuation * light.color * light.intensity * (diffuse + specular); //max(light.rgbIntensity * n_dot_l * (diffuse + specular), ambientIntensity);
}

void main() {
  vec3 lightPos = hasNormalMap ? fs_in.tangentLightPos : fs_in.light.position;
  
	if( picking ) {
		fragColour = vec4(material.kd + vec3(fs_in.uv_ES.x, fs_in.uv_ES.y, 0.0), 1.0) + vec4(phongModel(fs_in.position_ES, fs_in.normal_ES, lightPos), 1.0);
	} else if ( testing ) {
    vec3 normalColour = vec3(texture(normalMap, fs_in.uv_ES));
    fragColour = vec4(normalColour.x, normalColour.y, 0.0, 1.0);
  } else {

    if (hasNormalMap) {

      vec3 textureNormal = normalize(texture(normalMap, fs_in.uv_ES).rgb * 2.0 - 1.0);
      vec3 bumppedTexture = phongModel(fs_in.tangentViewPos, textureNormal, lightPos);
	    fragColour = vec4(bumppedTexture, 1.0);
    } else {
      fragColour = vec4(phongModel(fs_in.position_ES, fs_in.normal_ES, lightPos), 1.0);
    }
	}
}
