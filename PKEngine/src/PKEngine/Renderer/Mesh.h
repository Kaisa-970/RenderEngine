#pragma once

namespace PKEngine {
	class Mesh {
	public:
		Mesh() = default;
		Mesh(const std::string& filepath);
		~Mesh();
		//void LoadMesh(const std::string& filepath);
	private:
		std::string m_Path;
	};
}