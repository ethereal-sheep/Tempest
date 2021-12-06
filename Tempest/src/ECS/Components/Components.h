/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include <bitset>
#include "Util.h"
#include "TMath.h"
#include "ECS\Entity.h"
#include "Physics/PhysicsObject.h"
#include "Graphics/Basics/RenderPipeline.h"
#include "Util/range.h"
#include "Scripting/Graph/graph.h"
#include "Graphics/Basics/Model.h"

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
		Example, Destroyed, Transform, Local, Meta, Script, Rigidbody, Mesh, Model,
		Character, Weapon, Statline, ConflictGraph, ActionGraph, ResolutionGraph, Graph, 
		Tile, Wall, Shape, Door, Obstacle, Unit, Collision
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
		struct Tile
		{
			static const char* get_type() { return "Tile"; }

			template <typename Archiver>
			friend Archiver& operator&(Archiver& ar, Tile&) { ar.StartObject(); return ar.EndObject(); }
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
			quat rotation = { 1.f, 0.f, 0.f, 0.f };
			vec3 scale = { 1.f, 1.f, 1.f };

			friend bool operator==(const Transform& lhs, const Transform& rhs) {
				return
					glm::all(glm::epsilonEqual(lhs.position, rhs.position, glm::epsilon<float>())) &&
					glm::all(glm::epsilonEqual(lhs.rotation, rhs.rotation, glm::epsilon<float>())) &&
					glm::all(glm::epsilonEqual(lhs.scale, rhs.scale, glm::epsilon<float>()));
			}
			friend bool operator!=(const Transform& lhs, const Transform& rhs) {
				return !(lhs == rhs);
			}

		};

		struct Local
		{
			static const char* get_type() { return "Local"; }

			template <typename Archiver>
			friend Archiver& operator&(Archiver& ar, Local& component)
			{
				ar.StartObject();
				ar.Member("LocalPosition", component.local_position);
				ar.Member("LocalRotation", component.local_rotation);
				ar.Member("LocalScale", component.local_scale);
				return ar.EndObject();
			}

			vec3 local_position;
			quat local_rotation = { 1.f, 0.f, 0.f, 0.f };
			vec3 local_scale = { 1.f, 1.f, 1.f };
		};


		struct Door
		{
			static const char* get_type() { return "Door"; }

			template <typename Archiver>
			friend Archiver& operator&(Archiver& ar, Door& component) 
			{
				ar.StartObject();
				ar.Vector("States", component.states);
				component.states.resize((int)State::End);
				return ar.EndObject(); 
			}

			enum struct State {
				CLOSE,
				OPEN,
				End
			};

			void update(float dt);

			auto get_current_state() const { return next; }
			auto get_current_local() const { return current_local; }

			bool change_state(State new_state);


			tvector<Local> states = tvector<Local>((int)State::End);
			State curr = State::CLOSE;
		private:
			float interpolation_time = 1.f;
			float current_time = 0.f;
			Local current_local;

			State next = State::CLOSE;
			State prev = State::CLOSE;
		};

		struct Obstacle
		{
			static const char* get_type() { return "Obstacle"; }

			template <typename Archiver>
			friend Archiver& operator&(Archiver& ar, Obstacle&) { 
				ar.StartObject(); 
				return ar.EndObject(); 
			}
		};

		struct Unit
		{
			static const char* get_type() { return "Unit"; }

			template <typename Archiver>
			friend Archiver& operator&(Archiver& ar, Unit&) { ar.StartObject(); return ar.EndObject(); }

			void update(float dt);
			bool set_path(const tvector<glm::ivec2>& path, const Transform& curr);
			bool is_moving() { return moving; }
			bool is_end_frame() { return end_frame; }
			Transform get_current_transform() const { return curr_xform; };

		private:
			Transform prev_xform;
			Transform curr_xform;
			Transform next_xform;
			tvector<glm::ivec2> path;

			float interpolation_time = 1.f;
			float current_time = 0.f;

			bool end_frame = false;
			bool moving = false;

		};

		struct Wall
		{
			static const char* get_type() { return "Wall"; }

			template <typename Archiver>
			friend Archiver& operator&(Archiver& ar, Wall& component) {
				ar.StartObject();
				ar.Member("Cover", component.cover);
				return ar.EndObject();
			}

			enum struct Cover
			{
				FULL,
				HALF,
				End
			};
			Cover cover = Cover::FULL;
		};

		struct Collision
		{
			static const char* get_type() { return "Collision"; }

			template <typename Archiver>
			friend Archiver& operator&(Archiver& ar, Collision& component) {
				ar.StartObject();
				ar.Member("Cover", component.cover);
				return ar.EndObject();
			}

			enum struct Cover
			{
				FULL,
				HALF,
				End
			};
			Cover cover = Cover::FULL;
		};


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



		struct Shape
		{
			static const char* get_type() { return "Shape"; }

			template <typename Archiver>
			friend Archiver& operator&(Archiver& ar, Shape& component)
			{
				ar.StartObject();
				ar.Member("X", component.x);
				ar.Member("Y", component.y);
				return ar.EndObject();
			}

			int x = 1;
			int y = 1;
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
				ar.Member("Code", component.code);
				return ar.EndObject();
			}

			Mesh(MeshCode _code = MeshCode::SPHERE) : code(_code) {}

			MeshCode code;
		};
		struct Model
		{
			static const char* get_type() { return "Model"; }

			template <typename Archiver>
			friend Archiver& operator&(Archiver& ar, Model& component)
			{
				ar.StartObject();
				ar.Member("Path", component.path);
				ar.Member("TexturePath", component.texPath);
				return ar.EndObject();
			}
			Model(string _path = "Models/Chair.fbx") : path{ _path } {}
			string path;
			string texPath;
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

		static const size_t STAT_TOTAL = 10;

		struct Character
		{
			
			static const char* get_type() { return "Character"; }

			template <typename Archiver>
			friend Archiver& operator&(Archiver& ar, Character& component)
			{
				ar.StartObject();
				ar.Member("Charater_Name", component.name);
				ar.Member("Color", component.color);
				ar.Vector("Weapon_Ids", component.weapons);
				ar.Vector("Charater_Stats", component.stats);
				//ar.Vector("Charater_DeltaStats", component.StatsDelta);
				ar.Vector("Actions", component.actions);

				return ar.EndObject();
			}

			Character() : stats(STAT_TOTAL,0), StatsDelta(STAT_TOTAL,0)
			{

			}

			[[deprecated("No more removal of stat")]]
			void remove_stat(size_t index)
			{
				if (index >= STAT_TOTAL)
					return;

				stats.erase(stats.begin() + index);
				stats.push_back(0);
			}

			void set_stat(size_t index, int val)
			{
				if (index >= STAT_TOTAL)
					return;

				stats[index] = val;
			}
			void set_statDelta(size_t index, int val)
			{
				if (index >= STAT_TOTAL)
					return;

				StatsDelta[index] = val;
			}

			// If pair.second is true return stats
			// else return statsDelta
			[[nodiscard]] int& operator[](std::pair<size_t,bool> index)
			{
				if (index.second)
					return stats[index.first];
				else
					return StatsDelta[index.first];
			}


			[[nodiscard]] int& get_stat(size_t index)
			{
				return stats[index];
			}
			[[nodiscard]] int& get_statDelta(size_t index)
			{
				return StatsDelta[index];
			}

			[[deprecated("Iterate statline and get stat via []")]]
			[[nodiscard]] const tvector<int>& get_stats() const
			{
				return stats;
			}

			[[deprecated("Iterate statline and get stat via []")]]
			[[nodiscard]] auto get_stats_range() 
			{
				return make_range(stats);
			}


			string name = "Char";
			Entity chosen_weapon = UNDEFINED; // no need to serialize this
			tvector<Entity> weapons;
			tvector<Entity> actions;
			vec3 color = { 1.f, 0.f, 0.f };
		private:
			tvector<int> stats;
			tvector<int> StatsDelta;

		};

		struct Weapon
		{
			static const char* get_type() { return "Weapon"; }


			template <typename Archiver>
			friend Archiver& operator&(Archiver& ar, Weapon& component)
			{
				ar.StartObject();
				ar.Member("Weapon_Name", component.name);
				ar.Member("Main_Stat", component.main_stat);
				ar.Vector("Weapon_Stats", component.stats);
				return ar.EndObject();
			}

			Weapon() : stats(STAT_TOTAL, 0)
			{
				
			}

			[[deprecated("No more removal of stat")]]
			void remove_stat(size_t index)
			{
				if (index >= STAT_TOTAL)
					return;

				stats.erase(stats.begin() + index);
				stats.push_back(0);
			}

			void set_stat(size_t index, int val)
			{
				if (index >= STAT_TOTAL)
					return;

				stats[index] = val;
			}

			[[nodiscard]] int& get_stat(size_t index)
			{
				return stats[index];
			}

			[[nodiscard]] int& operator[](size_t index)
			{
				return stats[index];
			}

			[[deprecated("Iterate statline and get stat via []")]]
			[[nodiscard]] const tvector<int>& get_stats() const
			{
				return stats;
			}

			[[nodiscard]] auto get_stats_range()
			{
				return make_range(stats);
			}

			void set_main_stat(size_t index)
			{
				if (index >= STAT_TOTAL)
					return;
				main_stat = index;
			}

			[[nodiscard]] size_t get_main_stat() const
			{
				return main_stat;
			}



			string name = "Weapon";
		private:
			tvector<int> stats;
			size_t main_stat = 1; // set to ATK as main
		};

		struct Statline
		{

			static const char* get_type() { return "Statline"; }

			template <typename Archiver>
			friend Archiver& operator&(Archiver& ar, Statline& component)
			{
				ar.StartObject();
				ar.Vector("Statline_Data", component.stat_list);

				// first 5 always true and correct name
				component.stat_list[0] = tpair<bool, string>(true, "HP");
				component.stat_list[1] = tpair<bool, string>(true, "ATK");
				component.stat_list[2] = tpair<bool, string>(true, "DEF");
				component.stat_list[3] = tpair<bool, string>(true, "Range");
				component.stat_list[4] = tpair<bool, string>(true, "Move");


				return ar.EndObject();
			}

			Statline() : stat_list(STAT_TOTAL)
			{
				/*stats.push_back("HP");
				stats.push_back("ATK");
				stats.push_back("DEF");*/

				stat_list[0] = tpair<bool, string>(true, "HP");
				stat_list[1] = tpair<bool, string>(true, "ATK");
				stat_list[2] = tpair<bool, string>(true, "DEF");
				stat_list[3] = tpair<bool, string>(true, "Range");
				stat_list[4] = tpair<bool, string>(true, "Move");

				stat_list[5] = tpair<bool, string>(false, "Stat1");
				stat_list[6] = tpair<bool, string>(false, "Stat2");
				stat_list[7] = tpair<bool, string>(false, "Stat3");
				stat_list[8] = tpair<bool, string>(false, "Stat4");
				stat_list[9] = tpair<bool, string>(false, "Stat5");
			}

			[[deprecated("No more removal of stat")]] 
			bool remove_stat(size_t )
			{
				return false;
			}

			[[deprecated("No more removal of stat")]]
			bool remove_stat(const string& )
			{
				return false;
			}

			[[deprecated("No more adding of stat")]]
			bool add_stat(const string& )
			{
				return false;
			}

			[[deprecated("Rename stat via get_stats")]]
			bool rename_stat(size_t , const string& )
			{
				return false;
			}

			[[deprecated("No more removal of stat")]]
			bool rename_stat(const string& , const string& )
			{
				return false;
			}

			[[deprecated("No more removal of stat")]]
			size_t index_of_stat(const string& name) const
			{
				auto f = std::find_if(stat_list.begin(), stat_list.end(), [&](auto pair)
					{
						return pair.second == name;
					});
				return f - stat_list.begin();
			}

			[[nodiscard]] string& operator[](size_t index)
			{
				return stat_list[index].second;
			}

			[[nodiscard]] bool& operator()(size_t index)
			{
				return stat_list[index].first;
			}

			[[nodiscard]] size_t count(const string& name) const
			{
				auto f = std::find_if(stat_list.begin(), stat_list.end(), [&](auto pair)
					{
						return pair.second == name;
					});
				if (f == stat_list.end()) return 0;
				return 1;
			}

			[[nodiscard]] bool exist(const string& name) const
			{
				auto f = std::find_if(stat_list.begin(), stat_list.end(), [&](auto pair)
					{
						return pair.second == name;
					});
				if (f == stat_list.end()) return false;
				return true;
			}

			[[nodiscard]] size_t size() const
			{
				return stat_list.size();
			}

			[[deprecated("use get_stat_range()")]]
			[[nodiscard]] const tvector<string>& get_stats() const
			{
				return stats;
			}

			[[nodiscard]] auto get_stat_range()
			{
				return make_range(stat_list);
			}

		private:

			tvector<string> stats;

			tvector<tpair<bool, string>> stat_list;
		};

		struct ConflictGraph
		{
			static const char* get_type() { return "ConflictGraph"; }

			template <typename Archiver>
			friend Archiver& operator&(Archiver& ar, ConflictGraph& )
			{
				ar.StartObject();
				return ar.EndObject();
			}
		};

		struct ActionGraph
		{
			static const char* get_type() { return "ActionGraph"; }

			template <typename Archiver>
			friend Archiver& operator&(Archiver& ar, ActionGraph& component)
			{
				ar.StartObject();
				ar.Member("Action_Category", component.category);
				return ar.EndObject();
			}

			enum ACTION_CAT : int
			{
				AC_NONE = 0,
				AC_ATTK = 1,
				AC_DEF  = 2,
				AC_ATTK_DEF = AC_ATTK | AC_DEF
			};
			ACTION_CAT category{ AC_ATTK_DEF }; // set to attack + defend by default
		};

		struct ResolutionGraph
		{
			static const char* get_type() { return "ResolutionGraph"; }

			template <typename Archiver>
			friend Archiver& operator&(Archiver& ar, ResolutionGraph& )
			{
				ar.StartObject();
				return ar.EndObject();
			}
		};

		struct Graph
		{
			static const char* get_type() { return "Graph"; }

			template <typename Archiver>
			friend Archiver& operator&(Archiver& ar, Graph& component)
			{
				if constexpr (Archiver::IsSaving())
					return component.g._serialize(ar);
				else
					return component.g._deserialize(ar);
			}

			Graph(string empty = "Empty", graph_type type = graph_type::regular)
				: g(empty, type) {}

			graph g;
		};


	}
	namespace tc = Tempest::Components;


#define COMPONENT_CASE(ComponentName)								\
	case ComponentType::ComponentName:								\
	{																\
		if constexpr (ECS::is_entity_keyed) {						\
			Entity entity;											\
			tc::ComponentName c;									\
			reader.Member("Entity", entity);						\
			reader.Member("Component", c);							\
			ecs.force_create(entity);								\
			ecs.emplace<tc::ComponentName>(entity, std::move(c));	\
		}				\
		else {														\
			auto c = ecs.try_emplace<tc::ComponentName>();			\
			if(c) reader.Member("Component", *c);					\
		}												\
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
			COMPONENT_CASE(Local);
			COMPONENT_CASE(Meta);
			COMPONENT_CASE(Script);
			COMPONENT_CASE(Rigidbody);
			COMPONENT_CASE(Mesh);
			COMPONENT_CASE(Model);
			COMPONENT_CASE(Character);
			COMPONENT_CASE(Weapon);
			COMPONENT_CASE(Statline);

			COMPONENT_CASE(Unit);
			COMPONENT_CASE(Obstacle);
			COMPONENT_CASE(Door);
			COMPONENT_CASE(Tile);
			COMPONENT_CASE(Wall);
			COMPONENT_CASE(Collision);
			COMPONENT_CASE(Shape);

			COMPONENT_CASE(ConflictGraph);
			COMPONENT_CASE(ActionGraph);
			COMPONENT_CASE(ResolutionGraph);
			COMPONENT_CASE(Graph);

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
