#pragma once

// a convenience structure to pass around positions
struct scenePosition
{
	double x;
	double y;
	double z;
};

//  A SceneObject is a virtual class that sets up some necessary 
//  attributes for dynamic objects in the scene.  Implementing
//  subclasses should create appropriate rigid bodies to represent
//  themselves in the Bullet sim, and they should know how to draw
//  themselves in an OpenGL context.
class SceneObject
{
public:
	SceneObject();
	virtual ~SceneObject();
	btRigidBody* getRigidBody();
	scenePosition getPosition() const;
	void setPosition(double x, double y, double z);
	bool isKinematic() const;
	virtual void drawOpenGL() = 0; // must be overridden by implementing classes
	virtual std::string getMoreInfo() const = 0; // must be overridden by implementing classes
	friend std::ostream& operator<<(std::ostream& os, const SceneObject& obj);

protected:
	void setRigidBody(btRigidBody* newBody);
	void setKinematic(bool isKinematic);

private:
	btRigidBody* body = NULL;
	bool kinematic = false;
};

// override the << operator so we can write debug data to streams
std::ostream& operator<<(std::ostream& os, const SceneObject& obj);


