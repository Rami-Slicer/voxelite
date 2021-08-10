#pragma once

#include <vector>
#include <string>
#include <glm/vec3.hpp>

#include <voxel.hpp>

typedef std::pair<std::string, Voxel> voxel_pair;
const std::string AIR = "air";
class VoxelPalette {
    public:
        VoxelPalette();

        void registerType(Voxel voxel, std::string name);
        Voxel fromID(std::string);
        int get_size();

        std::vector<voxel_pair> * iter();

    private:
        std::vector<voxel_pair> voxels;
        
};