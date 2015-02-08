#include "stdafx.h"
#include "SceneControl.h"

// initialize the singleton to null
SceneControl* SceneControl::singleton = NULL;

// singleton accessor, creates it if it doesn't exist yet
SceneControl* SceneControl::getSceneControl(int* argcp, char** argv, double magnetDistance)
{
	if (singleton == NULL)
	{
		singleton = new SceneControl(argcp, argv, magnetDistance);
	}
	return singleton;
}

// private constructor used to initialize both the model and view
SceneControl::SceneControl(int* argcp, char** argv, double magnetDistance)
{
	this->magnetDistance = magnetDistance;
	magnetMode = false;
	sceneModel = new BulletScene();
	lastTime = std::chrono::system_clock::now();
	GlutViewer::initialize(argcp, argv, this);
}

// this shouldn't be called, as we shouldn't be deleteing the singleton
// after it is created, but just in case, clean up the model object
SceneControl::~SceneControl()
{
	delete sceneModel;
}

// stores the current time for use in updating Bullet in the next time
// step, and starts the viewer redrawing.
void SceneControl::start()
{
	lastTime = std::chrono::system_clock::now();
	GlutViewer::start();
}

// Causes the Bullet scene to update based on the time that has elapsed 
// since this was last called and tells the viewer to redraw.
void SceneControl::update()
{
	// figure out how long it has been since the last update
	std::chrono::time_point<std::chrono::system_clock> newTime = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed = newTime - lastTime;
	lastTime = newTime;

	// step the simulation
	sceneModel->stepSim(elapsed.count(), 10);

	// after stepping the simulation, if we're in magnet mode,
	// check the proximities of all the objects to the main object
	// and add spring constraints
	scenePosition pos = mainObject->getPosition();
	if (magnetMode)
	{
		// currently we're just iterating through all the objects in the scene
		// there aren't too many objects as to make this prohibitive, but
		// this could certainly be improved with some sort of space partitioning
		for (std::vector<SceneObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
		{
			if ((*it) != mainObject)  // make sure to not constrain the main object to itself
			{
				scenePosition pos2 = (*it)->getPosition();
				if (length(pos, pos2) < magnetDistance)
				{
					// this fuction will only create a constraint if none exists already
					constrainToMain(*it);
				}
			}
		}
	}

	// update the viewer
	GlutViewer::setLookAt(pos.x, pos.y, pos.z);
	GlutViewer::postRedisplay();
}

// This function will be called by the viewer after any viewer-specific
// work has been done (camera transforms, etc).  We'll use this place to
// make OpenGL calls used in drawing the scene and the dynamic objects in it.
void SceneControl::draw()
{
	// a light
	glPushMatrix();
	GLfloat light0p[4] = { 1.0, -1.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light0p);
	glPopMatrix();
	
	// draw all the objects in the scene
	for (std::vector<SceneObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		(*it)->drawOpenGL();
	}
}

// add an object to the scene
void SceneControl::addSceneObject(SceneObject* addObject)
{
	objects.push_back(addObject);
	sceneModel->addBody(addObject->getRigidBody());
}

// sets a SceneObject to be the "main object" for this scene.  The user
// will have control over this object, and it is used to constrain to for
// the "magnet" mode.
// Note: the ScenObject passed to this function will still need to be
// added with "addSceneObject" in order for it to draw itself in the viewer.
void SceneControl::setMainObject(SceneObject* lookAt)
{
	mainObject = lookAt;
}

// Moves the "main object" by a given amount on the x, y, and z axis.
void SceneControl::moveMainObject(double x, double y, double z)
{
	scenePosition pos = mainObject->getPosition();
	pos.x += x;
	pos.y += y;
	pos.z += z;
	mainObject->setPosition(pos.x, pos.y, pos.z);
	GlutViewer::postRedisplay();
}

// Create a spring constraint between the provided SceneObject and
// the "main object". Be sure not to pass in the main object. You 
// shouldn't constrain an object to itself.
// This function will only create a spring constraint if none exists
// already for the given ScenObject.
void SceneControl::constrainToMain(SceneObject* constrainObject)
{
	if (!sceneModel->hasSpring(constrainObject->getRigidBody()))
	{
		sceneModel->addSpring(constrainObject->getRigidBody(), mainObject->getRigidBody());
	}
}

// Toggles "magnet mode". When magnet mode is on, spring constraints
// will be created between any SceneObject and the main object whenever
// the distance between their centroids falls below the "magnetDistance"
// that was provided when creating this SceneControl.
void SceneControl::toggleMagnet()
{
	magnetMode = !magnetMode;
	if (!magnetMode) // if turning off magnet mode, remove the constraints
	{
		sceneModel->clearSprings();
	}
}

// A convenience function for getting the distance between two objects.
inline double SceneControl::length(scenePosition pos1, scenePosition pos2)
{
	scenePosition ray;
	ray.x = pos1.x - pos2.x;
	ray.y = pos1.y - pos2.y;
	ray.z = pos1.z - pos2.z;
	return sqrt((ray.x * ray.x) + (ray.y * ray.y) + (ray.z * ray.z));
}
