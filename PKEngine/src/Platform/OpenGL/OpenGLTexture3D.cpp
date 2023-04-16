#include "pkpch.h"
#include "OpenGLTexture3D.h"
#include "glad/glad.h"
#include "stb_image.h"

namespace PKEngine
{
	OpenGLTexture3D::OpenGLTexture3D(const std::vector<std::string>& path)
	{
		PK_CORE_ASSERT(path.size() == 6, "OpenGL Texture3D path is not equal 6!");
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
		for (int i = 0; i < 6;i++)
		{
			int width, height, channel;
			//stbi_set_flip_vertically_on_load(1);
			stbi_uc* data = stbi_load(path[i].c_str(), &width, &height, &channel, 0);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			m_Width = width;
			m_Height = height;
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	OpenGLTexture3D::~OpenGLTexture3D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture3D::SetData(void* data, uint32_t size)
	{

	}

	void OpenGLTexture3D::Bind(uint32_t slot) const
	{
		//glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
		glBindTextureUnit(slot, m_RendererID);
	}
}