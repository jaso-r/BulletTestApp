#pragma once

//  The idea here is that a GlutViewer encapsulate all the
//  necessary functionality of running OpenGL in Glut.  Ideally,
//  we could swap out a Glut viewer for, say, a Qt viewer and everything
//  would still work (assuming that OpenGL calls are made the same
//  way in each context).
//  In actuality, this is all set up in a bit of a hacky way to
//  get Glut callbacks to work in a C++ context -- i.e. everything
//  has to be static. This might be ok, though, if you think about
//  this as being a pseudo-singleton approach.
class GlutViewer
{
public:
	static void initialize(int* argcp, char** argv, SceneControl* controlForCallback);
	static void start();
	static void postRedisplay();
	static const scenePosition getCameraPosition();
	static void setCameraPosition(GLdouble x, GLdouble y, GLdouble z);
	static const scenePosition getLookAt();
	static void setLookAt(GLdouble x, GLdouble y, GLdouble z);

private:
	enum moveAxis { moveY, moveZ };
	static moveAxis toggleAxis;
	static GLdouble cameraPosition[3];
	static GLdouble cameraLookAt[3];
	static int lastMouseX;
	static int lastMouseY;
	static int width;
	static int height;

	static SceneControl* controlCallback;

	// all the Glut callbacks
	static void renderScene();
	static void changeSize(int w, int h);
	static void idle();
	static void motionFunc(int x, int y);
	static void mouseFunc(int button, int state, int x, int y);
	static void arrowFunc(int key, int x, int y);
	static void keyboardFunc(unsigned char key, int x, int y);

	GlutViewer() {};  // private -- there will be no instances of this class
};
