#include "MeshLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Mesh* MeshLoader::load_from_file(std::string file_path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file_path, aiProcess_Triangulate |
											 aiProcess_ValidateDataStructure |
											 aiProcess_ImproveCacheLocality  |
											 aiProcess_FlipWindingOrder);
	std::vector<Triangle> triangles;
	if(scene && scene->HasMeshes()) {
		aiMesh* mesh = scene->mMeshes[0];
		for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for(unsigned int j = 0; j < face.mNumIndices / 3; j++) {
				Vector3 v0, v1, v2;
				v0.set_x(mesh->mVertices[face.mIndices[j*3]].x);
				v0.set_y(mesh->mVertices[face.mIndices[j*3]].y);
				v0.set_z(mesh->mVertices[face.mIndices[j*3]].z);

				v1.set_x(mesh->mVertices[face.mIndices[j*3 + 1]].x);
				v1.set_y(mesh->mVertices[face.mIndices[j*3 + 1]].y);
				v1.set_z(mesh->mVertices[face.mIndices[j*3 + 1]].z);

				v2.set_x(mesh->mVertices[face.mIndices[j*3 + 2]].x);
				v2.set_y(mesh->mVertices[face.mIndices[j*3 + 2]].y);
				v2.set_z(mesh->mVertices[face.mIndices[j*3 + 2]].z);
				triangles.push_back(Triangle(v0, v1, v2));
			}
		}
	}
	return new Mesh(triangles);
}
