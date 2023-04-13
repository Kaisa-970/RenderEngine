
#type vertex
#version 330 core
layout(location = 0) in vec4 a_Position;

//uniform mat4 u_ModelMat;
uniform mat4 u_ViewProjectionMat;
//uniform mat4 u_ModelMat;
out vec3 o_TexCoord;

void main()
{
	gl_Position = u_ViewProjectionMat * a_Position;
	o_TexCoord = a_Position.xyz;

};

#type fragment
#version 330 core
out vec4 color;
uniform samplerCube u_Texture;
in vec3 o_TexCoord;

void main()
{
	//TODO:Texture Tilling
	vec3 dir = normalize(o_TexCoord);
	color = texture(u_Texture,-o_TexCoord);
};