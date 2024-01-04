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
class IRenderer
{
public:
    /// @brief Default constructor
    /// @details Initializes the renderer to the default values
    IRenderer() {}

    /// @brief Renders the given scene graph to the output
    /// @details This is the main function of the renderer
    /// @param sceneGraph The scene graph to render
    virtual void render(const SceneGraph &sceneGraph) = 0;

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

struct RenderSettings
{
public:
    int width;
    int height;
    float fov;
    float nearPlane;
    float farPlane;

    // RenderSettings() : width(0), height(0), fov(0.0f), near(0.0f), far(0.0f) {}
    RenderSettings(int width, int height, float fov, float nearPlane, float farPlane) : width(width), height(height), fov(fov), nearPlane(nearPlane), farPlane(farPlane) {}
    RenderSettings(const RenderSettings &settings) : width(settings.width), height(settings.height), fov(settings.fov), nearPlane(settings.nearPlane), farPlane(settings.farPlane) {}

    std::string toString() const
    {
        std::stringstream ss;
        ss << "RenderSettings("
           << "\n";
        ss << "  width: " << this->width << "\n";
        ss << "  height: " << this->height << "\n";
        ss << "  fov: " << this->fov << "\n";
        ss << "  nearPlane: " << this->nearPlane << "\n";
        ss << "  farPlane: " << this->farPlane << "\n";
        ss << "  range: " << this->farPlane - this->nearPlane << "\n";
        ss << ")";
        return ss.str();
    }
};

/// @brief The RASCII renderer
/// @details This renderer renders the scene graph to a texture
/// @details The texture is then rendered to the screen via a displayer
class RasciiRenderer : public IRenderer
{
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
    }

    /// @brief Renders the given scene graph to the output
    void render(const SceneGraph &sceneGraph)
    {
        // fill the texture with black
        this->_textureDrawer.fill(Color::greyscale(0.0f));
        for (auto node : sceneGraph)
        {
            if (node == nullptr || node->renderInfo.mesh == nullptr)
            {
                // std::cout << "Skipping node" << std::endl;
                continue;
            }

            // std::cout << "Rendering node" << std::endl;
            // std::cout << node->toString() << std::endl;

            Transform transform = node->transform;
            Matrix transformationMatrix = node->toTransformationMatrix();

            // transform the mesh
            Mesh transformedMesh = node->renderInfo.mesh->transform(transformationMatrix);

            for (auto &triangle : transformedMesh)
            {
                // convert the triangle from world space to screen space
                Vec v1 = this->worldToTexture(triangle.v1.position);
                Vec v2 = this->worldToTexture(triangle.v2.position);
                Vec v3 = this->worldToTexture(triangle.v3.position);

                // draw the triangle
                this->_textureDrawer.drawTriangle(v1, v2, v3, Color::greyscale(1.0f));
            }
        }
    }

    /// @brief Prepares the renderer for rendering
    /// @details This function is called before rendering
    void prepare()
    {
        this->generateMatrices();
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
    Vec worldToScreen(const Vec &worldPos)
    {
        return this->_projectionMatrix * worldPos;
    }

    /// @brief Converts the given screen position to a texture position
    /// @param screenPos The screen position
    /// @return The texture position
    Vec screenToTexture(Vec screenPos)
    {
        // ensure that the w component is 1
        screenPos.w = 1.0f;
        return this->_viewMatrix * screenPos;
    }

    /// @brief Converts a given world position to a texture position
    /// @details More optimized than two separate calls to worldToScreen and screenToTexture
    /// @param worldPos The world position
    /// @return The texture position
    Vec worldToTexture(Vec worldPos)
    {
        // convert to screen space
        Vec screenPos = this->_projectionMatrix * worldPos;
        screenPos = screenPos / screenPos.w;

        // convert to texture space
        Vec texturePos = this->_viewMatrix * screenPos;
        texturePos = texturePos/texturePos.w;

        return texturePos;
    }

    void generateMatrices()
    {
        // generate the projection matrix
        float aspectRatio = (float)this->_settings.height / (float)this->_settings.width;
        float fov = this->_settings.fov;
        float nearPlane = this->_settings.nearPlane;
        float farPlane = this->_settings.farPlane;
        float fovRad = 1.0f / tanf(fov * 0.5f / 180.0f * 3.14159f);
        float range = farPlane - nearPlane;

        // Projection Matrix
        // | aspectRatio / (tan(fov / 2)) | 0 | 0 | 0 |
        // | 0 | 1 / tan(fov / 2) | 0 | 0 |
        // | 0 | 0 | (far) / range | 1 |
        // | 0 | 0 | (-far * near) / range | 0 |

        this->_projectionMatrix = Matrix();
        this->_projectionMatrix.set(0, 0, aspectRatio * fovRad);
        this->_projectionMatrix.set(1, 1, fovRad);
        this->_projectionMatrix.set(2, 2, farPlane / range);
        this->_projectionMatrix.set(3, 2, (-farPlane * nearPlane) / range);
        this->_projectionMatrix.set(2, 3, 1.0f);
        this->_projectionMatrix.set(3, 3, 0.0f);

        // std::cout << "Projection Matrix: " << std::endl;
        // std::cout << this->_projectionMatrix.toString() << std::endl;

        // generate the view matrix
        // the view matrix converts the normalized screen position to a texture position
        // ie (-1,-1) to (1,1) to (0,0) to (width, height)
        this->_viewMatrix = Matrix();
        this->_viewMatrix.set(0, 0, this->_settings.width / 2.0f);
        this->_viewMatrix.set(1, 1, this->_settings.height / 2.0f);
        this->_viewMatrix.set(0, 3, this->_settings.width / 2.0f);
        this->_viewMatrix.set(1, 3, this->_settings.height / 2.0f);

        // std::cout << "View Matrix: " << std::endl;
        // std::cout << this->_viewMatrix.toString() << std::endl;

        // generate the pv matrix
        this->_pvMatrix = this->_projectionMatrix * this->_viewMatrix;

        // std::cout << "PV Matrix: " << std::endl;
        // std::cout << this->_pvMatrix.toString() << std::endl;
    }
};

#endif // __RENDER_H__