#include "MeshViewerApp.h"
#include "LeapController.h"

int main(int argc, char** argv)
{
	std::string fileName;

	if (argc < 2)
	{
		fileName = "bunny.m";
	}
	else
	{
		fileName = argv[1];
	}

	LeapController leapDevice;
	leapDevice.initLeap(argc, argv);

	MeshViewerApp::getInstance()->createWindow("Interactive Mesh Viewer", 1024, 768);

	MeshViewerApp::getInstance()->run(fileName);
	
	leapDevice.stopLeap();

	return 0;
}
