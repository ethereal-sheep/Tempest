/**********************************************************************************
* \author		Lim Ziyi Jean(ziyijean.lim@digipen.edu),
*				Huang Xurong(h.xurong@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
                or disclosure of this file or its contents without the prior
                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Events/EventManager.h"
#include <Editor/src/Triggers/Triggers.h>
#include "../Viewport/CameraControls.h"
#include "Util/interpolater.h"

struct Interactive_DoorParticle_3D;

namespace Tempest
{
    class RuntimeStartupOverlay : public Window
    {
        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override;


        void show(Instance&) override;
        void call_first(Instance& instance);

        bool first = true;
        bool should_play = true;
        bool should_play_ended = false;
        float timer = 0.f;
        vec3 midpt = vec3{ 0,0,0 };
        quat start = glm::quat_identity<float, glm::highp>();
        CameraControls cam_ctrl;
        interpolater<float> inter;

        // Not suppose to be here.... @Jun Hao
        std::map<Entity, std::weak_ptr<Interactive_DoorParticle_3D>> m_map_interactiveEmitter_3D;
    };
}