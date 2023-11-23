
file(GLOB SRC_FILES *.cpp)
# set(SRC_FILES main.cpp util.cpp Scene.cpp)

# Create executable
add_executable(${PROJECT_NAME} ${SRC_FILES}) # add files to exectuable

# include directories for glm, imgui, glfw, glad
target_include_directories(
        ${PROJECT_NAME} PUBLIC
        ${imgui-glfw-glad-glm_SOURCE_DIR}/include
        ${imgui-glfw-glad-glm_SOURCE_DIR}/include/glad
        ${imgui-glfw-glad-glm_SOURCE_DIR}/include/glm
        ${imgui-glfw-glad-glm_SOURCE_DIR}/include/imgui
)

# Link libraries
# It is sufficient to only link glm and ImGui since the latter one already contains Glad and GLFW
target_link_libraries(${PROJECT_NAME} imgui glm)

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC ../include)
