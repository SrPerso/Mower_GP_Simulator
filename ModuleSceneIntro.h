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
	void WorldUpdate();

	Timer playerTime;

	//SENSORS
	
	
	

	bool kicked = false;
private:
	// --- Audio
	AudioMusic BSO;
	AudioFX fxTurnOn;
	AudioFX fxTurnOff;
	AudioFX fxWoodImpact;
	AudioFX fxbaleImpact;
	AudioMusic fxMiddle;
	// --- Audio


	// --- car
	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;
	// --- car


	p2List<PhysBody3D*> bales_body;
	p2List<Cube*> bales;

	p2List<PhysBody3D*> greenbales_body;
	p2List<Cube*> greenbales;

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

	
	p2List<Cube*> Buldings;

	p2List<Cube*> Cow_corps;
	p2List<PhysBody3D*> CowCorps_body;
	p2List<PhysBody3D*> CowLegs_body;
	p2List<Cylinder*>Cow_legs;

	p2List<noCube*> invisibles;
	p2List<PhysBody3D*> invisible_bodies;

	Cylinder* silo1;
	Cylinder* silo2;


	p2List<Cylinder*> BuldingsCy;

	p2List<Cube*> Doors;
	p2List<PhysBody3D*> Doors_body;
	
	Cube* redFence1;
	Cube* redFence2;
	noCube* invDoor;
	PhysBody3D* invDoor_body;

	//sensor
	p2List<PhysBody3D*> sensors_body;
	p2List<Cylinder*> sensors;
	p2List<PhysBody3D*> sensors_bodycube;
	p2List<Cube*> sensorscube;

	p2List<Cylinder*> obstacles;
	


	//pb_snake[i]->GetTransform(&(s_snake[i].transform));
public:

	void CreateCylinderToBuldings(const float x, const float y, const  float z, const  float angle, const vec3&, Color colorr, const float r, const float h);

	void CreateCubeToBuldings(const float x, const float y, const  float z, const  float angle, const vec3&, Color colorr, const float w, const float h, const float l);
	void CreateBulding(float x, float y, float z);
	void CreateFarm();
	void CreateBridge(float x, float y, float z);

	void CreateFence(float distance, float tall, vec3 Position, vec3 rotationvec, float angle, int magicX, int magicZ);
	void CreateFences();

	void CreateBale(const float x, const float y, const  float z, const  float angle, const vec3&);
	void CreateBales();

	void CreateGreenBale(const float x, const float y, const float z, const float angle, const vec3& rotationAxis, float sizeX, float sizeY, float sizeZ);
	void CreateGreenBales();

	void CreateRock(const float x, const float y, const float z, const float angle, const vec3& rotationAxis);
	void CreateRocks();
	
	void CreateTree(const float x, const float y, const float z, const float tall, const float radious);
	void CreateTrees();

	void CreatePlane(const float x, const float y, const float z, const float width, const float height, Color colorr, const  float angle, const vec3& rotationAxis);
	void CreatePlanes();

	void CreateCow(const float x, const float y, const float z, const float angle, const vec3 RotationAxis);
	void CreateCows();

	void CreateInvisibleWall(const float x, const float y, const float z, const vec3 box, const float angle, const vec3 RotationAxis);
	void CreateInvisibleWalls();

	void CreateSensor( const float x, const float y, const float z, const float angle, const vec3& rotationAxis, int i);
	void CreateSensors();

	void CreateObstacle(const float x, const float y, const float z, const float angle, const vec3& rotationAxis);
	void CreateObstacles();
};

#endif // _SCENE_INTRO_