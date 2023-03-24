#pragma once

namespace PKEngine {
	class Mesh {
	public:
		Mesh();
		~Mesh();
		void Load(const std::string& filename);
	};
}