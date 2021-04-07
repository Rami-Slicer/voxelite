#pragma once

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
// #include <filesystem>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <util.hpp>

#include <glad/glad.h>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
    aiString path;
};

struct Model {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    bool invisible; // don't bother rendering this
};

Model loadModel(std::string path);

Model merge(Model a, Model b);

unsigned int TextureFromFile(std::string name, std::string dir);
