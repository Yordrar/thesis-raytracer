#include "MeshLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Mesh MeshLoader::load_from_file(std::string file_path)
{
	Mesh m;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file_path, aiProcess_CalcTangentSpace |
											 aiProcess_Triangulate |
											 aiProcess_JoinIdenticalVertices |
											 aiProcess_GenNormals |
											 aiProcess_ValidateDataStructure |
											 aiProcess_ImproveCacheLocality |
											 aiProcess_SortByPType);
	if(scene && scene->HasMeshes()) {
		aiMesh* mesh = scene->mMeshes[0];
		for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
			aiVector3D vertex = mesh->mVertices[i];
			m.add_vertex(Vector3(vertex.x, vertex.y, vertex.z));
		}
		for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for(unsigned int j = 0; j < face.mNumIndices / 3; j++) {
				m.add_triangle(face.mIndices[j*3], face.mIndices[j*3+1], face.mIndices[j*3+2]);
			}
		}
	}
	return m;
}
