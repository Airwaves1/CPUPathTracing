
# GLOB_RECURSE 的作用是将 src 目录下的所有 cpp 文件都添加到 SOURCES 变量中
# CONFIGURE_DEPENDS 的作用是当源文件发生变化时，CMake 会重新配置项目
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(CPUPathTracing ${SOURCES})

target_include_directories(CPUPathTracing PUBLIC include)
target_compile_definitions(CPUPathTracing PUBLIC GLM_FORCE_DEPTH_ZERO_TO_ONE GLM_FORCE_RADIANS GLM_FORCE_LEFT_HANDED)
target_link_libraries(CPUPathTracing PUBLIC glm)