#include <voxel.hpp>

const std::string CUBE_PATH = "./assets/cube.obj";

Voxel::Voxel(std::string mPath, bool transparent) {
    // Model test;
    // test.vertices = {
    //     Vertex {glm::vec3(0,0,0)},
    //     Vertex {glm::vec3(1,0,0)},
    //     Vertex {glm::vec3(0,1,0)},
    //     Vertex {glm::vec3(1,1,1)}
    // };
    // test.indices = {
    //     0,1,3,
    //     1,2,3
    // };
    // this->model = test;
    this->model = loadModel(mPath);
    this->transparent = transparent;
    
}

Model Voxel::getModel() {
    return this->model;
}

bool Voxel::isTransparent() {
    return this->transparent;
}

void Voxel::setInvisible(bool i) {
    this->model.invisible = i;
}