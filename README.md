# pathtracer-cmake

A path tracing rendering engine created in C++ with CMake compatibility.

## Requirements
 - [Conan](https://conan.io/)
 - [libpng-conan](https://conan.io/center/libpng)
 - [glm-conan](https://conan.io/center/glm)
 
 ### Build from source
 ```bash
cd pathtracer-cmake
mkdir build && mkdir conan-build && cd conan-build
conan install .. --build
cd ../build && cmake ..
 ```
Be sure to have a valid `bin/imgs/` directory (`../imgs/ relative to the executable`) for the program to output to.

![Example 1](https://github.com/Xeladarocks/pathtracer-cmake/blob/master/imgs/Trophy2.png?raw=true)
