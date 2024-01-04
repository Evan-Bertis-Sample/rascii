#ifndef __MESH_H__
#define __MESH_H__

// Header file for all things related to meshes
// MeshVertex, Triangles, Mesh, and MeshImporters

// notes for development:
// - possibly implement material system
// - meshes ought to only contain their triangles
// - for rendering purposes, we need more information than just the triangles, like materials, orientation, placement in object hierarchy, etc.
// - I ought to implement some MeshObject class that contains a mesh and all of the information needed for rendering
// - perhaps that meshrenderer can map a set of traingles to a material, and then render that material

// Dependencies
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <map>

#include "vec.hpp"
#include "matrix.hpp"

struct MeshVertex {
    Vec position; // 16 bytes
    Vec normal; // 16 bytes

    /// @brief Default constructor
    /// @details Initializes the vertex to the default values
    MeshVertex() {}

    /// @brief Constructor
    /// @details Initializes the vertex to the given values
    /// @param position The position of the vertex
    /// @param normal The normal of the vertex
    MeshVertex(const Vec& position, const Vec& normal) : position(position), normal(normal) {}

    /// @brief Copy constructor
    /// @details Initializes the vertex to the given vertex
    /// @param vertex The vertex to copy
    MeshVertex(const MeshVertex& vertex) : position(vertex.position), normal(vertex.normal) {}

};

/// @brief A triangle is a collection of 3 vertices
/// @details A mesh is considered to be visible when the triangles are ordered in a clockwise fashion (respecting the view)
struct Triangle {
    MeshVertex v1, v2, v3;

    /// @brief Default constructor
    /// @details Initializes the triangle to the default values
    Triangle() {}

    /// @brief Constructor
    /// @details Initializes the triangle to the given values
    /// @param v1 The first vertex of the triangle
    /// @param v2 The second vertex of the triangle
    /// @param v3 The third vertex of the triangle
    Triangle(const MeshVertex& v1, const MeshVertex& v2, const MeshVertex& v3) : v1(v1), v2(v2), v3(v3) {}

    /// @brief Constructor
    /// @details Initializes the triangle based upon the given vertices -- automatically calculates the normal
    /// @param v1 The first vertex of the triangle
    /// @param v2 The second vertex of the triangle
    /// @param v3 The third vertex of the triangle
    Triangle(const Vec& v1, const Vec& v2, const Vec& v3) : v1(MeshVertex(v1, Vec())), v2(MeshVertex(v2, Vec())), v3(MeshVertex(v3, Vec())) {
        // calculate the normal
        Vec normal = (v2 - v1).cross(v3 - v1);
        normal.normalizeSelf();
        this->setNormal(normal);
    }

    /// @brief Copy constructor
    /// @details Initializes the triangle to the given triangle
    /// @param triangle The triangle to copy
    Triangle(const Triangle& triangle) : v1(triangle.v1), v2(triangle.v2), v3(triangle.v3) {}

    /// @brief Returns a triangle centered at the origin
    /// @details Returns a triangle centered at the origin (if -x is to the left, +x is to the right, -y is down, +y is up, the triangle visible)
    static Triangle centeredTriangle() {
        return Triangle(Vec(-1, -1, 0), Vec(0, 1, 0), Vec(-1, 1, 0));
    }

    /// @brief Sets the normal of the triangle to the given normal
    /// @param normal 
    void setNormal(const Vec& normal) {
        this->v1.normal = normal;
        this->v2.normal = normal;
        this->v3.normal = normal;
    }

    /// @brief Sets the normal of the vertex to be the auto-calculated normal
    void setAutoNormal() {
        Vec normal = (this->v2.position - this->v1.position).cross(this->v3.position - this->v1.position);
        normal.normalizeSelf();
        this->setNormal(normal);
    }

    /// @brief Returns a copy of the triangle with the vertices reversed
    Triangle reverse () {
        return Triangle(this->v3, this->v2, this->v1);
    }

    /// @brief Reverses the vertices of the triangle
    void reverseSelf() {
        MeshVertex temp = this->v1;
        this->v1 = this->v3;
        this->v3 = temp;
    }

    /// @brief Returns a string representation of the triangle
    std::string toString() const {
        std::stringstream ss;
        ss << "Triangle(" << std::endl;
        ss << "  " << this->v1.position.toString() << std::endl;
        ss << "  " << this->v2.position.toString() << std::endl;
        ss << "  " << this->v3.position.toString() << std::endl;
        ss << ")";
        return ss.str();
    }
};

/// @brief A mesh is a collection of triangles
class Mesh {
public:
    std::vector<Triangle> triangles;

    /// @brief Default constructor
    /// @details Initializes the mesh to the default values
    Mesh() : triangles(std::vector<Triangle>()) {}

    /// @brief Constructor
    /// @details Initializes the mesh to the given values
    /// @param triangles The triangles of the mesh
    Mesh(std::vector<Triangle> triangles) : triangles(triangles) {
        // shrink the vector to fit
        this->triangles.shrink_to_fit();
    }

    /// @brief Copy constructor
    /// @details Initializes the mesh to the given mesh
    /// @param mesh The mesh to copy
    Mesh(const Mesh& mesh) : triangles(mesh.triangles) {}

    /// @brief Returns a quad centered at the origin
    /// @details Returns a quad centered at the origin (if -x is to the left, +x is to the right, -y is down, +y is up, the quad visible)
    static Mesh centeredQuad() {
        return Mesh(std::vector<Triangle>({
            Triangle(Vec(1, 1, 0), Vec(1, -1, 0), Vec(-1, -1, 0)),
            Triangle(Vec(-1, -1, 0), Vec(-1, 1, 0), Vec(1, 1, 0))
        }));
    }

    /// @brief Returns the number of triangles in the mesh
    int getTriangleCount() const {
        return this->triangles.size();
    }

    /// @brief Returns the number of vertices in the mesh
    int getVertexCount() const {
        return this->getTriangleCount() * 3;
    }

    Mesh transform(const Matrix& transformationMatrix) const {
        Mesh transformedMesh = Mesh();
        int triCount = this->getTriangleCount();
        if (triCount == 0) {
            return transformedMesh;
        }
        transformedMesh.triangles = std::vector<Triangle>(triCount);
        for (int i = 0; i < triCount; i++) {
            std::cout << "Transforming triangle " << i << std::endl;
            Triangle triangle = this->triangles[i];
            Triangle newTri = Triangle(
                MeshVertex(transformationMatrix * triangle.v1.position, transformationMatrix * triangle.v1.normal),
                MeshVertex(transformationMatrix * triangle.v2.position, transformationMatrix * triangle.v2.normal),
                MeshVertex(transformationMatrix * triangle.v3.position, transformationMatrix * triangle.v3.normal)
            );
            transformedMesh.triangles[i] = newTri;
        }
        return transformedMesh;
    }

    Mesh move(const Vec& translation) const {
        Matrix transformationMatrix = Matrix::translation(translation);
        return this->transform(transformationMatrix);
    }

    std::string toString() const {
        std::stringstream ss;
        ss << "Mesh(" << std::endl;
        ss << "  " << "Triangle Count: " << this->getTriangleCount() << std::endl;
        ss << "  " << "Vertex Count: " << this->getVertexCount() << std::endl;
        ss << "  " << "Triangles:" << std::endl; 
        for (const Triangle& triangle : this->triangles) {
            ss << "  " << triangle.toString() << std::endl;
        }
        ss << ")";
        return ss.str();
    }

    // iterator
    typedef std::vector<Triangle>::iterator iterator;
    typedef std::vector<Triangle>::const_iterator const_iterator;

    iterator begin() {
        return this->triangles.begin();
    }

    iterator end() {
        return this->triangles.end();
    }

};

/// @brief An interface that all mesh importers must implement
class MeshImporter {
public:
    /// @brief Default constructor
    /// @details Initializes the mesh importer to the default values
    MeshImporter() {}

    /// @brief Imports the mesh from the given file
    /// @details Imports the mesh from the given file
    /// @param filename The name of the file to import from
    virtual Mesh import(const std::string& filename) = 0;
};

#endif // __MESH_H__