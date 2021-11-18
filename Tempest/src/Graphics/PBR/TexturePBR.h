#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <glew.h>

namespace Tempest
{
    class TexturePBR
    {
    public:
        enum TextureFileType
        {
            STBImage,
            DDS,
            HDR,
            Cube,
            
            NoTexture
        };

        GLuint texID = 0, texWidth = 0, texHeight = 0, texComponents = 0;
        GLfloat anisoFilterLevel = 0.0f;
        GLenum texType, texInternalFormat, texFormat;
        std::string tName = {};
        std::string tPath = {};
        bool texFlip = true;
        TextureFileType texFileType = TextureFileType::STBImage;

        TexturePBR();
        ~TexturePBR();
        void setTexture(const char* texPath, std::string texName, bool texFlip);
        void setTexturePath(const char* texPath, std::string texName, bool texFlip = true, TextureFileType TexType = NoTexture);
        void setTextureHDR(const char* texPath, std::string texName, bool texFlip);
        void setTextureHDR(GLuint width, GLuint height, GLenum format, GLenum internalFormat, GLenum type, GLenum minFilter);
        void setTextureCube(std::vector<const char*>& faces, bool texFlip);
        void setTextureCube(GLuint width, GLenum format, GLenum internalFormat, GLenum type, GLenum minFilter);
        void computeTexMipmap();
        GLuint getTexID();
        GLuint getTexWidth();
        GLuint getTexHeight();
        std::string getTexName();
        void useTexture();
        void setTextureDDS(const char* texPath, std::string texName, [[maybe_unused]] bool texFlip);
    };


}
