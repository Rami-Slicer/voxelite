#pragma once

#include <chunk.hpp>
#include <noise/noise.h>


class World {
    public:
        World(int chunk_size, VoxelPalette * palette);

        void setRenderDistance(float distance);

        void addChunk(int x, int y, int z);

        void generateAt(int x, int y, int z);

        void render(Shader * shader, glm::vec3 pos);

        VoxelChunk getChunk(glm::ivec3 at);
        void setChunk(glm::ivec3 at, VoxelChunk chunk);
        bool chunkExists(glm::ivec3 at);

        std::vector<VoxelChunk> getChunks();

    private:
        // vector of all the chunks
        std::vector<VoxelChunk> chunks;

        // the maximum distance, in chunks, that a chunk can be from the camera and be rendered.
        float renderDistance = 4;

        // pointer to the voxel palette to use
        VoxelPalette * palette;

        int chunk_size;

};