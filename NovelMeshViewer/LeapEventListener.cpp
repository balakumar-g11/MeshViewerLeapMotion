#include "LeapEventListener.h"
#include <iostream>
#include <sstream>
#include "MeshViewerApp.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

LeapEventListener::LeapEventListener()
{

}

LeapEventListener::~LeapEventListener()
{
}

void LeapEventListener::initListener()
{
	
}

void LeapEventListener::onConnect(const Leap::Controller& controller)
{
	
}

bool LeapEventListener::stopGesture()
{
	bool isGestureStopped = false;

	Leap::FingerList fingers = _currFrame.hands().rightmost().fingers();

	std::vector<Leap::Vector> tipPositions;
	float maxDist = 0.0f;

	for (auto finger : fingers)
	{
		tipPositions.push_back(finger.tipPosition());
	}

	for (size_t i = 1; i < tipPositions.size(); i++)
	{
		float dist = tipPositions[0].distanceTo(tipPositions[i]);

		if (dist > maxDist)
			maxDist = dist;
	}

	if (maxDist <= 50.0f)
		isGestureStopped = true;
	
	return isGestureStopped;
}

void LeapEventListener::checkStartGesture()
{
	Leap::FingerList fingers = _currFrame.hands().rightmost().fingers();
	bool indexEnabled = false;
	bool otherFingerEnabled = false;

	for (auto finger : fingers)
	{
		if (finger.type() == Leap::Finger::TYPE_INDEX && finger.isExtended())
			indexEnabled = true;
		if (finger.type() != Leap::Finger::TYPE_INDEX && finger.isExtended())
		{
			otherFingerEnabled = true;
			break;
		}
	}

	if (indexEnabled && !otherFingerEnabled)
		_gestureStarted = true;
}

void LeapEventListener::onFrame(const Leap::Controller& controller)
{
	_currFrame = controller.frame();

	Leap::Hand rightHand = _currFrame.hands().rightmost();
	
	if (rightHand.isRight() && rightHand.isValid())
	{
		if (_gestureStarted){
			if (stopGesture())
			{
				_gestureStarted = false;
				return;
			}

			if (_frameCount == 0)
			{
				_beginFrame = _currFrame;
				_frameCount = 1;
				_grabCount += rightHand.grabStrength();
			}
			else if (_frameCount < 5)
			{
				_frameCount++;
				_grabCount += rightHand.grabStrength();
			}
			else if (_frameCount == 5)
			{
				_lastFrame = _currFrame;
				_frameCount = 0;

				if (_grabCount / 5.0f > 0.5)
				{
					processRotation();
				}
				else
				{
					processTranslation();
				}
				_grabCount = 0;
			}
		}
		else
		{
			checkStartGesture();
		}
	}
}

void LeapEventListener::processTranslation()
{
	Leap::Hand endFrameHand = _lastFrame.hands().rightmost();

	Leap::Vector transLeap = endFrameHand.translation(_beginFrame);
	
	glm::vec3 translation(0.0f);

	translation.x = 0.005f * transLeap.x;
	translation.y = 0.005f * transLeap.y;
	translation.z = 0.005f * transLeap.z;

	MeshViewerApp::getInstance()->updateTranslation(translation);
}

void LeapEventListener::processRotation()
{
	Leap::Hand endFrameHand = _lastFrame.hands().rightmost();

	Leap::Matrix rotMatrix = endFrameHand.rotationMatrix(_beginFrame);

	glm::mat4 rotationMatrix = glm::mat4(0);

	float rotArray[16] = { 0.0f };

	rotMatrix.toArray4x4(rotArray);

	int cnt = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			rotationMatrix[i][j] = rotArray[cnt];
			cnt++;
		}
	}

	MeshViewerApp::getInstance()->updateRotation(rotationMatrix);
}

