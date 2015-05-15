#pragma once

#include <vector>
#include <glm/glm.hpp>

struct MeshModel{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normal;

	std::vector<unsigned int> indexList;
};

struct Line{
	glm::vec3 startPoint;
	glm::vec3 endPoint;
};

struct Lighting{
	glm::vec4 ambientMaterial;
	glm::vec4 diffuseMaterial;
	glm::vec4 specularMaterial;
	
	float specularPower = 0.15f;

	glm::vec3 lightPosition;
};

