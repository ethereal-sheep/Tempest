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
        GLuint texID, texWidth, texHeight, texComponents;
        GLfloat anisoFilterLevel;
        GLenum texType, texInternalFormat, texFormat;
        std::string texName;

        TexturePBR();
        ~TexturePBR();
        void setTexture(const char* texPath, std::string texName, bool texFlip);
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
        void setTextureDDS(const char* texPath, std::string texName, bool texFlip);
    };


}