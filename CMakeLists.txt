cmake_minimum_required(VERSION 2.6)
SET(CMAKE_CXX_COMPILER /usr/bin/g++)
add_compile_options(-std=c++11)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/build/CMakeModules/")

    project(CGrender)

    find_package(SOIL REQUIRED)

    find_package(OpenGL REQUIRED)
    include_directories(${OpenGL_INCLUDE_DIR})

    find_package(glm REQUIRED)
    include_directories(${GLM_INCLUDE_DIR})

    find_package(GLEW REQUIRED)

    set(ENV{PKG_CONFIG_PATH} /usr/lib/x86_64-linux-gnu/pkgconfig)
    find_package(PkgConfig)
    pkg_search_module(GLFW REQUIRED glfw3)
    message(STATUS "GLFW include is: ${GLFW_INCLUDE_DIRS}")
    message(STATUS "GLFW lib is: ${GLFW_LIBRARIES} meaning ${GLFW_LDFLAGS}")

    find_package(FREEGLUT REQUIRED)
    include_directories(${FREEGLUT_INCLUDE_DIR})

    add_executable(${PROJECT_NAME} project.cpp )

    target_link_libraries(${PROJECT_NAME} ${SOIL_LIBRARY} ${OPENGL_LIBRARY} ${FREEGLUT_LIBRARY} ${GLEW_LIBRARY} ${GLFW_LDFLAGS})
