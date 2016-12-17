#pragma once
#ifndef _SCENE_INTRO_
#define _SCENE_INTRO_
       
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "p2List.h"

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

private:
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



	p2List<PhysBody3D*> bales_body;
	p2List<Cube*> bales;

	p2List<PhysBody3D*> rocks_body;
	p2List<Cylinder*> rocks;

	p2List<PhysBody3D*> trees_top_body;
	p2List<TreeTop*> trees_top;

	p2List<PhysBody3D*> trees_trunk_body;
	p2List<Cylinder*> trees_trunk;

	p2List<PhysBody3D*> postes_body_body;
	p2List<Cylinder*> postes;

	p2List<PhysBody3D*> transversales_body;
	p2List<Cube*> transversales;

	p2List<Plane*> Planos;

	p2List<PhysBody3D*> Farm_body;
	p2List<Cube*> Farm;

	p2List<Cube*> Cow_corps;
	p2List<PhysBody3D*> CowCorps_body;
	p2List<PhysBody3D*> CowLegs_body;
	p2List<Cylinder*>Cow_legs;


	Cylinder* silo1;
	Cylinder* silo2;


	//pb_snake[i]->GetTransform(&(s_snake[i].transform));
public:

	void CreateFarm();
	void CreateCubeToFarm(const float x, const float y, const  float z, const  float angle, const vec3&, Color colorr, const float w, const float h, const float l, bool physics=false);

	void CreateFence(float distance, float tall, vec3 Position, vec3 rotationvec, float angle, int magicX, int magicZ);
	void CreateFences();

	void CreateBale(const float x, const float y, const  float z, const  float angle, const vec3&);
	void CreateBales();

	void CreateRock(const float x, const float y, const float z, const float angle, const vec3& rotationAxis);
	void CreateRocks();


	void CreateTree(const float x, const float y, const float z, const float tall, const float radious);
	void CreateTrees();

	void CreatePlane(const float x, const float y, const float z, const float width, const float height, Color colorr, const  float angle, const vec3& rotationAxis);
	void CreatePlanes();

	void CreateCow(const float x, const float y, const float z, const float angle, const vec3 RotationAxis);
	void CreateCows();
};

#endif // _SCENE_INTRO_