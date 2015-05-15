#include "MFileReader.h"
#include <fstream>
#include <sstream>
#include <algorithm>

MFileReader::MFileReader()
{
	_model = new MeshModel();
}


MFileReader::~MFileReader()
{
}

bool MFileReader::readFile(const std::string& fileName)
{
	bool isSuccess = false;
	std::ifstream fileReader(fileName);

	if (fileReader.is_open())
	{
		std::string strLine;
		while (std::getline(fileReader, strLine))
		{
			std::stringstream lineStream(strLine);
			std::string lineType;

			lineStream >> lineType;

			if (lineType.compare("Vertex") == 0)
			{
				parseAndPushVertex(lineStream);
			}
			else if (lineType.compare("Face") == 0)
			{
				parseAndPushFace(lineStream);
			}
		}
		isSuccess = true;
	}

	return isSuccess;
}

glm::vec3 MFileReader::getNormalInfo(std::stringstream& normalInfo)
{
	glm::vec3 normal;

	std::string normalStr = normalInfo.str();

	size_t initPos = normalStr.find_first_of('(');
	size_t endPos = normalStr.find_first_of(')');

	std::string normalVal;
	normalVal.assign((normalStr.begin() + initPos + 1), (normalStr.begin() + endPos));

	float valx = 0.0f, valy = 0.0f, valz = 0.0f;

	std::stringstream normalStream(normalVal);

	normalStream >> valx;
	normalStream >> valy;
	normalStream >> valz;

	normal = glm::vec3(valx, valy, valz);

	return normal;
}

void MFileReader::parseAndPushVertex(std::stringstream& vertexInfo)
{
	float valx = 0.0f, valy = 0.0f, valz = 0.0f;

	int vertID;
	vertexInfo >> vertID;

	vertexInfo >> valx;
	vertexInfo >> valy;
	vertexInfo >> valz;

	glm::vec3 vertex(valx, valy, valz);

	_model->vertices.push_back(vertex);
	_model->normal.push_back(getNormalInfo(vertexInfo));
}

void MFileReader::parseAndPushFace(std::stringstream& faceInfo)
{
	int vertID;

	faceInfo >> vertID;

	while (faceInfo >> vertID)
	{
		_model->indexList.emplace_back((vertID-1));
	}
}

MeshModel* MFileReader::getMeshModel()
{
	return _model;
}
