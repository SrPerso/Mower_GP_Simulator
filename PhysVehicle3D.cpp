#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{

}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
	body->setUserPointer(this);
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Black;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	//chassis base
	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);


	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();

	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();
	
	// motor
	Cube motor(info.chassis_size.x*0.8f, info.chassis_size.y *2.4f, info.chassis_size.z*0.55f);

	btVector3 offsetMotor(info.Motor_offset.x, info.Motor_offset.y, info.Motor_offset.z);
	offsetMotor = offsetMotor.rotate(q.getAxis(), q.getAngle());

	vehicle->getChassisWorldTransform().getOpenGLMatrix(&motor.transform);
	
	motor.transform.M[12] += offsetMotor.getX();
	motor.transform.M[13] += offsetMotor.getY();
	motor.transform.M[14] += offsetMotor.getZ();

	motor.color = Green_Ground;

	//sit

	Cube SitUp(info.chassis_size.x*0.9f, info.chassis_size.y *3.0f, info.chassis_size.z*0.1f);

	btVector3 offsetSit(info.Sit_offset.x, info.Sit_offset.y, info.Sit_offset.z);
	offsetSit = offsetSit.rotate(q.getAxis(), q.getAngle());

	vehicle->getChassisWorldTransform().getOpenGLMatrix(&SitUp.transform);

	SitUp.transform.M[12] += offsetSit.getX();
	SitUp.transform.M[13] += offsetSit.getY();
	SitUp.transform.M[14] += offsetSit.getZ();

	SitUp.color = grey;


	Cube SitDown(info.chassis_size.x*0.9f, info.chassis_size.y *0.7f, info.chassis_size.z*0.3f);

	btVector3 offsetSitd(info.Sit2_offset.x, info.Sit2_offset.y, info.Sit2_offset.z);
	offsetSitd = offsetSitd.rotate(q.getAxis(), q.getAngle());

	vehicle->getChassisWorldTransform().getOpenGLMatrix(&SitDown.transform);

	SitDown.transform.M[12] += offsetSitd.getX();
	SitDown.transform.M[13] += offsetSitd.getY();
	SitDown.transform.M[14] += offsetSitd.getZ();

	SitDown.color = grey;




	Cube bin(info.chassis_size.x*1.1f, info.chassis_size.y *3.2f, info.chassis_size.z*0.55f);

	btVector3 offsetbin(info.bin_offset.x, info.bin_offset.y, info.bin_offset.z);
	offsetbin = offsetbin.rotate(q.getAxis(), q.getAngle());

	vehicle->getChassisWorldTransform().getOpenGLMatrix(&bin.transform);

	bin.transform.M[12] += offsetbin.getX();
	bin.transform.M[13] += offsetbin.getY();
	bin.transform.M[14] += offsetbin.getZ();

	bin.color = Green_Ground;


	bin.Render();
	SitDown.Render();
	SitUp.Render();
	motor.Render();
	chassis.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}