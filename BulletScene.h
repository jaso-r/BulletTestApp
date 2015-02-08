#pragma once

//  BulletScene is used as a container to wrap up all of the
//  Bullet-specific functionality of our program -- the dynamics world,
//  the sovler, the collision dispatcher, etc.
//  It is, essentially, "model" in MVC
class BulletScene
{
public:
	BulletScene();
	~BulletScene();

	void stepSim(double dt, int steps);
	void addBody(btRigidBody* body);
	void removeBody(btRigidBody* body);
	void addSpring(btRigidBody* body, btRigidBody* constrainTo);
	bool hasSpring(btRigidBody* body);
	void clearSprings();

private:
	btDiscreteDynamicsWorld* dynamics;
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* configuration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	std::unordered_map<btRigidBody*, btTypedConstraint*> springMap;
};

