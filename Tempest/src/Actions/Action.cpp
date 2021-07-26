
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
}
