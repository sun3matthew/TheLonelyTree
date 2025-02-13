# Minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(TheLonelyTree VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find all source files in src/
file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/src/*.c)

# macOS specific settings
if ( BUILD_TARGET STREQUAL "OSX" )
    message("Building for OSX target")
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SRC_FILES})
    set_target_properties(${CUR_TARGET} PROPERTIES
        BUNDLE True
        # MACOSX_BUNDLE_GUI_IDENTIFIER my.domain.style.identifier.${PROJECT_NAME}
        MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
        MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/Info.plist.in
    )
    
    # Copy resources directory to the build folder
    set(BUNDLE_RESOURCES_DIR "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app/Contents/resources")
    file(COPY ${CMAKE_SOURCE_DIR}/resources/ DESTINATION ${BUNDLE_RESOURCES_DIR})
else ()
    message("Building for Base target")

    add_executable(${PROJECT_NAME} ${SRC_FILES})
endif () 


# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include )

include_directories(external/PerlinNoise)
include_directories(external/http)

set(TINYGLTF_HEADER_ONLY ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL OFF CACHE INTERNAL "" FORCE)
add_subdirectory(external/tinygltf)
target_include_directories(${PROJECT_NAME} PRIVATE "external/tinygltf")

if ( BUILD_TARGET STREQUAL "OSX" )
    set(OPENSSL_USE_STATIC_LIBS ON)
    find_package(OpenSSL REQUIRED)

    # Assuming OpenSSL was found correctly
    set(OPENSSL_LIBS ${OPENSSL_LIBRARIES})
    set(OPENSSL_INCLUDE_DIR ${OPENSSL_INCLUDE_DIR})

    # Copy OpenSSL libraries to the app's Frameworks directory
    set(APP_FRAMEWORKS_DIR "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app/Contents/Frameworks")
    file(COPY ${OPENSSL_LIBS} DESTINATION ${APP_FRAMEWORKS_DIR})

    # Link OpenSSL to your app
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENSSL_LIBS})

else ()
    find_package(OpenSSL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL)
endif ()

# Add Steamworks SDK
set(STEAMWORKS_DIR ${CMAKE_SOURCE_DIR}/external/steamworks)
include_directories(${STEAMWORKS_DIR}/public)
if (APPLE)
    set(STEAM_LIB_PATH ${STEAMWORKS_DIR}/redistributable_bin/osx/libsteam_api.dylib)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${STEAM_LIB_PATH})

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${STEAM_LIB_PATH} $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
elseif (UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${STEAMWORKS_DIR}/redistributable_bin/linux64/libsteam_api.so)
endif()

set(STEAM_APPID_PATH ${CMAKE_SOURCE_DIR}/steam_appid.txt)
set(STEAM_APPID_DEST ${CMAKE_BINARY_DIR}/steam_appid.txt)
add_custom_target(copy_steam_appid ALL
    COMMAND ${CMAKE_COMMAND} -E copy ${STEAM_APPID_PATH} ${STEAM_APPID_DEST}
)
add_dependencies(${PROJECT_NAME} copy_steam_appid)

add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

add_library(stb STATIC external/stb/src/stb_image.cpp)
target_include_directories(stb PUBLIC external/stb/include)
target_link_libraries(${PROJECT_NAME} PRIVATE stb)

add_subdirectory(external/freetype)
target_link_libraries(${PROJECT_NAME} PRIVATE freetype) # TODO make sure this is correctly included into the build

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

