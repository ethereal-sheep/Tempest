#pragma once

#include "../Entity.h"

namespace Tempest
{
    class registry
    {

    public:
        registry()
        {
            for (int i = 1; i < MAX_ENTITY; ++i) available_pool.push_back(i);
        }


        bool valid(Entity entity) const
        {
            return entities.count(entity);
        }

        size_t size() const
        {
            return entities.size();
        }
        size_t available() const
        {
            return available_pool.size();
        }

        Entity create()
        {
            // assert here
            // assert(available() != 0)

            auto id = available_pool.front();
            entities.insert(id);
            available_pool.pop_front();
            return id;
        }

        bool destroy(Entity entity)
        {
            if (!valid(entity))
                return false;
            // warn here
            
            available_pool.push_back(entity);
            entities.erase(entity);
        }

    private:

        tset<Entity> entities;
        tqueue<Entity> available_pool;

    };
}