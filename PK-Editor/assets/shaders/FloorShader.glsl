
#type vertex
#version 330 core
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
uniform mat4 u_ModelMat;
uniform mat4 u_ViewProjectionMat;
out vec2 o_TexCoord;
out vec3 o_Normal;
out vec3 o_WorldPos;
void main()
{
	gl_Position = u_ViewProjectionMat * u_ModelMat * a_Position;
	o_TexCoord = a_TexCoord;
	o_Normal = a_Normal.xyz*0.5+0.5;
	o_WorldPos = (u_ModelMat * a_Position).xyz;
};

#type fragment
#version 330 core
out vec4 color;
uniform sampler2D u_Texture;
uniform samplerCube u_Skybox;
uniform vec3 u_CameraPos;
uniform vec3 u_LightPos;
uniform vec3 u_LightColor;
in vec2 o_TexCoord;
in vec3 o_Normal;
in vec3 o_WorldPos;
void main()
{
	vec3 lightPos = u_LightPos;
	vec3 lightColor = u_LightColor * 2.0;//颜色乘光强
	vec3 normal = normalize(o_Normal);
	vec3 lightDir = normalize(lightPos-o_WorldPos);
	vec3 viewDir = normalize(u_CameraPos-o_WorldPos);
	vec3 halfVector = normalize(lightDir+viewDir);
	float dhn = dot(halfVector,normal);
	float dhv = dot(halfVector,viewDir);
	float dln = dot(lightDir,normal);
	float dvn = dot(viewDir,normal);

	float d = length(lightPos-o_WorldPos);
	float attenuation = 1/(d*d);
	//*******Blinn-phong********//
	vec3 envColor = 0.1 * vec3(1.0);
	vec3 kd = texture(u_Texture,o_TexCoord).rgb;
	vec3 diffColor = kd * attenuation * lightColor * dln;
	float ks = 0.5;
	vec3 specColor = ks * attenuation * vec3(1,1,1) * pow(max(dhn,0),8);
	color = vec4(envColor + diffColor + specColor,1.0f);
	//float dep = gl_FragCoord.z;
	//color = vec4(dep,dep,dep,1);
};