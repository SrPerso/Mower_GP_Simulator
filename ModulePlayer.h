#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1500.0f
#define MAX_DESACCELERATION -2000.0f
#define TURN_DEGREES 19.0f * DEGTORAD
#define BRAKE_POWER 1000.0f
#define MAXTIME 80.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();
	
	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	float inTime=0;
	void RestartAll();
	float thistime;
	bool restart=false;
	float lessScore= MAXTIME;
	bool losed = false;
	bool win = false;
	float MaxTime = MAXTIME;
	
public:
	bool MowerON = false;
	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;
	mat4x4 initial_matrix;

	bool debugCameramode = true;

};