#include "stdafx.h"
#include "GroundPlane.h"

// Creates a GroundPlane object, including the associated Bullet rigid
// body, and sets its collision shape to a static plane.
// This object will never by kinematic, so we don't bother setting
// the parent's kinematic flag to true.
// height: the height on the Y axis at which to place the plane.
GroundPlane::GroundPlane(double height)
{
	groundHeight = height;
	btCollisionShape* groundCollision = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	btDefaultMotionState* groundState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, height, 0)));
	btRigidBody::btRigidBodyConstructionInfo groundInfo(0, groundState, groundCollision, btVector3(0, 0, 0));
	
	// set the rigid body associated with this SceneObject.
	setRigidBody(new btRigidBody(groundInfo));
}

GroundPlane::~GroundPlane()
{
}

// Draws a brown quad in OpenGL.
// Currently it is centered at the origin, but at the
// appropriate height on the Y axis, and is 300x300.
// We might alter the implementation a bit later to add
// more control over how the plane is displayed.
void GroundPlane::drawOpenGL()
{
	GLfloat groundColor[4] = { 0.5, 0.4, 0.2, 1.0 };
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, groundColor);
	glBegin(GL_QUADS);
	glVertex3f(-150, groundHeight, -150);
	glVertex3f(150, groundHeight, -150);
	glVertex3f(150, groundHeight, 150);
	glVertex3f(-150, groundHeight, 150);
	glEnd();
	glPopMatrix();
}

// Returns a string with extra information specific to
// GroundPlane objects.
std::string GroundPlane::getMoreInfo() const
{
	std::string infoString = "      == Ground Plane ==\n";
	infoString += "        height = " + std::to_string(groundHeight) + "\n";
	return infoString;
}