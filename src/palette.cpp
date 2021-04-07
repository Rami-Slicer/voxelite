#include <vector>
#include <string>
#include <glm/vec3.hpp>

#include <palette.hpp>
#include <voxel.hpp>

VoxelPalette::VoxelPalette() {
    Voxel empty = Voxel("assets/air.obj", true);
    empty.setInvisible(true);
    this->registerType(empty);
    
}

void VoxelPalette::registerType(Voxel voxel) {
    this->voxels.push_back(voxel);
    
}

Voxel VoxelPalette::fromID(int id) {
    return voxels[id];
}

int VoxelPalette::get_size() {
    return this->voxels.size();
}