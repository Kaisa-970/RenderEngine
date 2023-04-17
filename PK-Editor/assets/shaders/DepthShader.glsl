
#type vertex
#version 330 core
layout(location = 0) in vec4 a_Position;
uniform mat4 u_ModelMat;
uniform mat4 u_ViewProjectionMat;

void main()
{
	gl_Position = u_ViewProjectionMat * u_ModelMat * a_Position;
};

#type fragment
#version 330 core

void main()
{
	// float dep = LinearizeDepth(gl_FragCoord.z) / far_plane;
	// gl_FragColor = vec4(vec3(dep),1.0f);
};

