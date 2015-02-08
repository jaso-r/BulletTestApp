#include "stdafx.h"
#include "SceneObject.h"

// Constructor for SceneObject
// won't be called because SceneObject is abstract.
SceneObject::SceneObject()
{
}

// Destructor for a SceneObject
// This method will attempt to delete the associated
// rigid body given that the pointer is not NULL
// So implementing classes do not have to do this themselves.
SceneObject::~SceneObject()
{
	if (body != NULL)
	{
		delete body->getMotionState();
		delete body->getCollisionShape();
		delete body;
		body = NULL;
	}
}

// Return a pointer to the Bullet rigid body associated
// with this scene object.  This pointer should default
// to NULL if one was never created.
btRigidBody* SceneObject::getRigidBody()
{
	return body;
}

// Sets the Bullet rigid body associated with this scene
// object.  Subclasses should call this to store their
// implementation-specific rigid body at creation time.
void SceneObject::setRigidBody(btRigidBody* newBody)
{
	body = newBody;
}

// gets the position of this object
scenePosition SceneObject::getPosition() const
{
	btTransform trans;
	scenePosition pos;
	body->getMotionState()->getWorldTransform(trans);
	pos.x = trans.getOrigin().getX();
	pos.y = trans.getOrigin().getY();
	pos.z = trans.getOrigin().getZ();
	return pos;
}

// sets the position of this object, but only if
// it is kinematic
void SceneObject::setPosition(double x, double y, double z)
{
	if (kinematic)
	{
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(btVector3(x, y, z));
		btMotionState* motionState = body->getMotionState();
		body->setWorldTransform(trans);
		motionState->setWorldTransform(trans);
	}
}

// sets the object's kinematic flag
void SceneObject::setKinematic(bool isKinematic)
{
	kinematic = isKinematic;
}

// get the value of the object's kinematic flag
bool SceneObject::isKinematic() const
{
	return kinematic;
}

// implementation of the << operator on a SceneObject.
// prints some useful debugging info to the stream.
std::ostream& operator<<(std::ostream& os, const SceneObject& obj)
{
	scenePosition pos = obj.getPosition();
	std::string info = obj.getMoreInfo();

	os << "=============== Scene Object ================" << std::endl;
	os << "    Position:" << std::endl;
	os << "        x = " << pos.x << ", " << std::endl;
	os << "        y = " << pos.y << ", " << std::endl;
	os << "        z = " << pos.z << std::endl;
	os << "    Kinematic: " << (obj.kinematic ? "TRUE" :"FALSE") << std::endl;
	os << "    More Info:" << std::endl;
	os << info;
	os << "=============================================" << std::endl;
	return os;
}
