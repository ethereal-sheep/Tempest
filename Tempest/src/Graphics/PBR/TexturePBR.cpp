/**********************************************************************************
* \author		Lim Yong Kiang, Darren (lim.y@digipen.edu)
* \author		Tiong Jun Ming, Jerome (j.tiong@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
                or disclosure of this file or its contents without the prior
                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "TexturePBR.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glew.h>

#include "stb_image.h"
#include "tinyddsloader.h"
#include "Logger/Log.h"
#include <cstring>

#define TINYDDSLOADER_IMPLEMENTATION
using namespace tinyddsloader;
namespace Tempest
{


    struct GLSwizzle
    {
        GLenum m_r, m_g, m_b, m_a;
    };

    struct GLFormat
    {
        DDSFile::DXGIFormat m_dxgiFormat;
        GLenum m_type;
        GLenum m_format;
        GLSwizzle m_swizzle;
    };

    TexturePBR::TexturePBR() 
    {

    }


    TexturePBR::~TexturePBR()
    {
        glDeleteTextures(1, &this->texID);
    }

    bool TranslateFormat(DDSFile::DXGIFormat fmt, GLFormat* outFormat) {
        static const GLSwizzle sws[] = {
            {GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA},
            {GL_BLUE, GL_GREEN, GL_RED, GL_ALPHA},
            {GL_BLUE, GL_GREEN, GL_RED, GL_ONE},
            {GL_RED, GL_GREEN, GL_BLUE, GL_ONE},
            {GL_RED, GL_ZERO, GL_ZERO, GL_ZERO},
            {GL_RED, GL_GREEN, GL_ZERO, GL_ZERO},
        };
        using DXGIFmt = DDSFile::DXGIFormat;
        static const GLFormat formats[] = {
            {DXGIFmt::R8G8B8A8_UNorm, GL_UNSIGNED_BYTE, GL_RGBA, sws[0]},
            {DXGIFmt::B8G8R8A8_UNorm, GL_UNSIGNED_BYTE, GL_RGBA, sws[1]},
            {DXGIFmt::B8G8R8X8_UNorm, GL_UNSIGNED_BYTE, GL_RGBA, sws[2]},
            {DXGIFmt::BC1_UNorm, 0, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, sws[0]},
            {DXGIFmt::BC2_UNorm, 0, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, sws[0]},
            {DXGIFmt::BC3_UNorm, 0, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, sws[0]},
            {DXGIFmt::BC4_UNorm, 0, GL_COMPRESSED_RED_RGTC1_EXT, sws[0]},
            {DXGIFmt::BC4_SNorm, 0, GL_COMPRESSED_SIGNED_RED_RGTC1_EXT, sws[0]},
            {DXGIFmt::BC5_UNorm, 0, GL_COMPRESSED_RED_GREEN_RGTC2_EXT, sws[0]},
            {DXGIFmt::BC5_SNorm, 0, GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT,
            sws[0]},
        };
        for (const auto& format : formats) {
            if (format.m_dxgiFormat == fmt) {
                if (outFormat) {
                    *outFormat = format;
                }
                return true;
            }
        }
        return false;
    }

    void TexturePBR::setTexture(const char* texPath, std::string texName, bool texFlip1)
    {
        if (!strcmp(texName.c_str(), "0"))
        {
            this->texID = 0;
            return;
        }

        this->texType = GL_TEXTURE_2D;

        std::string tempPath = std::string(texPath);
        
        if (texFlip1)
            stbi_set_flip_vertically_on_load(true);
        else
            stbi_set_flip_vertically_on_load(false);

        glGenTextures(1, &this->texID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->texID);
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoFilterLevel);  // Request the maximum level of anisotropy the GPU used can support and use it
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, this->anisoFilterLevel);

        int width, height, numComponents;
        unsigned char* texData = stbi_load(tempPath.c_str(), &width, &height, &numComponents, 0);

        this->texWidth = width;
        this->texHeight = height;
        this->texComponents = numComponents;
        this->tName = texName;

        if (texData)
        {
            if (numComponents == 1)
                this->texFormat = GL_RED;
            else if (numComponents == 3)
                this->texFormat = GL_RGB;
            else if (numComponents == 4)
                this->texFormat = GL_RGBA;
            this->texInternalFormat = this->texFormat;

            glTexImage2D(GL_TEXTURE_2D, 0, this->texInternalFormat, this->texWidth, this->texHeight, 0, this->texFormat, GL_UNSIGNED_BYTE, texData);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);     // Need AF to get ride of the blur on textures
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glGenerateMipmap(GL_TEXTURE_2D);
        }

        else
        {
            std::cerr << "TEXTURE FAILED - LOADING : " << texPath << std::endl;
        }

        stbi_image_free(texData);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    bool IsCompressed(GLenum fmt) {
        switch (fmt) {
        case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
        case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
        case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
        case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
        case GL_COMPRESSED_RED_RGTC1_EXT:
        case GL_COMPRESSED_SIGNED_RED_RGTC1_EXT:
        case GL_COMPRESSED_RED_GREEN_RGTC2_EXT:
        case GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT:
            return true;
        default:
            return false;
        }
    }

    void TexturePBR::setTextureDDS(const char* texPath, std::string texName, bool texFlip1)
    {
        (void*)texFlip1;
        DDSFile Texture;
        auto ret = Texture.Load(texPath);
        if (tinyddsloader::Result::Success != ret) 
        {
            std::string errorText = "Failed to load ["; 
            errorText += texPath;
            errorText += "]. Result :  ";
            errorText += std::to_string(int(ret));
            errorText += ".";
            LOG_ERROR(errorText.c_str());
            return;
        }

        GLenum Target = GL_INVALID_ENUM; 
        bool isArray = false;
        if (Texture.GetTextureDimension() == DDSFile::TextureDimension::Texture1D) 
        {
            if (Texture.GetArraySize() > 1) 
            {
                Target = GL_TEXTURE_1D_ARRAY;
                isArray = true;
            } 
            else 
            {
                Target = GL_TEXTURE_1D;
            }
        } 
        else if (Texture.GetTextureDimension() ==DDSFile::TextureDimension::Texture2D) 
        {
            if (Texture.GetArraySize() > 1) 
            {
                if (Texture.IsCubemap()) 
                {
                    if (Texture.GetArraySize() > 6) 
                    {
                        Target = GL_TEXTURE_CUBE_MAP_ARRAY;
                        isArray = true;
                    } 
                    else 
                    {
                        Target = GL_TEXTURE_CUBE_MAP;
                    }
                } 
                else 
                {
                    Target = GL_TEXTURE_2D_ARRAY;
                    isArray = true;
                }
            } 
            else 
            {
                Target = GL_TEXTURE_2D;
            }
        } 
        else if (Texture.GetTextureDimension() == DDSFile::TextureDimension::Texture3D) 
        {
            Target = GL_TEXTURE_3D;
        }
        
        GLFormat Format;
        if (!TranslateFormat(Texture.GetFormat(), &Format)) 
        {

            LOG_CRITICAL("Translate format dds error");
            return;
        }

        unsigned int  TextureName = 0;
        glGenTextures(1, &TextureName);
        glBindTexture(Target, TextureName);
        glTexParameteri(Target, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(Target, GL_TEXTURE_MAX_LEVEL, Texture.GetMipCount() - 1);
        glTexParameteri(Target, GL_TEXTURE_SWIZZLE_R, Format.m_swizzle.m_r);
        glTexParameteri(Target, GL_TEXTURE_SWIZZLE_G, Format.m_swizzle.m_g);
        glTexParameteri(Target, GL_TEXTURE_SWIZZLE_B, Format.m_swizzle.m_b);
        glTexParameteri(Target, GL_TEXTURE_SWIZZLE_A, Format.m_swizzle.m_a);

        //glm::tvec3<GLsizei> const Extent(Texture.extent());
        //GLsizei const FaceTotal = static_cast<GLsizei>(Texture.layers() * Texture.faces());

        switch (Target) {
        case GL_TEXTURE_1D:
            glTexStorage1D(Target, Texture.GetMipCount(), Format.m_format, Texture.GetWidth());
            break;
        case GL_TEXTURE_1D_ARRAY:
            glTexStorage2D(Target, Texture.GetMipCount(), Format.m_format, Texture.GetWidth(), Texture.GetArraySize());
            break;
        case GL_TEXTURE_2D:
            glTexStorage2D(Target, Texture.GetMipCount(), Format.m_format, Texture.GetWidth(), Texture.GetHeight());
            break;
        case GL_TEXTURE_CUBE_MAP:
            glTexStorage2D(Target, Texture.GetMipCount(), Format.m_format, Texture.GetWidth(), Texture.GetHeight());
            break;
        case GL_TEXTURE_2D_ARRAY:
            glTexStorage3D(Target, Texture.GetMipCount(), Format.m_format, Texture.GetWidth(), Texture.GetHeight(), Texture.GetArraySize());
            break;
        case GL_TEXTURE_3D:
            glTexStorage3D(Target, Texture.GetMipCount(), Format.m_format, Texture.GetWidth(), Texture.GetHeight(), Texture.GetDepth());
            break;
        case GL_TEXTURE_CUBE_MAP_ARRAY:
            glTexStorage3D(Target, Texture.GetMipCount(), Format.m_format, Texture.GetWidth(), Texture.GetHeight(), Texture.GetArraySize());
            break;
        default:
            glBindTexture(Target, 0);

            LOG_CRITICAL("No target texture in dds");
            return;
        }
        this->texType = Target;
        Texture.Flip();

        uint32_t numFaces = Texture.IsCubemap() ? 6 : 1;
        for (uint32_t layer = 0; layer < Texture.GetArraySize(); layer++) 
        {
            for (uint32_t face = 0; face < numFaces; face++) 
            {
                for (uint32_t level = 0; level < Texture.GetMipCount(); level++) 
                {
                    GLenum target2 = Texture.IsCubemap() ? (GL_TEXTURE_CUBE_MAP_POSITIVE_X + face) : Target;
                    auto imageData = Texture.GetImageData(level, layer * numFaces);
                    switch (Target) 
                    {
                    case GL_TEXTURE_1D:
                        if (IsCompressed(Format.m_format)) 
                            glCompressedTexSubImage1D( target2, level, 0, imageData->m_width, Format.m_format, imageData->m_memSlicePitch, imageData->m_mem);
                        else 
                            glTexSubImage1D(target2, level, 0, imageData->m_width, Format.m_format, Format.m_type, imageData->m_mem);
                        break;
                    case GL_TEXTURE_1D_ARRAY:
                    case GL_TEXTURE_2D:
                    case GL_TEXTURE_CUBE_MAP: 
                    {
                        auto w = imageData->m_width;
                        auto h = isArray ? layer : imageData->m_height;
                        if (IsCompressed(Format.m_format)) 
                            glCompressedTexSubImage2D( target2, level, 0, 0, w, h, Format.m_format, imageData->m_memSlicePitch, imageData->m_mem);
                        else
                            glTexSubImage2D(target2, level, 0, 0, w, h, Format.m_format, Format.m_type,  imageData->m_mem);
                        break;
                    }
                    default:
                        glBindTexture(Target, 0);

                        LOG_CRITICAL("No target texture in dds");
                        return ;
                    }
                }
            }
        }
        this->texWidth =  Texture.GetWidth();
        this->texHeight =  Texture.GetHeight();
        //this->texComponents = Texture.;
        this->tName = texName;
        this->texID = TextureName;
        //return TextureName;
    }
    
    void TexturePBR::setTextureHDR(const char* texPath, std::string texName, bool texFlip1)
    {
        this->texType = GL_TEXTURE_2D;

        std::string tempPath = std::string(texPath);

        if (texFlip1)
            stbi_set_flip_vertically_on_load(true);
        else
            stbi_set_flip_vertically_on_load(false);

        glGenTextures(1, &this->texID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->texID);

        if (stbi_is_hdr(tempPath.c_str()))
        {
            int width, height, numComponents;
            float* texData = stbi_loadf(tempPath.c_str(), &width, &height, &numComponents, 0);

            this->texWidth = width;
            this->texHeight = height;
            this->texComponents = numComponents;
            this->tName = texName;

            if (texData)
            {
                // Need a higher precision format for HDR to not lose informations, thus 32bits floating point
                if (numComponents == 3)
                {
                    this->texInternalFormat = GL_RGB32F;
                    this->texFormat = GL_RGB;
                }
                else if (numComponents == 4)
                {
                    this->texInternalFormat = GL_RGBA32F;
                    this->texFormat = GL_RGBA;
                }

                glTexImage2D(GL_TEXTURE_2D, 0, this->texInternalFormat, this->texWidth, this->texHeight, 0, this->texFormat, GL_FLOAT, texData);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                glGenerateMipmap(GL_TEXTURE_2D);
            }

            else
            {
                std::cerr << "HDR TEXTURE - FAILED LOADING : " << texPath << std::endl;
            }

            stbi_image_free(texData);
        }

        else
        {
            std::cerr << "HDR TEXTURE - FILE IS NOT HDR : " << texPath << std::endl;
        }

        glBindTexture(GL_TEXTURE_2D, 0);
    }


    void TexturePBR::setTextureHDR(GLuint width, GLuint height, GLenum format, GLenum internalFormat, GLenum type, GLenum minFilter)
    {
        (void)type;
        this->texType = GL_TEXTURE_2D;
        glGenTextures(1, &this->texID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->texID);

        this->texWidth = width;
        this->texHeight = height;
        this->texFormat = format;
        this->texInternalFormat = internalFormat;

        if (format == GL_RED)
            this->texComponents = 1;
        else if (format == GL_RG)
            this->texComponents = 2;
        else if (format == GL_RGB)
            this->texComponents = 3;
        else if (format == GL_RGBA)
            this->texComponents = 4;

        glTexImage2D(GL_TEXTURE_2D, 0, this->texInternalFormat, this->texWidth, this->texHeight, 0, this->texFormat, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLfloat)minFilter);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
    }


    void TexturePBR::setTextureCube(std::vector<const char*>& faces, bool texFlip1)
    {
        this->texType = GL_TEXTURE_CUBE_MAP;

        std::vector<std::string> cubemapFaces;

        for (GLuint j = 0; j < faces.size(); j++)
        {
            std::string tempPath = std::string(faces[j]);
            cubemapFaces.push_back(tempPath);
        }

        if (texFlip1)
            stbi_set_flip_vertically_on_load(true);
        else
            stbi_set_flip_vertically_on_load(false);

        glGenTextures(1, &this->texID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(this->texType, this->texID);

        int width, height, numComponents;
        unsigned char* texData;

        for (GLuint i = 0; i < 6; i++)
        {
            texData = stbi_load(cubemapFaces[i].c_str(), &width, &height, &numComponents, 0);

            if (this->texWidth == NULL && this->texHeight == NULL && this->texComponents == NULL)
            {
                this->texWidth = width;
                this->texHeight = height;
                this->texComponents = numComponents;
            }

            if (texData)
            {
                if (numComponents == 1)
                    this->texFormat = GL_RED;
                else if (numComponents == 3)
                    this->texFormat = GL_RGB;
                else if (numComponents == 4)
                    this->texFormat = GL_RGBA;
                this->texInternalFormat = this->texFormat;

                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, this->texInternalFormat, this->texWidth, this->texHeight, 0, this->texFormat, GL_UNSIGNED_BYTE, texData);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                glGenerateMipmap(this->texType);
            }

            else
            {
                std::cerr << "CUBEMAP TEXTURE - FAILED LOADING : " << cubemapFaces[i] << std::endl;
            }

            stbi_image_free(texData);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(this->texType, 0);
    }


    void TexturePBR::setTextureCube(GLuint width, GLenum format, GLenum internalFormat, GLenum type, GLenum minFilter)
    {
        this->texType = GL_TEXTURE_CUBE_MAP;

        glGenTextures(1, &this->texID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(this->texType, this->texID);

        for (GLuint i = 0; i < 6; ++i)
        {
            if (this->texWidth == NULL && this->texHeight == NULL && this->texComponents == NULL)
            {
                this->texWidth = width;
                this->texHeight = width;
                this->texFormat = format;
                this->texInternalFormat = internalFormat;
            }

            if (format == GL_RED)
                this->texComponents = 1;
            else if (format == GL_RGB)
                this->texComponents = 3;
            else if (format == GL_RGBA)
                this->texComponents = 4;

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, this->texInternalFormat, this->texWidth, this->texHeight, 0, this->texFormat, type, nullptr);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(this->texType, 0);
    }


    void TexturePBR::computeTexMipmap()
    {
        glBindTexture(this->texType, this->texID);
        glGenerateMipmap(this->texType);
    }


    GLuint TexturePBR::getTexID()
    {
        return this->texID;
    }


    GLuint TexturePBR::getTexWidth()
    {
        return this->texWidth;
    }


    GLuint TexturePBR::getTexHeight()
    {
        return this->texHeight;
    }


    std::string TexturePBR::getTexName()
    {
        return this->tName;
    }


    void TexturePBR::useTexture()
    {
        glBindTexture(this->texType, this->texID);
    }
    void TexturePBR::setTexturePath(const char* texPath, std::string texName, bool texFlip1, TextureFileType TexType)
    {
        this->texFileType = TexType;
        this->tName = texName;
        this->tPath = texPath;
        this->texFlip = texFlip1;
    }
}
