#pragma once

#include <glew.h>
#include <stb_image.h>
#include "Logger/Log.h"
#include "TMath.h"

namespace Tempest
{
	enum struct Texture_Type
	{
		TEXTURE_1D,
		TEXTURE_2D,
		TEXTURE_3D,
		TEXTURE_1D_ARRAY,
		TEXTURE_2D_ARRAY,
		TEXTURE_RECTANGLE,
		TEXTURE_CUBE_MAP,
		TEXTURE_CUBE_MAP_ARRAY,
		TEXTURE_BUFFER,
		TEXTURE_2D_MULTISAMPLE,
		TEXTURE_2D_MULTISAMPLE_ARRAY
	};

	enum struct Texture_Format
	{
		RGB,
		RGBA,

		RGB16F,
		RGBA16F,

		RGB32F,
		RGBA32F,

		SRGB,
		SRGBA,

		RED,
		GREEN,
		BLUE,
		ALPHA,

		DEPTH24F,
		DEPTH32F,
		DEPTH32F_STENCIL8
	};

	enum struct Texture_Wrap
	{
		REPEAT,
		MIRRORED_REPEAT,
		CLAMP_TO_EDGE,
		CLAMP_TO_BORDER,
		MIRROR_CLAMP_TO_EDGE,
	};

	enum struct Texture_Filter
	{
		NEAREST,
		LINEAR,
		NEAREST_MIPMAP_NEAREST,
		LINEAR_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_LINEAR,
	};

	struct TextureData
	{
		int32_t m_Width = 0;
		int32_t m_Height = 0;
		int32_t m_Channels = 0;
	};

	class Texture
	{
	private:

		TextureData tex;
		uint32_t m_ID = 0;
		std::string m_Name = "nameless";

		Texture_Type m_Type = Texture_Type::TEXTURE_2D;
		Texture_Format m_Format = Texture_Format::RGB;

	public:
		explicit Texture() = default;
		explicit Texture(const std::string & file, bool flip = true);
		explicit Texture(const std::string & right,
			const std::string & left,
			const std::string & top,
			const std::string & bottom,
			const std::string & front,
			const std::string & back);
		explicit Texture(uint32_t width, uint32_t height, Texture_Format format, Texture_Type type);
		~Texture();

		void InitStorage(uint32_t width, uint32_t height, Texture_Format format, Texture_Type type);
		void SetSubData(void* data, uint32_t lod = 0, uint32_t xOffset = 0, uint32_t yOffset = 0) const;
		void SetSubData(void* data, uint32_t lod, uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height) const;
		void SetCubeMapSubData(const std::array<void*, 6>&data, uint32_t xOffset = 0, uint32_t yOffset = 0) const;

		void SetByteAlignment(int alignment) const;
		void SetWrapping(Texture_Wrap s, Texture_Wrap t) const;
		void SetWrapping(Texture_Wrap s, Texture_Wrap t, Texture_Wrap r) const;
		void SetFilters(Texture_Filter min, Texture_Filter mag) const;
		void SetBorderColor(const glm::vec4 & color) const;
		void SetLOD(uint32_t lod) const;

		void GenerateMipmaps() const;

		uint32_t GetID() const;
		void Bind(uint32_t slot) const;
		static void Unbind(uint32_t slot);

		uint32_t GetWidth() const;
		uint32_t GetHeight() const;
		std::string GetName() const;
		Texture_Type GetType() const;
		Texture_Format GetFormat() const;

	};
}

