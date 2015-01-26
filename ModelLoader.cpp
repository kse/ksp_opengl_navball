#include "ModelLoader.hpp"
#include <stdio.h>

ModelLoader::ModelLoader(const std::string &file) {
	const aiScene *scene;
	Assimp::Importer importer;

	scene = importer.ReadFile(file.c_str(),
			aiProcess_Triangulate);

	if (!scene) {
		fprintf(stderr, "Error importing model '%s': %s\n", file.c_str(),
				importer.GetErrorString());
		return;
	}

	if (scene->mNumMeshes == 0) {
		fprintf(stderr, "No meshes in '%s'\n", file.c_str());
		return;
	}

	aiMesh *mesh = scene->mMeshes[0];
	unsigned int nVertices = mesh->mNumVertices;
	unsigned int i;
	printf("nVertices: %u\n", nVertices);

	for (i = 0; i < nVertices; i++) {
		aiVector3D vec = mesh->mVertices[i];
		vertices.push_back(vec.x);
		vertices.push_back(vec.y);
		vertices.push_back(vec.z);
	}

	unsigned int nFaces = mesh->mNumFaces;
	printf("nFaces: %u\n", nFaces);

	for (i = 0; i < nFaces; i++) {
		aiFace face = mesh->mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}
}
