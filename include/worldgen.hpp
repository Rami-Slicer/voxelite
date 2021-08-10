#pragma once

#include <iostream>
#include <glm/glm.hpp>

#include <noise/noise.h>
// #include <world.hpp>
#include <palette.hpp>

/**
 * @brief Base class that world generators should implement.
 * 
 */
class BaseGenerator {
    
    public:

    void init(VoxelPalette * palette) {
        this->palette = palette;
    }

    virtual std::string blockAt(glm::vec3 g_pos) {
        return AIR;
    };

    protected:

    VoxelPalette * palette;
};

class TestGenerator : public BaseGenerator {
    public:
    TestGenerator();

    std::string blockAt(glm::vec3 g_pos);

    private:
    
    int bottom_level = 20;
};