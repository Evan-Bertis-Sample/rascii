#ifndef __SCENE_GRAPH_H__
#define __SCENE_GRAPH_H__

// Header file for all things related to the scene graph
// Transforms, transform hierarchies, and the scene graph

// Dependencies
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <stack>

#include "vec.hpp"
#include "matrix.hpp"
#include "quaternion.hpp"
#include "mesh.hpp"

/// @brief A component is a piece of data that is attached to an entity
/// @details Every entity has a transform
class Transform
{
public:
    Vec position;
    Quaternion rotation;
    Vec scale;

    Transform() : position(Vec()), rotation(Quaternion()), scale(Vec(1, 1, 1)) {}
    Transform(Vec position, Quaternion rotation, Vec scale) : position(position), rotation(rotation), scale(scale) {}
    Transform(const Transform &t) : position(t.position), rotation(t.rotation), scale(t.scale) {}

    /// @brief Gets the transformation matrix of the transform
    /// @details Returns the transformation matrix of the transform
    Matrix toTransformationMatrix() const
    {
        Matrix transformationMatrix = Matrix();

        // Set the translation information
        transformationMatrix.set(0, 3, this->position.x);
        transformationMatrix.set(1, 3, this->position.y);
        transformationMatrix.set(2, 3, this->position.z);

        // Set the scale information
        transformationMatrix.set(0, 0, this->scale.x);
        transformationMatrix.set(1, 1, this->scale.y);
        transformationMatrix.set(2, 2, this->scale.z);

        Matrix rotationMatrix = this->rotation.toRotationMatrix();
        transformationMatrix = transformationMatrix * rotationMatrix;

        return transformationMatrix;
    }
};

/// @brief Additonal information that is attached to a TransformNode for rendering
/// @details Outlines the material, mesh, and other information that is needed for rendering
class RenderInfo
{
public:
    std::shared_ptr<Mesh> mesh;
    // TODO: implement material system

    RenderInfo() : mesh(nullptr) {}
    RenderInfo(std::shared_ptr<Mesh> mesh) : mesh(mesh) {}
    RenderInfo(const RenderInfo &renderInfo) : mesh(renderInfo.mesh) {}
};

/// @brief A node in the scene graph
/// @details A node is an entity that has a parent and children
class TransformNode
{
public:
    std::shared_ptr<TransformNode> parent;
    std::vector<std::shared_ptr<TransformNode>> children;
    Transform transform;
    RenderInfo renderInfo;

    TransformNode() : parent(nullptr), children(std::vector<std::shared_ptr<TransformNode>>()), transform(Transform()) {}
    TransformNode(Transform transform, RenderInfo rInfo) : parent(nullptr), children(std::vector<std::shared_ptr<TransformNode>>()), transform(transform), renderInfo(rInfo) {}
    TransformNode(const TransformNode &node) : parent(node.parent), children(node.children), transform(node.transform), renderInfo(node.renderInfo) {}

    /// @brief Adds the given node as a child of this node
    /// @details Adds the given node as a child of this node
    /// @param node The node to add as a child
    void addChild(std::shared_ptr<TransformNode> node)
    {
        this->children.push_back(node);
        node->parent = std::shared_ptr<TransformNode>(this);
    }

    /// @brief Gets the transformation matrix of the node
    /// @details Returns the transformation matrix of the node
    Matrix toTransformationMatrix() const
    {
        Matrix transformationMatrix = this->transform.toTransformationMatrix();

        if (this->parent != nullptr)
        {
            transformationMatrix = this->parent->toTransformationMatrix() * transformationMatrix;
        }

        return transformationMatrix;
    }

    /// @brief Returns the local transformationMatrix of this node -- independent of parents
    /// @details Returns the local transformationMatrix of this node -- independent of parents
    Matrix toLocalTransformationMatrix() const
    {
        return this->transform.toTransformationMatrix();
    }

    // Iterator setup
    class TransformNodeIterator
    {
    public:
        TransformNodeIterator(std::shared_ptr<TransformNode> rootNode)
        {
            // Perform depth-first search starting from the root node
            if (rootNode != nullptr)
            {
                traverse(rootNode);
            }
        }

        // Overload the * operator to return the current node
        std::shared_ptr<TransformNode> operator*() const
        {
            return currentNode;
        }

        // Overload the ++ operator to move to the next node
        TransformNodeIterator &operator++()
        {
            if (!nodeStack.empty())
            {
                currentNode = nodeStack.top();
                nodeStack.pop();

                // Traverse the children of the current node
                for (auto child : currentNode->children)
                {
                    traverse(child);
                }
            }
            else
            {
                // If there are no more nodes, set the iterator to nullptr
                currentNode = nullptr;
            }

            return *this;
        }

        // Overload the != operator to check for inequality
        bool operator!=(const TransformNodeIterator &other) const
        {
            return currentNode != other.currentNode;
        }

    private:
        std::shared_ptr<TransformNode> currentNode;
        std::stack<std::shared_ptr<TransformNode>> nodeStack;

        // Helper function to perform depth-first search
        void traverse(std::shared_ptr<TransformNode> node)
        {
            nodeStack.push(node);
        }
    };

    TransformNodeIterator begin()
    {
        return TransformNodeIterator(std::shared_ptr<TransformNode>(this));
    }

    TransformNodeIterator end()
    {
        return TransformNodeIterator(nullptr);
    }
};

/// @brief A scene graph is a collection of nodes
/// @details The root, by default, is the identity matrix
class SceneGraph
{
public:
    std::shared_ptr<TransformNode> root;

    SceneGraph() : root(std::shared_ptr<TransformNode>(new TransformNode())) {}
    SceneGraph(const SceneGraph &sceneGraph) : root(sceneGraph.root) {}

    /// @brief Adds the given node as a child of the root node
    /// @details Adds the given node as a child of the root node
    /// @param node The node to add as a child
    void addChild(std::shared_ptr<TransformNode> node)
    {
        this->root->addChild(node);
    }

    /// @brief Gets the transformation matrix of the scene graph
    /// @details Returns the transformation matrix of the scene graph
    Matrix toTransformationMatrix() const
    {
        return this->root->toTransformationMatrix();
    }

    // Iterator
    TransformNode::TransformNodeIterator begin()
    {
        return this->root->begin();
    }

    TransformNode::TransformNodeIterator end()
    {
        return this->root->end();
    }
};

#endif // __SCENE_GRAPH_H__