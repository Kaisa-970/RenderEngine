
#type vertex
#version 330 core
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec2 a_Texcoord;
layout(location = 2) in vec3 a_Normal;
//layout(location = 1) in vec2 a_TexCoord;
uniform mat4 u_ModelMat;
uniform mat4 u_ViewProjectionMat;
out vec2 o_TexCoord;
out vec3 o_Normal;
out vec3 o_WorldPos;
void main()
{
	o_WorldPos = (u_ModelMat * a_Position).xyz;
	gl_Position = u_ViewProjectionMat * u_ModelMat * a_Position;
	o_Normal = a_Normal.xyz*0.5+0.5;
	o_TexCoord = a_Texcoord;
};

#type fragment
#version 330 core
out vec4 color;
in vec2 o_TexCoord;
in vec3 o_Normal;
in vec3 o_WorldPos;
//uniform vec3 u_CameraPos;
uniform vec3 u_LightColor;

void main()
{
	color = vec4(u_LightColor,1.0f);
};