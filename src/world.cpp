#include <world.hpp>

using namespace noise;

const float scale = 0.1;

World::World(int chunk_size, VoxelPalette * palette) {
    this->chunk_size = chunk_size;
    this->palette = palette;
    
}

float distance(glm::vec3 a, glm::vec3 b) {
    glm::vec3 diff = a-b;
    diff = glm::pow(diff, glm::vec3(2));
    return glm::sqrt(diff.x + diff.y + diff.z);
}

void World::addChunk(int cx, int cy, int cz) {
    if(chunkExists(glm::ivec3(cx, cy, cz))) {
        return;
    }
    VoxelChunk toAdd = VoxelChunk(this->chunk_size, this->palette, glm::ivec3(cx, cy, cz));

    this->chunks.push_back(toAdd);
}

void World::generateAt(int cx, int cy, int cz) {
    if(chunkExists(glm::ivec3(cx, cy, cz))) {
        return;
    }

    VoxelChunk toAdd = VoxelChunk(this->chunk_size, this->palette, glm::ivec3(cx, cy, cz));
    
    module::Perlin perlin;

    for(int x = 0; x < this->chunk_size; x++) {
        for(int y = 0; y < this->chunk_size; y++) {
            for(int z = 0; z < this->chunk_size; z++) {
                glm::vec3 coords = glm::vec3(this->chunk_size*cx+x, this->chunk_size*cy+y, this->chunk_size*cz+z);

                double value = perlin.GetValue(coords.x*scale, coords.y*scale, coords.z*scale);
                if(value < 0.0) {
                    toAdd.setVoxel(x, y, z, 1);
                }
            }
        }
    }

    for(int x = 1; x < this->chunk_size-1; x++) {
        for(int z = 1; z < this->chunk_size-1; z++) {
            for(int y = 1; y < this->chunk_size-1; y++) {
                toAdd.updateVisible(x,y,z);
            }
        }
    }

    this->chunks.push_back(toAdd);
}

void World::render(Shader * shader, glm::vec3 pos) {
    for(VoxelChunk &chunk : this->chunks) {
        glm::vec3 chunk_center = chunk.getChunkPos() * this->chunk_size;
        chunk_center += glm::vec3(this->chunk_size/2);

        if(distance(-pos, chunk_center*glm::vec3(2.0)) <= this->renderDistance*this->chunk_size*2.0) {
            chunk.render(shader);
        }

        
    }
}

void World::setRenderDistance(float distance) {
    this->renderDistance = distance;
}

VoxelChunk World::getChunk(glm::ivec3 at) {
    for(VoxelChunk &chunk : this->chunks) {
        if(chunk.getChunkPos() == at) {
            return chunk;
        }
    }
    VoxelChunk fallback = VoxelChunk(this->chunk_size, this->palette, at);
    return fallback; 
}

bool World::chunkExists(glm::ivec3 at) {
    for(VoxelChunk &chunk : this->chunks) {
        if(chunk.getChunkPos() == at) {
            return true;
        }
    }
    return false;
}

void World::setChunk(glm::ivec3 at, VoxelChunk chunk) {
    for(int i = 0; i < this->chunks.size(); i++) {
        if(this->chunks[i].getChunkPos() == at) {
            this->chunks[i] = chunk;
        }
    }
}

std::vector<VoxelChunk> World::getChunks() {
    return this->chunks;
}