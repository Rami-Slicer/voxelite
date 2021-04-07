#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <palette.hpp>

enum {
    UP,
    DOWN,
    NORTH,
    SOUTH,
    EAST,
    WEST
};

struct RenderData {
    std::vector<Model> models;
    std::vector<int> counts;
    std::vector<std::vector<glm::mat4>> transforms;
    // std::vector<std::vector<glm::vec3>> transforms;
};

/**
 * @brief A single voxel chunk.
 * 
 */
class VoxelChunk {
    public:
        /**
         * @brief Construct a new Voxel Chunk object
         * 
         * @param size Size of the chunk. Chunks are always cubic. Bugs may be caused by oversized or diferent-sized chunks.
         * @param palette The block palette to use for this chunk.
         * @param position The chunk level position for this chunk, e.g. if this chunk is 16 blocks large 1 chunk position = 16 blocks
         */
        VoxelChunk(int size, VoxelPalette * palette, glm::ivec3 position);

        glm::ivec3 getChunkPos();

        // ~VoxelChunk();

        /**
         * @brief Set a voxel at a given position.
         * 
         * @param x 
         * @param y 
         * @param z 
         * @param id 
         */
        void setVoxel(int x, int y, int z, int id);
        
        std::vector<Model> getModels();

        void render(Shader * shader);

        void draw(Shader * shader, Model model);

        void draw_instanced(Shader * shader, RenderData * renderData);

        void clear();
        
        void updateVisible(int x, int y, int z);

    protected:
        int calcIndice(int x, int y, int z);
        

    private:
        RenderData * genData();

        // chunk position
        glm::ivec3 chunk_position;

        int size;
        bool needs_update;
        std::vector<Model> model_cache;
        RenderData renderData_cache;

        // std::vector<glm::vec3> corners;

        VoxelPalette * palette;
        std::vector<int> data;
        // blocks are visible if any of the 8 blocks around them are transparent or empty
        std::vector<bool> visible;
        unsigned int VAO, VBO, EBO;

        
};
