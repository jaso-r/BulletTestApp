#pragma once
#include "SceneObject.h"

// A GroundPlane is a SceneObject that represents
// a static infinite collision plane on the XZ axis
// at a particular height.
class GroundPlane : public SceneObject
{
public:
	GroundPlane(double height);
	virtual ~GroundPlane();
	virtual void drawOpenGL();
	virtual std::string getMoreInfo() const;

private:
	double groundHeight;
};

