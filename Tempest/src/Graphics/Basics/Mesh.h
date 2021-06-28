#pragma once
#include "Core.h"
#include "VertexArray.h"

namespace Tempest
{
    class Mesh : public std::enable_shared_from_this<Mesh>
    {
        VAO m_VAO;

    public:
        tsptr<Mesh> GetShared()
        {
            return shared_from_this();
        };


    };

}