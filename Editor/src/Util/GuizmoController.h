/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
                or disclosure of this file or its contents without the prior
                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Extern/imgui/imgui.h"
#include "Extern/imgui/ImGuizmo.h"
#include "TMath.h"

namespace Tempest
{
    class GuizmoController
    {
    public:

        enum struct Operation {
            TRANSLATE = ImGuizmo::OPERATION::TRANSLATE,
            ROTATE = ImGuizmo::OPERATION::ROTATE,
            SCALE = ImGuizmo::OPERATION::SCALE,
            BOUNDS = ImGuizmo::OPERATION::BOUNDS,
        };

        enum struct Mode {
            LOCAL = ImGuizmo::MODE::LOCAL,
            WORLD = ImGuizmo::MODE::WORLD
        };

        enum struct State {
            NOT_IN_USE,
            START_USE,
            END_USE,
            IN_USE
        };

        void Draw();
        void SetViewportBounds(const els::vec2& pos, const els::vec2& extends);
        void SetTranslateRotationScale(const els::vec3& t, const els::vec3& r, const els::vec3& s);
        void SetTransformMatrix(const float* data);
        void SetOperation(Operation);
        void SetMode(Mode);
        void SetViewMatrix(const float*);
        void SetProjMatrix(const float*);
        void SetEnabled(bool);
        void Enable();
        void Disable();

        bool IsEnabled() const { return m_IsEnabled; }
        void GetTranslateRotationScale(els::vec3&, els::vec3&, els::vec3&);
        void GetDelta(els::vec3& translate, els::vec3& rotation, els::vec3& scale);

        Operation GetOperation() const { return m_Operation; }
        Mode GetMode() const { return m_Mode; }
        State GetState() const { return m_State; }

    private:
        Operation m_Operation{ Operation::TRANSLATE};
        Mode m_Mode{ Mode::LOCAL};
        State m_State{ State::NOT_IN_USE };


        els::mat4 m_ViewMatrix{ els::mat4::I };
        els::mat4 m_ProjMatrix{ els::mat4::I };
        els::mat4 m_Transform{ els::mat4::I };
        els::mat4 m_DeltaTransform{ els::mat4::I };

        els::vec3 m_CurrentPos{};
        els::vec2 m_ViewportPos{};
        els::vec2 m_ViewportExtends{};

        bool m_IsEnabled{true};

    };
}