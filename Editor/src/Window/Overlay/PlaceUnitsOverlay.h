/**********************************************************************************
* \author		Lim Ziyi Jean(ziyijean.lim@digipen.edu),
*				Huang Xurong(h.xurong@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
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
#include <Tempest/src/Audio/AudioEngine.h>

struct CharacterSpawnEmitter_3D;
struct CharacterDespawnEmitter_3D;

namespace Tempest
{
    class PlaceUnitsOverlay : public Window
    {
        bool OpenCombat = true;
        bool OverlayOpen = true;
        CameraControls cam_ctrl;
        int selected = -1;
        tvector<Entity> chars; 
        tvector<Entity> entities;
        //tvector<Entity> sheets;

        const char* window_name() override
        {
            return "";
        }
        void init(Instance&) override;
        void open_popup(const Event& e);

        void show(Instance&) override;
        void DrawSideBar(Instance& instance, ImVec2 size);

        int tutorial_index{ 0 };
        ChannelID voice_line{ 0 };
        bool voice_played[3] = { false, false, false };
        std::weak_ptr<CharacterSpawnEmitter_3D> m_characterSpawnEmitter;
    };
}