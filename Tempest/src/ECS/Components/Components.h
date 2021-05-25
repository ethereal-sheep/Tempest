#pragma once

#include "Util.h"
#include "TMath.h"
#include "ECS\Entity.h"

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
* 
* @warning 
		ALL COMPONENTS MUST BE DEFAULT, COPY, AND MOVE CONSTRUCTABLE
		(therefore please use ids instead of pointers)
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
		template <typename Archive>
		friend Archive& operator&(Archive& ar, Example& component)
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
		Transform, Meta, Example,
		END
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

			template <typename Archive>
			friend Archive& operator&(Archive& ar, Example& component)
			{
				ar.StartObject();
				ar.Member("Member1", component.member1);
				ar.Member("Member2", component.member2);
				return ar.EndObject();
			}
			int member1;
			int member2;
		};

		struct Transform
		{
			static string get_type() { return "Transform"; }

			template <typename Archive>
			friend Archive& operator&(Archive& ar, Transform& component)
			{
				ar.StartObject();
				ar.vec3("Position", component.position);
				ar.vec4("Rotation", component.rotation);
				ar.vec3("Scale", component.scale);
				return ar.EndObject();
			}

			vec3 position;
			quat rotation;
			vec3 scale;
		};

		struct Meta
		{
			static string get_type() { return "Meta"; }

			template <typename Archive>
			friend Archive& operator&(Archive& ar, Meta& component)
			{
				ar.StartObject();
				ar.Member("Name", component.name);
				return ar.EndObject();
			}

			string name{};
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
			COMPONENT_CASE(Transform);
			COMPONENT_CASE(Meta);

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
