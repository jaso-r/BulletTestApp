// BulletTestApp.cpp : Defines the entry point for the console application.
// Creates the SceneControl object which does most of the heavy lifting, and
// makes some geometry for the simulation.

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	SceneControl* sceneControl = SceneControl::getSceneControl(&argc, (char **)&argv, 7.5);

	// static ground plane
	GroundPlane ground(0.0);
	sceneControl->addSceneObject(&ground);
	
	// sphere object which will be controlable 
	double spherePos[] = { 0, 15, 10 };
	Sphere ball(spherePos, 3, 1, true);
	sceneControl->addSceneObject(&ball);
	sceneControl->setMainObject(&ball);

	// a pile of cubes
	double position[3];
	double dimensions[3] = { 1.95, 1.95, 1.95 };
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			for (int k = 0; k < 10; k++)
			{
				position[0] = -5.0 + (i * 2.0);
				position[1] = 2.5 + (j * 2.0);
				position[2] = -5.0 + (k * 2.0);
				sceneControl->addSceneObject(new Box(position, dimensions, 1, false));
			}
		}
	}

	// start the simulation and viewer
	sceneControl->start();

	return 0;
}
