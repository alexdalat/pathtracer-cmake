cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
project(pathtracer-cmake)

# Remove the Conan setup
# include(conan-build/conanbuildinfo.cmake)
# conan_basic_setup()

# Add the FetchContent module
include(FetchContent)

# Fetch libpng
FetchContent_Declare(
    libpng
    URL https://github.com/glennrp/libpng/archive/refs/tags/v1.6.37.tar.gz
)
FetchContent_MakeAvailable(libpng)

# Fetch glm
FetchContent_Declare(
    glm
    URL https://github.com/g-truc/glm/archive/refs/tags/0.9.9.8.tar.gz
)
FetchContent_MakeAvailable(glm)

# Fetch zlib - since libpng depends on it
FetchContent_Declare(
    zlib
    URL https://github.com/madler/zlib/archive/refs/tags/v1.2.11.tar.gz
)
FetchContent_MakeAvailable(zlib)


add_subdirectory(src)

