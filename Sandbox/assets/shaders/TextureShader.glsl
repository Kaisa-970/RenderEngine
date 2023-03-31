
#type vertex
#version 330 core
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec2 a_TexCoord;
uniform mat4 u_ModelMat;
uniform mat4 u_ViewProjectionMat;
out vec2 o_TexCoord;
void main()
{
	gl_Position = u_ViewProjectionMat * u_ModelMat * a_Position;
	o_TexCoord = a_TexCoord;
};

#type fragment
#version 330 core
out vec4 color;
uniform vec4 u_Color;
uniform float u_Tilling;
uniform sampler2D u_Texture;
in vec2 o_TexCoord;
void main()
{
	color = texture(u_Texture, o_TexCoord * u_Tilling) * u_Color;
};