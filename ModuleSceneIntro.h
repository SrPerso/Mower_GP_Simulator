#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

struct MatrixCreator {
public: 
	MatrixCreator(int x,int y, int z,int angle): x(x),y(y),z(z),angle(angle) {
	}
	int x, y, z;
	int angle;

};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	p2List<Cube*> bales;
	p2List<TreeTop*> trees_top;
	p2List<Cylinder*> trees_trunk;

	Cube* CreateBale(const float x, const float y, const  float z, const  float angle, const vec3&);
	void CreateBales();

	void CreateTree(const float x, const float y, const float z, const float tall , const float radious);
	void CreateTrees();
};
