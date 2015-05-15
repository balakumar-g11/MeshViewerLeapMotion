#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	void lookAt(const glm::vec3& cameraPos, const glm::vec3& pointTo, const glm::vec3& upDir);

	void perspective(const float fovy, const float aspect, const float zNear, const float zFar);

	void translateCamera(const glm::vec3& translate);

	void rotateCamera(const glm::mat4& rotationMatrix);

	glm::mat4& getMVP(const glm::mat4& model);
private:
	glm::mat4 _perspective;
	glm::mat4 _lookAt;
	glm::mat4 _mvp;

	glm::vec3 _cameraPos;
	glm::vec3 _pointTo;
	glm::vec3 _upDir;
};

