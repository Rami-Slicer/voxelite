cmake_minimum_required(VERSION 3.0.0)
project(voxelite VERSION 0.1.0)

include(CTest)
enable_testing()

# file(GLOB voxelite_SRC DEPENDS_SOU)

# set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
# set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

add_subdirectory(src/)

# add_library(voxelite_render,)





link_libraries(glfw GL X11 pthread Xrandr Xi dl assimp noise z boost_iostreams)
find_package(Boost REQUIRED)
link_directories(lib/)

add_executable(voxelite src/main.cpp)

target_include_directories(voxelite PUBLIC include/)

target_link_libraries(voxelite libvoxelite)

add_custom_command(TARGET voxelite POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E rm -rf $<TARGET_FILE_DIR:voxelite>/assets
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:voxelite>/assets
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
