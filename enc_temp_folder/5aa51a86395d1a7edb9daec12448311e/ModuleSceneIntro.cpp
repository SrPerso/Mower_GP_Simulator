#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(-20.0f, 30.0f,-20.0f));
	App->camera->LookAt(vec3(30,0, 30));



	//bales
	

	CreateBales();
	
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");



	for (int i = 0; i < bales.count(); i++)
		delete bales[i];


	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	

	//--------------------tree
	TreeTop t1(2,12);
	
	t1.SetRotation(90, vec3(0, 0, 1));
	t1.SetPos(-5, 9, 6);
	t1.color = Green;
	t1.axis = true;
	t1.Render();

	Cylinder t1_trunk(0.35f, 3);

	t1_trunk.SetRotation(90, vec3(0, 0, 1));
	t1_trunk.SetPos(-5, 1.5f, 6);

	t1_trunk.color = Brown;
	t1_trunk.Render();

	//-----------------------------


	//--- render bales

	p2List_item<Cube*>* iterator;
	iterator = bales.getFirst();

	while (iterator != nullptr) {
		iterator->data->Render();

		iterator = iterator->next;
	}

	//--------Plane-------
	Plane p(0, 1, 0, 0);
	p.color = Green;
	p.axis = true;
	p.Render();
	//---------------------
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

Cube* ModuleSceneIntro::CreateBale(const float x, const float y, const float z, const float angle, const vec3& rotationAxis)
{

	Cube* heno = new Cube(1, 1, 2.2f);

	heno->SetPos(x, y, z);
	heno->SetRotation(angle, rotationAxis);
	heno->color = Yellow;
	
	
	bales.add(heno);

	return heno;
}

void ModuleSceneIntro::CreateBales()
{

	for (int i = 0, j = 5; i <= 12; i++) {
		CreateBale(3.5, 0.5f, j, 0, vec3{ 1,0,0 });
		j += 3;
	}
		
	for (int i = 0, j = 5; i <= 12; i++) {
		CreateBale(-3.5, 0.5f, j, 0, vec3{ 1,0,0 });
		j += 3;
	}
	
	//curva 1 -1

	CreateBale(-3.5, 0.5f, 44, 0, vec3{ 0,1,0 });
	CreateBale(-3, 0.5f, 47, 20, vec3{ 0,1,0 });
	CreateBale(-1.5, 0.5f, 49.5, 40, vec3{ 0,1,0 });
	CreateBale(0.5, 0.5f, 51.5, 60, vec3{ 0,1,0 });	
	CreateBale(3, 0.5f, 52.5, 80, vec3{ 0,1,0 });

	//curva1
	CreateBale(5.5, 0.5f, 52.5, 100, vec3{ 0,1,0 });
	CreateBale(8.0, 0.5f, 51.5, 120, vec3{ 0,1,0 });
	CreateBale(10, 0.5f, 49.5, 140, vec3{ 0,1,0 });
	CreateBale(11, 0.5f, 47, 160, vec3{ 0,1,0 });
	CreateBale(11.5, 0.5f, 44, 0, vec3{ 0,1,0 });


	for (int i = 0, j = 41; i <= 4; i++) {
		CreateBale(11.5, 0.5f, j, 0, vec3{ 0,1,0 });
		j -= 3;
	}
	for (int i = 0, j = 44; i <= 5; i++) {
		CreateBale(16.5, 0.5f, j, 0, vec3{ 0,1,0 });
		j -= 3;
	}

	for (int i = 0, j = 47; i <= 6; i++) {
		CreateBale(25, 0.5f, j, 0, vec3{ 0,1,0 });
		j -= 3;
	}

	//curva 4 - 2 
	CreateBale(-1.5, 0.5f, 64, 0, vec3{ 0,1,0 });
	CreateBale(-1, 0.5f, 67, 20, vec3{ 0,1,0 });
	CreateBale(0.5, 0.5f, 69.5, 40, vec3{ 0,1,0 });
	CreateBale(2.5, 0.5f, 71.5, 60, vec3{ 0,1,0 });
	CreateBale(5, 0.5f, 72.5, 80, vec3{ 0,1,0 });


}
