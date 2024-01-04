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
        // first rotate then translate
        transformationMatrix = transformationMatrix * rotationMatrix;

        return transformationMatrix;
    }

    // Movement functions
    /// @brief Moves the transform by the given vector
    /// @details Moves the transform by the given vector
    /// @param v The vector to move the transform by
    void move(const Vec &v)
    {
        this->position = this->position + v;
    }

    /// rotates the transform by the given quaternion
    /// @details rotates the transform by the given quaternion
    /// @param q The quaternion to rotate the transform by
    void rotate(const Quaternion &q)
    {
        this->rotation = this->rotation * q;
    }

    /// @brief Scales the transform by the given vector
    /// @details Scales the transform by the given vector
    /// @param v The vector to scale the transform by
    void scaleBy(const Vec &v)
    {
        this->scale = this->scale * v;
    }

    /// @brief Scales the transform by the given scalar
    /// @details Scales the transform by the given scalar
    /// @param s The scalar to scale the transform by
    void scaleBy(float s)
    {
        this->scale = this->scale * s;
    }


    /// @brief Returns a string representation of this transform
    /// @details Returns a string representation of this transform
    std::string toString() const
    {
        std::stringstream ss;
        ss << "Transform(" << this->position.toString() << ", " << this->rotation.toString() << ", " << this->scale.toString() << ")";
        return ss.str();
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

    /// @brief Returns a string representation of this render info
    /// @details Returns a string representation of this render info
    std::string toString() const
    {
        std::stringstream ss;
        std::string meshStr = this->mesh == nullptr ? "nullptr" : this->mesh->toString();
        ss << "RenderInfo(" << meshStr << ")";
        return ss.str();
    }
};

/// @brief A node in the scene graph
/// @details A node is an entity that has a parent and children
class TransformNode : public std::enable_shared_from_this<TransformNode>
{
public:
    std::shared_ptr<TransformNode> parent;
    std::vector<std::shared_ptr<TransformNode>> children;
    Transform transform;
    RenderInfo renderInfo;

    TransformNode() : parent(nullptr), children(std::vector<std::shared_ptr<TransformNode>>()), transform(Transform()), renderInfo() {}
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

    /// @brief returns the string representation of this node
    /// @details returns the string representation of this node
    std::string toString() const
    {
        std::stringstream ss;
        ss << "TransformNode(\n";
        ss << "  " << this->transform.toString() << ",\n";
        ss << "  " << this->renderInfo.toString() << "\n";
        return ss.str();
    }

    // Iterator setup
    class TransformNodeIterator
    {
    public:
        TransformNodeIterator() = default; // End iterator
        explicit TransformNodeIterator(std::shared_ptr<const TransformNode> root)
        {
            if (root)
            {
                stack.push(root);
            }
        }

        // The dereference operator
        std::shared_ptr<const TransformNode> operator*() const
        {
            return stack.top();
        }

        // The increment operator
        TransformNodeIterator &operator++()
        {
            if (stack.empty())
            {
                throw std::out_of_range("Iterator cannot be incremented past end");
            }

            auto current = stack.top();
            stack.pop();

            for (auto it = current->children.rbegin(); it != current->children.rend(); ++it)
            {
                stack.push(*it);
            }

            return *this;
        }

        // The equality comparison operator
        bool operator==(const TransformNodeIterator &other) const
        {
            return (stack.empty() && other.stack.empty()) ||
                   (!stack.empty() && !other.stack.empty() && stack.top() == other.stack.top());
        }

        // The inequality comparison operator
        bool operator!=(const TransformNodeIterator &other) const
        {
            return !(*this == other);
        }

    private:
        std::stack<std::shared_ptr<const TransformNode>> stack;
    };

    TransformNodeIterator begin() const
    {
        return TransformNodeIterator(shared_from_this());
    }

    TransformNodeIterator end() const
    {
        return TransformNodeIterator();
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
    TransformNode::TransformNodeIterator begin() const
    {
        return this->root->begin();
    }

    TransformNode::TransformNodeIterator end() const
    {
        return this->root->end();
    }
};

#endif // __SCENE_GRAPH_H__