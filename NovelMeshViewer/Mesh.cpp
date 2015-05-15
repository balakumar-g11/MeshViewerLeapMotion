#include "Mesh.h"
#include <iostream>
#include <glm/gtx/transform.hpp>

Mesh::Mesh()
{
	_shaderProgram = new Shader();
}


Mesh::~Mesh()
{
	glDeleteBuffers(1, &_vertexBufferObject);
	glDeleteBuffers(1, &_elementBufferObject);
}

void Mesh::loadMesh(MeshModel* meshModel)
{
	_meshModel = meshModel;
	
	_drawCount = meshModel->indexList.size();

	_vertexCount = meshModel->vertices.size();

	initShader();

	glGenBuffers(1, &_vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * _vertexCount,
		&_meshModel->vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &_normalBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, _normalBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * _vertexCount,
		&_meshModel->normal[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &_elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * _drawCount, &_meshModel->indexList[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::updateMVP(const glm::mat4& mvp)
{
	GLuint mvpLoc = glGetUniformLocation(_shaderProgram->getProgramID(), "MVP");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);
}

void Mesh::updateModel(const glm::mat4& model)
{
	GLuint normalMatLoc = glGetUniformLocation(_shaderProgram->getProgramID(), "Normal");
	glUniformMatrix4fv(normalMatLoc, 1, GL_FALSE, &model[0][0]);
}

void Mesh::updateLighting(const Lighting& light)
{
	GLuint ambMaterialLoc = glGetUniformLocation(_shaderProgram->getProgramID(), "ambMaterial");
	glUniform4fv(ambMaterialLoc, 1, &light.ambientMaterial[0]);

	GLuint diffuseMaterialLoc = glGetUniformLocation(_shaderProgram->getProgramID(), "diffuseMaterial");
	glUniform4fv(diffuseMaterialLoc, 1, &light.diffuseMaterial[0]);

	GLuint specularMaterialLoc = glGetUniformLocation(_shaderProgram->getProgramID(), "speculatMaterial");
	glUniform4fv(specularMaterialLoc, 1, &light.specularMaterial[0]);

	GLuint specularPowerLoc = glGetUniformLocation(_shaderProgram->getProgramID(), "specularPower");
	glUniform1f(specularPowerLoc, light.specularPower);

	GLuint lightPositionLoc = glGetUniformLocation(_shaderProgram->getProgramID(), "lightPos");
	glUniform3fv(lightPositionLoc, 1, &light.lightPosition[0]);
}

void Mesh::draw(const glm::mat4& model, const glm::mat4& mvp, const Lighting& light)
{
	_shaderProgram->bind();
	updateMVP(mvp);
	updateLighting(light);
	updateModel(model);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferObject);

	glDrawElements(GL_TRIANGLES, _drawCount, GL_UNSIGNED_INT, (const GLvoid*)0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	_shaderProgram->unbind();
}

void Mesh::initShader()
{
	_shaderProgram->createShader("Shaders/phongShader");
	_shaderProgram->addAttribute("vertexPos_modelspace", 0);
	_shaderProgram->addAttribute("vertexNormal", 1);
}
