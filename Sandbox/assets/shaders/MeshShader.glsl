
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
uniform vec3 u_CameraPos;
uniform sampler2D u_Texture;
void main()
{
	vec3 lightPos = vec3(2,2,0);
	vec3 lightColor = vec3(1.0,0.8,0.2);
	vec3 normal = normalize(o_Normal);
	vec3 lightDir = normalize(lightPos-o_WorldPos);
	vec3 viewDir = normalize(u_CameraPos-o_WorldPos);
	vec3 halfVector = normalize(lightDir+viewDir);
	float dhn = dot(halfVector,normal);
	float dhv = dot(halfVector,viewDir);
	float dln = dot(lightDir,normal);

	vec3 envColor = texture(u_Texture,o_TexCoord).rgb * 0.1;

	float d = length(lightPos-o_WorldPos);
	float k = 1/(d*d);
	vec3 diffColor = k * lightColor * dln;

	vec3 specColor = k * vec3(1,1,1) * pow(max(dhn,0),8);
	color = vec4(envColor + diffColor + specColor,1.0f);
	//color = vec4(o_Normal,1.0f);
	//color = vec4(0.8,0.8,0.2,1.0f);
};