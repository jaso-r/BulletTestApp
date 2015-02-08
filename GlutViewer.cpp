#include "stdafx.h"
#include "GlutViewer.h"


// initialize the static variables here
GLdouble GlutViewer::cameraPosition[] = { 0, 25, 50 };
GLdouble GlutViewer::cameraLookAt[] = { 0, 0, 0 };
SceneControl* GlutViewer::controlCallback = NULL;
int GlutViewer::lastMouseX = -1;
int GlutViewer::lastMouseY = -1;
GlutViewer::moveAxis GlutViewer::toggleAxis = moveY;
int GlutViewer::width = 800;
int GlutViewer::height = 800;

// initialize performs the necessary actions to get a Glut window
// up and running, including setting up the callbacks.
// This SHOULD only ever be called once and MUST be called to provide
// a SceneControl object before the render and idlea functions are ever called.
void GlutViewer::initialize(int* argcp, char** argv, SceneControl* controlForCallback)
{
	glutInit(argcp, argv);
	glutInitWindowPosition(-1, -1); // leave it up to the window manager
	glutInitWindowSize(width, height);  // completely arbitrary for now
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Dumb Little Bullet Test");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	setLookAt(0, 0, 0);
	setCameraPosition(0, 25, 50);

	// set the ScenControl to do callbacks for
	// updating the scene and doing scene-specific drawing
	controlCallback = controlForCallback;

	// set up the Glut callbacks to our static functions
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);
	glutSpecialFunc(arrowFunc);
	glutKeyboardFunc(keyboardFunc);
	glutIdleFunc(idle);
}

// start actually begins the glut main loop, which should display the window
void GlutViewer::start()
{
	glutMainLoop();
}

// sets where the camera is looking in world space
void GlutViewer::setLookAt(GLdouble x, GLdouble y, GLdouble z)
{
	cameraLookAt[0] = x;
	cameraLookAt[1] = y;
	cameraLookAt[2] = z;
}

// returns where the camera is looking in world space
// as a GLdouble[3]
const scenePosition GlutViewer::getLookAt()
{
	scenePosition pos;
	pos.x = cameraLookAt[0];
	pos.y = cameraLookAt[1];
	pos.z = cameraLookAt[2];
	return pos;
}

// sets where the camera is positioned in world space
void GlutViewer::setCameraPosition(GLdouble x, GLdouble y, GLdouble z)
{
	cameraPosition[0] = x;
	cameraPosition[1] = y;
	cameraPosition[2] = z;
}

// returns the camera's position in world space
// as a GLdouble[3]
const scenePosition GlutViewer::getCameraPosition()
{
	scenePosition pos;
	pos.x = cameraPosition[0];
	pos.y = cameraPosition[1];
	pos.z = cameraPosition[2];
	return pos;
}

// Causes a glutPostRedisplay, which should redraw the scene
void GlutViewer::postRedisplay()
{
	glutPostRedisplay();
}

// Glut callback for when the scene should be rendered
// This requires that "initialize" has already been called to
// pass in the SceneControl object for drawing callback.
void GlutViewer::renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2],
		cameraLookAt[0], cameraLookAt[1], cameraLookAt[2], 0, 1, 0);

	glPushMatrix();

	assert(controlCallback != NULL);
	controlCallback->draw();

	glPopMatrix();

	// draw some informational text on the screen
	const unsigned char infoString[] = "Click and move mouse to move sphere\nor use arrow keys.\nq - quit\nt - toggle movement axis\nm - magnet mode";
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	glRasterPos2i(5, height - 25);
	// not sure why this isn't changing the text color, but I also don't
	// care enough to investigate further.
	glColor3f(1.0, 1.0, 1.0);
	glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, infoString);
	glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

// Glut idle callback, should be used to update the scene state
// This requires that "initialize" has already been called to
// pass in the SceneControl for the update callback.
void GlutViewer::idle()
{
	assert(controlCallback != NULL);
	controlCallback->update();
}

// Glut callback for when the window size is changed
void GlutViewer::changeSize(int w, int h)
{
	if (h == 0)
	{
		h = 1;
	}

	float ratio = ((float)w) / ((float)h);

	width = w;
	height = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45, ratio, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
}

// Glut callback for when a mouse button is pressed
// on mouse down we record the position for reference
// in the mouse motion function
void GlutViewer::mouseFunc(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		lastMouseX = x;
		lastMouseY = y;
	}
}

// Glut callback for when the mouse moves with the
// button pressed.  This movement is then mapped to movement
// on the SceneControl's "main object".  Depending on
// toggleAxis, the object is either moved in the XY or XZ plane.
// This fucntion requires that the initialize function has been
// called to set a SceneControl callback object.
void GlutViewer::motionFunc(int x, int y)
{
	assert(controlCallback != NULL);
	if (lastMouseX != -1 && lastMouseY != -1)
	{
		int xdiff = x - lastMouseX;
		int ydiff = y - lastMouseY;
		if (toggleAxis == moveY)
		{
			controlCallback->moveMainObject(0.1 * xdiff, -0.1 * ydiff, 0);
		}
		else
		{
			controlCallback->moveMainObject(0.1 * xdiff, 0, 0.1 * ydiff);
		}
	}
	lastMouseX = x;
	lastMouseY = y;
}

// Glut callback for when the arrow keys are pressed.
// This movment is mapped to movement on the ScenControl's
// "main object".  Depending on toggleAxis, the object is
// either moved in the XY or XZ plane.
// This function requires that the initialize function has been
// called to set a SceneControl callback object.
void GlutViewer::arrowFunc(int key, int x, int y)
{
	assert(controlCallback != NULL);
	switch (key)
	{
	case GLUT_KEY_UP:
		toggleAxis == moveY ? 
			controlCallback->moveMainObject(0, 0.5, 0) : 
			controlCallback->moveMainObject(0, 0, -0.5);
		break;
	case GLUT_KEY_DOWN:
		toggleAxis == moveY ?
			controlCallback->moveMainObject(0, -0.5, 0) :
			controlCallback->moveMainObject(0, 0, 0.5);
		break;
	case GLUT_KEY_LEFT:
		controlCallback->moveMainObject(-0.5, 0, 0);
		break;
	case GLUT_KEY_RIGHT:
		controlCallback->moveMainObject(0.5, 0, 0);
		break;
	}
}

// Glut callback for when keyboard keys are pressed.
// One of the keys (m) is used to toggle the "magnet mode" on
// the SceneControl, so it is necessary that initialize be called
// first to set the SceneControl callback object.
void GlutViewer::keyboardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
	case 'Q':
		// quit
		exit(0);
		break;
	case 't':
	case 'T':
		// toggle the movement plane
		if (toggleAxis == moveY)
		{
			toggleAxis = moveZ;
		}
		else
		{
			toggleAxis = moveY;
		}
		break;
	case 'm':
	case 'M':
		// toggle "magnet mode" on the SceneControl
		assert(controlCallback != NULL);
		controlCallback->toggleMagnet();
		break;
	}
}