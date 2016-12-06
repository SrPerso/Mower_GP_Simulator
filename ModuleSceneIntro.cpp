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
	CreateTrees();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	for (int i = 0; i < bales.count(); i++)
		delete bales[i];

	for (int i = 0; i < trees_top.count(); i++)
		delete trees_top[i];

	for (int i = 0; i < trees_trunk.count(); i++)
		delete trees_trunk[i];

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	

	//--------------------tree


	//-----------------------------
	//--- render trees
	//-----------------------------

	p2List_item<TreeTop*>* iteratorTop;
	p2List_item<Cylinder*>* iteratorTrunk;

	iteratorTop = trees_top.getFirst();
	iteratorTrunk = trees_trunk.getFirst();

	while (iteratorTop != nullptr) {

		iteratorTop->data->Render();
		iteratorTrunk->data->Render();

		iteratorTop = iteratorTop->next;
		iteratorTrunk = iteratorTrunk->next;
	}
	//-----------------------------
	//--- render bales
	//-----------------------------
	p2List_item<Cube*>* iteratorBale;
	iteratorBale = bales.getFirst();

	while (iteratorBale != nullptr) {
		iteratorBale->data->Render();

		iteratorBale = iteratorBale->next;
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
	//1
	for (int i = 0, j = 5; i <= 12; i++) {
		CreateBale(3.5, 0.5f, j, 0, vec3{ 1,0,0 });
		j += 3;
	}
	//2	
	for (int i = 0, j = 5; i <= 12; i++) {
		CreateBale(-3.5, 0.5f, j, 0, vec3{ 1,0,0 });
		j += 3;
	}
	//12
	for (int i = 0, j = 52; i <= 4; i++) {
		CreateBale(-1.5, 0.5f, j, 0, vec3{ 1,0,0 });
		j += 3;
	}
	
	//3

	CreateBale(-3.5, 0.5f, 44, 0, vec3{ 0,1,0 });
	CreateBale(-3, 0.5f, 47, 20, vec3{ 0,1,0 });
	CreateBale(-1.5, 0.5f, 49.5, 40, vec3{ 0,1,0 });
	CreateBale(0.5, 0.5f, 51.5, 60, vec3{ 0,1,0 });	
	CreateBale(3, 0.5f, 52.5, 80, vec3{ 0,1,0 });

	//4
	CreateBale(5.5, 0.5f, 52.5, 100, vec3{ 0,1,0 });
	CreateBale(8.0, 0.5f, 51.5, 120, vec3{ 0,1,0 });
	CreateBale(10, 0.5f, 49.5, 140, vec3{ 0,1,0 });
	CreateBale(11, 0.5f, 47, 160, vec3{ 0,1,0 });
	CreateBale(11.5, 0.5f, 44, 0, vec3{ 0,1,0 });

	//5
	for (int i = 0, j = 41; i <5; i++) {
		CreateBale(11.5, 0.5f, j, 0, vec3{ 0,1,0 });
		j -= 3;
	}
	//9
	for (int i = 0, j = 44; i <6; i++) {
		CreateBale(16.5, 0.5f, j, 0, vec3{ 0,1,0 });
		j -= 3;
	}
	//8
	for (int i = 0, j = 47; i <7; i++) {
		CreateBale(25, 0.5f, j, 0, vec3{ 0,1,0 });
		j -= 3;
	}
	//6
	for (float i = 0, j = 15.5; i < 2; i++) {
		CreateBale(j, 0.5f, 27, 90, vec3{ 0,1,0 });
		j -= 3;
	}

	//7
	for (float i = 0, j = 20; i < 5; i++) {
		CreateBale(j, 0.5f, 19, 90, vec3{ 0,1,0 });
		j -= 3;
	}

	//13
	CreateBale(-1.5, 0.5f, 64, 0, vec3{ 0,1,0 });
	CreateBale(-1, 0.5f, 67, 20, vec3{ 0,1,0 });
	CreateBale(0.5, 0.5f, 69.5, 40, vec3{ 0,1,0 });
	CreateBale(2.5, 0.5f, 71.5, 60, vec3{ 0,1,0 });
	CreateBale(5, 0.5f, 72.5, 80, vec3{ 0,1,0 });

	//10
	for (float i = 0, j =9 ,w = 63; i < 8; i++) {
		CreateBale(j, 0.5f,w , 90, vec3{ 0,1,0 });
		j += 3;
		w += 0.25;
	}

	//14
	for (float i = 0, j = 7.5; i < 9; i++) {
		CreateBale(j, 0.5f, 73, 90, vec3{ 0,1,0 });
		j += 3;
	}
}

void ModuleSceneIntro::CreateTree(const float x, const float y, const float z, const float tall, const float radious)
{


	TreeTop* ttop = new TreeTop(radious, (tall / 4)*3);

	ttop->SetRotation(90, vec3(0, 0, 1));
	ttop->SetPos(x, y + (tall / 4)*2 + 1.5f , z);
	ttop->color = Green;

	Cylinder* ttrunk = new Cylinder(0.175f*radious, (tall / 4));
	
	ttrunk->SetRotation(90, vec3(0, 0, 1));
	ttrunk->SetPos(x, y + 1.5f, z);
	ttrunk->color = Brown;

	trees_top.add(ttop);
	trees_trunk.add(ttrunk);
}

void ModuleSceneIntro::CreateTrees()
{
	CreateTree(-6, 0, 15, 15, 2);
	CreateTree(-6, 0, 30, 15, 2);
	CreateTree(-6, 0, 45, 12, 1.75);
	CreateTree(-6, 0, 60, 15, 2);

	CreateTree(15, 0, 42, 15, 2);
	CreateTree(13, 0, 35, 20, 3);
	CreateTree(13.5, 0, 29, 15, 2);
}
