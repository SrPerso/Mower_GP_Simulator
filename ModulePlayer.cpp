#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"


ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	
	// Car properties ----------------------------------------
	car.chassis_size.Set(1.0f,0.3f, 2.2f);
	
	car.chassis_offset.Set(0, 0.4, 0);
	car.Motor_offset.Set(0, 0.8f, 0.485f);
	car.Sit_offset.Set(0, 0.9f, -1.0f);
	car.Sit2_offset.Set(0, 0.7f, -.60f);
	car.bin_offset.Set(0, 0.8f, -1.70f);

	car.mass = 1000.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.3f;
	float wheel_width = 0.3f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.1f * wheel_width+0.2f, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = true;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.1f * wheel_width-0.2f, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = true;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.1f * wheel_width+0.2f, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.1f * wheel_width-0.2f, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0, 0, -20);
	vehicle->GetTransform(&initial_matrix);
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;
	
	if (App->scene_intro->playerTime.Read() == 10.0f) {
		vehicle->SetPos(0, 0, -20);
		vehicle->Brake(1200);
	}

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		vehicle->Brake(brake);
		acceleration = MAX_ACCELERATION;
	}

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if(turn < TURN_DEGREES)
			turn +=  TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if(turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		acceleration = MAX_DESACCELERATION;
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		RestartAll();		
	}
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
		MaxTime = 80.0f;
	}
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
		MaxTime = 70.0f;
	}
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) {
		MaxTime = 60.0f;
	}
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN) {
		MaxTime = 50.0f;
	}


	if (vehicle->GetPos().x >= 200 || vehicle->GetPos().x <= -200 || vehicle->GetPos().z > 200 || vehicle->GetPos().z < -200) {
		vehicle->SetPos(0, 0, -20);
	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	

	vehicle->Render();
	
	if (thistime <MaxTime) {		

	}
	else {
		RestartAll();
		losed = true;
	}

	char title[1000];
	thistime = (float)App->scene_intro->playerTime.Read() / 1000;	
	
	if (restart == true) {
		thistime = 0;
	}
	
	sprintf_s(title, "%.1f Km/h, Time: %.2f, Less Time: %.2f ,MaxTime %0.2f", vehicle->GetKmh(), thistime, lessScore,MaxTime);
	App->window->SetTitle(title);
	losed = false;

	return UPDATE_CONTINUE;
}


void ModulePlayer::RestartAll() {

	restart = true;	
	vehicle->SetPos(0, 0, -20);
	vehicle->get_rigidbody()->setLinearVelocity({ 0,0,0 });
	vehicle->get_rigidbody()->setAngularVelocity({ 0,0,0 });
	vehicle->SetTransform(&initial_matrix);

	App->scene_intro->playerTime.Stop();
	for (uint i = 0; i <7; i++) {
		App->scene_intro->checkpoints[i] = false;
	}
	App->scene_intro->kicked = false;
	App->scene_intro->fxTurnOn.play();
	App->scene_intro->fxMiddle.play(0);
	App->scene_intro->invDoor_body->SetPos(60 + 0.25f, 4 + 0, -69);
}
