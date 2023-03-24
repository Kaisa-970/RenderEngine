#pragma once
#pragma once

namespace PKEngine {
	class Mesh {
	public:
		Mesh() = default;
		Mesh(const std::string& filename);
		~Mesh();
	};
}