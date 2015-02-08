#include "stdafx.h"
#include "Sphere.h"

// Creates a Sphere object, including the associated Bullet rigid
// body, and sets its collision shape to a bullet sphere shape.
// If the object is specified to be kinematic, then the appropriate setup
// is done to the rigid body to make it kinematic within Bullet.
// position: where to create the sphere.
// radius: the radius of the sphere
// density: the density of the sphere (which, with the radius, determines its mass)
// isKinematic: flag to determine if this object will be kinematic or not
Sphere::Sphere(double position[3], double radius, double density, bool isKinematic)
{
	this->radius = radius;
	double mass = density * ((4.0 / 3.0) * PI * radius * radius * radius);
	
	btCollisionShape* sphereCollision = new btSphereShape(radius);
	btDefaultMotionState* sphereState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(position[0], position[1], position[2])));
	btVector3 sphereInertia(0, 0, 0);
	sphereCollision->calculateLocalInertia(mass, sphereInertia);
	btRigidBody::btRigidBodyConstructionInfo sphereInfo(mass, sphereState, sphereCollision, sphereInertia);
	btRigidBody* rb = new btRigidBody(sphereInfo);
	
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

Sphere::~Sphere()
{
}

// Draws a blue-grey sphere in OpenGL of the appropriate radius
// using the transform provided by Bullet.
void Sphere::drawOpenGL()
{
	// get the Bullet transform
	btTransform trans;
	getRigidBody()->getMotionState()->getWorldTransform(trans);

	// Bullet has a nice function for this
	GLfloat glTrans[16];
	trans.getOpenGLMatrix(glTrans);

	glPushMatrix();
	glMultMatrixf(glTrans);
	GLfloat ballColor[4] = { 0.75, 0.8, 0.95, 1.0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ballColor);

	// I'm cheating a bit here and using the glutSolidSphere
	// I would need to do this with more standard OpenGL calls to make
	// this more portable.
	glutSolidSphere(radius, 30, 30);
	
	glPopMatrix();
}

// Returns a string with extra information specific to
// Sphere objects.
std::string Sphere::getMoreInfo() const
{
	std::string infoString = "      == Sphere ==\n";
	infoString += "        radius = " + std::to_string(radius) + "\n";
	return infoString;
}
