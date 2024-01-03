#ifndef __ECS_H__
#define __ECS_H__

// Header file for all things related to the ECS (Entity Component System)
// Entities, Components, Systems, etc.

// Dependencies
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <map>

#include "vec.hpp"
#include "matrix.hpp"
#include "quaternion.hpp"
#include "scene_graph.hpp"

/// @brief A component is a piece of data that is attached to an entity
class Component
{
public:
    // points to the entity that this component is attached to
    std::shared_ptr<Entity> entity;

    /// @brief Constructor
    /// @details Initializes the component -- each component must have a parameterless Constructor
    Component() {}

    /// @brief Start function
    /// @details Called when the component is added to an entity or when the entity is started
    virtual void Start() = 0;

    /// @brief Update function
    /// @details Called every frame
    virtual void Update() = 0;
};

/// @brief An entity is an object in the game
/// @details Each entity has a transform and a list of components
class Entity
{
public:
    Transform transform;

    /// @brief Constructor
    /// @details Initializes the entity to the default values
    Entity() : transform(Transform()) {}

    /// @brief Constructor
    /// @details Initializes the entity to the given values 
    /// @param transform The transform of the entity
    Entity(Transform transform) : transform(transform) {}

    /// @brief Copy constructor
    /// @details Initializes the entity to the given entity
    /// @param entity The entity to copy
    Entity(const Entity &entity) : transform(entity.transform) {}

    /// @brief Adds the given component to the entity
    /// @details Adds the given component to the entity
    /// @param component The component to add
    void AddComponent(std::shared_ptr<Component> component)
    {
        // Set the entity of the component
        component->entity = std::shared_ptr<Entity>(this);        
        this->_components.push_back(std::move(component));
        component->Start();
    }

    /// @brief Gets the component of the given type
    /// @details Returns the component of the given type
    /// @tparam T The type of the component to get
    template <typename T>
    std::shared_ptr<T> GetComponent ()
    {
        for (auto &component : this->_components)
        {
            T *t = dynamic_cast<T *>(component.get());
            if (t != nullptr)
            {
                return std::shared_ptr<T>(t);
            }
        }
        return nullptr;
    }

    /// @brief Start function
    /// @details Called when the entity is started
    void Start()
    {
        for (auto &component : this->_components)
        {
            component->Start();
        }
    }

    /// @brief Update function
    /// @details Called every frame
    void Update()
    {
        for (auto &component : this->_components)
        {
            component->Update();
        }
    }

private:
    std::vector<std::shared_ptr<Component>> _components;
};

#endif // __ECS_H__