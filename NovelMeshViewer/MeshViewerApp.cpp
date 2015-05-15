#include "MeshViewerApp.h"
#include <algorithm>

MeshViewerApp* MeshViewerApp::_instance = nullptr;

static float getScaleFactor(std::vector<glm::vec3>& vertices)
{
	float scaleFactor = 1.0f;

	if (vertices.empty())
		return scaleFactor;

	auto max_X = std::max_element(vertices.begin(), vertices.end(),
		[](glm::vec3 lhs, glm::vec3 rhs){
		return lhs.x < rhs.x;
	});

	auto max_Y = std::max_element(vertices.begin(), vertices.end(),
		[](glm::vec3 lhs, glm::vec3 rhs){
		return lhs.y < rhs.y;
	});

	auto max_Z = std::max_element(vertices.begin(), vertices.end(),
		[](glm::vec3 lhs, glm::vec3 rhs){
		return lhs.z < rhs.z;
	});

	if (max_X->x >= max_Y->y && max_X->x >= max_Z->z)
		scaleFactor = max_X->x;
	else if (max_Y->y >= max_X->x && max_Y->y >= max_Z->z)
		scaleFactor = max_Y->y;
	else
		scaleFactor = max_Z->z;

	return (1 / scaleFactor);
}


MeshViewerApp::MeshViewerApp()
{
	_camera = new Camera();
	_mesh = new Mesh();
	_modelReader = new MFileReader();
	_modelMesh = new MeshModel();
	_light = new Lighting();
	_shaderGrid = new Shader();
	_shaderCoord = new Shader();
}


MeshViewerApp::~MeshViewerApp()
{
	glBindVertexArray(0);

	glDeleteBuffers(1, &_gridLinesVBO);

	delete _light;
	delete _modelMesh;
	delete _modelReader;
	delete _mesh;
	delete _camera;
	delete _shaderGrid;
	delete _shaderCoord;
}

MeshViewerApp* MeshViewerApp::getInstance()
{
	if (_instance == nullptr)
		_instance = new MeshViewerApp();

	return _instance;
}

void MeshViewerApp::initVAOs()
{
	glGenVertexArrays(1, &_vertexArrayObject);
	glBindVertexArray(_vertexArrayObject);
}

void MeshViewerApp::initGridLines()
{
	_shaderGrid->createShader("Shaders/gridLinesShader");
	_shaderGrid->addAttribute("vertex", 2);

	int count = 0;
	for (int i = -25; i <= 25; i++)
	{
		glm::vec3 startPoint = glm::vec3((float)i, 0.0f, -25.0f);
		_gridLines.push_back(startPoint);
		_gridIndex.emplace_back(count++);

		glm::vec3 endPoint = glm::vec3((float)i, 0.0f, 25.0f);
		_gridLines.push_back(endPoint);
		_gridIndex.emplace_back(count++);
	}

	for (int i = -25; i <= 25; i++)
	{
		glm::vec3 startPoint = glm::vec3(-25.0f, 0.0f, (float)i);
		_gridLines.push_back(startPoint);
		_gridIndex.emplace_back(count++);

		glm::vec3 endPoint = glm::vec3(25.0f, 0.0f, (float)i);
		_gridLines.push_back(endPoint);
		_gridIndex.emplace_back(count++);
	}

	glGenBuffers(1, &_gridLinesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _gridLinesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * _gridLines.size(),
		&_gridLines[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	

	glGenBuffers(1, &_gridLinesEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _gridLinesEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * _gridIndex.size(), &_gridIndex[0], GL_STATIC_DRAW);
}

void MeshViewerApp::drawGridLines(const glm::mat4& mvp)
{
	_shaderGrid->bind();

	GLuint colorLoc = glGetUniformLocation(_shaderGrid->getProgramID(), "Color");
	glUniform4fv(colorLoc, 1, &_gridColor[0]);

	GLuint mvpLoc = glGetUniformLocation(_shaderGrid->getProgramID(), "MVP");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _gridLinesEBO);
	
	glDrawElements(GL_LINES, _gridIndex.size(), GL_UNSIGNED_INT, (const void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	_shaderGrid->unbind();
}

void MeshViewerApp::initCoordinateAxes(const float radius)
{
	_shaderCoord->createShader("Shaders/coordAxesShader");
	_shaderCoord->addAttribute("vertex", 3);

	float x = 0.0f;
	float y = 0.0f;

	float angle = 2.0f * 3.14159f / 20.0f;

	int count = 0;
	for (int i = 0; i <= 20; i++) {
		float x = cosf(i * angle) * radius;
		float y = sinf(i * angle) * radius;
		_cylinder.push_back(glm::vec3(0.0f, x, y));
		_cylinderIndex.emplace_back(count++);
		_cylinder.push_back(glm::vec3(1.0f, x, y));
		_cylinderIndex.emplace_back(count++);
	}

	glGenBuffers(1, &_coordAxesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _coordAxesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * _cylinder.size(),
		&_cylinder[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);

	glGenBuffers(1, &_coordAxesEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _coordAxesEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * _cylinderIndex.size(), &_cylinderIndex[0], GL_STATIC_DRAW);
}

void MeshViewerApp::drawCoordinateAxes(const glm::mat4& mvp, const glm::vec4& color)
{
	_shaderCoord->bind();

	GLuint colorLoc = glGetUniformLocation(_shaderCoord->getProgramID(), "Color");
	glUniform4fv(colorLoc, 1, &color[0]);

	GLuint mvpLoc = glGetUniformLocation(_shaderCoord->getProgramID(), "MVP");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _coordAxesEBO);

	glDrawElements(GL_TRIANGLE_STRIP, _cylinderIndex.size(), GL_UNSIGNED_INT, (const void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	_shaderCoord->unbind();
}

void MeshViewerApp::updateTranslation(const glm::vec3& translation)
{
	_camera->translateCamera(translation);
}

void MeshViewerApp::updateRotation(const glm::mat4& rotMatrix)
{
	_camera->rotateCamera(rotMatrix);
}

void MeshViewerApp::updateCameraResize(int width, int height)
{
	_screenWidth = width;
	_screenHeight = height;

	_camera->perspective(70.0f, (float)_screenWidth / (float)_screenHeight, 0.1f, 100.0f);
}

void MeshViewerApp::resize(GLFWwindow* window, int width, int height)
{
	glfwMakeContextCurrent(window);

	glfwSwapBuffers(window);

	glViewport(0, 0, width, height);

	MeshViewerApp::getInstance()->updateCameraResize(width, height);
}

bool MeshViewerApp::createWindow(const std::string& appName, const int screenWidth, const int screenHeight)
{
	bool retVal = false;

	if (!initWindow())
		return retVal;

	_window = glfwCreateWindow(screenWidth, screenHeight, "Interactive Mesh Viewer", NULL, NULL);

	if (_window == NULL)
	{
		std::cerr << "Failed to open GLFW window" << std::endl;
		glfwTerminate();
		return retVal;
	}

	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	glfwSetWindowSizeCallback(_window, MeshViewerApp::resize);

	retVal = createContext();

	return retVal;
}

bool MeshViewerApp::createContext()
{
	glfwMakeContextCurrent(_window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return false;
	}

	glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);

	glClearColor(0.27f, 0.24f, 0.26f, 1.0f);

	return true;
}

bool MeshViewerApp::initWindow()
{
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return false;
	}

	return true;
}

void MeshViewerApp::initCamera()
{
	_camera->lookAt(_cameraPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	_camera->perspective(70.0f, (float)_screenWidth / (float)_screenHeight, 0.1f, 100.0f);

	_model = glm::mat4(1.0f);
}

void MeshViewerApp::loadMesh(const std::string& fileName)
{
	float scaleFactor = 1.0f;

	_modelReader->readFile("Models/" + fileName);

	_modelMesh = _modelReader->getMeshModel();

	scaleFactor = getScaleFactor(_modelMesh->vertices);

	_mesh->loadMesh(_modelMesh);

	_model = _model * glm::scale(glm::vec3(scaleFactor, scaleFactor, scaleFactor));

	_mvp = _camera->getMVP(_model);
}

void MeshViewerApp::run(const std::string& fileName)
{
	initCamera();

	setupLighting();

	initVAOs();

	initGridLines();

	initCoordinateAxes(0.01f);

	loadMesh(fileName);

	draw();
}

void MeshViewerApp::draw()
{
	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_mesh->draw(_model, _camera->getMVP(_model), *_light);

		drawGridLines(_camera->getMVP(glm::mat4()));

		drawCoordinateAxes(_camera->getMVP(glm::mat4()), _redColor);

		glm::mat4 transMat(0.0f, 1.0f, 0.0f, 0.0f,
							-1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f);

		drawCoordinateAxes(_camera->getMVP(transMat), _greenColor);

		glm::mat4 transMat1(0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							-1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f);

		drawCoordinateAxes(_camera->getMVP(transMat1), _blueColor);

		glfwSwapBuffers(_window);
		glfwPollEvents();
	} while (glfwGetKey(_window, GLFW_KEY_ESCAPE) != GLFW_PRESS
		&& glfwWindowShouldClose(_window) == 0);
}

void MeshViewerApp::setupLighting()
{
	_light->ambientMaterial = glm::vec4(0.1f, 0.1f, 0.1f, 0.1f);
	_light->specularMaterial = glm::vec4(0.6f, 0.2f, 0.4f, 1.0f);
	_light->diffuseMaterial = glm::vec4(0.960f, 0.639f, 0.321f, 1.0f);

	_light->specularPower = 15.0f;

	_light->lightPosition = glm::vec3(0.0f, 0.0f, -3.0f);
}
