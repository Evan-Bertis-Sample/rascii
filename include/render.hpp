#ifndef __RENDER_H__
#define __RENDER_H__

// Header file for all things related to rendering (independent of display)
// Rendering, rasterization, etc.

// notes for development:
// - look into the possibility of using a raytracing-based pipeline -- that would be funny

// Dependencies
#include <iostream>
#include <string>
#include <algorithm>
#include <memory>

#include "tex.hpp"
#include "vec.hpp"
#include "matrix.hpp"
#include "mesh.hpp"
#include "scene_graph.hpp"

/// @brief The interface that all renderers must implement
/// @details A renderer is responsible for taking a scene graph and rendering it into a texture representation
/// @details This texture representation can then be rendered to the screen via a displayer
/// @note This is an interface, and should not be instantiated
class IRenderer {
public:
    /// @brief Default constructor
    /// @details Initializes the renderer to the default values
    IRenderer() {}

    /// @brief Renders the given scene graph to the output
    /// @details This is the main function of the renderer
    /// @param sceneGraph The scene graph to render
    virtual void render(const SceneGraph& sceneGraph) = 0;

    /// @brief Prepares the renderer for rendering
    /// @details This function is called before rendering
    virtual void prepare() = 0;

    /// @brief Cleanup output
    /// @details This function is called after rendering
    virtual void cleanup() = 0;

    /// @brief Gets the output texture
    /// @details Returns the output texture
    /// @return The output texture
    virtual std::shared_ptr<Texture> getOutput() const = 0;
};

struct RenderSettings {
    int width;
    int height;
    float fov;
    float near;
    float far;
};

/// @brief The RASCII renderer
/// @details This renderer renders the scene graph to a texture
/// @details The texture is then rendered to the screen via a displayer
class RasciiRenderer : public IRenderer {
public:
    /// @brief Default constructor
    /// @details Initializes the renderer to the default values
    RasciiRenderer();


    /// @brief Constructor
    /// @details Initializes the renderer to the given values
    RasciiRenderer(RenderSettings settings) : _settings(settings)
    {
        this->_outputPtr = std::make_shared<Texture>(settings.width, settings.height);
        this->_textureDrawer = TextureDrawer(this->_outputPtr);

        // calculate the 
    }

    /// @brief Renders the given scene graph to the output
    void render(const SceneGraph& sceneGraph)
    {
        for (auto &node : sceneGraph)
        {
            Transform transform = node->transform;
            Matrix transformationMatrix = transform.toTransformationMatrix();

            // transform the mesh
            Mesh transformedMesh = node->renderInfo.mesh->transform(transformationMatrix);

            for (auto &triangle : transformedMesh)
            {
                // convert the triangle from world space to screen space

            }
        }
    }

    /// @brief Prepares the renderer for rendering
    /// @details This function is called before rendering
    void prepare()
    {

    }

    /// @brief Cleanup output
    /// @details This function is called after rendering
    void cleanup()
    {

    }

    /// @brief Gets the output texture
    /// @details Returns the output texture
    /// @return The output texture
    std::shared_ptr<Texture> getOutput() const
    {
        return this->_outputPtr;
    }

private:
    std::shared_ptr<Texture> _outputPtr;
    TextureDrawer _textureDrawer;
    RenderSettings _settings;

    Matrix _projectionMatrix;
    Matrix _viewMatrix;
    Matrix _pvMatrix; // projection * view

    /// @brief Converts the given world position to a normalized screen position (-1,-1) to (1,1)
    /// @param worldPos 
    /// @return The normalized screen position
    Vec worldToScreen(const Vec& worldPos)
    {
        return this->_projectionMatrix * worldPos;
    }

    /// @brief Converts the given screen position to a texture position
    /// @param screenPos The screen position
    /// @return The texture position
    Vec screenToTexture(const Vec& screenPos)
    {
        return this->_viewMatrix * screenPos;
    }

    /// @brief Converts a given world position to a texture position
    /// @details More optimized than two separate calls to worldToScreen and screenToTexture
    /// @param worldPos The world position
    /// @return The texture position
    Vec worldToTexture(const Vec& worldPos)
    {
        return this->_pvMatrix * worldPos;
    }

    void generateMatrices()
    {
        // generate the projection matrix
    }
};

#endif // __RENDER_H__