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



<div>
  <img src="/Trophies/Trophy2.png?raw=true" width="400" height="400" />
  <img src="https://github.com/alexdalat/pathtracer-cmake/blob/master/Trophies/Bear%20Spin.gif?raw=true" width="400" height="400" /> 
</div>
