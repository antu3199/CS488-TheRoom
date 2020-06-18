

#version 330

out vec4 fragColour;

in VsOutFsIn {
  vec3 position_M;
} fs_in;

uniform float absTime;

uniform int p[512];

  float fade(float t)
  {
    return pow(t, 3) * (t * (t * 6 - 15) + 10);
  }

  float lerp(
      float t,
      float a,
      float b)
  {
    return a + t * (b - a);
  }

  float grad(
      int hash,
      float x,
      float y,
      float z)
  {
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
  }

  float noise(
      float x,
      float y,
      float z)
  {
    int X = int(floor(x)) & 255;
    int Y = int(floor(y)) & 255;
    int Z = int(floor(z)) & 255;
    x = x - floor(x);
    y = y - floor(y);
    z = z - floor(z);

    float u = fade(x);
    float v = fade(y);
    float w = fade(z);

    int A = p[X] + Y;
    int AA = p[A] + Z;
    int AB = p[A + 1] + Z;
    int B = p[X + 1] + Y;
    int BA = p[B] + Z;
    int BB = p[B + 1] + Z;

    return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z), grad(p[BA], x - 1, y, z)), lerp(u, grad(p[AB], x, y - 1, z), grad(p[BB], x - 1, y - 1, z))),
           lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1), grad(p[BA + 1], x - 1, y, z - 1)),
           lerp(u, grad(p[AB + 1], x, y - 1, z - 1), grad(p[BB + 1], x - 1, y - 1, z - 1))));
  }


  float negOneTOneToZeroTOne(float t) {
    return (t + 1.0) / 2.0;
  }

void main() {
    float x = negOneTOneToZeroTOne(fs_in.position_M.x);
    float y = negOneTOneToZeroTOne(fs_in.position_M.z);
    float scale = 25.0;
    float noiseMag = noise(x * scale, y * scale, absTime);
    // Map -1 to 1  -> 0 to 1
    noiseMag = negOneTOneToZeroTOne(noiseMag);
    vec3 noiseCol = vec3(noiseMag);
    fragColour = vec4(noiseCol, 1.0);
}
