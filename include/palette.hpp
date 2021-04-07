#pragma once

#include <vector>
#include <string>
#include <glm/vec3.hpp>

#include <voxel.hpp>

class VoxelPalette {
    public:
        VoxelPalette();

        void registerType(Voxel voxel);
        Voxel fromID(int id);
        int get_size();
    private:
        std::vector<Voxel> voxels;
};