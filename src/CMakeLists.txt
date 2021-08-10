project(voxelite VERSION 0.1.0)

set(voxelite_SRC
    ${voxelite_SRC}
    src/stb_image.cpp
    src/glad.c
    src/voxel.cpp
    src/chunk.cpp
    src/palette.cpp
    src/model.cpp
    # src/main.cpp
    src/util.cpp
    src/world.cpp
    src/serializer.cpp
    src/worldgen.cpp

    PARENT_SCOPE
)

# set(voxelite_SRC
#     ${voxelite_SRC}
#     stb_image.cpp
#     glad.c
#     voxel.cpp
#     chunk.cpp
#     palette.cpp
#     model.cpp
#     # src/main.cpp
#     util.cpp
#     world.cpp
#     serializer.cpp
#     PARENT_SCOPE
# )



# target_include_directories(libvoxelite PUBLIC ../include/)
