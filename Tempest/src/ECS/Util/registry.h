/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
                or disclosure of this file or its contents without the prior
                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "../Entity.h"
#include "../../Memory.h"


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
		 * @brief Constructs the registry object; fills the available pool with 
         * new entities
         * @param mem Pointer to a polymorphic memory resource; defaults to
		 * default resource provided by the standard library
		 */
        registry(m_resource* mem = std::pmr::get_default_resource()) : entities(mem), available_pool(mem)
        {
            available_pool.reserve(MAX_ENTITY);
            for (int i = MAX_ENTITY; i > 0 ; --i) available_pool.push_back(i);
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

            auto id = available_pool.back();
            entities.insert(id);
            available_pool.pop_back();
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
            return true;
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
            for (int i = MAX_ENTITY; i > 0; --i) available_pool.push_back(i);
        }
        
		/**
		 * @brief Clears all live entities. Default using hard_clear strategy.
		 */
        void clear()
        {
            hard_clear();
        }

        /**
         * @brief Force creates an entity in the registry
         * @warning Registry must be cleaned after to maintain safe id!
         * @return New entity identifier
         */
        Entity force_create(Entity entity)
        {
            // assert not invalid
            if (entity == INVALID) return INVALID;

            if (!entities.count(entity))
                entities.insert(entity);

            return entity;
        }

        /**
         * @brief Cleans the available pool to maintain safe id
         * @warning Registry must be cleaned after any force_create!
         */
        void clean()
        {
            available_pool.clear();
            for (int i = MAX_ENTITY; i > 0; --i) 
                if(!entities.count(i))
                    available_pool.push_back(i);
        }

    private:

        tset<Entity> entities;
        EnVector available_pool;

    };
}