#pragma once
#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include <glm/vec3.hpp>
#include <util.hpp>
#include <model.hpp>


const float SIZE = 0.5;

/**
 * @brief Base voxel class.
 */
class Voxel {
    public:
        Voxel(std::string mPath, bool transparent);

        Model getModel();
        bool isTransparent();
        void setInvisible(bool i);
    private:
        Model model;
        bool transparent;
};






