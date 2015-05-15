#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "DataTypes.h"

class Shader
{
public:
	Shader();
	~Shader();
	
	void createShader(const std::string& shaderFile);

	GLuint getProgramID() { return _programID; }

	void bind();

	void unbind();

	void addAttribute(const std::string& attribName, const int attridID);

	int getAttributeLocation(const std::string& attribName);
private:
	void compileShader(const std::string& vertShaderCode, const std::string& fragShaderCode);

	void checkShaderError();

	void linkShader();

	GLuint _programID;

	GLuint _vertexShaderID;
	GLuint _fragmentShaderID;
};

