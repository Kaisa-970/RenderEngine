#include <PKEngine.h>
#include <imgui.h>
#include <glm/gtx/transform.hpp>
#include <GLFW/glfw3.h>

class ExampleLayer : public PKEngine::Layer {
public:
	ExampleLayer() : m_Camera(-1.6f, 1.6f, -0.9f, 0.9f) , m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(PKEngine::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f,0.0f,1.0f,1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f,0.0f,1.0f,1.0f,
			 0.0f,  0.5f, 0.0f,	1.0f,1.0f,0.0f,1.0f
		};

		uint32_t indices[3] = {
			0,1,2
		};

		std::shared_ptr<PKEngine::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(PKEngine::VertexBuffer::Create(vertices, sizeof(vertices)));

		PKEngine::BufferLayout layout = {
				{PKEngine::ShaderDataType::Float3,"a_Position"},
				{PKEngine::ShaderDataType::Float4,"a_Color",true}
		};
		m_VertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		std::shared_ptr<PKEngine::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(PKEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);



		m_SqureVA.reset(PKEngine::VertexArray::Create());

		float squreVertices[8 * 8] = {
	-0.5f, -0.5f, -0.5f, 0.0f,0.0f,0.0f,0.0f,-1.0f,
	 0.5f, -0.5f, -0.5f, 1.0f,0.0f,0.0f,0.0f,-1.0f,
	 0.5f,  0.5f, -0.5f, 1.0f,1.0f,0.0f,0.0f,-1.0f,
	 -0.5f, 0.5f, -0.5f, 0.0f,1.0f,0.0f,0.0f,-1.0f,

	 -0.5f, -0.5f,-1.0f, 0.0f,0.0f,0.0f,0.0f,1.0f,
	 0.5f, -0.5f, -1.0f, 1.0f,0.0f,0.0f,0.0f,1.0f,
	 0.5f,  0.5f, -1.0f, 1.0f,1.0f,0.0f,0.0f,1.0f,
	 -0.5f, 0.5f, -1.0f, 0.0f,1.0f,0.0f,0.0f,1.0f,
		};

		uint32_t squreIndices[] = {
			0,1,2,
			2,3,0,

			1,5,6,
			6,2,1,

			4,7,6,
			6,5,4,

			0,1,2,
			2,3,0,

			0,1,2,
			2,3,0,

			0,1,2,
			2,3,0,
		};

		std::shared_ptr<PKEngine::VertexBuffer> squreVB;
		squreVB.reset(PKEngine::VertexBuffer::Create(squreVertices, sizeof(squreVertices)));
		PKEngine::BufferLayout squreLayout = {
		{PKEngine::ShaderDataType::Float3,"a_Position"},
		{PKEngine::ShaderDataType::Float2,"a_Texcoord"},
		{PKEngine::ShaderDataType::Float3,"a_Normal"}
		};
		squreVB->SetLayout(squreLayout);
		m_SqureVA->AddVertexBuffer(squreVB);

		std::shared_ptr<PKEngine::IndexBuffer> squreIB;
		squreIB.reset(PKEngine::IndexBuffer::Create(squreIndices, sizeof(squreIndices) / sizeof(uint32_t)));
		m_SqureVA->SetIndexBuffer(squreIB);

		std::string pbrVertexS = R"(
			#version 330 core

layout(location=0) in vec3 a_Position;
layout(location=1) in vec2 a_Texcoord;
layout(location=2) in vec3 a_Normal;

uniform mat4 u_ViewProjectionMat;
uniform mat4 u_ModelMat;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

void main()
{
    WorldPos = (u_ModelMat * vec4(a_Position,1.0)).xyz;
    gl_Position = u_ViewProjectionMat * vec4(WorldPos,1.0);

    TexCoords = a_Texcoord;

    Normal = mat3(u_ModelMat) * a_Normal;
}
)";

		std::string pbrFragS = R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

// material parameters
uniform vec3  albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

// lights
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

uniform vec3 camPos;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main()
{       
    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - WorldPos);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 4; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);
        float distance    = length(lightPositions[i] - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance     = lightColors[i] * attenuation;        

        // cook-torrance brdf
        float NDF = DistributionGGX(N, H, roughness);        
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;     

        vec3 nominator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; 
        vec3 specular     = nominator / denominator;

        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);                
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; 
    }   

    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  

    FragColor = vec4(color, 1.0);
}  
)";


		glm::vec3 lightPositions[] = {
			glm::vec3(-1.0f,  0.0f, 1.0f),
			glm::vec3(1.0f,  0.0f, 1.0f),
			glm::vec3(-1.0f, -0.0f, 1.0f),
			glm::vec3(1.0f, -0.0f, 1.0f),
		};
		glm::vec3 lightColors[] = {
			glm::vec3(300.0f, 300.0f, 300.0f),
			glm::vec3(300.0f, 300.0f, 300.0f),
			glm::vec3(300.0f, 300.0f, 300.0f),
			glm::vec3(300.0f, 300.0f, 300.0f)
		};

		m_PBRShader.reset(new PKEngine::Shader(pbrVertexS, pbrFragS));
		m_PBRShader->Bind();
		glm::mat4 model = glm::mat4(1.0f);
		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
		{
			glm::vec3 newPos = lightPositions[i];// +glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			newPos = lightPositions[i];
			m_PBRShader->SetUniform3f("lightPositions[" + std::to_string(i) + "]", newPos);
			m_PBRShader->SetUniform3f("lightColors[" + std::to_string(i) + "]", lightColors[i]);

			//model = glm::mat4(1.0f);
			//model = glm::translate(model, newPos);
			//model = glm::scale(model, glm::vec3(0.5f));
			//m_PBRShader->SetUniformMat4f("model", model);
			//renderSphere();
		}

		m_PBRShader->SetUniform3f("albedo", glm::vec3(0.5,0.2,0.3));
		m_PBRShader->SetUniform1f("metallic", 0.0);
		m_PBRShader->SetUniform1f("roughness", 0.0);
		m_PBRShader->SetUniform1f("ao", 1);
	}
	~ExampleLayer() {}
	virtual void OnImGuiRender()override {
		
	}

	virtual void OnUpdate(PKEngine::Timestep ts) override {
		//PK_TRACE("Delta time:{0}s ({1}ms)", ts.GetSeconds(), ts.GetMillionSeconds());
		float deltaTime = ts;
		if (PKEngine::Input::IsKeyPressed(PK_KEY_W)) {
			m_CameraPosition.y += deltaTime * m_CameraMoveSpeed;
		}
		else if (PKEngine::Input::IsKeyPressed(PK_KEY_S)) {
			m_CameraPosition.y -= deltaTime * m_CameraMoveSpeed;
		}

		if (PKEngine::Input::IsKeyPressed(PK_KEY_A)) {
			m_CameraPosition.x -= deltaTime * m_CameraMoveSpeed;
		}
		else if (PKEngine::Input::IsKeyPressed(PK_KEY_D)) {
			m_CameraPosition.x += deltaTime * m_CameraMoveSpeed;
		}

		if (PKEngine::Input::IsMouseButtonPressed(0)) {
			m_CameraRotation -= deltaTime * m_CameraRotateSpeed;
		}
		else if (PKEngine::Input::IsMouseButtonPressed(1)) {
			m_CameraRotation += deltaTime * m_CameraRotateSpeed;
		}


		PKEngine::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		PKEngine::RenderCommand::Clear();
		PKEngine::Renderer::BeginScene(m_Camera);

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
		auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));

		auto transform = glm::translate(glm::mat4(1.0f), m_CameraPosition) * scale;

		m_PBRShader->Bind();
		m_PBRShader->SetUniform3f("camPos", m_CameraPosition);
		PKEngine::Renderer::Submit(m_SqureVA, m_PBRShader, transform);


		//m_SqureShader->Bind();
		//for (int i = 0; i < 20; i++) {
		//	for (int j = 0; j < 20; j++)
		//	{
		//		glm::vec3 pos(i * 0.11f, j * 0.11f, 0);
		//		auto transform = glm::translate(glm::mat4(1.0f), pos) * scale;
		//		PKEngine::Renderer::Submit(m_SqureVA, m_SqureShader, transform);
		//	}
		//	
		//}


		//PKEngine::Renderer::Submit(m_VertexArray, m_Shader);

		PKEngine::Renderer::EndScene();
	}

	virtual void OnEvent(PKEngine::Event& e) override {
		//PKEngine::EventDispatcher dispatcher(e);
		//dispatcher.Dispatch<PKEngine::KeyPressedEvent>(PK_BIND_EVENT_FN(ExampleLayer::OnKeyPressed));
	}

	bool OnKeyPressed(PKEngine::KeyPressedEvent& e) {
		if (e.GetKeyCode() == PK_KEY_W) {
			m_CameraPosition.y += m_CameraMoveSpeed;
		}
		else if (e.GetKeyCode() == PK_KEY_S) {
			m_CameraPosition.y -= m_CameraMoveSpeed;
		}
		else if (e.GetKeyCode() == PK_KEY_A) {
			m_CameraPosition.x -= m_CameraMoveSpeed;
		}
		else if (e.GetKeyCode() == PK_KEY_D) {
			m_CameraPosition.x += m_CameraMoveSpeed;
		}
		return false;
	}

private:
	std::shared_ptr<PKEngine::VertexArray> m_VertexArray;
	std::shared_ptr<PKEngine::Shader> m_Shader;


	std::shared_ptr<PKEngine::VertexArray> m_SqureVA;
	std::shared_ptr<PKEngine::Shader> m_SqureShader;
	std::shared_ptr<PKEngine::Shader> m_PBRShader;

	PKEngine::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 1.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotateSpeed = 120.0f;

};

class Sandbox : public PKEngine::Application {
public:
	Sandbox() {
		//PushOverlay(new PKEngine::ImGuiLayer());
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

PKEngine::Application* PKEngine::CreateApplication() {
	PK_INFO("Create!");
	return new Sandbox();
}
