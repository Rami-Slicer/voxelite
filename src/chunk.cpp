#include <chunk.hpp>

// void CheckGLError() {	GLuint err = glGetError();	if (err != GL_NO_ERROR)		std::cerr << std::hex << err << std::endl;}
// #ifndef NDEBUG
// #	define CHECK_GL_ERROR CheckGLError();
// #else
// #	define CHECK_GL_ERROR
// #endif

glm::mat4 rotation = glm::mat4(1.0);

VoxelChunk::VoxelChunk(int size, VoxelPalette * palette, glm::ivec3 position) {


    // std::cout << size;
    // std::cout << ", ";
    // std::cout << (pow(size, 3));
    // std::cout << " blocks in total" << std::endl;
    this->size = size;
    this->palette = palette;
    this->chunk_position = position;
    for(int x = 0; x < size; x++) {
        for(int y = 0; y < size; y++) {
            for(int z = 0; z < size; z++) {
                this->data.push_back("air");
                this->visible.push_back(true);
            }
        }
    }

    this->needs_update = true;

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);
    // std::cout << "generated some buffers" << std::endl;
}

glm::ivec3 VoxelChunk::getChunkPos() {
    return this->chunk_position;
}

// float distance(glm::vec3 a, glm::vec3 b) {
//     glm::vec3 diff = a-b;
//     diff = glm::pow(diff, glm::vec3(2));
//     return glm::sqrt(diff.x + diff.y + diff.z);
// }

int VoxelChunk::calcIndice(int x, int y, int z) {
    int indice = x + y*size + z*size*size;
    return indice;
}

void VoxelChunk::setVoxel(int x, int y, int z, std::string id) {
    int indice = calcIndice(x, y, z);
    data[indice] = id;

    // Model chunk_model = this->getModel();

    // std::vector<Vertex> vertices = chunk_model.vertices;
    // std::vector<unsigned int> indices = chunk_model.indices;

    // std::cout << "vertices:" << std::endl;
    // for(int i = 0; i < vertices.size(); i++) {
    //     std::cout << vertices[i].Position.x << ", " << vertices[i].Position.y << ", " << vertices[i].Position.z << std::endl;

    // }
    // std::cout << "indices:" << std::endl;
    // for(int i = 0; i < indices.size(); i++) {
    //     std::cout << indices[i] << std::endl;

    // }

}




std::vector<Model> VoxelChunk::getModels() {

    if(!this->needs_update) {
        return this->model_cache;
    }
    

    VoxelPalette palette = *this->palette;

    std::vector<Model> models;

    std::vector<voxel_pair> iter = *palette.iter();
    
    for(voxel_pair &pair : iter) {

        Model full;

        full = palette.fromID(pair.first).getModel();
        
        models.push_back(full);
    }

    
    

    std::cout << "updated chunk" << std::endl;
    this->model_cache.swap(models);
    
    
    return this->model_cache;
    // Model full_model;

    // for (int i = 0; i < models.size(); ++i) {
    //     Model vModel = models[i];
    //     std::vector<uint> indices = vModel.indices;
    //     std::vector<Vertex> vertices = vModel.vertices;
    //     uint max = get_max(full_model.indices);

    //     for (int j = 0; i < indices.size(); ++i) {
    //         indices[j] = indices[j] + max;
    //     }
    //     full_model.indices.insert(full_model.indices.end(), indices.begin(), indices.end());
    //     full_model.vertices.insert(full_model.vertices.end(), vertices.begin(), vertices.end());
    // }

    

    // return full_model;
}

void VoxelChunk::updateVisible(int x, int y, int z) {
    int indice = calcIndice(x, y, z);

    std::vector<int> iAdj = {
        calcIndice(x, y+1, z),
        calcIndice(x, y-1, z),
        calcIndice(x+1, y, z),
        calcIndice(x-1, y, z),
        calcIndice(x, y, z+1),
        calcIndice(x, y, z-1)
    };
    
    VoxelPalette palette = *this->palette;

    for(int i = 0; i < iAdj.size(); i++) {

        std::string id = this->data[iAdj[i]]; 

        if(palette.fromID(id).isTransparent()) {
            this->visible[indice] = true;
            return;
        }
    }

    this->visible[indice] = false;
}

RenderData * VoxelChunk::genData() {
    
    if(!needs_update) {
        return &this->renderData_cache;
    }

    VoxelPalette palette = *this->palette;

    RenderData renderData;
    
    for(int i = 0; i < palette.get_size(); i++) {
        int count = 0;
        std::vector<glm::mat4> mat_vec;
        // std::vector<glm::vec3> pos_vec;
        std::string id = palette.iter()->at(i).first;
        
        for(int x = 0; x < this->size; x++) {
            for(int y = 0; y < this->size; y++) {
                for(int z = 0; z < this->size; z++) {
                    int indice = calcIndice(x, y, z);

                    if(this->data[indice] == id) {
                        if(this->visible[indice]) {
                            count++;
                            // int x = x*2+2*this->size*this->chunk_position.x;
                            // int y = y*2+2*this->size*this->chunk_position.y;
                            // int z = z*2+2*this->size*this->chunk_position.z;

                            // glm::vec3 vec = glm::vec3(x, y, z);
                            // pos_vec.push_back(vec);

                            glm::mat4 transform = glm::mat4(1.0);
                            transform = glm::translate(transform, glm::vec3(x*2, y*2, z*2));
                            transform = glm::translate(transform, glm::vec3(
                                2*this->size*this->chunk_position.x, 
                                2*this->size*this->chunk_position.y, 
                                2*this->size*this->chunk_position.z
                            ));
                            
                            mat_vec.push_back(transform);
                        }
                    }
                }
            }
        }
        renderData.counts.push_back(count);
        // renderData.transforms.push_back(pos_vec);
        renderData.transforms.push_back(mat_vec);
    }

    
    renderData.models = this->getModels();
    

    // renderData.data = this->data;
    this->needs_update = false;
    this->renderData_cache = renderData;
    
    return &this->renderData_cache;
}

void VoxelChunk::render(Shader * shader) {
    RenderData * renderData = this->genData();
    

    draw_instanced(shader, renderData);
}

// VoxelChunk::~VoxelChunk() {
//     glDeleteVertexArrays(1, &VAO);
//     glDeleteBuffers(1, &VBO);
//     glDeleteBuffers(1, &EBO);
// }

void VoxelChunk::draw_instanced(Shader * shaderptr, RenderData * renderDataptr) {
    // Shader base_shader = Shader("../assets/base_shader.vsh", "../assets/base_shader.fsh");
    // rotation = glm::rotate(rotation, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    Shader shader = *shaderptr;
    RenderData renderData = *renderDataptr;


    // glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
    // glEnable(GL_DEPTH_TEST);
    unsigned int iPosBuffer;
    glGenBuffers(1, &iPosBuffer);

    // std::cout << "chunk.cpp: base shader ptr: " << &shader << std::endl;
    int vert_count = 0;
    
    for(int i = 0; i < renderData.models.size(); i++) {
        int count = renderData.counts[i];
        Model model = renderData.models[i];
        std::vector<glm::mat4> transforms;
        // transforms = renderData.transforms[i];
        transforms.reserve(renderData.transforms.size());
        transforms.swap(renderData.transforms[i]);

        
        // std::vector<glm::vec3> transforms = renderData.transforms[i];


        std::vector<Vertex> vertices = model.vertices;
        std::vector<unsigned int> indices = model.indices;
        std::vector<Texture> textures = model.textures;

        vert_count += indices.size()*count;
    
        glBindVertexArray(this->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);  

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                    &indices[0], GL_DYNAMIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);	
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);
        
        
        // shader.use();
        // shader.setMat4("view", rotation);

        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        // std::cout << "model " << i << " has: " << textures.size() << std::endl;
        for(unsigned int tex = 0; tex < textures.size(); tex++)
        {
            glActiveTexture(GL_TEXTURE0 + tex); // activate proper texture unit before binding
            glBindTexture(GL_TEXTURE_2D, textures[tex].id);
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string name = textures[tex].type;
            if(name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if(name == "texture_specular")
                number = std::to_string(specularNr++);

            
            
            shader.setInt((name + number).c_str(), tex);
        }
        // glBindTexture(GL_TEXTURE_2D, 0);
        // glActiveTexture(GL_TEXTURE0);
        
        //instance positions
        
        glBindBuffer(GL_ARRAY_BUFFER, iPosBuffer);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::mat4), transforms.data(), GL_DYNAMIC_DRAW);
        
        


        glBindVertexArray(this->VAO);


        // glEnableVertexAttribArray(3);
        // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

        std::size_t vec4Size = sizeof(glm::vec4);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
        glEnableVertexAttribArray(4); 
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
        glEnableVertexAttribArray(5); 
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
        glEnableVertexAttribArray(6); 
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

        // glVertexAttribDivisor(3, 1);

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
        
        glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, count);
        glBindVertexArray(0);
        
    }
    glDeleteBuffers(1, &iPosBuffer);
    // std::cout << "Rendered " << vert_count << " vertices" << std::endl;
}

// void VoxelChunk::draw(Shader * shader, Model model) {
//     // Shader base_shader = Shader("../assets/base_shader.vsh", "../assets/base_shader.fsh");

//     Model chunk_model = model;

//     std::vector<Vertex> vertices = chunk_model.vertices;
//     std::vector<unsigned int> indices = chunk_model.indices;
//     std::vector<Texture> textures = chunk_model.textures;
  
//     glBindVertexArray(this->VAO);
//     glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

//     glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
//                  &indices[0], GL_DYNAMIC_DRAW);

//     // vertex positions
//     glEnableVertexAttribArray(0);	
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
//     // vertex normals
//     glEnableVertexAttribArray(1);
//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
//     // // vertex texture coords
//     glEnableVertexAttribArray(2);
//     glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

//     glBindVertexArray(0);

//     shader.use();

//     unsigned int diffuseNr = 1;
//     unsigned int specularNr = 1;
//     for(unsigned int i = 0; i < textures.size(); i++)
//     {
//         glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
//         // retrieve texture number (the N in diffuse_textureN)
//         std::string number;
//         std::string name = textures[i].type;
//         if(name == "texture_diffuse")
//             number = std::to_string(diffuseNr++);
//         else if(name == "texture_specular")
//             number = std::to_string(specularNr++);

//         shader.setFloat((name + number).c_str(), i);
//         glBindTexture(GL_TEXTURE_2D, textures[i].id);
//     }
//     glActiveTexture(GL_TEXTURE0);

//     glBindVertexArray(this->VAO);
//     glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//     glBindVertexArray(0);
// }