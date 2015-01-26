#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class ModelLoader {
	public:
		ModelLoader(const std::string &file);
		std::vector<float> vertices;
		std::vector<int>   indices;
};
