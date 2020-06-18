#version 330

uniform bool picking;
uniform bool testing;

struct Light {
    vec3 position_WS;
    vec3 position_ES;
    vec3 position_TS;
    vec3 color;
    float intensity;
};

in VsOutFsIn {
  vec2 UV;
  vec3 position_WS;
  vec3 position_ES;
  vec3 position_TS;
  vec3 normal_TS;
  Light lights[4];
} fs_in;


out vec4 fragColour;

struct Material {
    vec3 kd;
    vec3 ks;
    float shininess;
};
uniform Material material;

// Ambient lights intensity for each RGB component.
uniform vec3 ambientIntensity;

// Texture stuff
uniform sampler2D texture0;
uniform sampler2D normalMap;

uniform bool hasTextureMap;
uniform bool hasNormalMap;

uniform samplerCube depthMap;
uniform float farPlane;

uniform bool isCelShaded;

uniform int numLights;


uniform bool toggleTexture;
uniform bool toggleNormalMap;
uniform bool toggleCelShading;
uniform bool toggleShadows;

// Const variables
float diffuseCelCutoff = 0.3;
float specCelCutoff = 0.5;

float ShadowCalculation(vec3 fragPos, vec3 lightPos, vec3 viewPos)
{
    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);

    // Smooth edges using PCF:
    // Source: https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows
     float shadow = 0.0;
     float bias = 0.05; 
     float samples = 1.5;
     float offset = 0.1;
     for(float x = -offset; x < offset; x += offset / (samples * 0.5))
     {
         for(float y = -offset; y < offset; y += offset / (samples * 0.5))
         {
             for(float z = -offset; z < offset; z += offset / (samples * 0.5))
             {
                 float closestDepth = texture(depthMap, fragToLight + vec3(x, y, z)).r; // use lightdir to lookup cubemap
                 closestDepth *= farPlane;
                 if(currentDepth - bias > closestDepth)
                     shadow += 1.0;
            }
         }
     }
     
    shadow /= (samples * samples * samples);

    return shadow;
}


vec3 computeColor(int i, vec3 n, vec3 v, vec3 diffuse) {
    vec3 color = vec3(0.0, 0.0, 0.0);
    if (i >= numLights) {
      return color;
    }

    float v_dot_n = dot(v, n);
    vec3 specular = vec3(0.0);
    float distance = length( fs_in.lights[i].position_WS - fs_in.position_WS );
    vec3 l = normalize(fs_in.lights[i].position_TS);
    float n_dot_l = max(dot(n, l), 0.0);

    float specularMask = 1.0;

    if (n_dot_l > 0) {
      vec3 h = normalize(v + l);
      float n_dot_h = max(dot(n, h), 0.0);
      vec3 r = normalize(-reflect(l, n));
      float specMag = pow(n_dot_h, material.shininess);
      specular = material.ks * pow(n_dot_h, material.shininess);
      if (toggleCelShading && isCelShaded == true) {
        specularMask = specMag > specCelCutoff ? 1.0 : 0.0;
      }
    }


    float atten = distance*distance;
    if (atten < 1) {
      atten = 1;
    }
    
    float shadow = 0.0;
    if (i == 0 && toggleShadows) {
      shadow = min(ShadowCalculation(fs_in.position_WS, fs_in.lights[i].position_WS, fs_in.position_ES), 0.5f);
    } 

    float edgeDetection = 1.0;
    float lightIntensity = fs_in.lights[i].intensity;
    if (toggleCelShading && isCelShaded == true) {
      edgeDetection = v_dot_n > diffuseCelCutoff ? 1.0 : 0.0; 
      lightIntensity = 1.0;
    }
    vec3 toonShadowColour = edgeDetection == 0.0 ? diffuse * 0.15 : vec3(0.0);



    color = toonShadowColour + (1.0 - shadow) * edgeDetection *
      ( diffuse * fs_in.lights[i].color * lightIntensity / atten +
        specularMask * specular * fs_in.lights[i].color * lightIntensity / atten);
    return color;
}


void main() {

  vec3 color = ambientIntensity;
	vec3 diffuse = material.kd;
  if (toggleTexture && hasTextureMap) {
     diffuse = texture( texture0, fs_in.UV ).rgb * material.kd;
  }

	vec3 TextureNormal_tangentspace = fs_in.normal_TS;
  if (toggleNormalMap && hasNormalMap) {
    TextureNormal_tangentspace = normalize(texture( normalMap, fs_in.UV).rgb*2.0 - 1.0);
  }
  
  // Independent of light variables
	vec3 n = TextureNormal_tangentspace;
  vec3 v = normalize(fs_in.position_TS);
  color += computeColor(0, n, v, diffuse);
  color += computeColor(1, n, v, diffuse);
  color += computeColor(2, n, v, diffuse);
  color += computeColor(3, n, v, diffuse);
  fragColour = vec4(color, 1.0);
  

}
