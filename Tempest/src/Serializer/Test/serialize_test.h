/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include <variant>
#include "Core.h"
#include "../Reader.h"
#include "../Writer.h"
#include "TMath.h"

namespace Tempest
{
	struct Student {
		Student() : name(), age(), height(), canSwim() {}
		Student(const std::string name, unsigned age, double height, bool canSwim) :
			name(name), age(age), height(height), canSwim(canSwim)
		{}

		std::string name;
		unsigned age;
		double height;
		bool canSwim;
	};

	template <typename Archiver>
	Archiver& operator&(Archiver& ar, Student& s) {
		ar.StartObject();
		ar.Member("name")& s.name;
		ar.Member("age")& s.age;
		ar.Member("height")& s.height;
		ar.Member("canSwim")& s.canSwim;
		return ar.EndObject();
	}

	std::ostream& operator<<(std::ostream& os, const Student& s) {
		return os << s.name << " " << s.age << " " << s.height << " " << s.canSwim;
	}

	void test1() {
		std::string json;

		// Serialize
		{
			Student s("Lua", 9, 150.5, true);

			Writer Writer;
			Writer& s;
			json = Writer.GetString();
			std::cout << json << std::endl;
		}

		// Deserialize
		{
			Student s;
			JsonReader Reader(json.c_str());
			Reader& s;
			std::cout << s << std::endl;
		}
	}

	struct Group {
		Group() : groupName(), students() {}
		std::string groupName;
		std::vector<Student> students;
	};

	template <typename Archiver>
	Archiver& operator&(Archiver& ar, Group& g) {
		ar.StartObject();

		ar.Member("groupName");
		ar& g.groupName;

		ar.Member("students");
		size_t studentCount = g.students.size();
		ar.StartArray(&studentCount);
		if (ar.IsLoading())
			g.students.resize(studentCount);
		for (size_t i = 0; i < studentCount; i++)
			ar& g.students[i];
		ar.EndArray();

		return ar.EndObject();
	}

	std::ostream& operator<<(std::ostream& os, const Group& g) {
		os << g.groupName << std::endl;
		for (std::vector<Student>::const_iterator itr = g.students.begin(); itr != g.students.end(); ++itr)
			os << *itr << std::endl;
		return os;
	}

	void test2() {
		std::string json;

		// Serialize
		{
			Group g;
			g.groupName = "Rainbow";

			Student s1("Lua", 9, 150.5, true);
			Student s2("Mio", 7, 120.0, false);
			g.students.push_back(s1);
			g.students.push_back(s2);

			Writer Writer;
			Writer& g;
			json = Writer.GetString();
			std::cout << json << std::endl;
		}

		// Deserialize
		{
			Group g;
			JsonReader Reader(json.c_str());
			Reader& g;
			std::cout << g << std::endl;
		}
	}

	class Shape {
	public:
		virtual ~Shape() {}
		virtual const char* GetType() const = 0;
		virtual void Print(std::ostream& os) const = 0;

	protected:
		Shape() : pos{} {}
		Shape(float x, float y) : pos{ x, y } {}

		template <typename Archiver>
		friend Archiver& operator&(Archiver& ar, Shape& s);

		vec2 pos;
	};

	template <typename Archiver>
	Archiver& operator&(Archiver& ar, Shape& s) {
		ar.vec2("position", s.pos);
		return ar;
	}

	class Circle : public Shape {
	public:
		Circle() : radius_() {}
		Circle(float x, float y, float radius) : Shape(x, y), radius_(radius) {}
		~Circle() {}

		const char* GetType() const { return "Circle"; }

		void Print(std::ostream& os) const {
			os << "Circle (" << pos.x << ", " << pos.y << ")" << " radius = " << radius_;
		}

	private:
		template <typename Archiver>
		friend Archiver& operator&(Archiver& ar, Circle& c);

		float radius_;
	};

	template <typename Archiver>
	Archiver& operator&(Archiver& ar, Circle& c) {
		ar& static_cast<Shape&>(c);
		ar.Member("radius")& c.radius_;
		return ar;
	}

	class Box : public Shape {
	public:
		Box() : width_(), height_() {}
		Box(float x, float y, float width, float height) : Shape(x, y), width_(width), height_(height) {}
		~Box() {}

		const char* GetType() const { return "Box"; }

		void Print(std::ostream& os) const {
			os << "Box (" << pos.x << ", " << pos.y << ")" << " width = " << width_ << " height = " << height_;
		}

	private:
		template <typename Archiver>
		friend Archiver& operator&(Archiver& ar, Box& b);

		float width_, height_;
	};

	template <typename Archiver>
	Archiver& operator&(Archiver& ar, Box& b) {
		ar& static_cast<Shape&>(b);
		ar.Member("width")& b.width_;
		ar.Member("height")& b.height_;
		return ar;
	}

	class Canvas {
	public:
		Canvas() : shapes_() {}
		/* ~Canvas() { Clear(); }

		 void Clear() {
			 for (std::vector<std::shared_ptr<Shape>>::iterator itr = shapes_.begin(); itr != shapes_.end(); ++itr)
				 delete *itr;
		 }*/

		void AddShape(std::shared_ptr<Shape> shape) { shapes_.push_back(shape); }

		void Print(std::ostream& os) {
			for (std::vector<std::shared_ptr<Shape>>::iterator itr = shapes_.begin(); itr != shapes_.end(); ++itr) {
				if (itr->get()) (*itr)->Print(os);
				std::cout << std::endl;
			}
		}

	private:
		template <typename Archiver>
		friend Archiver& operator&(Archiver& ar, Canvas& c);

		std::vector<std::shared_ptr<Shape>> shapes_;
	};

	template <typename Archiver>
	Archiver& operator&(Archiver& ar, std::shared_ptr<Shape>& shape) {
		std::string type = ar.IsLoading() ? "" : shape->GetType();
		ar.StartObject();
		ar.Member("type")& type;
		if (type == "Circle") {
			if (ar.IsLoading()) shape = std::make_shared<Circle>();
			ar& static_cast<Circle&>(*shape);
		}
		else if (type == "Box") {
			if (ar.IsLoading()) shape = std::make_shared<Box>();
			ar& static_cast<Box&>(*shape);
		}
		return ar.EndObject();
	}

	template <typename Archiver>
	Archiver& operator&(Archiver& ar, Canvas& c) {
		size_t shapeCount = c.shapes_.size();
		ar.StartArray(&shapeCount);
		if (ar.IsLoading()) {
			//c.Clear();
			c.shapes_.resize(shapeCount);
		}
		for (size_t i = 0; i < shapeCount; i++)
			ar& c.shapes_[i];
		return ar.EndArray();
	}

	class CanvasWithVariant
	{
	public:
		using Shapes = std::variant<Circle, Box>;

		CanvasWithVariant() : shapes_{} {}
		void AddShape(Shapes shape) {
			shapes_.emplace_back(shape);
		}

		void Print(std::ostream& os) {
			for (std::vector<Shapes>::iterator itr = shapes_.begin(); itr != shapes_.end(); ++itr) {
				std::visit([&](auto& s) {s.Print(os); }, (*itr));
				std::cout << std::endl;
			}
		}
	private:
		template <typename Archiver>
		friend Archiver& operator&(Archiver& ar, CanvasWithVariant& c);

		std::vector<Shapes> shapes_;
	};

	template <typename Archiver>
	Archiver& operator&(Archiver& ar, CanvasWithVariant& c)
	{
		size_t shapeCount = c.shapes_.size();
		ar.StartArray(&shapeCount);
		if (ar.IsLoading())
			c.shapes_.resize(shapeCount);
		for (size_t i = 0; i < shapeCount; ++i)
		{
			auto& shape = c.shapes_[i];
			ar.StartObject();
			std::string type = ar.IsLoading() ? "" : std::visit(
				[&](const auto& s)->std::string {return s.GetType(); }, shape
			);
			ar.Member("type")& type;
			if (type == "Circle") {
				if (ar.IsLoading()) shape = Circle();
			}
			else if (type == "Box") {
				if (ar.IsLoading()) shape = Box();
			}
			ar.Variant(shape);
			ar.EndObject();
		}
		return ar.EndArray();
	}

	void test3() {
		std::string json;

		// Serialize
		{
			Canvas c;
			c.AddShape(std::make_shared<Circle>(1.0f, 2.0f, 3.0f));
			c.AddShape(std::make_shared<Box>(4.0f, 5.0f, 6.0f, 7.0f));

			std::vector<Canvas> canvases;
			canvases.push_back(c);
			canvases.push_back(c);

			Writer Writer;
			size_t size = canvases.size();
			Writer.StartObject();
			Writer.Member("Canvases");
			Writer.StartArray(&size);
			for (auto idx = 0; idx < size; ++idx)
				Writer& canvases[idx];
			Writer.EndArray();
			Writer.EndObject();
			json = Writer.GetString();
			std::cout << json << std::endl;
		}

		// Deserialize
		{
			std::vector<Canvas> canvases;
			JsonReader Reader(json.c_str());
			auto size = canvases.size();
			Reader.StartObject();
			Reader.Member("Canvases");
			Reader.StartArray(&size);
			canvases.resize(size);
			for (auto idx = 0; idx < size; ++idx)
				Reader& canvases[idx];
			Reader.EndArray();
			Reader.EndObject();

			for (auto c : canvases)
				c.Print(std::cout);
		}
	}

	void test4()
	{
		std::string json;

		// Serialize
		{
			CanvasWithVariant c;
			c.AddShape(Circle(1.0, 2.0, 3.0));
			c.AddShape(Box(4.0, 5.0, 6.0, 7.0));

			std::vector<CanvasWithVariant> canvases;
			canvases.push_back(c);
			canvases.push_back(c);

			Writer Writer;
			size_t size = canvases.size();
			Writer.StartObject();
			Writer.Member("CanvasesWithVariant");
			Writer.StartArray(&size);
			for (auto idx = 0; idx < size; ++idx)
				Writer& canvases[idx];
			Writer.EndArray();
			Writer.EndObject();
			json = Writer.GetString();
			std::cout << json << std::endl;
		}

		// Deserialize
		{
			std::vector<CanvasWithVariant> canvases;
			JsonReader Reader(json.c_str());
			auto size = canvases.size();
			Reader.StartObject();
			Reader.Member("CanvasesWithVariant");
			Reader.StartArray(&size);
			canvases.resize(size);
			for (auto idx = 0; idx < size; ++idx)
				Reader& canvases[idx];
			Reader.EndArray();
			Reader.EndObject();

			for (auto c : canvases)
				c.Print(std::cout);
		}
	}

	enum struct Enum
	{
		One,
		Two
	};

	void test5()
	{
		std::string json;
		{
			std::unordered_map <std::string, int> map
			{
				{"One", 1},
				{"Two", 2}
			};

			Writer Writer;
			Writer.StartObject();
			Writer.Map("Enum", "Keys", "Value", map);
			Writer.EndObject();
			puts(Writer.GetString());
			json = Writer.GetString();
		}
		{
			JsonReader Reader(json.c_str());
			std::unordered_map<std::string, int> map;

			for (auto& [key, value] : map)
			{
				puts(std::string(key).append(std::to_string(value)).c_str());
			}
		}
	}

	void Test()
	{
		//test1();
		//test2();
		//test3();
		//test4();
		//test5();
		/*{
			Writer Writer;
			Cardinal::ECS::Entity e{"newentity"};
			Writer.StartObject();
			Writer.Member("Entity");
			Writer.StartObject();
			Writer & e;
			Writer.Member("Children");
			Writer.StartArray();
			Writer.StartObject();
			Writer & e;
			Writer.EndObject();
			Writer.EndArray();
			Writer.EndObject();
			Writer.EndObject();
			puts(Writer.GetString());
		}*/
	}
}