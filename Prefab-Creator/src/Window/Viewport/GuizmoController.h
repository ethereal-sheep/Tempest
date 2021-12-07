/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
                or disclosure of this file or its contents without the prior
                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Extern/imgui/imgui.h"
#include "Extern/imgui/ImGuizmo.h"
#include "Graphics/Basics/RenderSystem.h"
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


        void Draw(Camera& cam, tc::Transform& transform);
        void Draw(Camera& cam, tc::Local& transform);

        void Set(Camera& cam, tc::Transform& form);
        void Set(Camera& cam, tc::Local& form);

        void SetViewportBounds(const vec2& pos, const vec2& extends);
        void SetTranslateRotationScale(const vec3& t, const vec3& r, const vec3& s);
        void SetTransformMatrix(const float* data);

        void SetRotateSnapping(float s);
        void SetTranslateSnapping(float s);
        void SetScaleSnapping(float s);

        void SetOperation(Operation);
        void SetMode(Mode);
        void SetViewMatrix(const float*);
        void SetProjMatrix(const float*);
        void SetEnabled(bool);
        void Enable();
        void Disable();

        bool IsEnabled() const { return m_IsEnabled; }
        void GetTranslateRotationScale(vec3&, vec3&, vec3&);
        void GetDelta(vec3& translate, vec3& rotation, vec3& scale);

        vec3 GetTDelta() const;
        vec3 GetRDelta() const;
        vec3 GetSDelta() const;
        tc::Transform GetInitial() const;

        Operation GetOperation() const { return m_Operation; }
        Mode GetMode() const { return m_Mode; }
        State GetState() const { return m_State; }

        bool IsUsing() const { return m_State == State::START_USE || m_State == State::IN_USE; }
        bool IsEnd() const { return m_State == State::END_USE; }
        void ForceEnd() { m_State = State::NOT_IN_USE; }

    private:
        Operation m_Operation{ Operation::TRANSLATE };
        Mode m_Mode{ Mode::LOCAL };
        State m_State{ State::NOT_IN_USE };


        mat4 m_ViewMatrix{ 1.f };
        mat4 m_ProjMatrix{ 1.f };
        mat4 m_Transform{ 1.f };
        mat4 m_DeltaTransform{ 1.f };

        tc::Transform m_Initial;

        vec3 m_TDelta{};
        vec3 m_RDelta{};
        vec3 m_SDelta{};

        vec3 m_CurrentPos{};
        vec2 m_ViewportPos{};
        vec2 m_ViewportExtends{};

        float r_snap = 0.f;
        float t_snap = 0.f;
        float s_snap = 0.f;

        bool m_IsEnabled{ true };

    };
}