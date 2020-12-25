# pathtracer-cmake

A path tracing rendering engine created in C++ with CMake compatibility.

## Requirements
 - [Conan](https://conan.io/)
 - [libpng-conan](https://conan.io/center/libpng)
 - [glm-conan](https://conan.io/center/glm)
 
 ### Utilize source with conan
 ```bash
 cd pathtracer-cmake
 mkdir build && cd build
 conan install .. --build
 cmake ..
 ```

![Example 1](https://github.com/Xeladarocks/pathtracer-cmake/blob/master/imgs/Trophy2.png?raw=true)
