#pragma once

#include <vector>
#include "DataTypes.h"
#include <string>

class MFileReader
{
public:
	MFileReader();
	~MFileReader();

	bool readFile(const std::string& fileName);

	MeshModel* getMeshModel();
private:
	MeshModel* _model;

	glm::vec3 getNormalInfo(std::stringstream& normalInfo);

	void parseAndPushVertex(std::stringstream& vertexInfo);

	void parseAndPushFace(std::stringstream& faceInfo);
};

