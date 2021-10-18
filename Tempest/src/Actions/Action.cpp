/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
                or disclosure of this file or its contents without the prior
                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Action.h"

namespace Tempest
{

    /**********************************************************************
    * EditShape
    ***********************************************************************/
    EditShape::EditShape(Entity entity, SHAPE_TYPE old_Shape, SHAPE_TYPE new_Shape) 
        : ID{entity}, oldShape{old_Shape}, newShape{new_Shape}
    {
    }
    const char* EditShape::Name()
    {
        return "ChangeShape";
    }
    void EditShape::Undo(Instance& instance)
    {
        auto rb = instance.ecs.get_if<tc::Rigidbody>(ID);
        if (rb)
        {
            rb->shape_data.type = oldShape;
            physx::PxShape* tempShape = instance.po.CreateActorShape(rb->rb_config, rb->shape_data);
            const physx::PxU32 numShapes = rb->internal_rb.get()->getNbShapes();

            for (physx::PxU32 i = 0; i < numShapes; i++)
            {
                physx::PxShape* CurShape = nullptr;
                rb->internal_rb.get()->getShapes(&CurShape, 1, i);
                rb->internal_rb.get()->detachShape(*CurShape);
            }
            rb->internal_rb.get()->attachShape(*tempShape);
            tempShape->release();
        }
    }
    void EditShape::Redo(Instance& instance)
    {
        auto rb = instance.ecs.get_if<tc::Rigidbody>(ID);
        if (rb)
        {
            rb->shape_data.type = newShape;
            physx::PxShape* tempShape = instance.po.CreateActorShape(rb->rb_config, rb->shape_data);
            const physx::PxU32 numShapes = rb->internal_rb.get()->getNbShapes();

            for (physx::PxU32 i = 0; i < numShapes; i++)
            {
                physx::PxShape* CurShape = nullptr;
                rb->internal_rb.get()->getShapes(&CurShape, 1, i);
                rb->internal_rb.get()->detachShape(*CurShape);
            }
            rb->internal_rb.get()->attachShape(*tempShape);
            tempShape->release();
        }
    }

    /**********************************************************************
    * EditGravity
    ***********************************************************************/
    EditGravity::EditGravity(Entity entity, bool _gravity) 
        : ID{ entity }, gravity{ _gravity }
    {
    }
    const char* EditGravity::Name()
    {
        return "EditGravity";
    }
    void EditGravity::Undo(Instance& instance)
    {
        auto rb = instance.ecs.get_if<tc::Rigidbody>(ID);
        if (rb)
        {
            rb->rb_config.gravity = gravity;
        }
    }
    void EditGravity::Redo(Instance& instance)
    {
        auto rb = instance.ecs.get_if<tc::Rigidbody>(ID);
        if (rb)
        {
            rb->rb_config.gravity = !gravity;
        }
       
    }

    /**********************************************************************
    * EditStatic
    ***********************************************************************/
    EditStatic::EditStatic(Entity entity, bool _static)
        : ID { entity }, isStatic{ _static }
    {
    }
    const char* EditStatic::Name()
    {
        return "EditStatic";
    }
    void EditStatic::Undo(Instance& instance)
    {
        auto rb = instance.ecs.get_if<tc::Rigidbody>(ID);
        if (rb)
        {
            rb->rb_config.is_static = isStatic;
        }
    }
    void EditStatic::Redo(Instance& instance)
    {
        auto rb = instance.ecs.get_if<tc::Rigidbody>(ID);
        if (rb)
        {
            rb->rb_config.is_static = !isStatic;
        }
    }

    /**********************************************************************
    * EditPosition
    ***********************************************************************/
    EditPosition::EditPosition(Entity entity, vec3 oldPos, vec3 newPos)
        : ID{ entity }, old_position{ oldPos }, new_position{ newPos }
    {
    }
    const char* EditPosition::Name()
    {
        return "EditPosition";
    }
    void EditPosition::Undo(Instance& instance)
    {
        auto& transform = instance.ecs.get<tc::Transform>(ID);
        transform.position = old_position;
    }

    void EditPosition::Redo(Instance& instance)
    {
        auto& transform = instance.ecs.get<tc::Transform>(ID);
        transform.position = new_position;
    }

    /**********************************************************************
    * EditRotation
    ***********************************************************************/
    EditRotation::EditRotation(Entity entity, glm::highp_vec3 oldRotation, glm::highp_vec3 newRotation)
        : ID{ entity }, old_rotation{ oldRotation }, new_rotation{ newRotation }
    {
    }

    const char* EditRotation::Name()
    {
        return "EditRotation";
    }
    void EditRotation::Undo(Instance& instance)
    {
        auto& transform = instance.ecs.get<tc::Transform>(ID);
        transform.rotation = glm::quat(old_rotation);
    }

    void EditRotation::Redo(Instance& instance)
    {
        auto& transform = instance.ecs.get<tc::Transform>(ID);
        transform.rotation = glm::quat(new_rotation);;
    }

    /**********************************************************************
    * EdiEditScaletRotation
    ***********************************************************************/
    EditScale::EditScale(Entity entity, vec3 oldScale, vec3 newScale)
        : ID{ entity }, old_scale{ oldScale }, new_scale{ newScale }
    {
    }

    const char* EditScale::Name()
    {
        return "EditScale";
    }
    void EditScale::Undo(Instance& instance)
    {
        auto& transform = instance.ecs.get<tc::Transform>(ID);
        transform.scale = old_scale;

        auto rb = instance.ecs.get_if<tc::Rigidbody>(instance.selected);

        if (rb)
            rb->isDirty = true;
    }

    void EditScale::Redo(Instance& instance)
    {
        auto& transform = instance.ecs.get<tc::Transform>(ID);
        transform.scale = new_scale;

        auto rb = instance.ecs.get_if<tc::Rigidbody>(instance.selected);

        if (rb)
            rb->isDirty = true;
    }
}
