#include "Shader.h"
#include <fstream>
#include <iostream>
#include <vector>

Shader::Shader()
{
}


Shader::~Shader()
{
}

void Shader::createShader(const std::string& shaderFile)
{
	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertexShaderCode;
	std::ifstream vertexShaderStream(shaderFile+".vert", std::ios::in);
	if (vertexShaderStream.is_open())
	{
		std::string line = "";
		while (std::getline(vertexShaderStream, line))
			vertexShaderCode += "\n" + line;

		vertexShaderStream.close();
	}

	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream(shaderFile + ".frag", std::ios::in);
	if (fragmentShaderStream.is_open())
	{
		std::string line = "";
		while (std::getline(fragmentShaderStream, line))
			fragmentShaderCode += "\n" + line;

		fragmentShaderStream.close();
	}

	compileShader(vertexShaderCode, fragmentShaderCode);

	checkShaderError();

	linkShader();

	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragmentShaderID);
}

void Shader::addAttribute(const std::string& attribName, const int attribID)
{
	glBindAttribLocation(_programID, attribID, attribName.c_str());
}

void Shader::compileShader(const std::string& vertexShaderCode, const std::string& fragShaderCode)
{
#ifdef _DEBUG
	std::cout << "Compiling Vertex Shader" << std::endl;
#endif
	const char* vertexSourcePointer = vertexShaderCode.c_str();
	glShaderSource(_vertexShaderID, 1, &vertexSourcePointer, NULL);
	glCompileShader(_vertexShaderID);

#ifdef _DEBUG
	std::cout << "Compiling Fragment Shader" << std::endl;
#endif
	const char* fragmentSourcePointer = fragShaderCode.c_str();
	glShaderSource(_fragmentShaderID, 1, &fragmentSourcePointer, NULL);
	glCompileShader(_fragmentShaderID);
}

void Shader::checkShaderError()
{
	GLint Result = GL_FALSE;
	int infoLogLen;

	glGetShaderiv(_vertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(_vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLen);
	std::vector<char> vertexShaderErrorMessage(infoLogLen);
	glGetShaderInfoLog(_vertexShaderID, infoLogLen, NULL, &vertexShaderErrorMessage[0]);

	std::string errorMsg(vertexShaderErrorMessage.begin(), vertexShaderErrorMessage.end());
#ifdef _DEBUG
	std::cout << errorMsg << std::endl;
#endif
	glGetShaderiv(_fragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(_fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLen);
	std::vector<char> fragmentShaderErrorMessage(infoLogLen);
	glGetShaderInfoLog(_fragmentShaderID, infoLogLen, NULL, &fragmentShaderErrorMessage[0]);

	errorMsg.clear();

	errorMsg = std::string(fragmentShaderErrorMessage.begin(), fragmentShaderErrorMessage.end());
#ifdef _DEBUG
	std::cout << errorMsg << std::endl;
#endif
}

void Shader::linkShader()
{
	GLint Result = GL_FALSE;
	int infoLogLen;

	_programID = glCreateProgram();
	glAttachShader(_programID, _vertexShaderID);
	glAttachShader(_programID, _fragmentShaderID);
	glLinkProgram(_programID);

	glGetProgramiv(_programID, GL_LINK_STATUS, &Result);
	glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &infoLogLen);
	std::vector<char> ProgramErrorMessage(infoLogLen > 1 ? infoLogLen : 1);
	glGetProgramInfoLog(_programID, infoLogLen, NULL, &ProgramErrorMessage[0]);
}

void Shader::bind()
{
	glUseProgram(_programID);
}

void Shader::unbind()
{
	glUseProgram(0);
}

int Shader::getAttributeLocation(const std::string& attribName)
{
	return glGetAttribLocation(_programID, attribName.c_str());
}

