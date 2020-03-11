#pragma once

#include <geometry/mesh/Mesh.h>

#include <string>

class MeshLoader
{
private:
	MeshLoader();
	~MeshLoader();

public:
	static Mesh load_from_file(std::string file_path);
};

