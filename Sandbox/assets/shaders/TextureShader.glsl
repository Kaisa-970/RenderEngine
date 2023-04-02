
#type vertex
#version 330 core
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
//uniform mat4 u_ModelMat;
uniform mat4 u_ViewProjectionMat;
out vec2 o_TexCoord;
out vec4 o_Color;
void main()
{
	gl_Position = u_ViewProjectionMat * a_Position;
	o_TexCoord = a_TexCoord;
	o_Color = a_Color;
};

#type fragment
#version 330 core
out vec4 color;
uniform vec4 u_Color;
uniform float u_Tilling;
uniform sampler2D u_Texture;
in vec2 o_TexCoord;
in vec4 o_Color;
void main()
{
	//color = texture(u_Texture, o_TexCoord * u_Tilling) * u_Color;
	color = o_Color;
};