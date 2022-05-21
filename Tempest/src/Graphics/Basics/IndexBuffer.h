/**********************************************************************************
* \author		Linus Ng Hao Xuan (haoxuanlinus.ng@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
                or disclosure of this file or its contents without the prior
                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include <glew.h>
#include "Core.h"

namespace Tempest
{
    class IndexBuffer : std::enable_shared_from_this<IndexBuffer>
    {
        uint32_t m_ID;
        uint32_t m_Count;

    public:

        IndexBuffer(const uint32_t* array, uint32_t count);
        //IndexBuffer(const IndexBuffer&) = delete;
        //IndexBuffer& operator=(const IndexBuffer&) = delete;
        ~IndexBuffer();

        uint32_t GetID() const;
        uint32_t GetCount() const;

        void Bind() const;
        void Unbind() const;

        tsptr<IndexBuffer> GetShared();
    };
}