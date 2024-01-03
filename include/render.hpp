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
    /// @param output The output to render to
    virtual void render(const SceneGraph& sceneGraph, Texture& output) = 0;

    /// @brief Prepares the renderer for rendering
    /// @details This function is called before rendering
    virtual void prepare() = 0;

    /// @brief Cleanup output
    /// @details This function is called after rendering
    virtual void cleanup() = 0;
};

/// @brief The RASCII renderer
/// @details This renderer renders the scene graph to a texture
/// @details The texture is then rendered to the screen via a displayer
class RasciiRenderer : public IRenderer {
public:
    void render(const SceneGraph& sceneGraph, Texture& output);
    void prepare();
    void cleanup();
private:

};

#endif // __RENDER_H__