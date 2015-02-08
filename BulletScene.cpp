#include "stdafx.h"
#include "BulletScene.h"


// Contructor for a BulletScene.
// Creates and stores the many bullet objects needed to
// run a Bullet simulation.
BulletScene::BulletScene()
{
	// build the broadphase
	broadphase = new btDbvtBroadphase();

	// set up collision configuration and dispatcher
	configuration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(configuration);

	// create the solver and the world
	solver = new btSequentialImpulseConstraintSolver();
	dynamics = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, configuration);
	dynamics->setGravity(btVector3(0, -10, 0));
}


// Destructor for a BulletScene.
// Bullet assumes that we are responsible for cleaning up (deleting)
// any objects that we passed to it after we're done, so
// we do this here.
BulletScene::~BulletScene()
{
	delete dynamics;
	delete solver;
	delete dispatcher;
	delete configuration;
	delete broadphase;
}

// This simply updates the bullet Dynamics World by the provided
// time and using the provided number of steps
void BulletScene::stepSim(double dt, int steps)
{
	dynamics->stepSimulation(dt, steps);
}

// Adds a rigid body to the dynamics world.
void BulletScene::addBody(btRigidBody* body)
{
	dynamics->addRigidBody(body);
}

// Removes a rigid body from the dynamics world
void BulletScene::removeBody(btRigidBody* body)
{
	dynamics->removeRigidBody(body);
}

void BulletScene::addSpring(btRigidBody* body, btRigidBody* constrainTo)
{
	btTransform tr;
	tr.setIdentity();
	btGeneric6DofSpringConstraint* spring = new btGeneric6DofSpringConstraint(*body, *constrainTo, tr, tr, true);
	spring->setLinearUpperLimit(btVector3(5, 5, 5));
	spring->setLinearLowerLimit(btVector3(-5, -5, -5));
	spring->setAngularUpperLimit(btVector3(3, 3, 3));
	spring->setAngularLowerLimit(btVector3(-3, -3, -3));
	spring->enableSpring(0, true);
	spring->enableSpring(1, true);
	spring->enableSpring(2, true);
	spring->setStiffness(0, 15.0);
	spring->setStiffness(1, 15.0);
	spring->setStiffness(2, 15.0);
	spring->setDamping(0, 0.3);
	spring->setDamping(1, 0.3);
	spring->setDamping(2, 0.3);
	spring->setBreakingImpulseThreshold(150.0);
	springMap.insert({ body, spring });
	dynamics->addConstraint(spring, false);
}

bool BulletScene::hasSpring(btRigidBody* body)
{
	bool springInMap = true;
	try
	{
		springMap.at(body);
	}
	catch (const std::out_of_range& oor)
	{
		springInMap = false;
	}
	return springInMap;
}

void BulletScene::clearSprings()
{
	for (std::unordered_map<btRigidBody*, btTypedConstraint*>::iterator it = springMap.begin(); it != springMap.end(); ++it)
	{
		dynamics->removeConstraint(it->second);
		delete (it->second);
	}
	springMap.clear();
}
