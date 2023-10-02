
file(GLOB SRC_FILES *.cpp *.c)

add_executable(${PROJECT_NAME} ${SRC_FILES}) # add files to exectuable
target_include_directories(${PROJECT_NAME} PUBLIC ../include)

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBPNG_LIBRARIES} ${ZLIB_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBPNG_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS} glm)
