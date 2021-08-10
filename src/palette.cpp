#include <vector>
#include <algorithm>
#include <string>
#include <glm/vec3.hpp>

#include <palette.hpp>
#include <voxel.hpp>



VoxelPalette::VoxelPalette() {
    Voxel empty = Voxel("../assets/air.obj", true);
    empty.setInvisible(true);
    this->registerType(empty, AIR);
    
}

// register a new voxel with name for use
void VoxelPalette::registerType(Voxel voxel, std::string name) {
    this->voxels.push_back(voxel_pair(name, voxel));
    
}


// wall of code that finds and returns a voxel.
Voxel VoxelPalette::fromID(std::string id) {
    return std::find_if(
        this->voxels.begin(), 
        this->voxels.end(), 
        [id](voxel_pair pair) {
            return pair.first == id;
        }
    ).base()->second;
}



int VoxelPalette::get_size() {
    return this->voxels.size();
}

std::vector<voxel_pair> * VoxelPalette::iter() {
    return &this->voxels;
}