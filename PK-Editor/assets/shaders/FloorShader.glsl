
#type vertex
#version 330 core
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec3 a_Tangent;
uniform mat4 u_ModelMat;
uniform mat4 u_ViewProjectionMat;

uniform mat4 u_LightSpaceMatrix;

out vec2 o_TexCoord;
out vec3 o_Normal;
out vec3 o_WorldPos;
out mat3 o_BTN;
out vec4 o_LightSpacePos;
void main()
{
	gl_Position = u_ViewProjectionMat * u_ModelMat * a_Position;
	o_TexCoord = a_TexCoord;
	o_WorldPos = (u_ModelMat * a_Position).xyz;

	o_Normal = normalize(a_Normal.xyz);
	vec3 T = normalize(vec3(u_ModelMat * vec4(a_Tangent,0.0)));
	vec3 N = normalize(vec3(u_ModelMat * vec4(a_Normal,0.0)));
	vec3 B = normalize(vec3(u_ModelMat * vec4(cross(N,T),0.0)));
	o_BTN = mat3(T,B,N);

	o_LightSpacePos = u_LightSpaceMatrix * a_Position;
};

#type fragment
#version 330 core
out vec4 color;
uniform sampler2D u_Texture;
uniform samplerCube u_Skybox;
uniform sampler2D u_NormMap;
uniform vec3 u_CameraPos;
uniform vec3 u_LightPos;
uniform vec3 u_LightColor;
in vec2 o_TexCoord;
in vec3 o_Normal;
in vec3 o_WorldPos;
in mat3 o_BTN;
in vec4 o_LightSpacePos;

uniform sampler2D depthMap;
uniform float near_plane;
uniform float far_plane;

//float LinearizeDepth(float depth);
float ShawdowCalculation(vec4 lightSpacePos);
void main()
{
	vec3 texNorm = texture(u_NormMap,o_TexCoord).rgb;
	texNorm = normalize(texNorm*2.0-1.0);
	texNorm = normalize(o_BTN*texNorm);
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
	vec3 finalColor = (envColor + diffColor + specColor);// * ShawdowCalculation(o_LightSpacePos);
	color = vec4(finalColor ,1.0f);
	// float dep = ShawdowCalculation(o_LightSpacePos);
	// color = vec4(dep,dep,dep,1);
};

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

float ShawdowCalculation(vec4 lightSpacePos)
{
	vec3 ndcPos = lightSpacePos.xyz/lightSpacePos.w;
	ndcPos = ndcPos*0.5+0.5;
	float currentDep = ndcPos.z;

	float lightDep = texture(depthMap,ndcPos.xy).r;

	return currentDep>lightDep?0.0:1.0;
}