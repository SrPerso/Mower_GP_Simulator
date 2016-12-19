#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "Time.h"
#include "ModulePhysics3D.h"

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

	//--- Audio
	App->audio->Init();
	App->audio->setMusicVolume(80);

	fxWoodImpact = App->audio->LoadAudioFX("Sounds/FX/wood_impact_fx.wav");
	fxbaleImpact = App->audio->LoadAudioFX("Sounds/FX/bale_impact_fx.wav");
	fxTurnOff = App->audio->LoadAudioFX("Sounds/FX/TurnOff_Tractor_Sound.wav");
	fxTurnOn = App->audio->LoadAudioFX("Sounds/FX/TurnOn_Tractor_Sound.wav");
	fxMiddle = App->audio->LoadAudioMusic("Sounds/BSO/Game_Music1.ogg");	
	BSO = App->audio->LoadAudioMusic("Sounds/BSO/Intro_Music.ogg");
	BSO.play();

	//--- Audio

	srand(time(NULL));

	CreateBales();
	CreateTrees();
	CreateFences();
	CreatePlanes();
	CreateFarm();
	CreateCows();
	CreateRocks();
	CreateInvisibleWalls();




	//if (App->player->debugCameramode == true) {
		App->camera->Move(vec3(0, +0, 0.0));
		App->camera->LookAt(vec3(120, 1.5f, 60));
	
//	}
	/*else if(App->player->debugCameramode==false){
		App->camera->Move(vec3(130,70, -160));
		App->camera->LookAt(vec3(90, 2.6f, -70));
	}*/

	//bales
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
	
	//fences
	for (int i = 0; i < postes.count(); i++)
		delete postes[i];

	for (int i = 0; i < transversales.count(); i++)
		delete transversales[i];
	//
	for (int i = 0; i < Farm.count(); i++)
		delete Farm[i];

	for (int i = 0; i < Cow_corps.count(); i++)
		delete Cow_corps[i];

	for (int i = 0; i < Cow_legs.count(); i++)
		delete Cow_legs[i];


	delete silo1;
	delete silo2;

	//f1->Clear();
	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	WorldUpdate();

	
	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
	{
		if (App->player->MowerON == true) {
			App->player->MowerON = false;
			fxTurnOff.play();
			fxMiddle.stop();

		}
		else {
			App->audio->setMusicVolume(60);
			App->player->MowerON = true;
			fxTurnOn.play();
			fxMiddle.play(0);
		}

	}	


	//---------------------
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::WorldUpdate() {

	
	//-----------------------------
	//--- invisible walls
	//-----------------------------
	p2List_item<noCube*>* iteratorinv;
	p2List_item<PhysBody3D*>* iteratorinv_body;

	iteratorinv = invisibles.getFirst();
	iteratorinv_body = invisible_bodies.getFirst();

	while (iteratorinv_body != nullptr) {

		iteratorinv_body->data->GetTransform(&(iteratorinv->data->transform));
		iteratorinv->data->Render();

		iteratorinv = iteratorinv->next;
		iteratorinv_body = iteratorinv_body->next;
	}
	//--------------------
	silo2->Render();
	silo1->Render();
	//-----------------------------
	//--- Farm
	//-----------------------------
	p2List_item<Cube*>* iteratorFarm;
	p2List_item<PhysBody3D*>*iteratorFarm_body;
	iteratorFarm = Farm.getFirst();

	iteratorFarm_body = Farm_body.getFirst();
	while (iteratorFarm != nullptr) {

		//iteratorFarm_body->data->GetTransform(&(iteratorFarm->data->transform));
		iteratorFarm->data->Render();

		//iteratorFarm_body = iteratorFarm_body->next;
		iteratorFarm = iteratorFarm->next;
	}

	//-----------------------------
	//--- Fances
	//-----------------------------

	p2List_item<Cube*>* iteratorTrans;
	p2List_item<Cylinder*>* iteratorPostes;

	iteratorTrans = transversales.getFirst();

	while (iteratorTrans != nullptr) {
		iteratorTrans->data->Render();
		iteratorTrans = iteratorTrans->next;
	}
	iteratorPostes = postes.getFirst();
	while (iteratorPostes != nullptr) {
		iteratorPostes->data->Render();
		iteratorPostes = iteratorPostes->next;
	}

	//-----------------------------
	//--- render trees
	//-----------------------------

	p2List_item<TreeTop*>* iteratorTop;
	//p2List_item<PhysBody3D*>*iteratorTop_body;

	p2List_item<Cylinder*>* iteratorTrunk;
	p2List_item<PhysBody3D*>*iteratorTrunk_body;


	iteratorTop = trees_top.getFirst();
	//iteratorTop_body = trees_top_body.getFirst();

	iteratorTrunk = trees_trunk.getFirst();
	iteratorTrunk_body = trees_trunk_body.getFirst();

	while (iteratorTop != nullptr) {

		iteratorTrunk_body->data->GetTransform(&(iteratorTrunk->data->transform));

		iteratorTop->data->Render();
		iteratorTrunk->data->Render();

		iteratorTop = iteratorTop->next;
		//iteratorTop_body = iteratorTop_body->next;
		iteratorTrunk = iteratorTrunk->next;
		iteratorTrunk_body = iteratorTrunk_body->next;
	}
	//-----------------------------
	//--- render bales
	//-----------------------------
	p2List_item<Cube*>* iteratorBale;
	p2List_item<PhysBody3D*>* iteratorBale_body;

	iteratorBale = bales.getFirst();
	iteratorBale_body = bales_body.getFirst();

	while (iteratorBale != nullptr) {

		iteratorBale_body->data->GetTransform(&(iteratorBale->data->transform));
		iteratorBale->data->Render();

		iteratorBale = iteratorBale->next;
		iteratorBale_body = iteratorBale_body->next;
	}

	//-----------------------------
	//--- render ROCKS
	//-----------------------------

	p2List_item<Cylinder*>* iteratorRock;
	p2List_item<PhysBody3D*>* iteratorRock_body;

	iteratorRock = rocks.getFirst();
	iteratorRock_body = rocks_body.getFirst();

	while (iteratorRock != nullptr) {

		iteratorRock_body->data->GetTransform(&(iteratorRock->data->transform));
		iteratorRock->data->Render();

		iteratorRock = iteratorRock->next;
		iteratorRock_body = iteratorRock_body->next;
	}


	//-----------------------------
	//--- render Cows
	//-----------------------------

	p2List_item<Cylinder*>* iteratorlegs;
	p2List_item<Cube*>* iteratorbody;

	p2List_item<PhysBody3D*>* iteratorcow_Legs_body;
	p2List_item<PhysBody3D*>* iteratorcow_Corps_body;

	iteratorlegs = Cow_legs.getFirst();
	iteratorbody = Cow_corps.getFirst();

	iteratorcow_Legs_body = CowLegs_body.getFirst();
	iteratorcow_Corps_body = CowCorps_body.getFirst();

	while (iteratorlegs != nullptr) {

		iteratorcow_Legs_body->data->GetTransform(&(iteratorlegs->data->transform));
		iteratorlegs->data->Render();

		iteratorcow_Legs_body = iteratorcow_Legs_body->next;
		iteratorlegs = iteratorlegs->next;
	}
	while (iteratorbody != nullptr) {

		iteratorcow_Corps_body->data->GetTransform(&(iteratorbody->data->transform));
		iteratorbody->data->Render();

		iteratorcow_Corps_body = iteratorcow_Corps_body->next;
		iteratorbody = iteratorbody->next;
	}


	//-----------------------------
	//--- render Planes
	//-----------------------------

	p2List_item<Plane*>* iteratorPlane;
	iteratorPlane = Planos.getFirst();

	while (iteratorPlane != nullptr) {
		iteratorPlane->data->Render();

		iteratorPlane = iteratorPlane->next;
	}

}

//colision
void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{

}


//=========================================================================================================
//creators  =========================================================================================================
//=========================================================================================================
void ModuleSceneIntro::CreateCubeToFarm(const float x, const float y, const float z, const float angle, const vec3 & rotationAxis, Color colorr , const float w, const float h , const float l, bool physics)
{

	Cube* cube = new Cube(w, h, l);
	cube->SetPos(x, y, z);
	cube->SetRotation(angle, rotationAxis);
	cube->color = colorr;
	Farm.add(cube);
	if(physics==true)
	Farm_body.add(App->physics->AddBody(*cube,0));

}

void ModuleSceneIntro::CreateFarm()
{
	silo1 = new Cylinder(2, 35);
	silo1->SetPos(-12, 0, -5);
	silo1->SetRotation(90, vec3(0, 0, 1));
	silo1->color = White;


	silo2 = new Cylinder(2, 35);
	silo2->SetPos(-12, 0, 0);
	silo2->SetRotation(90, vec3(0, 0, 1));
	silo2->color = White;


	CreateCubeToFarm(0, 15.5, 0, +45, vec3(0, 0, 1), White, 0.5, 0.5, 21); //roof1
	CreateCubeToFarm(-2, 15, 0, +15, vec3(0, 0, 1), grey, 5, 0.25f, 20); //roof1
	CreateCubeToFarm(+2, 15, 0, -15, vec3(0, 0, 1), grey, 5, 0.25f, 20); //roof2
	CreateCubeToFarm(-6, 13, 0, +30, vec3(0, 0, 1), grey, 5, 0.25f, 20); //roof3
	CreateCubeToFarm(+6, 13, 0, -30, vec3(0, 0, 1), grey, 5, 0.25f, 20); //roof4

	CreateCubeToFarm(-8.3, 10.8, 0, +60, vec3(0, 0, 1), grey, 3, 0.25f, 20); //roof5
	CreateCubeToFarm(+8.3, 10.8, 0, -60, vec3(0, 0, 1), grey, 3, 0.25f, 20); //roof6

	//---------------- w1

	CreateCubeToFarm(+8.3, 5, -8.5, 90, vec3(0, 0, 1), White, 11, 0.25f, 0.5); //wall1
	
	float j = -7.7;

	for (int i = 0; i < 15; i++) {

		CreateCubeToFarm(+8.3, 5, j , 90, vec3(0, 0, 1), Red, 11, 0.25f, 1); //wall1
		j += 1.1;
	}
	CreateCubeToFarm(+8.4, 5, +0, 90, vec3(0, 0, 1), White, 10.5, 0.7, 0.5); //wall1
	CreateCubeToFarm(+8.3, 5, +8.5, 90, vec3(0, 0, 1), White, 11, 0.25f, 0.5);
	//---------------- w1
	//---------------- w2
	CreateCubeToFarm(-8.3, 5, -8.5, 90, vec3(0, 0, 1), White, 11, 0.25f, 0.5); //wall2

	j = -7.7;

	for (int i = 0; i < 15; i++) {

		CreateCubeToFarm(-8.3, 5, j, 90, vec3(0, 0, 1), Red, 11, 0.25f, 1); //wall2
		j += 1.1;
	}
	CreateCubeToFarm(-8.4, 5, +0, 90, vec3(0, 0, 1), White, 10.5, 0.7, 0.5); //wall2
	CreateCubeToFarm(-8.3, 5, +8.5, 90, vec3(0, 0, 1), White, 11, 0.25f, 0.5);
	//---------------- w2
	//---------------- w3
	CreateCubeToFarm(+8.5, 5, -9, 90, vec3(1, 0, 0), White, 0.5, 0.25f, 10.5); //wall3

	CreateCubeToFarm(+7.4, 5.5, -9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 13); //wall3
	CreateCubeToFarm(+6.3, 6, -9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 13.25); //wall3
	CreateCubeToFarm(+5.2, 6.35, -9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 13.75); //wall3
	CreateCubeToFarm(+4.1, 6.75, -9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 14.75); //wall3

	j = +3;
	float w = 5,u= 12;
	for (int i = 0; i < 7; i++) {

		if (i < 3) {
			CreateCubeToFarm(j, u, -9, 90, vec3(1, 0, 0), Red, 1, 0.25f, w); //wall3
			w += 0.25;	u += 0.125;
		}
		
		if (i >= 3 && i < 6) {
			CreateCubeToFarm(j, u, -9, 90, vec3(1, 0, 0), Red, 1, 0.25f, w); //wall3	
			w -= 0.25;	u -= 0.125;
		}		
		else 
			CreateCubeToFarm(j+0.25, 12, -9, 90, vec3(1, 0, 0), Red, 0.6, 0.25f, 5); //wall3			
	j-= 1.1;
	}

	CreateCubeToFarm(-7.5, 5.5, -9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 13); //wall3
	CreateCubeToFarm(-6.4, 6, -9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 13.25); //wall3
	CreateCubeToFarm(-5.3, 6.30, -9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 13.75); //wall3
	CreateCubeToFarm(-4.2, 6.75, -9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 14.25); //wall3

	CreateCubeToFarm(-8.5, 5, -9, 90, vec3(1, 0, 0), White, 0.5, 0.25f, 10.5); //wall3

//---------------- w3

	CreateCubeToFarm(0, 9.6, -9, +0, vec3(0, 0, 1), White, 12, 0.40, 0.40); //roof1
	CreateCubeToFarm(-3.5, 4.75, -9.1, +90, vec3(0, 0, 1), White, 9.5, 0.30, 0.30); //roof1
	CreateCubeToFarm(3.5, 4.75, -9.1, +90, vec3(0, 0, 1), White, 9.5, 0.30, 0.30); //roof1
//---------------- w3
	CreateCubeToFarm(-2, 14.8, -9.1, +15, vec3(0, 0, 1), White, 4.75, 0.25f, 0.5); //roof1
	CreateCubeToFarm(+2, 14.8, -9.1, -15, vec3(0, 0, 1), White, 4.75, 0.25f, 0.5); //roof2
	CreateCubeToFarm(-6, 12.8, -9.1, +30, vec3(0, 0, 1), White, 5, 0.25f, 0.5); //roof3
	CreateCubeToFarm(+6, 12.8, -9.1, -30, vec3(0, 0, 1), White, 5, 0.25f, 0.5); //roof4

	CreateCubeToFarm(-8.3, 10.6, -9.1, +60, vec3(0, 0, 1), White, 3, 0.25f, 0.5); //roof5
	CreateCubeToFarm(+8.3, 10.6, -9.1, -60, vec3(0, 0, 1), White, 3, 0.25f, 0.5); //roof6
//---------------- w3



//---------------- w4
	CreateCubeToFarm(+8.5, 5, +9, 90, vec3(1, 0, 0), White, 0.5, 0.25f, 10.5); //wall4

	CreateCubeToFarm(+7.4, 5.5, +9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 13); //wall4
	CreateCubeToFarm(+6.3, 6, +9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 13.25); //wall4
	CreateCubeToFarm(+5.2, 6.35, +9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 13.75); //wall4
	CreateCubeToFarm(+4.1, 6.75, +9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 14.75); //wall4

	j = +3;
	w = 5;
	u = 12;
	for (int i = 0; i < 7; i++) {

		if (i < 3) {
			CreateCubeToFarm(j, u, +9, 90, vec3(1, 0, 0), Red, 1, 0.25f, w); //wall4
			w += 0.25;	u += 0.125;
		}

		if (i >= 3 && i < 6) {
			CreateCubeToFarm(j, u, +9, 90, vec3(1, 0, 0), Red, 1, 0.25f, w); //wall4
			w -= 0.25;	u -= 0.125;
		}
		else
			CreateCubeToFarm(j + 0.25, 12, +9, 90, vec3(1, 0, 0), Red, 0.6, 0.25f, 5); //wall4	
		j -= 1.1;
	}

	CreateCubeToFarm(-7.5, 5.5, +9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 13); //wall4
	CreateCubeToFarm(-6.4, 6, +9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 13.25); //wall4
	CreateCubeToFarm(-5.3, 6.30, +9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 13.75); //wall4
	CreateCubeToFarm(-4.2, 6.75, +9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 14.25); //wall3

	CreateCubeToFarm(-8.5, 5, +9, 90, vec3(1, 0, 0), White, 0.5, 0.25f, 10.5); //wall4

//---------------- w4

	CreateCubeToFarm(0, 9.6, +9, +0, vec3(0, 0, 1), White, 12, 0.40, 0.40); //roof1
	CreateCubeToFarm(-3.5, 4.75, +9.1, +90, vec3(0, 0, 1), White, 9.5, 0.30, 0.30); //roof1
	CreateCubeToFarm(3.5, 4.75, +9.1, +90, vec3(0, 0, 1), White, 9.5, 0.30, 0.30); //roof1
 //---------------- w4
	CreateCubeToFarm(-2, 14.8, +9.1, +15, vec3(0, 0, 1), White, 4.75, 0.25f, 0.5); //roof1
	CreateCubeToFarm(+2, 14.8, +9.1, -15, vec3(0, 0, 1), White, 4.75, 0.25f, 0.5); //roof2
	CreateCubeToFarm(-6, 12.8, +9.1, +30, vec3(0, 0, 1), White, 5, 0.25f, 0.5); //roof3
	CreateCubeToFarm(+6, 12.8, +9.1, -30, vec3(0, 0, 1), White, 5, 0.25f, 0.5); //roof4

	CreateCubeToFarm(-8.3, 10.6, +9.1, +60, vec3(0, 0, 1), White, 3, 0.25f, 0.5); //roof5
	CreateCubeToFarm(+8.3, 10.6, +9.1, -60, vec3(0, 0, 1), White, 3, 0.25f, 0.5); //roof6
 //---------------- w4





}
//================
void ModuleSceneIntro::CreateFence(float distance, float tall, vec3 Position, vec3 rotationvec, float angle, int magicX, int magicZ)
{
	Cube* trans1 = new Cube(distance, 0.3f, 0.2f);

	trans1->SetPos(Position.x, (Position.y + 0.1f*tall), Position.z);
	trans1->SetRotation(angle, rotationvec);
	trans1->color = Brown2;
	transversales.add(trans1);

	Cube* trans2 = new Cube(distance, 0.3f, 0.2f);

	trans2->SetRotation(angle, rotationvec);	
	trans2->SetPos(Position.x, Position.y + 0.5f*tall, Position.z);
	trans2->color = Brown2;
	transversales.add(trans2);

	int Postes_num = 0, i = 0;
	float Postes_distance = 0;

	Postes_num = distance / 3;
	Postes_distance = (float)distance / (float)Postes_num;
	
	do {
		Cylinder* Poste = new Cylinder(0.15f, 1.2f*tall);
		Poste->SetRotation(90, vec3(0, 0, 1));
		//Poste->SetRotation(angle, rotationvec);
		Poste->SetPos(Position.x + (Postes_distance*i - distance/2)*magicX, Position.y, Position.z + (Postes_distance*i - distance / 2)*magicZ);
		Poste->color = Brown;

		postes.add(Poste);
		i++;
	} while (Postes_num >= i);
}

void ModuleSceneIntro::CreateFences()
{
	CreateFence(36, 2, vec3(54, 1, 23), vec3(0, 1, 0), 0, 1, 0);
	CreateFence(36, 2, vec3(54, 1, 78), vec3(0, 1, 0), 0, 1, 0);

	CreateFence(42, 2, vec3(36, 1, 44), vec3(0, 1, 0), 90, 0, 1);
	CreateFence(42, 2, vec3(72, 1, 57), vec3(0, 1, 0), 90, 0, 1);


	CreateFence(6, 2, vec3(36, 1, 75), vec3(0, 1, 0), 90, 0, 1);
	CreateFence(6, 2, vec3(72, 1, 26), vec3(0, 1, 0), 90, 0, 1);

	//RECT2
	for (float j = 94.5; j < 112; j++) {
		CreateFence(6, 2, vec3(j, 1, 72.5), vec3(0, 1,0), 0, 1, 0);
		j += 2;
	}
	//INTERIOR
	for (float j = 87.5; j < 112; j++) {
		CreateFence(6, 2, vec3(j, 1, 83), vec3(0, 1, 0), 0, 1, 0);
		j += 2;
	}
	//RECT3
	for (float j = 134; j < 156; j++) {
		CreateFence(6, 2, vec3(j, 1, 72.5), vec3(0, 1, 0), 0, 1, 0);		
		j += 2;
	}
	//INTERIOR
	for (float j = 133; j < 150; j++) {
		CreateFence(6, 2, vec3(j, 1, 83), vec3(0, 1, 0), 0, 1, 0);
		j += 2;
	}
}
//================
void ModuleSceneIntro::CreateBale(const float x, const float y, const float z, const float angle, const vec3& rotationAxis)
{
	Cube* heno = new Cube(1, 1, 2.2f);

	heno->SetPos(x, y, z);
	heno->SetRotation(angle, rotationAxis);
	heno->color = Yellow;	
	bales.add(heno);

	bales_body.add(App->physics->AddBody(*heno, 1500));


	
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

	//6
	for (float i = 0, j = 15.5; i < 2; i++) {
		CreateBale(j, 0.5f, 27, 90, vec3{ 0,1,0 });
		j -= 3;
	}

	//7
	for (float i = 0, j = 19.5; i < 4; i++) {
		CreateBale(j, 0.5f, 19, 90, vec3{ 0,1,0 });
		j -= 3;
	}
	CreateBale(7.7, 0.5f, 19.5, 120, vec3{ 0,1,0 });
	CreateBale(5.5, 0.5f, 21, 135, vec3{ 0,1,0 });

	

	//7-8

	CreateBale(22.5, 0.5f, 19.5, 60, vec3{ 0,1,0 });
	CreateBale(24.5, 0.5f, 21.5, 30, vec3{ 0,1,0 });

	//8
	for (int i = 0, j = 45; i <8; i++) {
		CreateBale(25.5, 0.5f, j, 0, vec3{ 0,1,0 });
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
	
	CreateBale(23.5, 0.5f, 50.5, 140, vec3{ 0,1,0 });
	CreateBale(25, 0.5f, 48, 160, vec3{ 0,1,0 });

	for (float i = 0, j = 21.5, w = 52.5; i < 4; i++) {
		CreateBale(j, 0.5f, w, 135, vec3{ 0,1,0 });
		j -= 2;
		w += 2;
	}
	CreateBale(13.0, 0.5f, 60.5, 120, vec3{ 0,1,0 });
	CreateBale(10.5, 0.5f, 61.5, 100, vec3{ 0,1,0 });

	//14
	for (float i = 0, j = 7.5; i < 9; i++) {
		CreateBale(j, 0.5f, 73, 90, vec3{ 0,1,0 });
		j += 3;
	}

	//11
	CreateBale(10, 0.5f, 52.5, 100, vec3{ 0,1,0 });
	CreateBale(12.5, 0.5f, 51.5, 120, vec3{ 0,1,0 });
	CreateBale(14.5, 0.5f, 49.5, 140, vec3{ 0,1,0 });
	CreateBale(16, 0.5f, 47, 160, vec3{ 0,1,0 });

	//marc
	//12

	//13 L
	//CURVE1
	CreateBale(75, 0.5f, 29, 90, vec3{ 0,1,0 });
	CreateBale(78, 0.5f, 29.5f,70, vec3{ 0,1,0 });
	CreateBale(81, 0.5f, 31, 50, vec3{ 0,1,0 });
	CreateBale(82.5, 0.5f, 33.5, 20, vec3{ 0,1,0 });
	CreateBale(83, 0.5f, 36.5, 0, vec3{ 0,1,0 });
	//RECT1
	for (float  j = 39.5; j < 68; j++) {
		CreateBale(83, 0.5f, j, 0, vec3{ 0,1,0 });
		j += 2;
	}
	//CURV2
	CreateBale(84, 0.5f, 69.5, 40, vec3{ 0,1,0 });
	CreateBale(86.5, 0.5f, 71.5, 60, vec3{ 0,1,0 });
	CreateBale(89.5, 0.5f, 72.5, 90, vec3{ 0,1,0 });
	//INTERIOR
	CreateBale(74, 0.5f, 79.5, 50, vec3{ 0,1,0 });
	CreateBale(76.5, 0.5f, 81.5, 70, vec3{ 0,1,0 });
	CreateBale(79.5, 0.5f, 82.5, 80, vec3{ 0,1,0 });
	CreateBale(82.5, 0.5f, 83, 90, vec3{ 0,1,0 });

	
	//BRIDGE

	

	//CURV3
	//CreateBale(158, 0.5f, 73, 70, vec3{ 0,1,0 });
	CreateBale(160.5, 0.5f, 74.5, 50, vec3{ 0,1,0 });
	CreateBale(162, 0.5f, 77, 20, vec3{ 0,1,0 });
	CreateBale(163, 0.5f, 80, 15, vec3{ 0,1,0 });
	CreateBale(163.5, 0.5f, 83, 10, vec3{ 0,1,0 });
	CreateBale(164, 0.5f, 86, 5, vec3{ 0,1,0 });
	CreateBale(164, 0.5f, 89, 0, vec3{ 0,1,0 });
	//INTERIOR
	CreateBale(150, 0.5f, 84, 20, vec3{ 0,1,0 });
	CreateBale(150, 0.5f, 87, -10, vec3{ 0,1,0 });
	CreateBale(148, 0.5f, 89, -70, vec3{ 0,1,0 });
	CreateBale(145, 0.5f, 89, -100, vec3{ 0,1,0 });
	CreateBale(142, 0.5f, 88, -120, vec3{ 0,1,0 });
	CreateBale(139, 0.5f, 86, -120, vec3{ 0,1,0 });
	

	//CURV4
	CreateBale(164, 0.5f, 92, -5, vec3{ 0,1,0 });
	CreateBale(163.5, 0.5f, 95, -20, vec3{ 0,1,0 });
	CreateBale(162, 0.5f, 97.5, -40, vec3{ 0,1,0 });
	CreateBale(160, 0.5f, 99.5, -50, vec3{ 0,1,0 });
	CreateBale(157.5, 0.5f, 101, -70, vec3{ 0,1,0 });
	CreateBale(154.5, 0.5f, 101.5, -90, vec3{ 0,1,0 });
	//SMALLRECT4
	for (float j = 151.5; j > 134.5; j--) {
		CreateBale(j, 0.5f, 101.5, -90, vec3{ 0,1,0 });
		j -= 2;
	}
	//CURV5
	CreateBale(132.5, 0.5f, 100.5, -110, vec3{ 0,1,0 });
	CreateBale(130, 0.5f, 98.5, -130, vec3{ 0,1,0 });
	CreateBale(128, 0.5f, 96, -140, vec3{ 0,1,0 });
	
	
	

}
//================
void ModuleSceneIntro::CreateRock(const float x, const float y, const float z, const float angle, const vec3& rotationAxis)
{
	Cylinder* material = new Cylinder(1,1);

	material->SetPos(x, y, z);
	material->SetRotation(angle, rotationAxis);
	material->color = grey;	
	rocks.add(material);

	rocks_body.add(App->physics->AddBody(*material, 10000));
}

void ModuleSceneIntro::CreateRocks() {

	CreateRock(126, 0.5f, 93.5, 90, vec3{ 0,0,1 });
	CreateRock(124, 0.5f, 90.5, 90, vec3{ 0,0,1 });
	CreateRock(122.5, 0.5f, 88.5,90, vec3{ 0,0,1 });
	CreateRock(121.5, 0.5f, 86,90, vec3{ 0,0,1 });
	CreateRock(120.5, 0.5f, 83.5, 90, vec3{ 0,0,1 });
	CreateRock(119.5, 0.5f, 81, 90, vec3{ 0,0,1 });
	CreateRock(119, 0.5f, 78.5, 90, vec3{ 0,0,1 });
	CreateRock(118.5, 0.5f, 76, 90, vec3{ 0,0,1 });

	//RECT5
	for (float j = 73.5; j > 50; j--) {
		CreateRock(118.5, 0.5f, j, 90, vec3{ 0,0,1 });
		j -= 1.5;
	}
	//OBSTACLES
	CreateRock(121, 0.5f, 70, 90, vec3{ 0,0,1 });
	CreateRock(125, 0.5f,60 , 90, vec3{ 0,0,1 });
	
	//CURVE
	CreateRock(119, 0.5f, 48.5, 90, vec3{ 0,0,1 });
	CreateRock(119.5, 0.5f, 46, 90, vec3{ 0,0,1 });
	CreateRock(120.5, 0.5f, 44, 90, vec3{ 0,0,1 });
	CreateRock(121.5, 0.5f, 42, 90, vec3{ 0,0,1 });
	CreateRock(122.5, 0.5f, 40, 90, vec3{ 0,0,1 });
	CreateRock(123.5, 0.5f, 38, 90, vec3{ 0,0,1 });
	CreateRock(123.5, 0.5f, 36, 90, vec3{ 0,0,1 });
	//RECT2
	for (float j = 34; j >50; j--) {
	CreateRock(130, 0.5f, j, 90, vec3{ 0,0,1 });
	j -= 1.5;
}

	//RECTA INTERIOR
	for (float j = 34; j >20; j--) {
		CreateRock(123, 0.5f, j, 90, vec3{ 0,0,1 });
		j -= 1.5;
	}
	//CURVE INTERN
	CreateRock(130, 0.5f, 50, 90, vec3{ 0,0,1 });
	CreateRock(131, 0.5f, 47.5, 90, vec3{ 0,0,1 });
	CreateRock(132, 0.5f, 45.5, 90, vec3{ 0,0,1 });
	CreateRock(133, 0.5f, 43.5, 90, vec3{ 0,0,1 });
	CreateRock(134, 0.5f, 41, 90, vec3{ 0,0,1 });
	CreateRock(135, 0.5f, 39, 90, vec3{ 0,0,1 });
	CreateRock(135, 0.5f, 36, 90, vec3{ 0,0,1 });
	//RECT2 INT
	for (float j = 34; j >20; j--) {
		CreateRock(135, 0.5f, j, 90, vec3{ 0,0,1 });
		j -= 1.5;
	}


}
//================
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

	trees_trunk_body.add(App->physics->AddBody((*ttrunk),0));
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

	CreateTree(25, 0, 75, 15, 2);

	CreateTree(50, 0, 80, 15, 2);
	CreateTree(56, 0, 80, 15, 2);
}
//================
void ModuleSceneIntro::CreatePlane(const float x, const float y, const float z, const float width, const float height, Color colorr,const float angle, const vec3& rotationAxis)
{
	Plane* plane = new Plane(x,y,z,width,height);

	plane->SetRotation(angle, rotationAxis);
	plane->SetPos(x,y,z);
	plane->color = colorr;
	Planos.add(plane);
}

void ModuleSceneIntro::CreatePlanes()
{
	//CreatePlane(0, 0, 0,100,50,Green_Ground,0, vec3(0, 1, 0));

	//ground
	//base
	CreatePlane(-200, 0, -200, 400, 400, Green_Ground,0, vec3(0, 1, 0));
	//race
	//tramo1
	CreatePlane(-3.5, 0.1, 0, 7, 45, Brown_way, 0, vec3(0, 1, 0));
	CreatePlane(3.5, 0.1, 20, 8, 25, Brown_way, 0, vec3(0, 1, 0));

	CreatePlane(16.5, 0.1, 20, 9, 30, Brown_way, 0, vec3(0, 1, 0));

	CreatePlane(-1, 0.1, 45, 10, 23, Brown_way, 0, vec3(0, 1, 0));
	CreatePlane(7, 0.1, 53, 8, 10, Brown_way, 150, vec3(0, 1, 0));//r
	CreatePlane(-4, 0.1, 45, 8, 10, Brown_way, 30, vec3(0, 1, 0));//r
	CreatePlane(-3, 0.1, 63, 8, 10, Brown_way, 30, vec3(0, 1, 0));//r

	CreatePlane(2, 0.1, 56.5, 8, 10, Brown_way, 45, vec3(0, 1, 0));//r
	CreatePlane(12, 0.1, 63, 8, 20, Brown_way, 135, vec3(0, 1, 0));


	CreatePlane(3.5, 0.1, 19.5, 22, 9, Brown_way, 0, vec3(0, 1, 0));

	CreatePlane(3, 0.1, 64, 34, 9, Brown_way, 0, vec3(0, 1, 0));
	//tramo2 

	//sky
	CreatePlane(200, 0, -200, 400, 400, Blue, 90, vec3(0, 0,1));
	CreatePlane(-200, +400, -200, 400, 400, Blue, 270, vec3(0, 0, 1));
	CreatePlane(-200, 0, 200, 400, 400, Blue, 270, vec3(1, 0, 0));
	CreatePlane(-200, +400, -200, 400, 400, Blue, 90, vec3(1, 0, 0));


}
//================
void ModuleSceneIntro::CreateCow(const float x, const float y, const float z, const float angle, const vec3 RotationAxis) {

	
	// -----------------------------------------------------------

	Cube* cube = new Cube(2, 1.3, 1);

	int nrandom;
	nrandom = rand() % 6;

	if (nrandom == 1 || nrandom == 0 || nrandom == 5 || nrandom == 4)
		cube->color = White;
	else if (nrandom == 2)
		cube->color = Brown;
	else
		cube->color = Black;

	cube->SetPos(x, y + 1.1, z);
	cube->SetRotation(angle, { 0,1,0 });
	Cow_corps.add(cube);

	PhysBody3D* bod = App->physics->AddBody((*cube),200);
	CowCorps_body.add(bod);

	// -----------------------------------------------------------

	Cube* head = new Cube(1, 1, 1);

	head->color = White;

	head->SetPos(x+1, y+1.6, z);
	head->SetRotation(angle, { 0,1,0 });
	Cow_corps.add(head);

	PhysBody3D* headb = App->physics->AddBody((*cube), 1);
	CowCorps_body.add(headb);

	App->physics->AddConstraintP2P(*bod, *headb, { 1.0f,0,0.25f }, { -0.45f,-0.5f,0 });

	// -----------------------------------------------------------


	Cylinder* leg1 = new Cylinder(0.25,0.75);

	int nrandom2;
	nrandom2 = rand() % 5;

	if (nrandom2 == 1 || nrandom2 == 0 || nrandom2 == 5)
		leg1->color = White;
	else if (nrandom2 == 2)
		leg1->color = Brown;
	else
		leg1->color = Black;

	leg1->SetPos(x - 0.35f, y + 0.25f, z-0.35);
	leg1->SetRotation(90, { 0,0,1 });

	Cow_legs.add(leg1);
	PhysBody3D* legg1 = App->physics->AddBody(*leg1, 200);
	CowLegs_body.add(legg1);
	App->physics->AddConstraintP2P(*bod,*legg1, {-0.35f,-0.5f,-0.35f }, { +0.7f,0,0 });

	// -----------------------------------------------------------

	Cylinder* leg2 = new Cylinder(0.25, 0.75);

	int nrandom3;
	nrandom3 = rand() % 5;

	if (nrandom3 == 1 || nrandom3 == 0 || nrandom3 == 5)
		leg2->color = White;
	else if (nrandom3 == 2)
		leg2->color = Brown;
	else
		leg2->color = Black;

	leg2->SetPos(x+0.35f, y + 0.25f, z-0.35f);
	leg2->SetRotation(90, { 0,0,1 });

	Cow_legs.add(leg2);
	PhysBody3D* legg2 = App->physics->AddBody(*leg2, 200);


	CowLegs_body.add(legg2);
	App->physics->AddConstraintP2P(*bod,*legg2, {+0.35f,-0.5f,-0.35f }, { 0.7f,0,0 });

	// -----------------------------------------------------------

	Cylinder* leg3 = new Cylinder(0.25, 0.75);

	nrandom = rand() % 5;

	if (nrandom == 1 || nrandom == 0 || nrandom == 5)
		leg3->color = White;
	else if (nrandom == 2)
		leg3->color = Brown;
	else
		leg3->color = Black;

	leg3->SetPos(x + 0.35f, y + 0.25f, z+0.35f);
	leg3->SetRotation(90, { 0,0,1 });
	Cow_legs.add(leg3);
	PhysBody3D* legg3 = App->physics->AddBody(*leg3, 200);
	CowLegs_body.add(legg3);

	App->physics->AddConstraintP2P(*bod,*legg3, { +0.35f,-0.5f,+0.35f }, { 0.7f,0,0 });

	// -----------------------------------------------------------

	Cylinder* leg4 = new Cylinder(0.25, 0.75);

	nrandom = rand() % 4;

	if (nrandom == 1 || nrandom == 0)
		leg4->color = White;
	else if (nrandom == 2)
		leg4->color = Brown;
	else
		leg4->color = Black;
	 
	leg4->SetPos(x-0.35f, y + 0.25f, z + 0.35f);
	leg4->SetRotation(90, { 0,0,1 });
	PhysBody3D* legg4 = App->physics->AddBody(*leg4, 200);
	Cow_legs.add(leg4);
	CowLegs_body.add(legg4);
	App->physics->AddConstraintP2P(*bod,*legg4, { -0.35f,-0.5f,+0.35f }, { 0.7f,0,0 });

}

void ModuleSceneIntro::CreateCows()
{
	

	CreateCow(60, 0, 60, 0, { 0,0,1 });
	CreateCow(64, 0, 68, 60, { 0,0,1 });

	CreateCow(40, 0, 30, 112, { 0,0,1 });
	CreateCow(40, 0, 60, 45, { 0,0,1 });

	CreateCow(60, 0, 50, 170, { 0,0,1 });
	CreateCow(50, 0, 49, 50, { 0,0,1 });
}

//================

void ModuleSceneIntro::CreateInvisibleWall(const float x, const float y, const float z, const vec3 box , const float angle, const vec3 RotationAxis) {

	noCube* inv = new noCube(box.x, box.y, box.z);
	inv->SetPos(x, y, z);
	inv->SetRotation(angle, RotationAxis);


	invisibles.add(inv);
	invisible_bodies.add(App->physics->AddBody(*inv));
	
}

void ModuleSceneIntro::CreateInvisibleWalls()
{
	//farm
	CreateInvisibleWall(6,0, 9, vec3{5,15,0.5f} , 0, vec3{ 1,0,0 });
	CreateInvisibleWall(-6, 0, 9, vec3{ 5,15,0.5f }, 0, vec3{ 1,0,0 });
	CreateInvisibleWall(6, 0, -9, vec3{ 5,15,0.5f }, 0, vec3{ 1,0,0 });
	CreateInvisibleWall(-6, 0,-9, vec3{ 5,15,0.5f }, 0, vec3{ 1,0,0 });

	CreateInvisibleWall(-8.5, 0, 0, vec3{ 18,15,0.5f }, 90, vec3{ 0,1,0 });
	CreateInvisibleWall(8.5, 0, 0, vec3{ 18,15,0.5f }, 90, vec3{ 0,1,0 });

}
