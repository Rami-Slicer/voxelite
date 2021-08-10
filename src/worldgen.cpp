#include <worldgen.hpp>

TestGenerator::TestGenerator() {
}

float scale = 0.02;

std::string TestGenerator::blockAt(glm::vec3 g_pos) {
    using namespace noise;

    module::Perlin noise;

    double value = noise.GetValue(g_pos.x*scale, 0.0, g_pos.z*scale);

    
    if(g_pos.y < abs(value)*40) {
        if(g_pos.y+1 > abs(value)*40) {
            return "grass";
        }
        return "dirt";
    }

    return "air";
}