#pragma once
#include "SceneObject.h"

// A Sphere object is a SceneObject representing a sphere with
// a given radius.
class Sphere : public SceneObject
{
public:
	Sphere(double position[3], double radius, double density, bool isKinematic);
	virtual ~Sphere();
	virtual void drawOpenGL();
	virtual std::string getMoreInfo() const;

private:
	double radius;
	const double PI = 3.141592653589793238463;
};

