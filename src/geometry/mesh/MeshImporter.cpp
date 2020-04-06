#include "MeshImporter.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

std::vector<Mesh*> MeshImporter::import_from_file(std::string file_path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file_path, aiProcess_Triangulate |
											 aiProcess_ValidateDataStructure |
											 aiProcess_ImproveCacheLocality);
	std::vector<Mesh*> meshes;
	if(scene && scene->HasMeshes()) {
		for(unsigned int m = 0; m < scene->mNumMeshes; m++) {
			aiMesh* mesh = scene->mMeshes[m];
			std::vector<Triangle> triangles;
			for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
				aiFace face = mesh->mFaces[i];
				for(unsigned int j = 0; j < face.mNumIndices / 3; j++) {
					auto index0 = face.mIndices[j*3];
					auto index1 = face.mIndices[j*3 + 1];
					auto index2 = face.mIndices[j*3 + 2];

					auto vertex0 = mesh->mVertices[index0];
					Vector3 v0(vertex0.x, vertex0.y, vertex0.z);

					auto vertex1 = mesh->mVertices[index1];
					Vector3 v1(vertex1.x, vertex1.y, vertex1.z);

					auto vertex2 = mesh->mVertices[index2];
					Vector3 v2(vertex2.x, vertex2.y, vertex2.z);

					Triangle t(v0, v1, v2);

					if(mesh->HasNormals()) {
						auto normal0 = mesh->mNormals[index0];
						Vector3 n0(normal0.x, normal0.y, normal0.z);

						auto normal1 = mesh->mNormals[index1];
						Vector3 n1(normal1.x, normal1.y, normal1.z);

						auto normal2 = mesh->mNormals[index2];
						Vector3 n2(normal2.x, normal2.y, normal2.z);
						t.set_normals(n0, n1, n2);
					}

					if(mesh->HasTextureCoords(0)) {
						auto uv0 = mesh->mTextureCoords[0][index0];
						Vector3 uv_0(uv0.x, uv0.y, uv0.z);

						auto uv1 = mesh->mTextureCoords[0][index1];
						Vector3 uv_1(uv1.x, uv1.y, uv1.z);

						auto uv2 = mesh->mTextureCoords[0][index2];
						Vector3 uv_2(uv2.x, uv2.y, uv2.z);
						t.set_uv_coords(uv_0, uv_1, uv_2);
					}

					triangles.push_back(t);
				}
			}
			meshes.push_back(new Mesh(triangles));
		}
	}
	return meshes;
}
