#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "Time.h"
#include "ModulePhysics3D.h"
#include "ModulePlayer.h"

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
	App->player->restart = true;
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
	//--- Creators
	CreateBales();
	CreateTrees();
	CreateFences();
	CreatePlanes();
	CreateFarm();
	CreateCows();
	CreateInvisibleWalls();
	CreateGreenBales();
	CreateBulding(60, 0, -69);
	CreateSensors();
	CreateExtras();
	CreateBridge(115, 0 ,77.5f);


		App->camera->Move(vec3(180, 30, 120));
		App->camera->LookAt(vec3(0, 0, -60));
	

		//set checkpoints 
		for (uint i = 0; i <7; i++) {
			checkpoints[i] = false;
		}
	//bales
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	if (sensors.getFirst() != nullptr) {
		for (int i = 0; i < sensors.count(); i++)
			delete sensors[i];
	}
	if (obstacles.getFirst() != nullptr) {
		for (int i = 0; i < obstacles.count(); i++)
			delete obstacles[i];
	}
	if (greenbales.getFirst() != nullptr) {
		for (int i = 0; i < greenbales.count(); i++)
			delete greenbales[i];
	}
	if (bales.getFirst() != nullptr) {
		for (int i = 0; i < bales.count(); i++)
			delete bales[i];
	}

	if (trees_top.getFirst() != nullptr) {
		for (int i = 0; i < trees_top.count(); i++)
			delete trees_top[i];
	}

	if (trees_trunk.getFirst() != nullptr) {
		for (int i = 0; i < trees_trunk.count(); i++)
			delete trees_trunk[i];
	}

	if (postes.getFirst() != nullptr) {
		for (int i = 0; i < postes.count(); i++)
			delete postes[i];
	}

	if (transversales.getFirst() != nullptr) {
		for (int i = 0; i < transversales.count(); i++)
			delete transversales[i];
	}

	if (Buldings.getFirst() != nullptr) {
		for (int i = 0; i < Buldings.count(); i++)
			delete Buldings[i];
	}

	if (Cow_corps.getFirst() != nullptr) {
		for (int i = 0; i < Cow_corps.count(); i++)
			delete Cow_corps[i];
	}

	if (Cow_legs.getFirst() != nullptr) {
		for (int i = 0; i < Cow_legs.count(); i++)
			delete Cow_legs[i];
	}



	if (BuldingsCy.getFirst() != nullptr) {
		for (int i = 0; i < BuldingsCy.count(); i++)
			delete BuldingsCy[i];
	}
	if (Doors.getFirst() != nullptr) {
		for (int i = 0; i < Doors.count(); i++)
			delete Doors[i];
	}

	delete silo1;
	delete silo2;

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
	if (invisibles.getFirst() != nullptr) {

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
	}
	//--------------------

	silo2->Render();

	silo1->Render();



	//-----------------------------
	//--- buldings cylinder
	//-----------------------------
	if (BuldingsCy.getFirst() != nullptr) {

		p2List_item<Cylinder*>* iteratorBuldingsCy;
		iteratorBuldingsCy = BuldingsCy.getFirst();


		while (iteratorBuldingsCy != nullptr) {

			iteratorBuldingsCy->data->Render();
			iteratorBuldingsCy = iteratorBuldingsCy->next;
		}
	}

	if (Doors.getFirst() != nullptr) {

		p2List_item<Cube*>* iteratorDoors;
		p2List_item<PhysBody3D*>* iteratorDoor_body;

		iteratorDoors = Doors.getFirst();
		iteratorDoor_body = Doors_body.getFirst();

		while (iteratorDoors != nullptr) {

			iteratorDoor_body->data->GetTransform(&(iteratorDoors->data->transform));
			iteratorDoors->data->Render();

			iteratorDoors = iteratorDoors->next;
			iteratorDoor_body = iteratorDoor_body->next;
		}
	}
	if (kicked == true) {
		invDoor_body->SetPos(60 + 0.25f, 4 + 0, -69);
		redFence1->Render();
		redFence2->Render();
	}

	//-----------------------------
	//--- buldings
	//-----------------------------
	if (Buldings.getFirst() != nullptr) {

		p2List_item<Cube*>* iteratorBuldings;
		iteratorBuldings = Buldings.getFirst();


		while (iteratorBuldings != nullptr) {

			iteratorBuldings->data->Render();
			iteratorBuldings = iteratorBuldings->next;
		}
	}

	//-----------------------------
	//--- Fances
	//-----------------------------
	if (transversales.getFirst() != nullptr) {


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
	}
	//-----------------------------
	//--- render trees
	//-----------------------------
	if (trees_top.getFirst() != nullptr|| trees_trunk.getFirst() != nullptr) {

		p2List_item<TreeTop*>* iteratorTop;
		p2List_item<Cylinder*>* iteratorTrunk;
		p2List_item<PhysBody3D*>*iteratorTrunk_body;
		
		iteratorTop = trees_top.getFirst();
		iteratorTrunk = trees_trunk.getFirst();
		iteratorTrunk_body = trees_trunk_body.getFirst();

		while (iteratorTop != nullptr) {

			iteratorTrunk_body->data->GetTransform(&(iteratorTrunk->data->transform));

			iteratorTop->data->Render();
			iteratorTrunk->data->Render();

			iteratorTop = iteratorTop->next;
			iteratorTrunk = iteratorTrunk->next;
			iteratorTrunk_body = iteratorTrunk_body->next;
		}
	}
	//-----------------------------
	//--- render bales
	//-----------------------------
	if (bales.getFirst() != nullptr) {

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
	}

	if (greenbales.getFirst() != nullptr) {
		p2List_item<Cube*>* iteratorGreenBale;
		p2List_item<PhysBody3D*>* iteratorGreenBale_body;

		iteratorGreenBale = greenbales.getFirst();
		iteratorGreenBale_body = greenbales_body.getFirst();

		while (iteratorGreenBale != nullptr) {

			iteratorGreenBale_body->data->GetTransform(&(iteratorGreenBale->data->transform));
			iteratorGreenBale->data->Render();

			iteratorGreenBale = iteratorGreenBale->next;
			iteratorGreenBale_body = iteratorGreenBale_body->next;
		}
	}
	//-----------------------------
	//--- render ROCKS
	//-----------------------------

	if (sensors.getFirst() != nullptr) {

		p2List_item<Cylinder*>* iteratorSensor;
		p2List_item<PhysBody3D*>* iteratorSensor_body;

		iteratorSensor = sensors.getFirst();
		iteratorSensor_body = sensors_body.getFirst();

		while (iteratorSensor != nullptr) {

			iteratorSensor_body->data->GetTransform(&(iteratorSensor->data->transform));
			iteratorSensor->data->Render();

			iteratorSensor = iteratorSensor->next;
			iteratorSensor_body = iteratorSensor_body->next;
		}
	}
	
	//-----------------------------
	//--- render Cows
	//-----------------------------
	if (Cow_legs.getFirst() != nullptr)
	{
		p2List_item<Cylinder*>* iteratorlegs;
		p2List_item<PhysBody3D*>* iteratorcow_Legs_body;
		iteratorlegs = Cow_legs.getFirst();
		iteratorcow_Legs_body = CowLegs_body.getFirst();

		while (iteratorlegs != nullptr) {

			iteratorcow_Legs_body->data->GetTransform(&(iteratorlegs->data->transform));
			iteratorlegs->data->Render();

			iteratorcow_Legs_body = iteratorcow_Legs_body->next;
			iteratorlegs = iteratorlegs->next;
		}
	}
	if (Cow_corps.getFirst() != nullptr)
	{

		p2List_item<Cube*>* iteratorbody;
		p2List_item<PhysBody3D*>* iteratorcow_Corps_body;
		iteratorbody = Cow_corps.getFirst();
		iteratorcow_Corps_body = CowCorps_body.getFirst();

		while (iteratorbody != nullptr) {
			iteratorcow_Corps_body->data->GetTransform(&(iteratorbody->data->transform));
			iteratorbody->data->Render();

			iteratorcow_Corps_body = iteratorcow_Corps_body->next;
			iteratorbody = iteratorbody->next;
		}
	
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

	for (uint i = 0; i <= 9; i++) {
		if(body1==sensors_body[i])
		if (kicked == false) {
			kicked = true;
		}
	}	

	for (uint i = 0; i <= 7; i++) {
		if (body1 == sensors_bodycube[i]) {
			isCheckpoint == true;
			CheckBehaviour(i);							
		}
	}	
	
	LOG("Hit!");
}

void ModuleSceneIntro::CheckBehaviour(int i) {
	if (i == 0) {
		if (checkpoints[i] == false) {
			App->player->restart = false;
			playerTime.Start();
			checkpoints[i] = true;
		}
	}
	else if (i == 1) {
		if (checkpoints[i - 1] == true) {
			checkpoints[i] = true;
		}
	}
	else if (i == 2) {
		if (checkpoints[i - 1] == true) {
			checkpoints[i] = true;
		}
	}
	else if (i == 3) {
		if (checkpoints[i - 1] == true) {
			checkpoints[i] = true;
		}
	}
	else if (i == 4) {
		if (checkpoints[i - 1] == true) {
			checkpoints[i] = true;
		}
	}
	else if (i == 5) {
		if (checkpoints[i - 1] == true) {
			checkpoints[i] = true;
		}
	}
	else if (i == 6) {
		if (checkpoints[i - 1] == true) {
			checkpoints[i] = true;
		}
	}
	else if (i == 7) {
		if (checkpoints[i - 1] == true) {
			if (App->player->lessScore > App->player->thistime) {
				App->player->lessScore = App->player->thistime;
			}
			
			App->player->RestartAll();
		}
	}
	
}
//=========================================================================================================
//creators  =========================================================================================================
//=========================================================================================================
void ModuleSceneIntro::CreateSensor(const float x, const float y, const float z, const float angle, const vec3& rotationAxis, const int i, const float sizeX, const float sizeY, const int pos) {
	if (i == 0) {
		Cylinder* sensor = new Cylinder(1, 1);
		sensor->SetPos(x, y, z);
		sensor->SetRotation(angle, rotationAxis);
		sensors.add(sensor);
		sensors_body.add(App->physics->AddBody(*sensor, 0));
		sensors_body.getLast()->data->SetAsSensor(true);
		sensors_body.getLast()->data->collision_listeners.add(this);
	}
	else if (i == 1) {
		Cube* sensor = new Cube(sizeX, sizeY, 1);
		sensor->SetPos(x, y, z);
		sensor->SetRotation(angle, rotationAxis);
		sensorscube.add(sensor);
		sensors_bodycube[pos] = (App->physics->AddBody(*sensor, 0.0f));
		sensors_bodycube[pos]->SetAsSensor(true);
		sensors_bodycube[pos]->collision_listeners.add(this);
	}


}
void ModuleSceneIntro::CreateSensors() {
	//Obstacles
	CreateSensor(150, 0.0f, 50, 90, vec3{ 0,0,1 }, 0);
	CreateSensor(158.5, 0.0f, 40.8, 90, vec3{ 0,0,1 }, 0);
	CreateSensor(150.4, 0.0f, 35.4, 90, vec3{ 0,0,1 }, 0);
	CreateSensor(129.5, 0.0f, 25, 90, vec3{ 0,0,1 }, 0);
	CreateSensor(127.9, 0.0f, 19, 90, vec3{ 0,0,1 }, 0);
	CreateSensor(110, 0.0f, 10.4, 90, vec3{ 0,0,1 }, 0);
	CreateSensor(105, 0.0f, 8.4, 90, vec3{ 0,0,1 }, 0);
	CreateSensor(111.5, 0.0f, 3.4, 90, vec3{ 0,0,1 }, 0);
	CreateSensor(106.5, 0.0f, -5, 90, vec3{ 0,0,1 }, 0);
	CreateSensor(125.5, 0.0f, -22, 90, vec3{ 0,0,1 }, 0);


	//
	CreateSensor(0, 0.9f, 2, 0, vec3{ 0,0,1 }, 1, 7, 1, 0);
	CreateSensor(7.5, 0.9f, 40, 0, vec3{ 0,1,0 }, 1, 6, 1, 1);
	CreateSensor(10, 0.9f, 67.5, 90, vec3{ 0,1,0 }, 1, 7.2, 1, 2);
	CreateSensor(72, 0.9f, 32.5, 90, vec3{ 0,1,0 }, 1, 6.5, 1, 3);
	CreateSensor(140, 0.9f, 77.8, 90, vec3{ 0,1,0 }, 1, 9, 9, 4);
	CreateSensor(153.6, 0.9f, -40, 0, vec3{ 0,1,0 }, 1, 9, 1, 5);
	CreateSensor(0, 0.9f, -56, 0, vec3{ 0,1,0 }, 1, 5, 1, 6);
	CreateSensor(0, 0.9f, -10, 0, vec3{ 0,1,0 }, 1, 5, 1, 7);

}
void ModuleSceneIntro::CreateCubeToBuldings(const float x, const float y, const float z, const float angle, const vec3 & rotationAxis, Color colorr, const float w, const float h, const float l)
{
	Cube* cube = new Cube(w, h, l);
	cube->SetPos(x, y, z);
	cube->SetRotation(angle, rotationAxis);
	cube->color = colorr;
	Buldings.add(cube);
}
void ModuleSceneIntro::CreateCylinderToBuldings(const float x, const float y, const float z, const float angle, const vec3 & rotationAxis, Color colorr, const float radio, const float h)
{
	Cylinder* cylinder = new Cylinder(radio, h);
	cylinder->SetPos(x, y, z);
	cylinder->SetRotation(angle, rotationAxis);
	cylinder->color = colorr;
	BuldingsCy.add(cylinder);
}
void ModuleSceneIntro::CreateBulding(float x, float y, float z)
{

#define BUIDINGWIDTH 20
#define BUIDINGLONG 46
#define BULDINGHEIGHT 18



	CreateCubeToBuldings(x, (10 * 0.5f) + y + 8, z + 0, ORTHOGONAL, vec3(PUTVERTICALxy), White, 10, 0.25f, 46); //wall1	


	CreateCubeToBuldings((x - (BUIDINGWIDTH *0.5f)), ((BULDINGHEIGHT *0.5f) + y + 0), (z + 23), ORTHOGONAL, vec3(PUTVERTICALzy), White, BUIDINGWIDTH, 0.25f, BULDINGHEIGHT); //wall2
	CreateCubeToBuldings((x - (BUIDINGWIDTH *0.5f)), ((BULDINGHEIGHT *0.5f) + y + 0), (z - 23), ORTHOGONAL, vec3(PUTVERTICALzy), White, BUIDINGWIDTH, 0.25f, BULDINGHEIGHT); //wall3

	CreateCubeToBuldings((x + 4), (y + 9.5), (z), 0, vec3(PUTVERTICALxy), White, 8, 0.5f, 16); //wall5

	CreateCubeToBuldings(x - BUIDINGWIDTH, (6 * 0.5f) + y, z + 3 + (21 * 0.5f), ORTHOGONAL, vec3(PUTVERTICALxy), White, 6, 0.25f, 19); //wall4
	CreateCubeToBuldings(x - BUIDINGWIDTH, (6 * 0.5f) + y, z - 3 - (21 * 0.5f), ORTHOGONAL, vec3(PUTVERTICALxy), White, 6, 0.25f, 19); //wall4	
	CreateCubeToBuldings(x - BUIDINGWIDTH, (12 * 0.5f) + y + 6, z + 0, ORTHOGONAL, vec3(PUTVERTICALxy), White, 12, 0.25f, BUIDINGLONG); //wall4	


	CreateCubeToBuldings(x - BUIDINGWIDTH, (BULDINGHEIGHT *0.5f) + y, z + BUIDINGLONG *0.5f, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 1, 1, BULDINGHEIGHT); //	retoc
	CreateCubeToBuldings(x - BUIDINGWIDTH, (BULDINGHEIGHT *0.5f) + y, z - BUIDINGLONG *0.5f, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 1, 1, BULDINGHEIGHT); //retoc
	CreateCubeToBuldings(x, (BULDINGHEIGHT *0.5f) + y, z + BUIDINGLONG *0.5f, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 1, 1, BULDINGHEIGHT); //retoc
	CreateCubeToBuldings(x, (BULDINGHEIGHT *0.5f) + y, z - BUIDINGLONG *0.5f, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 1, 1, BULDINGHEIGHT); //retoc


	CreateCubeToBuldings(x, (BULDINGHEIGHT*0.5f *0.5f) + y - 1, z + 5, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.5f, 0.5f, BULDINGHEIGHT*0.5f); //retoc  gates
	CreateCubeToBuldings(x, (BULDINGHEIGHT*0.5f *0.5f) + y - 1, z - 5, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.5f, 0.5f, BULDINGHEIGHT*0.5f); //retoc  gates
	CreateCubeToBuldings(x, (BULDINGHEIGHT*0.5f) + y - 1, z, 0, vec3(PUTVERTICALzy), Brown6, 0.5f, 0.5f, 12); //retoc  gates

	CreateCylinderToBuldings(x + 6, y + (BULDINGHEIGHT *0.5f *0.5f), z + 6, ORTHOGONAL, vec3(PUTVERTICALxy), Brown6, 0.5f, (BULDINGHEIGHT / 2) + 1);//retoc
	CreateCylinderToBuldings(x + 6, y + (BULDINGHEIGHT *0.5f *0.5f), z - 6, ORTHOGONAL, vec3(PUTVERTICALxy), Brown6, 0.5f, (BULDINGHEIGHT / 2) + 1);//retoc


	CreateCubeToBuldings(x - BUIDINGWIDTH, (BULDINGHEIGHT*0.5f *0.5f) + y - 1.5f, z + 3.75f, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.5f, 0.5f, 6); //retoc  gates
	CreateCubeToBuldings(x - BUIDINGWIDTH, (BULDINGHEIGHT*0.5f *0.5f) + y - 1.5f, z - 3.75f, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.5f, 0.5f, 6); //retoc  gates
	CreateCubeToBuldings(x - BUIDINGWIDTH, (BULDINGHEIGHT*0.5f) + y - 3, z, 0, vec3(PUTVERTICALzy), Brown6, 0.5f, 0.5f, 8.5); //retoc  gates

																															  //physics

	CreateInvisibleWall(x - BUIDINGWIDTH, (BULDINGHEIGHT *0.5f) + y, z + BUIDINGLONG *0.5f, vec3{ 1, 1, BULDINGHEIGHT }, ORTHOGONAL, vec3(PUTVERTICALzy)); //	retoc
	CreateInvisibleWall(x - BUIDINGWIDTH, (BULDINGHEIGHT *0.5f) + y, z - BUIDINGLONG *0.5f, vec3{ 1, 1, BULDINGHEIGHT }, ORTHOGONAL, vec3(PUTVERTICALzy)); //retoc
	CreateInvisibleWall(x, (BULDINGHEIGHT *0.5f) + y, z + BUIDINGLONG *0.5f, vec3{ 1, 1, BULDINGHEIGHT }, ORTHOGONAL, vec3(PUTVERTICALzy)); //retoc
	CreateInvisibleWall(x, (BULDINGHEIGHT *0.5f) + y, z - BUIDINGLONG *0.5f, vec3{ 1, 1, BULDINGHEIGHT }, ORTHOGONAL, vec3(PUTVERTICALzy)); //retoc

	CreateInvisibleWall((x - (BUIDINGWIDTH *0.5f)), ((BULDINGHEIGHT *0.5f) + y + 0), (z + 23), vec3{ BUIDINGWIDTH, 0.25f, BULDINGHEIGHT }, ORTHOGONAL, vec3(PUTVERTICALzy)); //wall2
	CreateInvisibleWall((x - (BUIDINGWIDTH *0.5f)), ((BULDINGHEIGHT *0.5f) + y + 0), (z - 23), vec3{ BUIDINGWIDTH, 0.25f, BULDINGHEIGHT }, ORTHOGONAL, vec3(PUTVERTICALzy)); //wall3

	CreateInvisibleWall(x - BUIDINGWIDTH, (6 * 0.5f) + y, z + 3 + (21 * 0.5f), vec3{ 6, 0.25f, 20 }, ORTHOGONAL, vec3(PUTVERTICALxy)); //wall4
	CreateInvisibleWall(x - BUIDINGWIDTH, (6 * 0.5f) + y, z - 3 - (21 * 0.5f), vec3{ 6, 0.25f, 20 }, ORTHOGONAL, vec3(PUTVERTICALxy)); //wall4	

	noCube* inv = new noCube(8, 0.15f, 10);
	inv->SetPos(x + 200, 4 + y, z + 300);
	inv->SetRotation(ORTHOGONAL, vec3(PUTVERTICALxy));
	invDoor = inv;
	invDoor_body = App->physics->AddBody(*invDoor);

	//indors


	CreateInvisibleWall((x - (BUIDINGWIDTH *0.5f)), ((BULDINGHEIGHT *0.5f) + y + 0), (z + 10), vec3{ BUIDINGWIDTH, 0.25f, BULDINGHEIGHT }, ORTHOGONAL, vec3(PUTVERTICALzy)); //wall2
	CreateInvisibleWall((x - (BUIDINGWIDTH *0.5f)), ((BULDINGHEIGHT *0.5f) + y + 0), (z - 10), vec3{ BUIDINGWIDTH, 0.25f, BULDINGHEIGHT }, ORTHOGONAL, vec3(PUTVERTICALzy)); //wall3

	CreateCubeToBuldings((x - (BUIDINGWIDTH *0.5f)), ((BULDINGHEIGHT *0.5f) + y + 0), (z + 10), ORTHOGONAL, vec3(PUTVERTICALzy), White, BUIDINGWIDTH, 0.25f, BULDINGHEIGHT); //wall2
	CreateCubeToBuldings((x - (BUIDINGWIDTH *0.5f)), ((BULDINGHEIGHT *0.5f) + y + 0), (z - 10), ORTHOGONAL, vec3(PUTVERTICALzy), White, BUIDINGWIDTH, 0.25f, BULDINGHEIGHT); //wall3

	for (int i = 1; i < 8; i++) {
		CreateCubeToBuldings(x - 2.5*i, y + 8 + 2, z, 0, vec3(PUTVERTICALzy), Brown6, 1, 1, 25); //window
	}
	CreateCubeToBuldings(x - (BUIDINGWIDTH *0.5f), y + 9.2 + 2, z, 0, vec3(PUTVERTICALzy), White, 20, 0.25f, 25); //window
	
																												  //windows

																												  //delante parte izquierda
																												  //1
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 2, z + 1.5 + 18, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 2, z - 1.5 + 18, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x + 0.25f, y + 0.125 + 2, z + 18, 0, vec3(PUTVERTICALzy), Brown6, 0.75, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 2, z + 18, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.25f, y - 0.125 + 5 + 2, z + 18, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 2, z + 18, 0, vec3(PUTVERTICALzy), Blue, 0.15, 5, 3); //window

	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 2, z + 20.25, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window
	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 2, z + 15.75, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window
																										   //2
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 2, z + 1.5 + 10, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 2, z - 1.5 + 10, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x + 0.25f, y + 0.125 + 2, z + 10, 0, vec3(PUTVERTICALzy), Brown6, 0.75, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 2, z + 10, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.25f, y - 0.125 + 5 + 2, z + 10, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 2, z + 10, 0, vec3(PUTVERTICALzy), Blue, 0.15, 5, 3); //window

	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 2, z + 12.25, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window
	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 2, z + 7.75, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window

																										  //3
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 11, z + 1.5 + 18, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 11, z - 1.5 + 18, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x + 0.25f, y + 0.125 + 11, z + 18, 0, vec3(PUTVERTICALzy), Brown6, 0.75, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 11, z + 18, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.25f, y - 0.125 + 5 + 11, z + 18, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 11, z + 18, 0, vec3(PUTVERTICALzy), Blue, 0.15, 5, 3); //window

	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 11, z + 20.25, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window
	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 11, z + 15.75, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window
																											//4
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 11, z + 1.5 + 10, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 11, z - 1.5 + 10, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x + 0.25f, y + 0.125 + 11, z + 10, 0, vec3(PUTVERTICALzy), Brown6, 0.75, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 11, z + 10, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.25f, y - 0.125 + 5 + 11, z + 10, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 11, z + 10, 0, vec3(PUTVERTICALzy), Blue, 0.15, 5, 3); //window

	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 11, z + 12.25, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window
	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 11, z + 7.75, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window
																										   //delante parte derecha

																										   //1
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 2, z + 1.5 - 18, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 2, z - 1.5 - 18, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x + 0.25f, y + 0.125 + 2, z - 18, 0, vec3(PUTVERTICALzy), Brown6, 0.75, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 2, z - 18, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.25f, y - 0.125 + 5 + 2, z - 18, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 2, z - 18, 0, vec3(PUTVERTICALzy), Blue, 0.15, 5, 3); //window

	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 2, z - 20.25, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window
	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 2, z - 15.75, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window

																										   //2
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 2, z + 1.5 - 10, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 2, z - 1.5 - 10, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x + 0.25f, y + 0.125 + 2, z - 10, 0, vec3(PUTVERTICALzy), Brown6, 0.75, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 2, z - 10, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.25f, y - 0.125 + 5 + 2, z - 10, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 2, z - 10, 0, vec3(PUTVERTICALzy), Blue, 0.15, 5, 3); //window

	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 2, z - 12.25, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window
	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 2, z - 7.75, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window

																										  //3
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 11, z + 1.5 - 18, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 11, z - 1.5 - 18, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x + 0.25f, y + 0.125 + 11, z - 18, 0, vec3(PUTVERTICALzy), Brown6, 0.75, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 11, z - 18, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.25f, y - 0.125 + 5 + 11, z - 18, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 11, z - 18, 0, vec3(PUTVERTICALzy), Blue, 0.15, 5, 3); //window

	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 11, z - 20.25, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window
	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 11, z - 15.75, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window
																											//4
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 11, z + 1.5 - 10, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 11, z - 1.5 - 10, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x + 0.25f, y + 0.125 + 11, z - 10, 0, vec3(PUTVERTICALzy), Brown6, 0.75, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 11, z - 10, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.25f, y - 0.125 + 5 + 11, z - 10, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 11, z - 10, 0, vec3(PUTVERTICALzy), Blue, 0.15, 5, 3); //window

	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 11, z - 12.25, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window
	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 11, z - 7.75, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window


																										   //lateral i<quierdo


	CreateCubeToBuldings(-10 + x + 1.5, y + 2.5 + 2, 23 + z + 0.25f, ORTHOGONAL, vec3(PUTVERTICALxy), Brown6, 5, 0.25, 0.25); //window
	CreateCubeToBuldings(-10 + x - 1.5, y + 2.5 + 2, 23 + z + 0.25f, ORTHOGONAL, vec3(PUTVERTICALxy), Brown6, 5, 0.25, 0.25); //window
	CreateCubeToBuldings(-10 + x, y + 0.125 + 2, 23 + z + 0.25f, 0, vec3(PUTVERTICALzy), Brown6, 3, 0.25, 0.75); //window
	CreateCubeToBuldings(-10 + x, y + 2.5 + 2, 23 + z + 0.25f, 0, vec3(PUTVERTICALzy), Brown6, 3, 0.25, 0.25); //window
	CreateCubeToBuldings(-10 + x, y - 0.125 + 5 + 2, 23 + z + 0.25f, 0, vec3(PUTVERTICALzy), Brown6, 3, 0.25, 0.25); //window
	CreateCubeToBuldings(-10 + x, y + 2.5 + 2, 23 + z + 0.15f, 0, vec3(PUTVERTICALzy), Blue, 3, 5, 0.15); //window

	CreateCubeToBuldings(-10 + x + 2.25, y + 2.5 + 2, 23 + z + 0.15f, 0, vec3(PUTVERTICALzy), Brown5, 1.5, 5, 0.15); //window
	CreateCubeToBuldings(-10 + x - 2.25, y + 2.5 + 2, 23 + z + 0.15f, 0, vec3(PUTVERTICALzy), Brown5, 1.5, 5, 0.15); //window


	CreateCubeToBuldings(-10 + x + 1.5, y + 2.5 + 11, 23 + z + 0.25f, ORTHOGONAL, vec3(PUTVERTICALxy), Brown6, 5, 0.25, 0.25); //window
	CreateCubeToBuldings(-10 + x - 1.5, y + 2.5 + 11, 23 + z + 0.25f, ORTHOGONAL, vec3(PUTVERTICALxy), Brown6, 5, 0.25, 0.25); //window
	CreateCubeToBuldings(-10 + x, y + 0.125 + 11, 23 + z + 0.25f, 0, vec3(PUTVERTICALzy), Brown6, 3, 0.25, 0.75); //window
	CreateCubeToBuldings(-10 + x, y + 2.5 + 11, 23 + z + 0.25f, 0, vec3(PUTVERTICALzy), Brown6, 3, 0.25, 0.25); //window
	CreateCubeToBuldings(-10 + x, y - 0.125 + 5 + 11, 23 + z + 0.25f, 0, vec3(PUTVERTICALzy), Brown6, 3, 0.25, 0.25); //window
	CreateCubeToBuldings(-10 + x, y + 2.5 + 11, 23 + z + 0.15f, 0, vec3(PUTVERTICALzy), Blue, 3, 5, 0.15); //window

	CreateCubeToBuldings(-10 + x + 2.25, y + 2.5 + 11, 23 + z + 0.15f, 0, vec3(PUTVERTICALzy), Brown5, 1.5, 5, 0.15); //window
	CreateCubeToBuldings(-10 + x - 2.25, y + 2.5 + 11, 23 + z + 0.15f, 0, vec3(PUTVERTICALzy), Brown5, 1.5, 5, 0.15); //window

																													  //lateral derecho

	CreateCubeToBuldings(-10 + x + 1.5, y + 2.5 + 2, -24 + z + 0.25f, ORTHOGONAL, vec3(PUTVERTICALxy), Brown6, 5, 0.25, 0.25); //window
	CreateCubeToBuldings(-10 + x - 1.5, y + 2.5 + 2, -24 + z + 0.25f, ORTHOGONAL, vec3(PUTVERTICALxy), Brown6, 5, 0.25, 0.25); //window
	CreateCubeToBuldings(-10 + x, y + 0.125 + 2, -24 + z + 0.25f, 0, vec3(PUTVERTICALzy), Brown6, 3, 0.25, 0.75); //window
	CreateCubeToBuldings(-10 + x, y + 2.5 + 2, -24 + z + 0.25f, 0, vec3(PUTVERTICALzy), Brown6, 3, 0.25, 0.25); //window
	CreateCubeToBuldings(-10 + x, y - 0.125 + 5 + 2, -24 + z + 0.25f, 0, vec3(PUTVERTICALzy), Brown6, 3, 0.25, 0.25); //window
	CreateCubeToBuldings(-10 + x, y + 2.5 + 2, -24 + z + 0.35f, 0, vec3(PUTVERTICALzy), Blue, 3, 5, 0.15); //window

	CreateCubeToBuldings(-10 + x + 2.25, y + 2.5 + 2, -24 + z + 0.35f, 0, vec3(PUTVERTICALzy), Brown5, 1.5, 5, 0.15); //window
	CreateCubeToBuldings(-10 + x - 2.25, y + 2.5 + 2, -24 + z + 0.35f, 0, vec3(PUTVERTICALzy), Brown5, 1.5, 5, 0.15); //window


	CreateCubeToBuldings(-10 + x + 1.5, y + 2.5 + 11, -24 + z + 0.25f, ORTHOGONAL, vec3(PUTVERTICALxy), Brown6, 5, 0.25, 0.25); //window
	CreateCubeToBuldings(-10 + x - 1.5, y + 2.5 + 11, -24 + z + 0.25f, ORTHOGONAL, vec3(PUTVERTICALxy), Brown6, 5, 0.25, 0.25); //window
	CreateCubeToBuldings(-10 + x, y + 0.125 + 11, -24 + z + 0.25f, 0, vec3(PUTVERTICALzy), Brown6, 3, 0.25, 0.75); //window
	CreateCubeToBuldings(-10 + x, y + 2.5 + 11, -24 + z + 0.25f, 0, vec3(PUTVERTICALzy), Brown6, 3, 0.25, 0.25); //window
	CreateCubeToBuldings(-10 + x, y - 0.125 + 5 + 11, -24 + z + 0.25f, 0, vec3(PUTVERTICALzy), Brown6, 3, 0.25, 0.25); //window
	CreateCubeToBuldings(-10 + x, y + 2.5 + 11, -24 + z + 0.35f, 0, vec3(PUTVERTICALzy), Blue, 3, 5, 0.15); //window

	CreateCubeToBuldings(-10 + x + 2.25, y + 2.5 + 11, -24 + z + 0.35f, 0, vec3(PUTVERTICALzy), Brown5, 1.5, 5, 0.15); //window
	CreateCubeToBuldings(-10 + x - 2.25, y + 2.5 + 11, -24 + z + 0.35f, 0, vec3(PUTVERTICALzy), Brown5, 1.5, 5, 0.15); //window

																													   //-----------
																													   //3
	CreateCubeToBuldings(x - 20.5 + 0.25f, y + 2.5 + 11, z + 1.5 + 18, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x - 20.5 + 0.25f, y + 2.5 + 11, z - 1.5 + 18, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x - 20.5 + 0.25f, y + 0.125 + 11, z + 18, 0, vec3(PUTVERTICALzy), Brown6, 0.75, 0.25, 3); //window
	CreateCubeToBuldings(x - 20.5 + 0.25f, y + 2.5 + 11, z + 18, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x - 20.5 + 0.25f, y - 0.125 + 5 + 11, z + 18, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x - 20.5 + 0.15f + 0.15f, y + 2.5 + 11, z + 18, 0, vec3(PUTVERTICALzy), Blue, 0.15, 5, 3); //window

	CreateCubeToBuldings(x - 20.5 + 0.15f + 0.15f, y + 2.5 + 11, z + 20.25, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window
	CreateCubeToBuldings(x - 20.5 + 0.15f + 0.15f, y + 2.5 + 11, z + 15.75, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window
																														   //4
	CreateCubeToBuldings(x - 20.5 + 0.25f, y + 2.5 + 11, z + 1.5 + 10, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x - 20.5 + 0.25f, y + 2.5 + 11, z - 1.5 + 10, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x - 20.5 + 0.25f, y + 0.125 + 11, z + 10, 0, vec3(PUTVERTICALzy), Brown6, 0.75, 0.25, 3); //window
	CreateCubeToBuldings(x - 20.5 + 0.25f, y + 2.5 + 11, z + 10, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x - 20.5 + 0.25f, y - 0.125 + 5 + 11, z + 10, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x - 20.5 + 0.15f + 0.15f, y + 2.5 + 11, z + 10, 0, vec3(PUTVERTICALzy), Blue, 0.15, 5, 3); //window

	CreateCubeToBuldings(x - 20.5 + 0.15f + 0.15f, y + 2.5 + 11, z + 12.25, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window
	CreateCubeToBuldings(x - 20.5 + 0.15f + 0.15f, y + 2.5 + 11, z + 7.75, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window

																														  //3
	CreateCubeToBuldings(x - 20.5 + 0.25f, y + 2.5 + 11, z + 1.5 - 18, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x - 20.5 + 0.25f, y + 2.5 + 11, z - 1.5 - 18, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x - 20.5 + 0.25f, y + 0.125 + 11, z - 18, 0, vec3(PUTVERTICALzy), Brown6, 0.75, 0.25, 3); //window
	CreateCubeToBuldings(x - 20.5 + 0.25f, y + 2.5 + 11, z - 18, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x - 20.5 + 0.25f, y - 0.125 + 5 + 11, z - 18, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x - 20.5 + 0.15f + 0.15f, y + 2.5 + 11, z - 18, 0, vec3(PUTVERTICALzy), Blue, 0.15, 5, 3); //window

	CreateCubeToBuldings(x - 20.5 + 0.15f + 0.15f, y + 2.5 + 11, z - 20.25, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window
	CreateCubeToBuldings(x - 20.5 + 0.15f + 0.15f, y + 2.5 + 11, z - 15.75, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window

																														   //4
	CreateCubeToBuldings(x + 0.25f - 20.5, y + 2.5 + 11, z + 1.5 - 10, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x + 0.25f - 20.5, y + 2.5 + 11, z - 1.5 - 10, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x + 0.25f - 20.5, y + 0.125 + 11, z - 10, 0, vec3(PUTVERTICALzy), Brown6, 0.75, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.25f - 20.5, y + 2.5 + 11, z - 10, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.25f - 20.5, y - 0.125 + 5 + 11, z - 10, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.15f - 20.5 + 0.15f, y + 2.5 + 11, z - 10, 0, vec3(PUTVERTICALzy), Blue, 0.15, 5, 3); //window

	CreateCubeToBuldings(x + 0.15f - 20.5 + 0.15f, y + 2.5 + 11, z - 12.25, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window
	CreateCubeToBuldings(x + 0.15f - 20.5 + 0.15f, y + 2.5 + 11, z - 7.75, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window

																														  //5
	CreateCubeToBuldings(x + 0.25f - 20.5, y + 2.5 + 11, z + 1.5, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x + 0.25f - 20.5, y + 2.5 + 11, z - 1.5, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x + 0.25f - 20.5, y + 0.125 + 11, z, 0, vec3(PUTVERTICALzy), Brown6, 0.75, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.25f - 20.5, y + 2.5 + 11, z, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.25f - 20.5, y - 0.125 + 5 + 11, z, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.15f - 20.5 + 0.15f, y + 2.5 + 11, z, 0, vec3(PUTVERTICALzy), Blue, 0.15, 5, 3); //window

	CreateCubeToBuldings(x + 0.15f - 20.5 + 0.15f, y + 2.5 + 11, z - 2.25, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window
	CreateCubeToBuldings(x + 0.15f - 20.5 + 0.15f, y + 2.5 + 11, z + 2.25, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window

																														  //balcon
	for (int i = 0; i<15; i++)
		CreateCubeToBuldings(x + 7, y + 11, z + 7 - 1.0f*i, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.20, 0.20, 3); //window

	for (int i = 0; i<10; i++)
		CreateCubeToBuldings(x + 7 - 1.0f*i, y + 11, z + 7, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.20, 0.20, 3); //window

	for (int i = 0; i<10; i++)
		CreateCubeToBuldings(x + 7 - 1.0f*i, y + 11, z - 7, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.20, 0.25, 3); //window

	CreateCubeToBuldings(x + 7, y + 11 + 1.5f, z, 0, vec3(PUTVERTICALzy), grey, 0.25, 0.25, 15); //window
	CreateCubeToBuldings(x + 3.5f, y + 11 + 1.5f, z + 7, 90, vec3(PUTHORIZONALxz), grey, 0.25, 0.25, 7); //window
	CreateCubeToBuldings(x + 3.5f, y + 11 + 1.5f, z - 7, 90, vec3(PUTHORIZONALxz), grey, 0.25, 0.25, 7); //window

	CreateCubeToBuldings(x + 7, y + 8.5 + 1.5f, z, 0, vec3(PUTVERTICALzy), Brown5, 0.25, 0.25, 14); //window
	CreateCubeToBuldings(x + 3.5f, y + 8.5 + 1.5f, z + 7, 90, vec3(PUTHORIZONALxz), Brown5, 0.25, 0.25, 7); //window
	CreateCubeToBuldings(x + 3.5f, y + 8.5 + 1.5f, z - 7, 90, vec3(PUTHORIZONALxz), Brown5, 0.25, 0.25, 7); //window


	CreateCubeToBuldings(x + 0.15f, y + 8.5 + 4.0f, z, 90, vec3(PUTHORIZONALxz), Brown6, 3, 5.5, 0.25); //window


	//-----------------DOORs----------------

																										//door1
	Cube* door1 = new Cube(0.25f, 7.75f, 4.5F);
	door1->SetPos(x, y + 4.0f, z + 2.35f);

	door1->SetRotation(0, vec3(PUTVERTICALxy));	door1->color = Brown5;	Doors.add(door1);		//door1
																								//

																								//wall1
	Cube* wall1 = new Cube(0.25f, 8, BULDINGHEIGHT);
	wall1->SetPos(x, (8 * 0.5f) + y, z + 5 + (BULDINGHEIGHT *0.5f));

	wall1->SetRotation(0, vec3(PUTVERTICALxy)); wall1->color = White;	Doors.add(wall1);		//wall1
																								//

	PhysBody3D* bodyA = App->physics->AddBody(*door1, 200);
	Doors_body.add(bodyA);
	PhysBody3D* bodyB = App->physics->AddBody(*wall1, 0);
	Doors_body.add(bodyB);

	App->physics->AddConstraintHinge(*bodyA, *bodyB, vec3{ 0, 0, 0 + 2.4f }, vec3{ 0,0, 0 - 0.25f - (BULDINGHEIGHT *0.5f) }, vec3(PUTHORIZONALxz), vec3(PUTHORIZONALxz), false);


	//door2
	Cube* door2 = new Cube(7.75f, 0.25f, 4.5F);
	door2->SetPos(x, y + 4.0f, z - 2.35f);

	door2->SetRotation(ORTHOGONAL, vec3(PUTVERTICALxy));	door2->color = Brown5;	Doors.add(door2);	//door2
																										//
																										//wall2
	Cube* wall2 = new Cube(8, 0.25f, BULDINGHEIGHT);
	wall2->SetPos(x, (8 * 0.5f) + y, z - 5 - (BULDINGHEIGHT *0.5f));

	wall2->SetRotation(ORTHOGONAL, vec3(PUTVERTICALxy));	wall2->color = White;	Doors.add(wall2);	//wall2o
																										//
																										//-----------------constrains----------------
	PhysBody3D* bodyC = App->physics->AddBody(*door2, 200);
	Doors_body.add(bodyC);
	PhysBody3D* bodyD = App->physics->AddBody(*wall2, 0);
	Doors_body.add(bodyD);

	App->physics->AddConstraintHinge(*bodyC, *bodyD, vec3{ 0, 0, 0 - 2.4f }, vec3{ 0,0, 0 + 0.25f + (BULDINGHEIGHT *0.5f) }, vec3(PUTVERTICALzy), vec3(PUTVERTICALzy), false);

	//--------------------------------------

	redFence1 = new Cube(0.25, 0.75, 3);
	redFence1->SetPos(x + 0.25, y + 2.0f, z);
	redFence1->SetRotation(0, vec3(PUTVERTICALzy));
	redFence1->color = Red;

	redFence2 = new Cube(0.25, 0.75, 3);
	redFence2->SetPos(x + 0.25, y + 6.0f, z);
	redFence2->SetRotation(0, vec3(PUTVERTICALzy));
	redFence2->color = Red;


	//roof

	CreateCubeToBuldings(x - 4, y + 21, z, -30, vec3(PUTVERTICALxy), grey, 14, 0.25f, 48); //roof1
	CreateCubeToBuldings(x - 5, y + 21, z, -30, vec3(PUTVERTICALxy), Brown6, 1, 1, 47.5); //roof1
	CreateCubeToBuldings(x, y + 18, z, -30, vec3(PUTVERTICALxy), Brown6, 1, 1, 47.5); //roof1
	CreateCubeToBuldings(x - 10, y + 23.75, z, -45, vec3(PUTVERTICALxy), Brown6, 1, 1, 47.5); //roofmiddle

	CreateCubeToBuldings(x - 16, y + 21, z, 30, vec3(PUTVERTICALxy), grey, 14, 0.25f, 48); //roof2
	CreateCubeToBuldings(x - 16, y + 20.5, z, 30, vec3(PUTVERTICALxy), Brown6, 1, 1, 47.5); //roof2
	CreateCubeToBuldings(x - 20, y + 18, z, 30, vec3(PUTVERTICALxy), Brown6, 1, 1, 47.5); //roof2

	CreateCubeToBuldings(x - 7, y + 17, z + 22.90, -30, vec3(PUTVERTICALxy), White, 12, 10, 0.25f); //roof1
	CreateCubeToBuldings(x - 13, y + 17, z + 22.90, +30, vec3(PUTVERTICALxy), White, 12, 10, 0.25f); //roof1

	CreateCubeToBuldings(x - 7, y + 17, z - 22.90, -30, vec3(PUTVERTICALxy), White, 12, 10, 0.25f); //roof1
	CreateCubeToBuldings(x - 13, y + 17, z - 22.90, +30, vec3(PUTVERTICALxy), White, 12, 10, 0.25f); //roof1


	CreateCubeToBuldings(x - 4, y + 21, z + 7, +0, vec3(PUTVERTICALxy), White, 9, 6, 0.25f); //roof1
	CreateCubeToBuldings(x - 4, y + 21, z + 13, +0, vec3(PUTVERTICALxy), White, 9, 6, 0.25f); //roof1

																							  //
	CreateCubeToBuldings(x - 3.5, y + 24.75, z + 8, -120, vec3(PUTVERTICALzy), grey, 9, 5, 0.25f); //roof1
	CreateCubeToBuldings(x - 3.5, y + 24.75, z + 12, +120, vec3(PUTVERTICALzy), grey, 9, 5, 0.25f); //roof1

	CreateCubeToBuldings(x, y + 23.25, z + 9.25, -120, vec3(PUTVERTICALzy), White, 0.25f, 4, 4); //roof1
	CreateCubeToBuldings(x, y + 23.25, z + 10.5, +120, vec3(PUTVERTICALzy), White, 0.25f, 4, 4); //roof1
	CreateCubeToBuldings(x, y + 21, z + 10, 0, vec3(PUTVERTICALzy), White, 0.25f, 6, 6); //roof1

	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 19, z + 1.5 + 10, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 19, z - 1.5 + 10, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x + 0.25f, y + 0.125 + 19, z + 10, 0, vec3(PUTVERTICALzy), Brown6, 0.75, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 19, z + 10, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.25f, y - 0.125 + 5 + 19, z + 10, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 19, z + 10, 0, vec3(PUTVERTICALzy), Blue, 0.15, 5, 3); //window

	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 19, z + 12.25, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window
	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 19, z + 7.75, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window


																										   //


	CreateCubeToBuldings(x - 4, y + 21, z - 7, +0, vec3(PUTVERTICALxy), White, 9, 6, 0.25f); //roof1
	CreateCubeToBuldings(x - 4, y + 21, z - 13, +0, vec3(PUTVERTICALxy), White, 9, 6, 0.25f); //roof1

																							  //
	CreateCubeToBuldings(x - 3.5, y + 24.75, z - 12, -120, vec3(PUTVERTICALzy), grey, 9, 5, 0.25f); //roof1
	CreateCubeToBuldings(x - 3.5, y + 24.75, z - 8, +120, vec3(PUTVERTICALzy), grey, 9, 5, 0.25f); //roof1

	CreateCubeToBuldings(x, y + 23.25, z - 10.5, -120, vec3(PUTVERTICALzy), White, 0.25f, 4, 4); //roof1
	CreateCubeToBuldings(x, y + 23.25, z - 9.25, +120, vec3(PUTVERTICALzy), White, 0.25f, 4, 4); //roof1
	CreateCubeToBuldings(x, y + 21, z - 10, 0, vec3(PUTVERTICALzy), White, 0.25f, 6, 6); //roof1

	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 19, z + 1.5 - 10, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 19, z - 1.5 - 10, ORTHOGONAL, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 5); //window
	CreateCubeToBuldings(x + 0.25f, y + 0.125 + 19, z - 10, 0, vec3(PUTVERTICALzy), Brown6, 0.75, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.25f, y + 2.5 + 19, z - 10, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.25f, y - 0.125 + 5 + 19, z - 10, 0, vec3(PUTVERTICALzy), Brown6, 0.25, 0.25, 3); //window
	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 19, z - 10, 0, vec3(PUTVERTICALzy), Blue, 0.15, 5, 3); //window

	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 19, z - 12.25, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window
	CreateCubeToBuldings(x + 0.15f, y + 2.5 + 19, z - 7.75, 0, vec3(PUTVERTICALzy), Brown5, 0.15, 5, 1.5); //window

	//RIVER
	CreateCubeToBuldings(124.5, 0.5f, 90, 30, vec3{ 0, 1, 0 }, grey, 1, 1, 10);
	CreateCubeToBuldings(120.8, 0.5f, 80.5, 10, vec3{ 0, 1, 0 }, grey, 1, 1, 10);
	CreateCubeToBuldings(121, 0.5f, 70.5, -15, vec3{ 0, 1, 0 }, grey, 1, 1, 10);
	CreateCubeToBuldings(125.8, 0.5f, 61.7, -40, vec3{ 0, 1, 0 }, grey, 1, 1, 10);
	CreateCubeToBuldings(133.5, 0.5f, 55.2, -60, vec3{ 0, 1, 0 }, grey, 1, 1, 10);
	CreateCubeToBuldings(142.7, 0.5f, 51, -70, vec3{ 0, 1, 0 }, grey, 1, 1, 10);
	CreateCubeToBuldings(148.2, 0.5f, 44.5, 0, vec3{ 0, 1, 0 }, grey, 1, 1, 10);
	
	CreateCubeToBuldings(143, 0.5f, 38, 70, vec3{ 0, 1, 0 }, grey, 1, 1, 10);
	CreateCubeToBuldings(127.5, 0.5f, 30, 60, vec3{ 0, 1, 0 }, grey, 1, 1, 25);
	CreateCubeToBuldings(111, 0.5f, 18.8, 50, vec3{ 0, 1, 0 }, grey, 1, 1, 15);
	CreateCubeToBuldings(103, 0.5f, 10, 30, vec3{ 0, 1, 0 }, grey, 1, 1, 10);
	
	
	CreateCubeToBuldings(100.5, 0.5f, -2.5, 0, vec3{ 0, 1, 0 }, grey, 1, 1, 16);
	CreateCubeToBuldings(106, 0.5f, -17.3, -40, vec3{ 0, 1, 0 }, grey, 1, 1, 17);
	CreateCubeToBuldings(121, 0.5f, -25, -80, vec3{ 0, 1, 0 }, grey, 1, 1, 20);
		
	CreateCubeToBuldings(135.8, 0.5f, 68.5, -40, vec3{ 0, 1, 0 }, grey, 1, 1, 10);
	CreateCubeToBuldings(143.5, 0.5f, 62.2, -60, vec3{ 0, 1, 0 }, grey, 1, 1, 10);
	CreateCubeToBuldings(152.7, 0.5f, 58, -70, vec3{ 0, 1, 0 }, grey, 1, 1, 10);
	CreateCubeToBuldings(161, 0.5f, 52.5, -40, vec3{ 0, 1, 0 }, grey, 1, 1, 10);
	CreateCubeToBuldings(164.5, 0.5f, 43.3, 0, vec3{ 0, 1, 0 }, grey, 1, 1, 10);
	CreateCubeToBuldings(161.5, 0.5f, 34.5, 40, vec3{ 0, 1, 0 }, grey, 1, 1, 10);
	CreateCubeToBuldings(153.5, 0.5f, 29, 70, vec3{ 0, 1, 0 }, grey, 1, 1, 10);
	CreateCubeToBuldings(144.3, 0.5f, 24.6, 60, vec3{ 0, 1, 0 }, grey, 1, 1, 10);
	CreateCubeToBuldings(129.5, 0.5f, 16, 60, vec3{ 0, 1, 0 }, grey, 1, 1, 25);
	CreateCubeToBuldings(115.5, 0.5f, 6, 40, vec3{ 0, 1, 0 }, grey, 1, 1, 10);
	CreateCubeToBuldings(112.5, 0.5f, -3, 0, vec3{ 0, 1, 0 }, grey, 1, 1, 10);

	CreateCubeToBuldings(115.5, 0.5f, -12, -40, vec3{ 0, 1, 0 }, grey, 1, 1, 10);
	CreateCubeToBuldings(126.5, 0.5f, -17, -80, vec3{ 0, 1, 0 }, grey, 1, 1, 15);

	CreateCubeToBuldings(153.6, 1, -50, 15, vec3(PUTVERTICALzy), grey, 10, 0.25f, 6);
	CreateInvisibleWall(153.6, 1, -50, vec3{ 10, 0.25f, 6 }, 15, vec3(PUTVERTICALzy));
	CreateCylinderToBuldings(153.6, 0.5, -50, 0, vec3{ PUTVERTICALxy }, Brown2, 0.5, 11);
	CreateCylinderToBuldings(153.6, 0.5, -51, 0, vec3{ PUTVERTICALxy }, Brown2, 0.5, 11);
	CreateCylinderToBuldings(153.6, 0.5, -52, 0, vec3{ PUTVERTICALxy }, Brown2, 0.5, 12);
	CreateCylinderToBuldings(153.6, 1, -51.5, 0, vec3{ PUTVERTICALxy }, Brown2, 0.5, 10);
	
		//before ramp
	CreateCylinderToBuldings(139, 1, -28.3, 13, vec3{ 0,1,0 }, Brown2, 0.6, 16);
	CreateInvisibleWall(139, 1, -28.3f, vec3{ 1.2f ,1.2f ,16 }, 102, vec3(0, 1, 0));

		//river
	CreateCylinderToBuldings(142, 1, 27, 60, vec3{ 0,1,0 }, Brown2, 0.5, 5);
	CreateCylinderToBuldings(131, 1, 29, 60, vec3{ 0,1,0 }, Brown2, 0.5, 5);
	CreateCylinderToBuldings(125, 1, 18, 60, vec3{ 0,1,0 }, Brown2, 0.5, 5);
	
	CreateCylinderToBuldings(108.5, 1, -2, 0, vec3{ 0,1,0 }, Brown2, 0.5, 5);
	CreateCylinderToBuldings(108.5, 1, -15, -40, vec3{ 0,1,0 }, Brown2, 0.5, 5);
	
	CreateInvisibleWall(142, 1, 27, vec3{ 1 ,1 ,5 }, -30, vec3(0, 1, 0));
	CreateInvisibleWall(131, 1, 29, vec3{ 1 ,1 ,5 }, -30, vec3(0, 1, 0));
	CreateInvisibleWall(125, 1, 18, vec3{ 1 ,1 ,5 }, -30, vec3(0, 1, 0));
	CreateInvisibleWall(108.5, 1, -2, vec3{ 1 ,1 ,5 }, 90, vec3(0, 1, 0));
	CreateInvisibleWall(108.5, 1, -15, vec3{ 1 ,1 ,5 }, 52, vec3(0, 1, 0));
	
		//RIVER
	CreateInvisibleWall(124.5, 0.5f, 90, vec3{ 1 ,1 ,10 }, 30, vec3(0, 1, 0));
	CreateInvisibleWall(120.8, 0.5f, 80.5, vec3{ 1 ,1 ,10 }, 10, vec3(0, 1, 0));
	CreateInvisibleWall(121, 0.5f, 70.5, vec3{ 1 ,1 ,10 }, -15, vec3(0, 1, 0));
	CreateInvisibleWall(125.8, 0.5f, 61.7, vec3{ 1 ,1 ,10 }, -40, vec3(0, 1, 0));
	CreateInvisibleWall(133.5, 0.5f, 55.2, vec3{ 1 ,1 ,10 }, -60, vec3(0, 1, 0));
	CreateInvisibleWall(142.7, 0.5f, 51, vec3{ 1 ,1 ,10 }, -70, vec3(0, 1, 0));
	CreateInvisibleWall(148.2, 0.5f, 44.5, vec3{ 1 ,1 ,10 }, 0, vec3(0, 1, 0));
	
	CreateInvisibleWall(143, 0.5f, 38, vec3{ 1 ,1 ,10 }, 70, vec3(0, 1, 0));
	CreateInvisibleWall(127.5, 0.5f, 30, vec3{ 1 ,1 ,25 }, 60, vec3(0, 1, 0));
	CreateInvisibleWall(111, 0.5f, 18.8, vec3{ 1 ,1 ,15 }, 50, vec3(0, 1, 0));
	CreateInvisibleWall(103, 0.5f, 10, vec3{ 1 ,1 ,10 }, 30, vec3(0, 1, 0));
	
	CreateInvisibleWall(100.5, 0.5f, -2.5, vec3{ 1 ,1 ,16 }, 0, vec3(0, 1, 0));
	CreateInvisibleWall(106, 0.5f, -17.3, vec3{ 1 ,1 ,17 }, -40, vec3(0, 1, 0));
	CreateInvisibleWall(121, 0.5f, -25, vec3{ 1 ,1 ,20 }, -80, vec3(0, 1, 0));
		
	
	CreateInvisibleWall(135.8, 0.5f, 68.5, vec3{ 1 ,1 ,10 }, -40, vec3(0, 1, 0));
	CreateInvisibleWall(143.5, 0.5f, 62.2, vec3{ 1 ,1 ,10 }, -60, vec3(0, 1, 0));
	CreateInvisibleWall(152.7, 0.5f, 58, vec3{ 1 ,1 ,10 }, -75, vec3(0, 1, 0));
	CreateInvisibleWall(161, 0.5f, 52.5, vec3{ 1 ,1 ,10 }, -40, vec3(0, 1, 0));
	CreateInvisibleWall(164.5, 0.5f, 43.3, vec3{ 1 ,1 ,10 }, 0, vec3(0, 1, 0));
	CreateInvisibleWall(161.5, 0.5f, 34.5, vec3{ 1 ,1 ,10 }, 40, vec3(0, 1, 0));
	CreateInvisibleWall(153.5, 0.5f, 29, vec3{ 1 ,1 ,10 }, 70, vec3(0, 1, 0));
	
	CreateInvisibleWall(143, 0.5f, 38, vec3{ 1 ,1 ,10 }, 70, vec3(0, 1, 0));
	CreateInvisibleWall(144.3, 0.5f, 24.6, vec3{ 1 ,1 ,10 }, 60, vec3(0, 1, 0));
	CreateInvisibleWall(129.5, 0.5f, 16, vec3{ 1 ,1 ,25 }, 60, vec3(0, 1, 0));
	CreateInvisibleWall(115.5, 0.5f, 6, vec3{ 1 ,1 ,10 }, 40, vec3(0, 1, 0));
	
	CreateInvisibleWall(112.5, 0.5f, -3, vec3{ 1 ,1 ,10 }, 0, vec3(0, 1, 0));
	CreateInvisibleWall(115.5, 0.5f, -12, vec3{ 1 ,1 ,10 }, -40, vec3(0, 1, 0));
	CreateInvisibleWall(126.5, 0.5f, -17, vec3{ 1 ,1 ,15 }, -80, vec3(0, 1, 0));


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


	CreateCubeToBuldings(0, 15.5, 0, +45, vec3(0, 0, 1), White, 0.5, 0.5, 21); //roof1
	CreateCubeToBuldings(-2, 15, 0, +15, vec3(0, 0, 1), grey, 5, 0.25f, 20); //roof1
	CreateCubeToBuldings(+2, 15, 0, -15, vec3(0, 0, 1), grey, 5, 0.25f, 20); //roof2
	CreateCubeToBuldings(-6, 13, 0, +30, vec3(0, 0, 1), grey, 5, 0.25f, 20); //roof3
	CreateCubeToBuldings(+6, 13, 0, -30, vec3(0, 0, 1), grey, 5, 0.25f, 20); //roof4

	CreateCubeToBuldings(-8.3, 10.8, 0, +60, vec3(0, 0, 1), grey, 3, 0.25f, 20); //roof5
	CreateCubeToBuldings(+8.3, 10.8, 0, -60, vec3(0, 0, 1), grey, 3, 0.25f, 20); //roof6

							 //---------------- w1

	CreateCubeToBuldings(+8.3, 5, -8.5, 90, vec3(0, 0, 1), White, 11, 0.25f, 0.5); //wall1

	float j = -7.7;

	for (int i = 0; i < 15; i++) {

		CreateCubeToBuldings(+8.3, 5, j, 90, vec3(0, 0, 1), Red, 11, 0.25f, 1); //wall1
		j += 1.1;
	}
	CreateCubeToBuldings(+8.4, 5, +0, 90, vec3(0, 0, 1), White, 10.5, 0.7, 0.5); //wall1
	CreateCubeToBuldings(+8.3, 5, +8.5, 90, vec3(0, 0, 1), White, 11, 0.25f, 0.5);
	//---------------- w1
	//---------------- w2
	CreateCubeToBuldings(-8.3, 5, -8.5, 90, vec3(0, 0, 1), White, 11, 0.25f, 0.5); //wall2

	j = -7.7;

	for (int i = 0; i < 15; i++) {

		CreateCubeToBuldings(-8.3, 5, j, 90, vec3(0, 0, 1), Red, 11, 0.25f, 1); //wall2
		j += 1.1;
	}
	CreateCubeToBuldings(-8.4, 5, +0, 90, vec3(0, 0, 1), White, 10.5, 0.7, 0.5); //wall2
	CreateCubeToBuldings(-8.3, 5, +8.5, 90, vec3(0, 0, 1), White, 11, 0.25f, 0.5);
	//---------------- w2
	//---------------- w3
	CreateCubeToBuldings(+8.5, 5, -9, 90, vec3(1, 0, 0), White, 0.5, 0.25f, 10.5); //wall3

	CreateCubeToBuldings(+7.4, 5.5, -9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 13); //wall3
	CreateCubeToBuldings(+6.3, 6, -9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 13.25); //wall3
	CreateCubeToBuldings(+5.2, 6.35, -9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 13.75); //wall3
	CreateCubeToBuldings(+4.1, 6.75, -9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 14.75); //wall3

	j = +3;
	float w = 5, u = 12;
	for (int i = 0; i < 7; i++) {

		if (i < 3) {
			CreateCubeToBuldings(j, u, -9, 90, vec3(1, 0, 0), Red, 1, 0.25f, w); //wall3
			w += 0.25;	u += 0.125;
		}

		if (i >= 3 && i < 6) {
			CreateCubeToBuldings(j, u, -9, 90, vec3(1, 0, 0), Red, 1, 0.25f, w); //wall3	
			w -= 0.25;	u -= 0.125;
		}
		else
			CreateCubeToBuldings(j + 0.25, 12, -9, 90, vec3(1, 0, 0), Red, 0.6, 0.25f, 5); //wall3			
		j -= 1.1;
	}

	CreateCubeToBuldings(-7.5, 5.5, -9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 13); //wall3
	CreateCubeToBuldings(-6.4, 6, -9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 13.25); //wall3
	CreateCubeToBuldings(-5.3, 6.30, -9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 13.75); //wall3
	CreateCubeToBuldings(-4.2, 6.75, -9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 14.25); //wall3

	CreateCubeToBuldings(-8.5, 5, -9, 90, vec3(1, 0, 0), White, 0.5, 0.25f, 10.5); //wall3

																				   //---------------- w3

	CreateCubeToBuldings(0, 9.6, -9, +0, vec3(0, 0, 1), White, 12, 0.40, 0.40); //roof1
	CreateCubeToBuldings(-3.5, 4.75, -9.1, +90, vec3(0, 0, 1), White, 9.5, 0.30, 0.30); //roof1
	CreateCubeToBuldings(3.5, 4.75, -9.1, +90, vec3(0, 0, 1), White, 9.5, 0.30, 0.30); //roof1
																					   //---------------- w3
	CreateCubeToBuldings(-2, 14.8, -9.1, +15, vec3(0, 0, 1), White, 4.75, 0.25f, 0.5); //roof1
	CreateCubeToBuldings(+2, 14.8, -9.1, -15, vec3(0, 0, 1), White, 4.75, 0.25f, 0.5); //roof2
	CreateCubeToBuldings(-6, 12.8, -9.1, +30, vec3(0, 0, 1), White, 5, 0.25f, 0.5); //roof3
	CreateCubeToBuldings(+6, 12.8, -9.1, -30, vec3(0, 0, 1), White, 5, 0.25f, 0.5); //roof4

	CreateCubeToBuldings(-8.3, 10.6, -9.1, +60, vec3(0, 0, 1), White, 3, 0.25f, 0.5); //roof5
	CreateCubeToBuldings(+8.3, 10.6, -9.1, -60, vec3(0, 0, 1), White, 3, 0.25f, 0.5); //roof6
																					  //---------------- w3



																					  //---------------- w4
	CreateCubeToBuldings(+8.5, 5, +9, 90, vec3(1, 0, 0), White, 0.5, 0.25f, 10.5); //wall4

	CreateCubeToBuldings(+7.4, 5.5, +9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 13); //wall4
	CreateCubeToBuldings(+6.3, 6, +9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 13.25); //wall4
	CreateCubeToBuldings(+5.2, 6.35, +9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 13.75); //wall4
	CreateCubeToBuldings(+4.1, 6.75, +9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 14.75); //wall4

	j = +3;
	w = 5;
	u = 12;
	for (int i = 0; i < 7; i++) {

		if (i < 3) {
			CreateCubeToBuldings(j, u, +9, 90, vec3(1, 0, 0), Red, 1, 0.25f, w); //wall4
			w += 0.25;	u += 0.125;
		}

		if (i >= 3 && i < 6) {
			CreateCubeToBuldings(j, u, +9, 90, vec3(1, 0, 0), Red, 1, 0.25f, w); //wall4
			w -= 0.25;	u -= 0.125;
		}
		else
			CreateCubeToBuldings(j + 0.25, 12, +9, 90, vec3(1, 0, 0), Red, 0.6, 0.25f, 5); //wall4	
		j -= 1.1;
	}

	CreateCubeToBuldings(-7.5, 5.5, +9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 13); //wall4
	CreateCubeToBuldings(-6.4, 6, +9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 13.25); //wall4
	CreateCubeToBuldings(-5.3, 6.30, +9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 13.75); //wall4
	CreateCubeToBuldings(-4.2, 6.75, +9, 90, vec3(1, 0, 0), Red, 1, 0.25f, 14.25); //wall3

	CreateCubeToBuldings(-8.5, 5, +9, 90, vec3(1, 0, 0), White, 0.5, 0.25f, 10.5); //wall4

																				   //---------------- w4

	CreateCubeToBuldings(0, 9.6, +9, +0, vec3(0, 0, 1), White, 12, 0.40, 0.40); //roof1
	CreateCubeToBuldings(-3.5, 4.75, +9.1, +90, vec3(0, 0, 1), White, 9.5, 0.30, 0.30); //roof1
	CreateCubeToBuldings(3.5, 4.75, +9.1, +90, vec3(0, 0, 1), White, 9.5, 0.30, 0.30); //roof1
																					   //---------------- w4
	CreateCubeToBuldings(-2, 14.8, +9.1, +15, vec3(0, 0, 1), White, 4.75, 0.25f, 0.5); //roof1
	CreateCubeToBuldings(+2, 14.8, +9.1, -15, vec3(0, 0, 1), White, 4.75, 0.25f, 0.5); //roof2
	CreateCubeToBuldings(-6, 12.8, +9.1, +30, vec3(0, 0, 1), White, 5, 0.25f, 0.5); //roof3
	CreateCubeToBuldings(+6, 12.8, +9.1, -30, vec3(0, 0, 1), White, 5, 0.25f, 0.5); //roof4

	CreateCubeToBuldings(-8.3, 10.6, +9.1, +60, vec3(0, 0, 1), White, 3, 0.25f, 0.5); //roof5
	CreateCubeToBuldings(+8.3, 10.6, +9.1, -60, vec3(0, 0, 1), White, 3, 0.25f, 0.5); //roof6
																					  //---------------- w4





}
void ModuleSceneIntro::CreateBridge(float x, float y, float z)
{

	CreateCubeToBuldings(x, y + 1.5, z, 20, vec3(PUTVERTICALxy), grey, 8, 0.25f, 10);
	CreateInvisibleWall(x, y + 1.5, z, vec3{ 8, 0.25f, 10 }, 20, vec3(PUTVERTICALxy));


	CreateCubeToBuldings(x + 20, y + 1.5, z, -20, vec3(PUTVERTICALxy), grey, 8, 0.25f, 10);
	CreateInvisibleWall(x + 20, y + 1.5, z, vec3{ 8, 0.25f, 10 }, -20, vec3(PUTVERTICALxy));

	CreateCubeToBuldings(x + 10, y + 2.95, z + 5, 0, vec3(PUTVERTICALxy), grey, 12.5f, 0.25f, 0.5f);
	CreateCubeToBuldings(x + 10, y + 2.95, z + 4, 0, vec3(PUTVERTICALxy), grey, 12.5f, 0.25f, 0.5f);
	CreateCubeToBuldings(x + 10, y + 2.95, z + 3, 0, vec3(PUTVERTICALxy), grey, 12.5f, 0.25f, 0.5f);
	CreateCubeToBuldings(x + 10, y + 2.95, z + 2, 0, vec3(PUTVERTICALxy), grey, 12.5f, 0.25f, 0.5f);
	CreateCubeToBuldings(x + 10, y + 2.95, z + 1, 0, vec3(PUTVERTICALxy), grey, 12.5f, 0.25f, 0.5f);
	CreateCubeToBuldings(x + 10, y + 2.95, z + 0, 0, vec3(PUTVERTICALxy), grey, 12.5f, 0.25f, 0.5f);
	CreateCubeToBuldings(x + 10, y + 2.95, z - 5, 0, vec3(PUTVERTICALxy), grey, 12.5f, 0.25f, 0.5f);
	CreateCubeToBuldings(x + 10, y + 2.95, z - 4, 0, vec3(PUTVERTICALxy), grey, 12.5f, 0.25f, 0.5f);
	CreateCubeToBuldings(x + 10, y + 2.95, z - 3, 0, vec3(PUTVERTICALxy), grey, 12.5f, 0.25f, 0.5f);
	CreateCubeToBuldings(x + 10, y + 2.95, z - 2, 0, vec3(PUTVERTICALxy), grey, 12.5f, 0.25f, 0.5f);
	CreateCubeToBuldings(x + 10, y + 2.95, z - 1, 0, vec3(PUTVERTICALxy), grey, 12.5f, 0.25f, 0.5f);

	CreateInvisibleWall(x + 10, y + 2.90, z, vec3{ 12, 0.25f, 10 }, 0, vec3(PUTVERTICALxy));


	CreateCubeToBuldings(x + 10, y + 2.95, z - 2, 0, vec3(PUTVERTICALxy), grey, 12.5f, 0.25f, 0.5f); //roof5


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
		
		Poste->SetPos(Position.x + (Postes_distance*i - distance / 2)*magicX, Position.y, Position.z + (Postes_distance*i - distance / 2)*magicZ);
		Poste->color = Brown;

		postes.add(Poste);
		i++;
	} while (Postes_num >= i);
}
void ModuleSceneIntro::CreateFences()
{
	CreateFence(36, 2, vec3(54, 1, 23), vec3(0, 1, 0), 0, 1, 0);
	CreateInvisibleWall(54, 1, 23, vec3{ 36,2,0.5f }, 0, vec3{ 0,1,0 });


	CreateFence(36, 2, vec3(54, 1, 78), vec3(0, 1, 0), 0, 1, 0);
	CreateInvisibleWall(54, 1, 78, vec3{ 36,2,0.5f }, 0, vec3{ 0,1,0 });


	CreateFence(42, 2, vec3(36, 1, 44), vec3(0, 1, 0), 90, 0, 1);
	CreateInvisibleWall(36, 1, 44, vec3{ 42,2,0.5f }, 90, vec3{ 0,1,0 });

	CreateFence(42, 2, vec3(72, 1, 57), vec3(0, 1, 0), 90, 0, 1);
	CreateInvisibleWall(72, 1, 57, vec3{ 42,2,0.5f }, 90, vec3{ 0,1,0 });

	CreateFence(6, 2, vec3(36, 1, 75), vec3(0, 1, 0), 90, 0, 1);
	CreateInvisibleWall(36, 1, 75, vec3{ 6,2,0.5f }, 90, vec3{ 0,1,0 });
	CreateFence(6, 2, vec3(72, 1, 26), vec3(0, 1, 0), 90, 0, 1);
	CreateInvisibleWall(72, 1, 26, vec3{ 6,2,0.5f }, 90, vec3{ 0,1,0 });
	//RECT2
	CreateFence(24, 2, vec3(103.25, 1, 72.5), vec3(0, 1, 0), 0, 1, 0);
	CreateInvisibleWall(103.25, 1, 72.5, vec3{ 22,2,0.5f }, 0, vec3{ 0,1,0 });

	CreateFence(32, 2, vec3(99.75, 1, 83), vec3(0, 1, 0), 0, 1, 0);
	CreateInvisibleWall(99.75, 1, 83, vec3{ 32,2,0.5f }, 0, vec3{ 0,1,0 });

	CreateFence(12, 2, vec3(145, 1, 83), vec3(0, 1, 0), 0, 1, 0);
	CreateInvisibleWall(145, 1, 83, vec3{ 12,2,0.5f }, 0, vec3{ 0,1,0 });

	CreateFence(64, 2, vec3(165, 1, 72.5), vec3(0, 1, 0), 0, 1, 0);
	CreateInvisibleWall(165, 1, 72.5, vec3{ 64,2,0.5f }, 0, vec3{ 0,1,0 });
	CreateFence(30, 2, vec3(180, 1, 83), vec3(0, 1, 0), 0, 1, 0);
	CreateInvisibleWall(180, 1, 83, vec3{ 30,2,0.5f }, 0, vec3{ 0,1,0 });

	CreateFence(150, 2, vec3(-10, 1, 10), vec3(0, 1, 0), 90, 0, 1);
	CreateInvisibleWall(-10, 1, 10, vec3{ 150,2,0.5f }, 90, vec3{ 0,1,0 });
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
	for (float i = 0, j = 9, w = 63; i < 8; i++) {
		CreateBale(j, 0.5f, w, 90, vec3{ 0,1,0 });
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

	
	
	//13 L
	//CURVE1
	CreateBale(75, 0.5f, 29, 90, vec3{ 0,1,0 });
	CreateBale(78, 0.5f, 29.5f, 70, vec3{ 0,1,0 });
	CreateBale(81, 0.5f, 31, 50, vec3{ 0,1,0 });
	CreateBale(82.5, 0.5f, 33.5, 20, vec3{ 0,1,0 });
	CreateBale(83, 0.5f, 36.5, 0, vec3{ 0,1,0 });
	//RECT1
	for (float j = 39.5; j < 68; j++) {
		CreateBale(83, 0.5f, j, 0, vec3{ 0,1,0 });
		j += 2;
	}
	//CURV2
	CreateBale(84, 0.5f, 69.5, 40, vec3{ 0,1,0 });
	CreateBale(86.5, 0.5f, 71.5, 60, vec3{ 0,1,0 });
	CreateBale(89.5, 0.5f, 72.5, 90, vec3{ 0,1,0 });

	CreateBale(74, 0.5f, 79.5, 50, vec3{ 0,1,0 });
	CreateBale(76.5, 0.5f, 81.5, 70, vec3{ 0,1,0 });
	CreateBale(79.5, 0.5f, 82.5, 80, vec3{ 0,1,0 });
	CreateBale(82.5, 0.5f, 83, 90, vec3{ 0,1,0 });

	//CURV3

	CreateBale(160.5, 0.5f, 74.5, 50, vec3{ 0,1,0 });
	CreateBale(162, 0.5f, 77, 20, vec3{ 0,1,0 });
	CreateBale(163, 0.5f, 80, 15, vec3{ 0,1,0 });
	CreateBale(163.5, 0.5f, 83, 10, vec3{ 0,1,0 });
	CreateBale(164, 0.5f, 86, 5, vec3{ 0,1,0 });
	CreateBale(164, 0.5f, 89, 0, vec3{ 0,1,0 });

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


	for (int j = -12.5; j >-60; j--) {
		CreateBale(3.5, 0.5f, j, 0, vec3{ 1,0,0 });
		j -= 2;
	}
	//last rect ext

	for (int j = -12.5; j >-76.5; j--) {
		CreateBale(-3.5, 0.5f, j, 0, vec3{ 1,0,0 });
		j -= 2;
	}

	for (int j = -3.5; j <25; j++) {
		CreateBale(j, 0.5f, -76.2, 90, vec3{ 0,1,0 });
		j += 2;
	}

	//last curv int
	CreateBale(5, 0.5f, -61, 135, vec3{ 0,1,0 });
	CreateBale(7, 0.5f, -62.5, 120, vec3{ 0,1,0 });
	CreateBale(10, 0.5f, -63.2, 90, vec3{ 0,1,0 });

	for (int j = 13; j <27; j++) {
		CreateBale(j, 0.5f, -63.2, 90, vec3{ 0,1,0 });
		j += 2;
	}
	// Jump 
	for (int j = -47; j <-30; j++) {
		CreateBale(148, 0.5f, j, 0, vec3{ 0,1,0 });
		j += 2;
	}
	
	//exerior
	for (int j = -47; j <-25; j++) {
		CreateBale(159.5, 0.5f, j, 0, vec3{ 0,1,0 });
		j += 2;
	}


	CreateBale(159, 0.5f, -23.5, -20, vec3{ 0,1,0 });//+3
	CreateBale(157.5, 0.5f, -21, -40, vec3{ 0,1,0 });//+2.5
	CreateBale(155.5, 0.5f, -19, -50, vec3{ 0,1,0 });//+2
	CreateBale(153, 0.5f, -17.5, -70, vec3{ 0,1,0 });//+1.5
	CreateBale(150, 0.5f, -17, -90, vec3{ 0,1,0 });//+1.5

	for (int j = 147; j >138; j--) {
		CreateBale(j, 0.5f, -17, 90, vec3{ 0,1,0 });
		j -= 2;
	}

	CreateBale(138, 0.5f, -17.5, -100, vec3{ 0,1,0 });
	CreateBale(135.5, 0.5f, -17.8, -100, vec3{ 0,1,0 });


}
void ModuleSceneIntro::CreateGreenBale(const float x, const float y, const float z, const float angle, const vec3& rotationAxis, float sizeX, float sizeY, float sizeZ) {

	Cube* bush = new Cube(sizeX, sizeY, sizeZ);

	bush->SetPos(x, y, z);
	bush->SetRotation(angle, rotationAxis);
	bush->color = Green;
	greenbales.add(bush);

	greenbales_body.add(App->physics->AddBody(*bush, 0));
}
void ModuleSceneIntro::CreateGreenBales() {
	//INTERIOR

	CreateGreenBale(120, 0.5f, -61.5, 90.5, vec3{ 0,1,0 }, 1, 1.5f, 80);
	CreateGreenBale(77, 0.5f, -43, -10, vec3{ 0,1,0 }, 1, 1.5f, 36);
	CreateGreenBale(59, 0.5f, -25, 90, vec3{ 0,1,0 }, 1, 1.5f, 30);


	CreateGreenBale(34, 0.5f, -35.5, 45, vec3{ 0,1,0 }, 1, 1.5f, 30);

	CreateGreenBale(23, 0.5f, -54.5, 0, vec3{ 0,1,0 }, 1, 1.5f, 17);


	//EXTERIOR
	CreateGreenBale(120, 0.5f, -75, 89.5, vec3{ 0,1,0 }, 1, 1.5f, 80);
	CreateGreenBale(77, 0.5f, -94, 10, vec3{ 0,1,0 }, 1, 1.5f, 36);
	CreateGreenBale(59, 0.5f, -112, 90, vec3{ 0,1,0 }, 1, 1.5f, 30);

	CreateGreenBale(34, 0.5f, -102, -45, vec3{ 0,1,0 }, 1, 1.5f, 30);
	CreateGreenBale(23, 0.5f, -83.5, 0, vec3{ 0,1,0 }, 1, 1.5f, 15);


}

//================
void ModuleSceneIntro::CreateTree(const float x, const float y, const float z, const float tall, const float radious)
{

	TreeTop* ttop = new TreeTop(radious, (tall / 4) * 3);

	ttop->SetRotation(90, vec3(0, 0, 1));
	ttop->SetPos(x, y + (tall / 4) * 2 + 1.5f, z);
	ttop->color = Green;

	Cylinder* ttrunk = new Cylinder(0.175f*radious, (tall / 4));

	ttrunk->SetRotation(90, vec3(0, 0, 1));
	ttrunk->SetPos(x, y + 1.5f, z);
	ttrunk->color = Brown;

	trees_top.add(ttop);
	trees_trunk.add(ttrunk);

	trees_trunk_body.add(App->physics->AddBody((*ttrunk), 0));
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

	CreateTree(90, 0, 2, 15, 2);
	CreateTree(-10, 0, 10, 15, 2);

	CreateTree(110, 0, 50, 15, 2);
	CreateTree(127, 0, 43, 15, 2);

	CreateTree(177, 0, -5, 15, 2);
	CreateTree(186, 0, -10, 15, 2);

	CreateTree(167, 0, 2, 15, 2);
	CreateTree(150, 0, 7, 15, 2);

	CreateTree(130, 0, 58, 15, 2);
	CreateTree(117, 0, 62, 15, 2);

	CreateTree(130, 0, 110, 15, 2);
	CreateTree(187, 0, 65, 15, 2);

	CreateTree(50, 0, 80, 15, 2);
	CreateTree(56, 0, 80, 15, 2);

	//casa -----------------------------
	CreateTree(160, 0, -59, 15, 2);
	CreateTree(145, 0, -59, 15, 2);
	CreateTree(130, 0, -59, 15, 2);
	CreateTree(115, 0, -59, 15, 2);
	CreateTree(100, 0, -59, 15, 2);
	CreateTree(85, 0, -59, 15, 2);
	CreateTree(76, 0, -25, 15, 2);
	CreateTree(38, 0, -34.5, 15, 2);
	CreateTree(28, 0, -45, 15, 2);

	CreateTree(160, 0, -77.5, 15, 2);
	CreateTree(145, 0, -77.5, 15, 2);
	CreateTree(130, 0, -77.5, 15, 2);
	CreateTree(115, 0, -77.5, 15, 2);
	CreateTree(100, 0, -77.5, 15, 2);
	CreateTree(85, 0, -77.5, 15, 2);
	CreateTree(76, 0, -112, 15, 2);
	CreateTree(38, 0, -102.5, 15, 2);
	CreateTree(27.5, 0, -92.5, 15, 2);

	
	

}
//================
void ModuleSceneIntro::CreatePlane(const float x, const float y, const float z, const float width, const float height, Color colorr, const float angle, const vec3& rotationAxis)
{
	Plane* plane = new Plane(x, y, z, width, height);

	plane->SetRotation(angle, rotationAxis);
	plane->SetPos(x, y, z);
	plane->color = colorr;
	Planos.add(plane);
}
void ModuleSceneIntro::CreatePlanes()
{
	CreatePlane(-400, 0, -400, 800, 800, Green_Ground, 0, vec3(0, 1, 0));

	CreatePlane(-3.5, 0.1, -73, 7, 280, Brown_way, 0, vec3(PUTHORIZONALxz));
	CreatePlane(-3, 0.1, 64, 41, 9, Brown_way, 0, vec3(PUTHORIZONALxz));
	CreatePlane(-3, 0.1, -74, 35, 9, Brown_way, 0, vec3(PUTHORIZONALxz));
	CreatePlane(73, 0.1, 73.5, 39, 9, Brown_way, 0, vec3(PUTHORIZONALxz));
	CreatePlane(137, 0.1, 73.5, 70, 9, Brown_way, 0, vec3(PUTHORIZONALxz));
	

	CreatePlane(35, 0.15, -39, 60, 30, Grey2, 90, vec3(PUTHORIZONALxz));
	CreatePlane(50, 0.15, -74, 180, 12, Grey2, 0, vec3(PUTHORIZONALxz));
	CreatePlane(130, 0.1, 100, 11,90 , Blue, 90, vec3(PUTHORIZONALxz));
	CreatePlane(130, 0.1,100, 25, 11, Blue, 120, vec3(PUTHORIZONALxz));
	CreatePlane(120, 0.1, 85, 20, 11, Blue, 85, vec3(PUTHORIZONALxz));
	CreatePlane(122, 0.1, 66, 11, 11, Blue, 45, vec3(PUTHORIZONALxz));
	CreatePlane(126, 0.1, 61, 25, 11, Blue, 30, vec3(PUTHORIZONALxz));
	CreatePlane(135, 0.1, 54, 25, 9, Blue, 20, vec3(PUTHORIZONALxz));

	CreatePlane(120, 0.1, 10, 35, 13, Blue,-30, vec3(PUTHORIZONALxz));
	CreatePlane(115, 0.1 ,7, 10, 12, Blue, -40, vec3(PUTHORIZONALxz));
	CreatePlane(113, 0.1, 3, 10, 12, Blue, -50, vec3(PUTHORIZONALxz));

	CreatePlane(148, 0.1, 33, 15, 20, Blue, 0, vec3(PUTHORIZONALxz));
	CreatePlane(148, 0.1, 25, 15, 12, Blue, -30, vec3(PUTHORIZONALxz));

	CreatePlane(112, 0.1, -2, 10, 12, Blue, -70, vec3(PUTHORIZONALxz));
	CreatePlane(112, 0.1, -10, 16, 12, Blue, -90, vec3(PUTHORIZONALxz));
	CreatePlane(100, 0.1, -10, 16, 12, Blue, 50, vec3(PUTHORIZONALxz));
	CreatePlane(112, 0.1, -23, 25, 9, Blue, 10, vec3(PUTHORIZONALxz));
	CreatePlane(132, 0.1, -53, 15, 35, Blue, 10, vec3(PUTHORIZONALxz));
	CreatePlane(132, 0.1, -53, 9, 80, Blue, 90, vec3(PUTHORIZONALxz));
	//sky
	CreatePlane(200, 0, -200, 600, 600, Blue, 90, vec3(PUTVERTICALxy));
	CreatePlane(-200, +400, -200, 600, 600, Blue, 270, vec3(PUTVERTICALxy));
	CreatePlane(-200, 0, 200, 400, 600, Blue, 270, vec3(PUTVERTICALzy));
	CreatePlane(-200, +400, -200, 600, 600, Blue, 90, vec3(PUTVERTICALzy));

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

	PhysBody3D* bod = App->physics->AddBody((*cube), 200);
	CowCorps_body.add(bod);

	// -----------------------------------------------------------

	Cube* head = new Cube(1, 1, 1);

	head->color = White;

	head->SetPos(x + 1, y + 1.6, z);
	head->SetRotation(angle, { 0,1,0 });
	Cow_corps.add(head);

	PhysBody3D* headb = App->physics->AddBody((*cube), 1);
	CowCorps_body.add(headb);

	App->physics->AddConstraintP2P(*bod, *headb, { 1.0f,0,0.25f }, { -0.45f,-0.5f,0 });

	// -----------------------------------------------------------


	Cylinder* leg1 = new Cylinder(0.25, 0.75);

	int nrandom2;
	nrandom2 = rand() % 5;

	if (nrandom2 == 1 || nrandom2 == 0 || nrandom2 == 5)
		leg1->color = White;
	else if (nrandom2 == 2)
		leg1->color = Brown;
	else
		leg1->color = Black;

	leg1->SetPos(x - 0.35f, y + 0.25f, z - 0.35);
	leg1->SetRotation(90, { 0,0,1 });

	Cow_legs.add(leg1);
	PhysBody3D* legg1 = App->physics->AddBody(*leg1, 200);
	CowLegs_body.add(legg1);
	App->physics->AddConstraintP2P(*bod, *legg1, { -0.35f,-0.5f,-0.35f }, { +0.7f,0,0 });

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

	leg2->SetPos(x + 0.35f, y + 0.25f, z - 0.35f);
	leg2->SetRotation(90, { 0,0,1 });

	Cow_legs.add(leg2);
	PhysBody3D* legg2 = App->physics->AddBody(*leg2, 200);


	CowLegs_body.add(legg2);
	App->physics->AddConstraintP2P(*bod, *legg2, { +0.35f,-0.5f,-0.35f }, { 0.7f,0,0 });

	// -----------------------------------------------------------

	Cylinder* leg3 = new Cylinder(0.25, 0.75);

	nrandom = rand() % 5;

	if (nrandom == 1 || nrandom == 0 || nrandom == 5)
		leg3->color = White;
	else if (nrandom == 2)
		leg3->color = Brown;
	else
		leg3->color = Black;

	leg3->SetPos(x + 0.35f, y + 0.25f, z + 0.35f);
	leg3->SetRotation(90, { 0,0,1 });
	Cow_legs.add(leg3);
	PhysBody3D* legg3 = App->physics->AddBody(*leg3, 200);
	CowLegs_body.add(legg3);

	App->physics->AddConstraintP2P(*bod, *legg3, { +0.35f,-0.5f,+0.35f }, { 0.7f,0,0 });

	// -----------------------------------------------------------

	Cylinder* leg4 = new Cylinder(0.25, 0.75);

	nrandom = rand() % 4;

	if (nrandom == 1 || nrandom == 0)
		leg4->color = White;
	else if (nrandom == 2)
		leg4->color = Brown;
	else
		leg4->color = Black;

	leg4->SetPos(x - 0.35f, y + 0.25f, z + 0.35f);
	leg4->SetRotation(90, { 0,0,1 });
	PhysBody3D* legg4 = App->physics->AddBody(*leg4, 200);
	Cow_legs.add(leg4);
	CowLegs_body.add(legg4);
	App->physics->AddConstraintP2P(*bod, *legg4, { -0.35f,-0.5f,+0.35f }, { 0.7f,0,0 });

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
void ModuleSceneIntro::CreateInvisibleWall(const float x, const float y, const float z, const vec3 box, const float angle, const vec3 RotationAxis) {

	noCube* inv = new noCube(box.x, box.y, box.z);
	inv->SetPos(x, y, z);
	inv->SetRotation(angle, RotationAxis);


	invisibles.add(inv);
	invisible_bodies.add(App->physics->AddBody(*inv));

}
void ModuleSceneIntro::CreateInvisibleWalls()
{
	//farm
	CreateInvisibleWall(6, 0, 9, vec3{ 5,15,0.5f }, 0, vec3{ 1,0,0 });
	CreateInvisibleWall(-6, 0, 9, vec3{ 5,15,0.5f }, 0, vec3{ 1,0,0 });
	CreateInvisibleWall(6, 0, -9, vec3{ 5,15,0.5f }, 0, vec3{ 1,0,0 });
	CreateInvisibleWall(-6, 0, -9, vec3{ 5,15,0.5f }, 0, vec3{ 1,0,0 });

	CreateInvisibleWall(-8.5, 0, 0, vec3{ 18,15,0.5f }, 90, vec3{ 0,1,0 });
	CreateInvisibleWall(8.5, 0, 0, vec3{ 18,15,0.5f }, 90, vec3{ 0,1,0 });

}
//================
void ModuleSceneIntro::CreateExtras() {

	//jump house
	CreateCubeToBuldings(153.6, 1, -50, 15, vec3(PUTVERTICALzy), grey, 10, 0.25f, 6);
	CreateInvisibleWall(153.6, 1, -50, vec3{ 10, 0.25f, 6 }, 15, vec3(PUTVERTICALzy));
	CreateCylinderToBuldings(153.6, 0.5, -50, 0, vec3{ PUTVERTICALxy }, Brown2, 0.5, 11);
	CreateCylinderToBuldings(153.6, 0.5, -51, 0, vec3{ PUTVERTICALxy }, Brown2, 0.5, 11);
	CreateCylinderToBuldings(153.6, 0.5, -52, 0, vec3{ PUTVERTICALxy }, Brown2, 0.5, 12);
	CreateCylinderToBuldings(153.6, 1, -51.5, 0, vec3{ PUTVERTICALxy }, Brown2, 0.5, 10);
	
	//before ramp
	CreateCylinderToBuldings(139, 1, -28.3, 13, vec3{ 0,1,0 }, Brown2, 0.6, 16);
	CreateInvisibleWall(139, 1, -28.3f, vec3{ 1.2f ,1.2f ,16 }, 102, vec3(0,1,0));
	

	CreateCylinderToBuldings(44, 1, 1.5, 90, vec3{ PUTVERTICALxy }, Blue, 5, 0.25f);
	CreateCylinderToBuldings(34, 1, 1.5, 90, vec3{ PUTVERTICALxy }, Blue, 12, 0.25f);
	CreateCylinderToBuldings(38, 1, 6.5, 90, vec3{ PUTVERTICALxy }, Blue, 8, 0.25f);

	CreateCubeToBuldings(153.6, 1, -50, 17, vec3(PUTVERTICALzy), grey, 10, 0.25f, 6);

	//walls
	CreateCubeToBuldings(170, 2, -53, 0, vec3(PUTVERTICALxy), Grey2, 1.5f, 4, 20);
	CreateCubeToBuldings(170, 2, -104, 0, vec3(PUTVERTICALxy), Grey2, 1.5f, 4, 60);

	CreateCubeToBuldings(170.5, 3, -63, 0, vec3(PUTVERTICALxy), Grey2, 2, 6, 2);
	CreateCubeToBuldings(170.5, 3, -74, 0, vec3(PUTVERTICALxy), Grey2, 2, 6, 2);

	//campos
	CreateCubeToBuldings(-140, 2, -160, 0, vec3(PUTVERTICALxy), Yellow, 90, 4, 120);
	CreateCubeToBuldings(0, 2, -160, 0, vec3(PUTVERTICALxy), Yellow2, 150, 4, 70);
	CreateCubeToBuldings(125, 2, -140, 0, vec3(PUTVERTICALxy), Yellow, 70, 4.5, 100);
	CreateCubeToBuldings(-110, 1.5, +150, 0, vec3(PUTVERTICALxy), Yellow2, 200, 3, 90);
	CreateCubeToBuldings(-130, 1.5, 5, 0, vec3(PUTVERTICALxy), Yellow3, 200, 3.5, 180);
	CreateCubeToBuldings(170, 1.5, +150, 0, vec3(PUTVERTICALxy), Yellow2, 100, 3, 80);

	CreateCubeToBuldings(45, 2, +140, 0, vec3(PUTVERTICALxy), Yellow, 70, 4, 100);


	//invisible walls
	CreateInvisibleWall(0, 3, 130.75f, vec3{ 400 ,6 ,0.25f }, 0, vec3(PUTVERTICALzy));
	CreateInvisibleWall(169.75f, 3, 0, vec3{ 0.25f,6 ,400 }, 0, vec3(PUTVERTICALzy));
	CreateInvisibleWall(-20, 3, 0, vec3{ 0.25f,6 ,400 }, 0, vec3(PUTVERTICALzy));
	CreateInvisibleWall(0, 3, -130.75f, vec3{ 400 ,6 ,0.25f }, 0, vec3(PUTVERTICALzy));

	CreateInvisibleWall(115, 3, -75, vec3{ 70 ,6 ,0.25f }, 0, vec3(PUTVERTICALzy));

	//river
	CreateCylinderToBuldings(142, 1, 27, 60, vec3{ 0,1,0 }, Brown2, 0.5, 5);
	CreateCylinderToBuldings(131, 1, 29, 60, vec3{ 0,1,0 }, Brown2, 0.5, 5);
	CreateCylinderToBuldings(125, 1, 18, 60, vec3{ 0,1,0 }, Brown2, 0.5, 5);

	CreateCylinderToBuldings(108.5, 1, -2, 0, vec3{ 0,1,0 }, Brown2, 0.5, 5);
	CreateCylinderToBuldings(108.5, 1, -15, -40, vec3{ 0,1,0 }, Brown2, 0.5, 5);

	CreateInvisibleWall(142, 1, 27, vec3{ 1 ,1 ,5}, -30, vec3(0,1,0));
	CreateInvisibleWall(131, 1, 29, vec3{ 1 ,1 ,5 }, -30, vec3(0, 1, 0));
	CreateInvisibleWall(125, 1, 18, vec3{ 1 ,1 ,5 }, -30, vec3(0, 1, 0));
	CreateInvisibleWall(108.5, 1, -2, vec3{ 1 ,1 ,5 }, 90, vec3(0, 1, 0));
	CreateInvisibleWall(108.5, 1, -15, vec3{ 1 ,1 ,5 }, 52, vec3(0, 1, 0));

	
	//RIVER
	CreateInvisibleWall(124.5, 0.5f, 90, vec3{ 1 ,1 ,10 }, 30, vec3(0, 1, 0));
	CreateInvisibleWall(120.8, 0.5f, 80.5, vec3{ 1 ,1 ,10 }, 10, vec3(0, 1, 0));
	CreateInvisibleWall(121, 0.5f, 70.5, vec3{ 1 ,1 ,10 }, -15, vec3(0, 1, 0));
	CreateInvisibleWall(125.8, 0.5f, 61.7, vec3{ 1 ,1 ,10 }, -40, vec3(0, 1, 0));
	CreateInvisibleWall(133.5, 0.5f, 55.2, vec3{ 1 ,1 ,10 }, -60, vec3(0, 1, 0));
	CreateInvisibleWall(142.7, 0.5f, 51, vec3{ 1 ,1 ,10 }, -70, vec3(0, 1, 0));
	CreateInvisibleWall(148.2, 0.5f, 44.5, vec3{ 1 ,1 ,10 }, 0, vec3(0, 1, 0));

	CreateInvisibleWall(143, 0.5f, 38, vec3{ 1 ,1 ,10 }, 70, vec3(0, 1, 0));
	CreateInvisibleWall(127.5, 0.5f, 30, vec3{ 1 ,1 ,25}, 60, vec3(0, 1, 0));
	CreateInvisibleWall(111, 0.5f, 18.8, vec3{ 1 ,1 ,15 }, 50, vec3(0, 1, 0));
	CreateInvisibleWall(103, 0.5f, 10, vec3{ 1 ,1 ,10 }, 30, vec3(0, 1, 0));

	CreateInvisibleWall(100.5, 0.5f, -2.5, vec3{ 1 ,1 ,16 }, 0, vec3(0, 1, 0));
	CreateInvisibleWall(106, 0.5f, -17.3, vec3{ 1 ,1 ,17 }, -40, vec3(0, 1, 0));
	CreateInvisibleWall(121, 0.5f, -25, vec3{ 1 ,1 ,20 }, -80, vec3(0, 1, 0));	
	


	CreateInvisibleWall(135.8, 0.5f, 68.5, vec3{ 1 ,1 ,10 }, -40, vec3(0, 1, 0));
	CreateInvisibleWall(143.5, 0.5f, 62.2, vec3{ 1 ,1 ,10 }, -60, vec3(0, 1, 0));
	CreateInvisibleWall(152.7, 0.5f, 58, vec3{ 1 ,1 ,10 }, -75, vec3(0, 1, 0));
	CreateInvisibleWall(161, 0.5f, 52.5, vec3{ 1 ,1 ,10 }, -40, vec3(0, 1, 0));
	CreateInvisibleWall(164.5, 0.5f, 43.3, vec3{ 1 ,1 ,10 }, 0, vec3(0, 1, 0));
	CreateInvisibleWall(161.5, 0.5f, 34.5, vec3{ 1 ,1 ,10 }, 40, vec3(0, 1, 0));
	CreateInvisibleWall(153.5, 0.5f, 29, vec3{ 1 ,1 ,10 }, 70, vec3(0, 1, 0));

	CreateInvisibleWall(143, 0.5f, 38, vec3{ 1 ,1 ,10 }, 70, vec3(0, 1, 0));
	CreateInvisibleWall(144.3, 0.5f, 24.6, vec3{ 1 ,1 ,10 }, 60, vec3(0, 1, 0));
	CreateInvisibleWall(129.5, 0.5f, 16, vec3{ 1 ,1 ,25 }, 60, vec3(0, 1, 0));
	CreateInvisibleWall(115.5, 0.5f, 6, vec3{ 1 ,1 ,10 }, 40, vec3(0, 1, 0));

	CreateInvisibleWall(112.5, 0.5f, -3, vec3{ 1 ,1 ,10 }, 0, vec3(0, 1, 0));
	CreateInvisibleWall(115.5, 0.5f, -12, vec3{ 1 ,1 ,10 }, -40, vec3(0, 1, 0));
	CreateInvisibleWall(126.5, 0.5f, -17, vec3{ 1 ,1 ,15 }, -80, vec3(0, 1, 0));
	
}