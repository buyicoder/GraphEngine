cmake_minimum_required(VERSION 3.16)
project(mesh-simplification LANGUAGES C CXX)

if (MSVC OR CMAKE_C_SIMULATE_ID STREQUAL "MSVC")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC "${PROJECT_SOURCE_DIR}/src/")
set(DEPS "${PROJECT_SOURCE_DIR}/deps/")

set(GLFW_DIR "${DEPS}/glfw")
set(GLAD_DIR "${DEPS}/glad")
set(IMGUI_DIR "${DEPS}/imgui")

set(EIGEN "${DEPS}/eigen-3.4.0")
set(EIGEN_UNS "${EIGEN}/unsupported")

set(SHADER_DIR "${SRC}/shaders")
add_compile_definitions(SHADER_DIR="${SHADER_DIR}")
set(RES_DIR "${PROJECT_SOURCE_DIR}/resources")
add_compile_definitions(RESOURCES_DIR="${RES_DIR}")

set(GLFW_INC "${GLFW_DIR}/include")
if (MSVC OR CMAKE_C_SIMULATE_ID STREQUAL "MSVC")
    set(GLFW_LIB "${GLFW_DIR}/lib-vc2022/glfw3dll.lib")
else()
    set(GLFW_LIB "${GLFW_DIR}/lib-mingw-w64/glfw3.dll")
endif()

set(GLAD_INC "${GLAD_DIR}/include")
set(GLAD_SRC "${GLAD_DIR}/src/glad.c")

file(GLOB_RECURSE PRJ_SRC "${SRC}*.cpp")
file(GLOB_RECURSE IMGUI_SRC "${IMGUI_DIR}*.cpp")

add_executable(${PROJECT_NAME} 
    ${PRJ_SRC}
    ${IMGUI_SRC} 
    ${GLAD_SRC}
 "src/utils/WindowSystem.cpp" "src/utils/ModelSystem.h" "src/utils/ModelSystem.cpp"  "src/utils/GraphicsSystem.cpp" "src/utils/InputSystem.h" "src/utils/InputSystem.cpp" "src/utils/GraphicsSystem.h" "src/utils/CameraSystem.h" "src/utils/CameraSystem.cpp" "src/utils/TimeSystem.h" "src/utils/TimeSystem.cpp" "src/utils/InteractSystem.h" "src/utils/InteractSystem.cpp"   "src/utils/FileSystem.h" "src/utils/FileSystem.cpp" "src/utils/InterModeSystem.h" "src/utils/InterModeSystem.cpp" "src/utils/LightSystem.h" "src/utils/LightSystem.cpp"  "src/utils/GridSystem.h" "src/utils/GridSystem.cpp")
target_link_libraries(${PROJECT_NAME} 
    ${GLFW_LIB}
)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${SRC}
    ${GLFW_INC} 
    ${GLAD_INC} 
    ${IMGUI_DIR}
    ${EIGEN}
    ${EIGEN_UNS}
    ${DEPS}
)
# 设置目标的输出路径
set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/out/build/x64-Debug")

# 添加自定义命令以复制glfw3.dll
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${GLFW_DIR}/lib-vc2022/glfw3.dll"
    "glfw3.dll"
)

