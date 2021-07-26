#pragma once

#include "Util.h"
#include "TMath.h"
#include "ECS\Entity.h"
#include "Physics/PhysicsObject.h"
#include "Graphics/OpenGL/RenderPipeline.h"

/**
* @brief 
*   =================================================================
*				   COMPONENTs MAINTENANCE INSTRUCTIONS        
*   =================================================================
* 
* Step 1. Decide on Component Name (MUST BE UNIQUE!)
* 
* Step 2. Add Component Name to ComponentType enum 
* 
* Step 3. Define new Component struct (see Example Component)
*		a. Declare member variables
*		b. (MUST) Component MUST be default, copy, and move constructable
*		c. (MUST) Define get_type to return the name of the Component
*		d. (MUST) Define operator& to serialize the Component members
* 
* Step 4. Add Component case to deserialize_helper function
*		a. COMPONENT_CASE(ComponentName)
* 
* @warning 
*		ALL COMPONENTS MUST BE DEFAULT, COPY, AND MOVE CONSTRUCTABLE
*		(therefore please use ids instead of pointers)
* @notes
*		if Component is not serializing/deserializing correctly, 
*			1. check if you have followed all steps
*			2. check if all the names are correct (get_type, enum, component case)
*           3. check if members are being serialized by archiver
*			4. check if component case is added
*		
*		if solution not compiling after adding new component
*			1. check if one of the members are non-trivial type
*				but using trivial ar.Member() call
*			2. check if component is default, copy, and move constructable
*			3. check if get_type function is declared and defined
*		
*/


/**
* @brief Example Component
*/
/** 
	ALL COMPONENTS MUST BE DEFAULT, COPY, AND MOVE CONSTRUCTABLE
	(therefore please use ids instead of pointers)

	struct Example
	{
		// get_type should return a string exactly as the name of the Component
		static const char* get_type() { return "Example"; }

		// define friend operator to take in a template archive reference object
		// and a reference to the component (not const)
		template <typename Archiver>
		friend Archiver& operator&(Archiver& ar, Example& component)
		{
			// must start with ar.StartObject()
			ar.StartObject();

			// for trivial types, the below is enough for both serialization,
			// and deserialization
			ar.Member("Member1", component.member1);

			// for vector types, glm and els both okay
			ar.vec2("Member2", component.member2);
			ar.vec3("Member3", component.member3);
			ar.vec4("Member4", component.member4);

			// for non-trivial types with different serialization/deserialization
			if constexpr (ar::IsLoading())
				; // check if we are loading or not

			// must end with ar.EndObject()
			return ar.EndObject();
		}

		// Declare member variables
		int member1;
		vec2 member2;
		vec3 member3;
		vec4 member4;

	};
*/



namespace Tempest
{
	enum struct ComponentType
	{
		Example, Destroyed, Transform, Meta, Script, Rigidbody, Mesh
		,END
	};

	namespace Components
	{
		/**
		* @brief Example Component to copy from
		* 
		* @warning
		*		ALL COMPONENTS MUST BE DEFAULT, COPY, AND MOVE CONSTRUCTABLE
		*		(therefore please use ids instead of pointers)
		*/
		struct Example
		{
			static const char* get_type() { return "Example"; }

			template <typename Archiver>
			friend Archiver& operator&(Archiver& ar, Example& component)
			{
				ar.StartObject();
				ar.Member("Member1", component.member1);
				ar.Member("Member2", component.member2);
				ar.Member("Member3", component.member3);
				ar.Member("Member4", component.member4);
				return ar.EndObject();
			}

			int member1;
			int member2;
			int member3;
			int member4;
		};

		struct Transform
		{
			static const char* get_type() { return "Transform"; }

			template <typename Archiver>
			friend Archiver& operator&(Archiver& ar, Transform& component)
			{
				ar.StartObject();
				ar.Member("Position", component.position);
				ar.Member("Rotation", component.rotation);
				ar.Member("Scale", component.scale);
				return ar.EndObject();
			}

			vec3 position;
			quat rotation = {1.f, 0.f, 0.f, 0.f};
			vec3 scale = {1.f, 1.f, 1.f};
		};

		struct Meta
		{
			static const char* get_type() { return "Meta"; }

			template <typename Archiver>
			friend Archiver& operator&(Archiver& ar, Meta& component)
			{
				ar.StartObject();
				ar.Member("Name", component.name);
				return ar.EndObject();
			}

			string name{};
		};

		struct Script
		{
			static const char* get_type() { return "Script"; }

			template <typename Archiver>
			friend Archiver& operator&(Archiver& ar, Script& component)
			{
				ar.StartObject();
				ar.Member("Script", component.script);
				return ar.EndObject();
			}

			string script{};
			bool built = false;
		};
		struct Rigidbody
		{
			
			static const char* get_type() { return "Rigidbody"; }
			rigidbody_config rb_config;
			shape shape_data;
			bool isDirty = false;
			tsptr<physx::PxRigidActor> internal_rb = nullptr;


			template <typename Archiver>
			friend Archiver& operator&(Archiver& ar, Rigidbody& component)
			{
				ar.StartObject();
				ar.Member("RigidBody_Config", component.rb_config);
				ar.Member("Shape_Data", component.shape_data);
				return ar.EndObject();
			}
		};

		struct Mesh
		{
			static const char* get_type() { return "Mesh"; }

			template <typename Archiver>
			friend Archiver& operator&(Archiver& ar, Mesh& component)
			{
				ar.StartObject();
				ar.Member("Shape", component.shape);
				return ar.EndObject();
			}

			Mesh(Shape _shape = Shape::SHAPE_SPHERE) : shape(_shape) {}

			Shape shape;
		};

		struct Destroyed 
		{
			static const char* get_type() { return "Destroyed"; }
			
			template <typename Archiver>
			friend Archiver& operator&(Archiver& ar, Destroyed&)
			{
				ar.StartObject();
				return ar.EndObject();
			}
		};
	}
	namespace tc = Tempest::Components;


#define COMPONENT_CASE(ComponentName)								\
	case ComponentType::ComponentName:								\
	{																\
		Entity entity;												\
		tc::ComponentName c;										\
		reader.Member("Entity", entity);							\
		reader.Member("Component", c);								\
		ecs.force_create(entity);									\
		ecs.emplace<tc::ComponentName>(entity, std::move(c));		\
	}																\
		break														\





	template <typename ECS>
	void deserialize_helper(Reader& reader, ECS& ecs, ComponentType type)
	{									
		switch (type)
		{

		/* BELOW THIS PLEASE*/

			COMPONENT_CASE(Example);
			COMPONENT_CASE(Destroyed);
			COMPONENT_CASE(Transform);
			COMPONENT_CASE(Meta);
			COMPONENT_CASE(Script);
			COMPONENT_CASE(Rigidbody);
			COMPONENT_CASE(Mesh);

		/* ABOVE THIS PLEASE */

		case Tempest::ComponentType::END: [[fallthrough]];
		default:
			break;
		}									
	}


	template <typename ECS>
	void deserialize_component(const tentry& component_file, ECS& ecs)
	{
		Serializer serializer;
		string json = serializer.GetJson(component_file.path());

		Reader reader(json.c_str());
		if (reader.HasError())
			return; // warn here


		reader.StartObject();

		// Meta Info
		string typeinfo;
		reader.StartMeta();
		reader.Member("Type", typeinfo);
		reader.EndMeta();

		auto type = magic_enum::enum_cast<ComponentType>(typeinfo);
		if (!type.has_value())
			return; // warn here


		size_t size;
		reader.StartArray("Components", &size);
		for (size_t i = 0; i < size; ++i)
		{
			reader.StartObject();
			deserialize_helper(reader, ecs, type.value());
			reader.EndObject();
		}
		reader.EndArray();

		reader.EndObject();
	}

}
