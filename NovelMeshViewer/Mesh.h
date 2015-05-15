#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Shader.h"
#include "DataTypes.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void draw(const glm::mat4& model, const glm::mat4& mvp, const Lighting& light);

	void loadMesh(MeshModel* meshModel);
private:
	void initShader();

	void updateMVP(const glm::mat4& mvp);

	void updateLighting(const Lighting& light);

	void updateModel(const glm::mat4& model);

	int _drawCount = 0;

	int _vertexCount = 0;

	MeshModel* _meshModel;

	Shader* _shaderProgram;

	size_t _offsetVertex;

	GLuint _vertexBufferObject;
	GLuint _normalBufferObject;
	GLuint _elementBufferObject;

	GLuint _vertexBufferObject1;
	GLuint _normalBufferObject1;
	GLuint _elementBufferObject1;

	GLuint _vertexShaderID;
	GLuint _normalShaderID;

	GLuint _vertexShaderID1;
	GLuint _normalShaderID1;
};


