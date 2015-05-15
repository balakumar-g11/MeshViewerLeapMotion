#include "LeapController.h"


LeapController::LeapController()
{
}


LeapController::~LeapController()
{
}

void LeapController::initLeap(int argc, char** argv)
{
	_eventListener.initListener();
	_controller.addListener(_eventListener);
	
	if (argc > 1 && strcmp(argv[1], "--bg") == 0)
		_controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);
}

void LeapController::stopLeap()
{
	_controller.removeListener(_eventListener);
}
