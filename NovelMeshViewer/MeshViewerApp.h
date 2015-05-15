#pragma once

#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "MFileReader.h"
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"

class MeshViewerApp
{
public:
	static MeshViewerApp* getInstance();

	bool createWindow(const std::string& appName, const int screenHeight, const int screenWidth);

	void run(const std::string& fileName);

	void updateRotation(const glm::mat4& rotMatrix);

	void updateTranslation(const glm::vec3& translation);

private:
	MeshViewerApp();

	~MeshViewerApp();

	MeshViewerApp(const MeshViewerApp&) {}

	void operator=(const MeshViewerApp&) {}

	bool initWindow();

	void loadMesh(const std::string& fileName);

	void draw();

	bool createContext();

	void setupLighting();

	void drawGridLines(const glm::mat4& mvp);

	void drawCoordinateAxes(const glm::mat4& mvp, const glm::vec4& color);

	void updateCameraResize(int width, int height);

	void initCamera();

	void initGridLines();

	void initCoordinateAxes(const float radius);

	void initVAOs();

	static void resize(GLFWwindow* window, int width, int height);

	GLFWwindow* _window;

	int _screenWidth;
	int _screenHeight;

	Mesh* _mesh;

	Camera* _camera;

	Lighting* _light;

	glm::mat4 _model;
	glm::mat4 _mvp;

	std::vector<glm::vec3> _gridLines;
	std::vector<unsigned int> _gridIndex;

	std::vector<glm::vec3> _cylinder;
	std::vector<unsigned int> _cylinderIndex;

	MFileReader* _modelReader;

	MeshModel* _modelMesh;

	Shader* _shaderGrid;
	Shader* _shaderCoord;

	glm::vec3 _cameraPos = glm::vec3(3, 2, 3);

	GLuint _gridLinesVBO;
	GLuint _gridLinesEBO;

	GLuint _coordAxesVBO;
	GLuint _coordAxesEBO;

	glm::vec4 _gridColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 _redColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 _greenColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 _blueColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	GLuint _vertexArrayObject;

	GLuint _vertexShaderID;

	static MeshViewerApp* _instance;
};

