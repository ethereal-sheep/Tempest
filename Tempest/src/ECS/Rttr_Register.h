#pragma once
#include "Util.h"
#include "TMath.h"
#include "ECS\Entity.h"

#include "Components/Components.h"
#include "rttr/type.h"
#include "rttr/registration.h"

namespace Tempest
{
	struct test
	{
		//static const char* get_type() { return "Test"; }
		int mem1;

	};
	RTTR_REGISTRATION
	{
		using namespace rttr;
	registration::class_<test>("Test")
		.constructor<>()
		.property("mem1", &test::mem1);

	registration::class_<tc::Example>(tc::Example::get_type())
		.constructor<>()
		.property("Member1", &tc::Example::member1)
		.property("Member2", &tc::Example::member2)
		.property("Member3", &tc::Example::member3)
		.property("Member4", &tc::Example::member4);

	registration::class_<tc::Transform>(tc::Transform::get_type())
		.constructor<>()
		.property("Position", &tc::Transform::position)
		.property("Rotation", &tc::Transform::rotation)
		.property("Scale", &tc::Transform::scale);


	registration::class_<tc::Rigidbody>(tc::Rigidbody::get_type())
		.constructor<>()
		.property("Rb_config", &tc::Rigidbody::rb_config)
		.property("Shape", &tc::Rigidbody::shape_data);

	registration::class_<rigidbody_config>("rigidbody_config")
		.constructor<>()
		.property("Mass", &rigidbody_config::mass)
		.property("Density", &rigidbody_config::density)
		.property("Linear_damping", &rigidbody_config::linear_damping)
		.property("Angular_damping", &rigidbody_config::angular_damping)
		.property("Is_static", &rigidbody_config::is_static)
		.property("Gravity", &rigidbody_config::gravity)
		.property("Linear_velocity", &rigidbody_config::linear_velocity)
		.property("Angular_velocity", &rigidbody_config::angular_velocity)
		.property("Material", &rigidbody_config::material);
	registration::class_<shape>("shape")
		.constructor<>()
		.property("Shape Type", &shape::type)
		.property("ShapeData", &shape::shapeData);

	
	}

	



}