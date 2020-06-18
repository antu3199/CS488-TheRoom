#version 330

uniform bool picking;
uniform bool testing;

struct Light {
    vec3 position_WS;
    vec3 color;
    float intensity;
};

in VsOutFsIn {
	vec3 position_ES; // eye space
  vec3 position_WS;
  vec2 uv;
  vec3 normal;
  vec3 bitangent;
  vec3 tangent;
	Light light;
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
    vec3 texColour = hasTextureMap ? vec3(texture(texture0, fs_in.uv)) : vec3(1.0);
    diffuse = material.kd * texColour;

    vec3 specular = vec3(0.0);

    if (n_dot_l > 0.0) {
      // Halfway vector.
      vec3 h = normalize(v + l);
      float n_dot_h = max(dot(fragNormal, h), 0.0);
      specular = material.ks * pow(n_dot_h, material.shininess);
    }

    float distanceToLight = length(lightPos - fragNormal);
    float attenuation = 1.0 / (1.0 + pow(distanceToLight, 2));
    
    return ambientIntensity + attenuation * light.color * light.intensity * (diffuse + specular); //max(light.rgbIntensity * n_dot_l * (diffuse + specular), ambientIntensity);
}


void main() {
  
	if( picking ) {
    vec3 normal = normalize(texture(normalMap, fs_in.uv).rgb * 2.0 - 1.0);
    normal = normalize( mat3( fs_in.tangent, fs_in.bitangent, fs_in.normal) * normal );
    fragColour = vec4(fs_in.normal.x, 1.0, 0.0, 1.0);
	} else if ( testing ) {
    vec3 normal = normalize(texture(normalMap, fs_in.uv).rgb * 2.0 - 1.0);
    normal = normalize( mat3( fs_in.tangent, fs_in.bitangent, fs_in.normal) * normal );
    fragColour = vec4(fs_in.normal.x, 1.0, 0.0, 1.0);
  } else {
    vec3 normal;
    if (hasNormalMap) {
      normal = normalize(texture(normalMap, fs_in.uv).rgb * 2.0 - 1.0);
    } else {
      normal = fs_in.normal;
    }
    mat3 tbn = transpose(mat3( fs_in.tangent, fs_in.bitangent, fs_in.normal));

    normal = normalize( tbn * normal );

    vec3 lightDirection = (tbn * fs_in.light.position) - fs_in.position_ES;
    vec3 l =  normalize(lightDirection);
    vec3 v = normalize(-fs_in.position_ES);

    vec3 diffuse = vec3(0.0);
    float diffuseIntensity = max(dot(normal, l), 0.0);

    vec3 texColour = hasTextureMap ? vec3(texture(texture0, fs_in.uv)) : vec3(1.0);
    
    vec3 specular = vec3(0.0);
    diffuse = material.kd * texColour;

    if (diffuseIntensity > 0) {
      vec3 h = normalize(v + l);
      float n_dot_h = max(dot(normal, h), 0.0);
      vec3 r = normalize(-reflect(l, normal));
      //specular = clamp(material.ks * pow(max(dot(r, eyeDirection), 0), p3d_Material.shininess), 0, 1);
      specular = material.ks * pow(n_dot_h, material.shininess);
    }
    

    float distanceToLight = length(fs_in.light.position - fs_in.position_WS);
    float attenuation = 1.0 / (1.0 + pow(distanceToLight, 2));
    
    vec3 col = ambientIntensity + attenuation * fs_in.light.color * fs_in.light.intensity * (diffuse + specular);

    fragColour = vec4(col, 1.0);
  }
}
