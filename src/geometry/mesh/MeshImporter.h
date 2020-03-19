#pragma once

#include <geometry/mesh/Mesh.h>

#include <string>

class MeshImporter
{
private:
	MeshImporter();
	~MeshImporter();

public:
	static Mesh* import_from_file(std::string file_path);
};

