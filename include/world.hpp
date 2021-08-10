#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chunk.hpp>
#include <noise/noise.h>
#include <worldgen.hpp>
#include <util.hpp>


class World {
    public:
        World(int chunk_size, VoxelPalette * palette, WindowInfo w_info);

        void setRenderDistance(float distance);

        void addChunk(int x, int y, int z);

        void generateAt(int x, int y, int z);

        void render(Shader * shader, glm::vec3 pos);

        void renderShadowMap(glm::vec3 light_dir);

        VoxelChunk getChunk(glm::ivec3 at);
        void setChunk(glm::ivec3 at, VoxelChunk chunk);
        bool chunkExists(glm::ivec3 at);

        std::vector<VoxelChunk> getChunks();

        void setBlock(glm::ivec3 at, int id);

        void setGenerator(BaseGenerator &gen);

        unsigned int getRenderBuffer();
        unsigned int getShadowBuffer();
        
    private:
        unsigned int render_buf;
        unsigned int shadow_buf;

        // vector of all the chunks
        std::vector<VoxelChunk> chunks;

        BaseGenerator *gen;

        // the maximum distance, in chunks, that a chunk can be from the camera and be rendered.
        float renderDistance = 4;

        // pointer to the voxel palette to use
        VoxelPalette * palette;

        int chunk_size;


};