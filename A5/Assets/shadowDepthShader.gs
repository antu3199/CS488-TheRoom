#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowMatrices[6];

out vec4 FragPos;

void handleVertex(int face, int vertex) {
    FragPos = gl_in[vertex].gl_Position;
    gl_Position = shadowMatrices[face] * FragPos;
    EmitVertex();
}

void handleFace(int face) {
  gl_Layer = face;
  handleVertex(face, 0);
  handleVertex(face, 1);
  handleVertex(face, 2);
  EndPrimitive();
}

void main()
{
    gl_Layer = 0;
    FragPos = gl_in[0].gl_Position;
    gl_Position = shadowMatrices[0] * FragPos;
    EmitVertex();
    FragPos = gl_in[1].gl_Position;
    gl_Position = shadowMatrices[0] * FragPos;
    EmitVertex();
    FragPos = gl_in[2].gl_Position;
    gl_Position = shadowMatrices[0] * FragPos;
    EmitVertex();
    EndPrimitive();


    gl_Layer = 1;
    FragPos = gl_in[0].gl_Position;
    gl_Position = shadowMatrices[1] * FragPos;
    EmitVertex();
    FragPos = gl_in[1].gl_Position;
    gl_Position = shadowMatrices[1] * FragPos;
    EmitVertex();
    FragPos = gl_in[2].gl_Position;
    gl_Position = shadowMatrices[1] * FragPos;
    EmitVertex();
    EndPrimitive();

    gl_Layer = 2;
    FragPos = gl_in[0].gl_Position;
    gl_Position = shadowMatrices[2] * FragPos;
    EmitVertex();
    FragPos = gl_in[1].gl_Position;
    gl_Position = shadowMatrices[2] * FragPos;
    EmitVertex();
    FragPos = gl_in[2].gl_Position;
    gl_Position = shadowMatrices[2] * FragPos;
    EmitVertex();
    EndPrimitive();

    gl_Layer = 3;
    FragPos = gl_in[0].gl_Position;
    gl_Position = shadowMatrices[3] * FragPos;
    EmitVertex();
    FragPos = gl_in[1].gl_Position;
    gl_Position = shadowMatrices[3] * FragPos;
    EmitVertex();
    FragPos = gl_in[2].gl_Position;
    gl_Position = shadowMatrices[3] * FragPos;
    EmitVertex();
    EndPrimitive();

    gl_Layer = 4;
    FragPos = gl_in[0].gl_Position;
    gl_Position = shadowMatrices[4] * FragPos;
    EmitVertex();
    FragPos = gl_in[1].gl_Position;
    gl_Position = shadowMatrices[4] * FragPos;
    EmitVertex();
    FragPos = gl_in[2].gl_Position;
    gl_Position = shadowMatrices[4] * FragPos;
    EmitVertex();
    EndPrimitive();

    gl_Layer = 5;
    FragPos = gl_in[0].gl_Position;
    gl_Position = shadowMatrices[5] * FragPos;
    EmitVertex();
    FragPos = gl_in[1].gl_Position;
    gl_Position = shadowMatrices[5] * FragPos;
    EmitVertex();
    FragPos = gl_in[2].gl_Position;
    gl_Position = shadowMatrices[5] * FragPos;
    EmitVertex();
    EndPrimitive();
} 

