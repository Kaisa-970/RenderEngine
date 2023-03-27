
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
	o_Normal =  mat3(transpose(inverse(u_ModelMat))) * a_Normal;//(u_ModelMat * vec4(a_Normal,0)).xyz;
	o_TexCoord = a_Texcoord;
};

#type fragment
#version 330 core
out vec4 color;
in vec2 o_TexCoord;
in vec3 o_Normal;
in vec3 o_WorldPos;
uniform vec3 u_CameraPos;
uniform vec3 u_LightPos;
uniform vec3 u_LightColor;
uniform sampler2D u_Texture;
uniform float u_Roughness;
uniform float u_Metallic;

#define PI 3.14159

float Fresnel(float dvn);
float NDF(float rough2,float dhn);
float GGX(float rough,float dln);

void main()
{
	vec3 lightPos = u_LightPos;
	vec3 lightColor = u_LightColor * 1.0;//颜色乘光强
	vec3 normal = normalize(o_Normal);
	vec3 lightDir = normalize(lightPos-o_WorldPos);
	vec3 viewDir = normalize(u_CameraPos-o_WorldPos);
	vec3 halfVector = normalize(lightDir+viewDir);
	float dhn = dot(halfVector,normal);
	float dhv = dot(halfVector,viewDir);
	float dln = dot(lightDir,normal);
	float dvn = dot(viewDir,normal);

	float roughness = u_Roughness;
	float metallic = u_Metallic;

	float d = length(lightPos-o_WorldPos);
	float attenuation = 1;//1/(d*d);
	//*******Blinn-phong********//
	// vec3 envColor = texture(u_Texture,o_TexCoord).rgb * 0.1;
	// float k = 1/(d*d);
	// vec3 diffColor = k * lightColor * dln;

	// vec3 specColor = k * vec3(1,1,1) * pow(max(dhn,0),8);
	// color = vec4(envColor + diffColor + specColor,1.0f);
	//**************************//

	//*******PBR********//
	vec3 ambDirect;

	float kd = 1/PI;
	vec3 abedo = vec3(1.0);//texture(u_Texture,o_TexCoord).rgb;
	
	vec3 diffDirect = kd*abedo*attenuation*lightColor*dln;

	float F = Fresnel(dvn);
	float D = NDF(roughness*roughness,dhn);
	float G = GGX(roughness*roughness,dln)*GGX(roughness*roughness,dvn);
	float ks = 1/(4*PI*dln*dvn);
	vec3 specDirect = ks*F*D*G*(attenuation*lightColor)*dln;

	color = vec4(ambDirect + diffDirect + specDirect,1.0f);
	//**************************//
	//color = vec4(o_Normal,1.0f);
	float value = F;
	//color = vec4(value,value,value,1.0f);
};

float Fresnel(float dvn)
{
	float F0 = 0.2;
	return F0+(1-F0)*pow((1-dvn),5);
}

float NDF(float rough2,float dhn){
	return rough2/(PI*pow((dhn*dhn*(rough2-1)+1),2));
}

float GGX(float rough,float dln){
	float k=(rough+1)*(rough+1)/8.0;
	return dln/mix(dln,1,k);
}