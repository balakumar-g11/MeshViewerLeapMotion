#include "Camera.h"
#include <glm/gtx/transform.hpp>

Camera::Camera()
{
	_perspective = glm::mat4(0.0f);
	_lookAt = glm::mat4(0.0f);
	_mvp = glm::mat4(0.0f);

	_cameraPos = glm::vec3(0.0f);
	_pointTo = glm::vec3(0.0f);
	_upDir = glm::vec3(0.0f);
}


Camera::~Camera()
{
}

void Camera::lookAt(const glm::vec3& cameraPos, const glm::vec3& pointTo, const glm::vec3& upDir)
{
	_cameraPos = cameraPos;
	_pointTo = pointTo;
	_upDir = upDir;

	_lookAt = glm::lookAt(cameraPos, pointTo, upDir);
}

void Camera::translateCamera(const glm::vec3& trans)
{
	_lookAt = _lookAt * glm::translate(trans);
}

void Camera::rotateCamera(const glm::mat4& rotationMatrix)
{
	_lookAt = _lookAt * rotationMatrix;
}

void Camera::perspective(const float fovy, const float aspect, const float zNear, const float zFar)
{
	_perspective = glm::perspective(fovy, aspect, zNear, zFar);
}

glm::mat4& Camera::getMVP(const glm::mat4& model)
{
	_mvp = _perspective * _lookAt * model;
	return _mvp;
}
