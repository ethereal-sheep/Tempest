#pragma once

#include "../Entity.h"

namespace Tempest
{
	/**
	 * @brief Entity registry system to register entity ids and recycle 
     * destroyed ids.
	 */
    class registry
    {

    public:
    
		/**
		 * @brief Default Constructor: fills the available pool with new 
         * entities
		 */
        registry()
        {
            for (int i = 1; i < MAX_ENTITY; ++i) available_pool.push_back(i);
        }

		/**
		 * @brief Checks if entity is valid or not
		 * @param entity An entity identifier, either valid or not
         * @return True if entity is valid; False if not
		 */
        [[nodiscard]] bool valid(Entity entity) const
        {
            return entities.count(entity);
        }

		/**
		 * @return Number of live entities
		 */
        [[nodiscard]] size_t size() const
        {
            return entities.size();
        }

		/**
		 * @return Number of available entities
		 */
        [[nodiscard]] size_t available() const
        {
            return available_pool.size();
        }

		/**
		 * @brief Creates a new empty entity
		 * @return New entity identifier
		 */
        [[nodiscard]] Entity create()
        {
            // assert here
            // assert(available() != 0)

            auto id = available_pool.front();
            entities.insert(id);
            available_pool.pop_front();
            return id;
        }

		/**
		 * @brief Destroys the entity and returns the success state
		 * @param entity An entity identifier, either valid or not
		 * @return Success state of the destruction
		 */
        bool destroy(Entity entity)
        {
            if (!valid(entity))
                return false;
            // warn here
            
            available_pool.push_back(entity);
            entities.erase(entity);
        }

		/**
		 * @brief Clears all live entities and adds them back to the pool
		 */
        void easy_clear()
        {
            // TODO
        }

		/**
		 * @brief Clears all live entities and available entities, then 
         * recreates the available list
		 */

        void hard_clear()
        {
            entities.clear();
            available_pool.clear();
            for (int i = 1; i < MAX_ENTITY; ++i) available_pool.push_back(i);
        }
        
		/**
		 * @brief Clears all live entities. Default using hard_clear strategy.
		 */
        void clear()
        {
            hard_clear();
        }

    private:

        tset<Entity> entities;
        tqueue<Entity> available_pool;

    };
}