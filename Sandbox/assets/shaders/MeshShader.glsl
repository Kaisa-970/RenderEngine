
#type vertex
#version 330 core
layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_Normal;
//layout(location = 1) in vec2 a_TexCoord;
uniform mat4 u_ModelMat;
uniform mat4 u_ViewProjectionMat;
out vec2 o_TexCoord;
out vec3 o_Normal;
void main()
{
	gl_Position = u_ViewProjectionMat * u_ModelMat * a_position;
	o_Normal = a_Normal.xyz*0.5+0.5;
	//o_TexCoord = a_TexCoord;
};

#type fragment
#version 330 core
out vec4 color;
in vec3 o_Normal;
//uniform vec3 u_Color;
//uniform sampler2D u_Texture;
//in vec2 o_TexCoord;
void main()
{
	color = vec4(o_Normal,1.0f);
};