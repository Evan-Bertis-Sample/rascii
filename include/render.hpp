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

/// @brief An interface that all renderers must implement
/// @details A renderer is responsible for taking a bunch of meshes and rendering it into a texture
class IRenderer {
public:
    /// @brief Default constructor
    /// @details Initializes the renderer to the default values
    IRenderer() {}

    /// @brief Renders the given meshes to the texture
    /// @details This is the main function of the renderer
    /// @param meshes The meshes to render
    /// @param tex The texture to render to
    virtual void render(const std::vector<Mesh>& meshes, Texture& tex) = 0;

    /// @brief Prepares the renderer for rendering
    /// @details This function is called before rendering
    virtual void prepare() = 0;

    /// @brief Cleanup output
    /// @details This function is called after rendering
    virtual void cleanup() = 0;
};

#endif // __RENDER_H__