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

struct CharacterCutsceneSpawnEmitter_3D;

namespace Tempest
{
    class PlaceUnitCinematic : public Window
    {
        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override;

        void popup(const Event& e);

        void show(Instance&) override;

        bool show_overlay = false;
        bool first = true;
        bool should_play = true;
        bool should_play_ended = false;
        float timer = 0.f;
        tvector<Entity> chars;
        vec3 midpt = vec3{ 0,0,0 };
        quat start = glm::quat_identity<float, glm::highp>();
        CameraControls cam_ctrl;
        interpolater<float> inter;

        std::vector<std::weak_ptr<CharacterCutsceneSpawnEmitter_3D>> m_characterSpawnEmitterVector;
    };
}