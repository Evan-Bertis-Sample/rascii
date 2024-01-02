# rascii

Rascii is a dependency-free, terminal-based 3D renderer written in C++ built for educational purposes. Because there are no extenral libraries (other than the C++ standard library and a little bit of the windows API), this project is completely terminal-based. **Yes, that means the entire output is in the terminal.**

This project was purely made for educational purposes, both as a way to learn about 3D rendering, but also as a way to practice C++ in a non-firmware environment. For those interested, I had to implement/will implement these things from scratch:

* A math library, including vectors, matrices, and transforms.
* A graphics library, including color representation, textures, and an ASCII rasterizer.
* An input library, which supports keyboard and mouse input from the terminal.
* A 3D renderer, including vertex, triangles, meshes, a camera, etc.
* A 3D model loader, including support for .obj files.
* A rendering pipeline, including vertex shading, rasterization, and fragment shading, as well as optimizations like culling and backface culling.

In the current state, the project is single-threaded, and is very-much inspired by the 3D renderers of the early days of game development.

## Building
### Pre-requisites
* A C++ compiler (I use GNU GCC)
* CMake (at least version 3.10)
* Make (at least version 4.1)

### Building
1. Clone the repository.
2. Run the shell script `build.sh` to build the project. This will create a `build` directory, and a `bin` directory in the root of the project.
3. Run the shell script `run.sh` to run the project. This will run the executable in the `bin` directory. *For the best experience, it is best to ensure that your terminal is large enough to display the full frame.*
