#include <PKEngine.h>
#include <PKEngine/Core/EntryPoint.h>
#include <imgui.h>
#include <Platform/OpenGL/OpenGLShader.h>
#include <glm/gtx/transform.hpp>
#include "glm/gtc/type_ptr.hpp"

#include "EditorLayer.h"

namespace PKEngine {


	class PKEditor : public PKEngine::Application {
	public:
		PKEditor():Application("PKEditor",2560,1440)
		{
			//PushLayer(new ExampleLayer());

			PushLayer(new EditorLayer());

		}

		~PKEditor() {

		}
	};

	PKEngine::Application* PKEngine::CreateApplication() {

		return new PKEditor();
	}
}