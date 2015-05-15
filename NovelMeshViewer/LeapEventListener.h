#pragma once

#include <Leap/Leap.h>
#include <string>
#include <vector>
#include <fstream>

class LeapEventListener : public Leap::Listener
{
public:
	LeapEventListener();

	~LeapEventListener();

	virtual void onConnect(const Leap::Controller&);

	virtual void onFrame(const Leap::Controller&);

	void initListener();
private:
	void processTranslation();

	void processScaling();

	void processRotation();

	bool stopGesture();

	void checkStartGesture();
	
	bool _isFocus = false;

	Leap::Frame _lastFrame;

	Leap::Frame _beginFrame;

	Leap::Frame _currFrame;

	bool _gestureStarted = false;

	int _presentGestureId = -1;

	float _grabCount = 0.0f;

	int _frameCount = 0;
};

