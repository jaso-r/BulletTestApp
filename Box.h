#pragma once
#include "SceneObject.h"

// A Box is a SceneObject that represents a box with
// a given height, width, and depth.
class Box : public SceneObject
{
public:
	Box(double position[3], double dimensions[3], double density, bool isKinematic);
	virtual ~Box();
	virtual void drawOpenGL();
	virtual std::string getMoreInfo() const;

private:
	double x, y, z;
};

