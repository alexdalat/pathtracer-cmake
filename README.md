# pathtracer-cmake

A path tracing rendering engine created in C++ with CMake compatibility.

## Build Requirements
 - [CMake](https://cmake.org/)
 - [Conan](https://conan.io/)
 
 ### Build from source
 ```bash
cd pathtracer-cmake
mkdir build && mkdir conan-build && cd conan-build
conan install .. --build && conan install ..
cd ../build && cmake ..
 ```
NOTE: Be sure to have a valid `bin/imgs/` directory (`../imgs/ relative to the executable`) for the program to output to.

<!--![Example 2](https://github.com/Xeladarocks/pathtracer-cmake/blob/master/imgs/Trophy3.png?raw=true)-->
![Example 1](https://github.com/Xeladarocks/pathtracer-cmake/blob/master/imgs/Trophy3.png?raw=true)
