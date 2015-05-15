#pragma once
#include <Leap/Leap.h>
#include "LeapEventListener.h"

class LeapController
{
public:
	LeapController();
	~LeapController();

	void initLeap(int argc, char** argv);

	void stopLeap();
private:
	Leap::Controller _controller;

	LeapEventListener _eventListener;
};

