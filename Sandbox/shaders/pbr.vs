#version 330 core

layout(location=0) in vec3 a_Position;
layout(location=1) in vec2 a_Texcoord;
layout(location=2) in vec3 a_Normal;

uniform mat4 u_ViewProjectionMat;
uniform mat4 u_ModelMat;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

void main()
{
    WorldPos = u_ModelMat * a_Position;
    gl_Position = u_ViewProjectionMat * WorldPos;

    TexCoords = a_Texcoord;

    Normal = mat3(u_ModelMat) * a_Normal;
}