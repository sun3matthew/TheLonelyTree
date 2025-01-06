# Minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(TheLonelyTree VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find all source files in src/
file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/src/*.c)

# Add executable target
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include )

set(TINYGLTF_HEADER_ONLY ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL OFF CACHE INTERNAL "" FORCE)
add_subdirectory(external/tinygltf)
target_include_directories(${PROJECT_NAME} PRIVATE "external/tinygltf")

add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

add_library(stb STATIC external/stb/src/stb_image.cpp)
target_include_directories(stb PUBLIC external/stb/include)
target_link_libraries(${PROJECT_NAME} PRIVATE stb)

add_subdirectory(external/glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

# Include OpenGL framework or libraries
if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework OpenGL")
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE GL)
endif()

# Compiler definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE GL_SILENCE_DEPRECATION)

# Copy resources directory to the build folder
file(COPY ${CMAKE_SOURCE_DIR}/resources/ DESTINATION ${CMAKE_BINARY_DIR}/resources)
