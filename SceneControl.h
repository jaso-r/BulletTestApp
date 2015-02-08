#pragma once

// SceneControl does most of the work managing the model (Bullet)
// and the view (Glut), and keeps them synchronized.
// There should only ever be one of these per application, so use
// the static "getSceneControl" to provide a singleton.
class SceneControl
{
public:
	static SceneControl* getSceneControl(int* argcp, char** argv, double magnetDistance);

	~SceneControl();
	void start();
	void update();
	void draw();
	void addSceneObject(SceneObject* addObject);
	void setMainObject(SceneObject* lookAt);
	void moveMainObject(double x, double y, double z);
	void toggleMagnet();

private:
	static SceneControl* singleton;

	bool magnetMode;
	double magnetDistance;
	BulletScene* sceneModel;
	SceneObject* mainObject;
	std::vector<SceneObject*> objects;
	std::chrono::time_point<std::chrono::system_clock> lastTime;

	SceneControl(int* argcp, char** argv, double magnetDistance);
	void initModel();
	void initView(int* argcp, char** argv);
	void constrainToMain(SceneObject* constrainObject);
	inline double length(scenePosition pos1, scenePosition pos2);
};
