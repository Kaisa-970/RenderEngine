
#type vertex
#version 330 core
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TextureIndex;
layout(location = 4) in float a_TillingFactor;
//uniform mat4 u_ModelMat;
uniform mat4 u_ViewProjectionMat;
out vec2 o_TexCoord;
out vec4 o_Color;
out float o_TextureSlotIdx;
out float o_TillingFactor;
void main()
{
	gl_Position = u_ViewProjectionMat * a_Position;
	o_TexCoord = a_TexCoord;
	o_Color = a_Color;
	o_TextureSlotIdx = a_TextureIndex;
	o_TillingFactor = a_TillingFactor;
};

#type fragment
#version 330 core
out vec4 color;
uniform sampler2D u_Texture[32];
in vec2 o_TexCoord;
in vec4 o_Color;
in float o_TextureSlotIdx;
in float o_TillingFactor;
void main()
{
	//TODO:Texture Tilling
	color = texture(u_Texture[int(o_TextureSlotIdx)], o_TexCoord * o_TillingFactor) * o_Color;
};