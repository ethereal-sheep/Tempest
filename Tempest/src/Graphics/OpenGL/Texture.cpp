/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Texture.h"

namespace Tempest
{
	constexpr GLenum GLTypeOf(Texture_Type type)
	{
		switch (type)
		{
		default: assert(0);
		case Texture_Type::TEXTURE_1D:						return GL_TEXTURE_1D;
		case Texture_Type::TEXTURE_2D:						return GL_TEXTURE_2D;
		case Texture_Type::TEXTURE_3D:						return GL_TEXTURE_3D;
		case Texture_Type::TEXTURE_1D_ARRAY:				return GL_TEXTURE_1D_ARRAY;
		case Texture_Type::TEXTURE_2D_ARRAY:				return GL_TEXTURE_2D_ARRAY;
		case Texture_Type::TEXTURE_RECTANGLE:				return GL_TEXTURE_RECTANGLE;
		case Texture_Type::TEXTURE_CUBE_MAP:				return GL_TEXTURE_CUBE_MAP;
		case Texture_Type::TEXTURE_CUBE_MAP_ARRAY:			return GL_TEXTURE_CUBE_MAP_ARRAY;
		case Texture_Type::TEXTURE_BUFFER:					return GL_TEXTURE_BUFFER;
		case Texture_Type::TEXTURE_2D_MULTISAMPLE:			return GL_TEXTURE_2D_MULTISAMPLE;
		case Texture_Type::TEXTURE_2D_MULTISAMPLE_ARRAY:	return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
		}
	}

	constexpr GLenum GLFormatOf(Texture_Format format)
	{
		switch (format)
		{
		default: assert(0);
		case Texture_Format::RGB:					return GL_RGB;
		case Texture_Format::RGB16F:				return GL_RGB;
		case Texture_Format::RGB32F:				return GL_RGB;
		case Texture_Format::RGBA:					return GL_RGBA;
		case Texture_Format::RGBA16F:				return GL_RGBA;
		case Texture_Format::RGBA32F:				return GL_RGBA;
		case Texture_Format::SRGB:					return GL_SRGB;
		case Texture_Format::SRGBA:					return GL_SRGB_ALPHA;
		case Texture_Format::RED:					return GL_RED;
		case Texture_Format::GREEN:					return GL_GREEN;
		case Texture_Format::BLUE:					return GL_BLUE;
		case Texture_Format::ALPHA:					return GL_ALPHA;
		case Texture_Format::DEPTH24F:				return GL_DEPTH_COMPONENT;
		case Texture_Format::DEPTH32F:				return GL_DEPTH_COMPONENT;
		case Texture_Format::DEPTH32F_STENCIL8:		return GL_DEPTH_STENCIL;
		}
	}

	constexpr GLenum GLStorageOf(Texture_Format format)
	{
		switch (format)
		{
		default: assert(0);
		case Texture_Format::RGB:					return GL_RGB8;
		case Texture_Format::RGB16F:				return GL_RGB16F;
		case Texture_Format::RGB32F:				return GL_RGB32F;
		case Texture_Format::RGBA:					return GL_RGBA8;
		case Texture_Format::RGBA16F:				return GL_RGBA16F;
		case Texture_Format::RGBA32F:				return GL_RGBA32F;
		case Texture_Format::SRGB:					return GL_SRGB8;
		case Texture_Format::SRGBA:					return GL_SRGB8_ALPHA8;
		case Texture_Format::RED:					return GL_R8;
		case Texture_Format::GREEN:					return GL_R8;
		case Texture_Format::BLUE:					return GL_R8;
		case Texture_Format::ALPHA:					return GL_R8;
		case Texture_Format::DEPTH24F:				return GL_DEPTH_COMPONENT24;
		case Texture_Format::DEPTH32F:				return GL_DEPTH_COMPONENT32F;
		case Texture_Format::DEPTH32F_STENCIL8:		return GL_DEPTH32F_STENCIL8;
		}
	}

	constexpr GLenum GLTypeOf(Texture_Format format)
	{
		switch (format)
		{
		default: assert(0);
		case Texture_Format::RGB:					return GL_UNSIGNED_BYTE;
		case Texture_Format::RGB16F:				return GL_FLOAT;
		case Texture_Format::RGB32F:				return GL_FLOAT;
		case Texture_Format::RGBA:					return GL_UNSIGNED_BYTE;
		case Texture_Format::RGBA16F:				return GL_FLOAT;
		case Texture_Format::RGBA32F:				return GL_FLOAT;
		case Texture_Format::SRGB:					return GL_UNSIGNED_BYTE;
		case Texture_Format::SRGBA:					return GL_UNSIGNED_BYTE;
		case Texture_Format::RED:					return GL_UNSIGNED_BYTE;
		case Texture_Format::GREEN:					return GL_UNSIGNED_BYTE;
		case Texture_Format::BLUE:					return GL_UNSIGNED_BYTE;
		case Texture_Format::ALPHA:					return GL_UNSIGNED_BYTE;
		case Texture_Format::DEPTH24F:				return GL_FLOAT;
		case Texture_Format::DEPTH32F:				return GL_FLOAT;
		case Texture_Format::DEPTH32F_STENCIL8:		return GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
		}
	}

	constexpr GLenum GLenumOf(Texture_Wrap wrap)
	{
		switch (wrap)
		{
		default: assert(0);
		case Texture_Wrap::REPEAT:						return GL_REPEAT;
		case Texture_Wrap::MIRRORED_REPEAT:				return GL_MIRRORED_REPEAT;
		case Texture_Wrap::CLAMP_TO_EDGE:				return GL_CLAMP_TO_EDGE;
		case Texture_Wrap::CLAMP_TO_BORDER:				return GL_CLAMP_TO_BORDER;
		case Texture_Wrap::MIRROR_CLAMP_TO_EDGE:		return GL_MIRROR_CLAMP_TO_EDGE;
		}
	}

	constexpr GLenum GLenumOf(Texture_Filter filter)
	{
		switch (filter)
		{
		default: assert(0);
		case Texture_Filter::NEAREST:						return GL_NEAREST;
		case Texture_Filter::LINEAR:						return GL_LINEAR;
		case Texture_Filter::NEAREST_MIPMAP_NEAREST:		return GL_NEAREST_MIPMAP_NEAREST;
		case Texture_Filter::LINEAR_MIPMAP_NEAREST:			return GL_LINEAR_MIPMAP_NEAREST;
		case Texture_Filter::NEAREST_MIPMAP_LINEAR:			return GL_NEAREST_MIPMAP_LINEAR;
		case Texture_Filter::LINEAR_MIPMAP_LINEAR:			return GL_LINEAR_MIPMAP_LINEAR;
		}
	}

	Texture::Texture(const std::string& file, bool flip)
	{
		stbi_set_flip_vertically_on_load(flip);
		unsigned char* m_data = stbi_load(file.c_str(), &tex.m_Width, &tex.m_Height, &tex.m_Channels, 0);
		if(!m_data)
			throw std::invalid_argument{std::string("Failed to load image! Filename: ") + file};

		InitStorage(tex.m_Width, tex.m_Height, tex.m_Channels == 4 ? Texture_Format::RGBA : Texture_Format::RGB, Texture_Type::TEXTURE_2D);
		SetSubData(m_data);
		SetWrapping(Texture_Wrap::REPEAT, Texture_Wrap::REPEAT);
		SetFilters(Texture_Filter::NEAREST_MIPMAP_LINEAR, Texture_Filter::LINEAR);
		GenerateMipmaps();

		stbi_image_free(m_data);

		m_Name = file;
	}

	Texture::Texture(const std::string& right, const std::string& left, const std::string& top, const std::string& bottom, const std::string& front, const std::string& back)
	{
		TextureData img[6];
		unsigned char* m_right = stbi_load(right.c_str(), &img[0].m_Width, &img[0].m_Height, &img[0].m_Channels, 0);
		unsigned char* m_left = stbi_load(left.c_str(), &img[1].m_Width, &img[1].m_Height, &img[1].m_Channels, 0);
		unsigned char* m_top = stbi_load(top.c_str(), &img[2].m_Width, &img[2].m_Height, &img[2].m_Channels, 0);
		unsigned char* m_bottom = stbi_load(bottom.c_str(), &img[3].m_Width, &img[3].m_Height, &img[3].m_Channels, 0);
		unsigned char* m_front = stbi_load(front.c_str(), &img[4].m_Width, &img[4].m_Height, &img[4].m_Channels, 0);
		unsigned char* m_back = stbi_load(back.c_str(), &img[5].m_Width, &img[5].m_Height, &img[5].m_Channels, 0);

		InitStorage(img[0].m_Width, img[0].m_Height, img[0].m_Channels == 4 ? Texture_Format::RGBA : Texture_Format::RGB, Texture_Type::TEXTURE_CUBE_MAP);

		SetCubeMapSubData({ m_right, m_left, m_top, m_bottom, m_front, m_back});
		SetWrapping(Texture_Wrap::CLAMP_TO_EDGE, Texture_Wrap::CLAMP_TO_EDGE, Texture_Wrap::CLAMP_TO_EDGE);
		SetFilters(Texture_Filter::NEAREST, Texture_Filter::NEAREST);
		GenerateMipmaps();

		stbi_image_free(m_right);
		stbi_image_free(m_left);
		stbi_image_free(m_top);
		stbi_image_free(m_bottom);
		stbi_image_free(m_front);
		stbi_image_free(m_back);

		m_Name = right;
	}

	Texture::Texture(uint32_t width, uint32_t height, Texture_Format format, Texture_Type type)
	{
		InitStorage(width, height, format, type);
	}

	Texture::~Texture()
	{
		if (m_ID != 0)
			glDeleteTextures(1, &m_ID);
	}

	void Texture::InitStorage(uint32_t width, uint32_t height, Texture_Format format, Texture_Type type)
	{
		glCreateTextures(GLTypeOf(type), 1, &m_ID);
		glTextureStorage2D(m_ID, 1, GLStorageOf(format), width, height);
		tex.m_Width = width;
		tex.m_Height = height;
		m_Format = format;
		m_Type = type;
	}

	void Texture::SetSubData(void* data, uint32_t lod, uint32_t xOffset, uint32_t yOffset) const
	{
		glTextureSubImage2D(m_ID, lod, xOffset, yOffset, tex.m_Width, tex.m_Height,
							GLFormatOf(m_Format), GLTypeOf(m_Format), data);
	}

	void Texture::SetSubData(void* data, uint32_t lod, uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height) const
	{
		glTextureSubImage2D(m_ID, lod, xOffset, yOffset, width, height,
							GLFormatOf(m_Format), GLTypeOf(m_Format), data);
	}

	void Texture::SetCubeMapSubData(const std::array<void*, 6>& data, uint32_t xOffset, uint32_t yOffset) const
	{
		const auto size = static_cast<int>(data.size());
		for (int i = 0; i < size; ++i)
		{
			glTextureSubImage3D(m_ID, 0, xOffset, yOffset, i,
				tex.m_Width, tex.m_Height, 1,
				GLFormatOf(m_Format), GLTypeOf(m_Format), data[i]);
		}
	}

	void Texture::SetByteAlignment(int alignment) const
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
	}

	void Texture::SetWrapping(Texture_Wrap s, Texture_Wrap t) const
	{
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GLenumOf(s));
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GLenumOf(t));
	}

	void Texture::SetWrapping(Texture_Wrap s, Texture_Wrap t, Texture_Wrap r) const
	{
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GLenumOf(s));
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GLenumOf(t));
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_R, GLenumOf(r));
	}

	void Texture::SetFilters(Texture_Filter min, Texture_Filter mag) const
	{
		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GLenumOf(min));
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GLenumOf(mag));
	}

	void Texture::SetBorderColor(const glm::vec4& color) const
	{
		glTextureParameterfv(m_ID, GL_TEXTURE_BORDER_COLOR, &color[0]);
	}

	void Texture::SetLOD(uint32_t lod) const
	{
		glTextureParameteri(m_ID, GL_TEXTURE_MIN_LOD, lod);
		glTextureParameteri(m_ID, GL_TEXTURE_MAX_LOD, lod);
	}

	void Texture::GenerateMipmaps() const
	{
		glGenerateTextureMipmap(m_ID);
	}

	uint32_t Texture::GetID() const
	{
		return m_ID;
	}

	void Texture::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_ID);
	}

	void Texture::Unbind(uint32_t slot)
	{
		glBindTextureUnit(slot, 0);
	}

	uint32_t Texture::GetWidth() const
	{
		return tex.m_Width;
	}

	uint32_t Texture::GetHeight() const
	{
		return tex.m_Height;
	}

	std::string Texture::GetName() const
	{
		return m_Name;
	}

	Texture_Type Texture::GetType() const
	{
		return m_Type;
	}

	Texture_Format Texture::GetFormat() const
	{
		return m_Format;
	}
}