# pathtracer-cmake

A path tracing rendering engine created in C++ with CMake compatibility.

## Build Requirements
 - [CMake](https://cmake.org/)
 - [Conan](https://conan.io/)
 
 ### Build from source
 ```bash
cd pathtracer-cmake
# set up directories
mkdir build && mkdir conan-build
# build requirements
cd conan-build && conan install .. --build=missing
# build project
cd ../build && cmake ..
 ```
NOTE: Be sure to have a valid `bin/imgs/` directory (`../imgs/ relative to the executable`) for the program to output to.



<div>
  <img src="/Trophies/Trophy2.png?raw=true" width="60%" height="60%" />
  <!--<img src="https://github.com/alexdalat/pathtracer-cmake/blob/master/Trophies/Bear%20Spin.gif?raw=true" width="610" height="350" />-->
</div>
