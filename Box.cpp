#include "stdafx.h"
#include "Box.h"

// Creates a Box object, including the associated Bullet rigid
// body, and sets its collision shape to a bullet box shape.
// If the object is specified to be kinematic, then the appropriate setup
// is done to the rigid body to make it kinematic within Bullet.
// position: where to create the box.
// dimensions: the dimensions of the box
// density: the density of the box (which, with the dimensions, determines its mass)
// isKinematic: flag to determine if this object will be kinematic or not
Box::Box(double position[3], double dimensions[3], double density, bool isKinematic)
{
	x = dimensions[0];
	y = dimensions[1];
	z = dimensions[2];
	double mass = x * y * z * density;
	
	btCollisionShape* boxCollision = new btBoxShape(btVector3(x * 0.5, y * 0.5, z * 0.5));
	btDefaultMotionState* boxState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(position[0], position[1], position[2])));
	btVector3 boxInertia(0, 0, 0);
	boxCollision->calculateLocalInertia(mass, boxInertia);
	btRigidBody::btRigidBodyConstructionInfo boxInfo(mass, boxState, boxCollision, boxInertia);
	btRigidBody* rb = new btRigidBody(boxInfo);
	
	// set the kinematic flag and set up the rigid body as kinematic if necessary
	setKinematic(isKinematic);
	if (isKinematic)
	{
		rb->setCollisionFlags(rb->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		rb->setActivationState(DISABLE_DEACTIVATION);
	}

	// set the rigid body associated with this SceneObject.
	setRigidBody(rb);
}

Box::~Box()
{
}

// Draws a reddish box of the correct dimensions in OpenGL
// using the transform provided by Bullet
void Box::drawOpenGL()
{
	// get the Bullet transform
	btTransform trans;
	getRigidBody()->getMotionState()->getWorldTransform(trans);

	// Bullet has a nice funciton for this.
	GLfloat glTrans[16];
	trans.getOpenGLMatrix(glTrans);

	glPushMatrix();
	glMultMatrixf(glTrans);
	GLfloat boxColor[4] = { 0.95, 0.7, 0.75, 1.0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, boxColor);

	// I'm cheating a bit here and using the glutSolidSphere
	// I would need to do this with more standard OpenGL calls to make
	// this more portable.
	glScaled(x, y, z);
	glutSolidCube(1.0);

	glPopMatrix();
}

// Returns a string with extra information specific to
// Box objects.
std::string Box::getMoreInfo() const
{
	std::string infoString = "      == Box ==\n";
	infoString += "        x = " + std::to_string(x) + "\n";
	infoString += "        y = " + std::to_string(y) + "\n";
	infoString += "        z = " + std::to_string(z) + "\n";
	return infoString;
}
