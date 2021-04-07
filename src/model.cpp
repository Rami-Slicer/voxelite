#include <model.hpp>

unsigned int TextureFromFile(std::string name, std::string dir) {
    int width, height, nrChannels;
    std::string path = dir + name;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0); 
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);  
    
    if (data)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "Loaded texture " << texture << " at: ";
        std::cout << path << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture at: ";
        std::cout << path << std::endl;
        std::cout << stbi_failure_reason() << std::endl;
    }
    stbi_image_free(data);
    return texture;
}

Model loadModel(std::string path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_CalcTangentSpace       |
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType
    );

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    }

    std::string directory = path.substr(0, path.find_last_of('/'));

    std::vector<uint> indices; 
    std::vector<Vertex> vertices; 
    std::vector<Texture> textures;
    
    
    // std::cout << std::filesystem::exists(path) << std::endl;
    int i = 0;
    std::cout << "meshes: " << sizeof(scene -> mMeshes[i] -> mFaces) << std::endl;

    aiMesh ** meshes = scene->mMeshes;

    // Loop through each mesh and merge vertice and indice data into their respective arrays.
    for(int iMesh = 0; iMesh < scene->mNumMeshes; iMesh++) {
        aiMesh * cMesh = meshes[iMesh];

        std::cout << cMesh->HasFaces() << std::endl;
        aiVector3D * mVertices = cMesh->mVertices;
        aiVector3D * mNormals = cMesh->mNormals;
        aiVector3D * mTextureCoords = *cMesh->mTextureCoords;

        aiFace * faces = cMesh->mFaces;

        for(int iFace = 0; iFace < cMesh -> mNumFaces; iFace++) {
            aiFace face = faces[iFace];

            for(int iIndice = 0; iIndice < face.mNumIndices; iIndice++) {
                unsigned int indice = face.mIndices[iIndice];
                indices.push_back(indice);
            }
        }

        for(int iVertex = 0; iVertex < cMesh -> mNumVertices; iVertex++) {
            aiVector3D mVertex = mVertices[iVertex];
            Vertex vertex;

            // std::cout << mVertex.x << ", " << mVertex.y << ", " << mVertex.z << std::endl;

            vertex.Position = glm::vec3(
                mVertex.x, 
                mVertex.y, 
                mVertex.z
            );

            vertex.Normal = glm::vec3(
                mNormals[iVertex].x,
                mNormals[iVertex].y,
                mNormals[iVertex].z
            );

            vertex.TexCoords = glm::vec2(
                mTextureCoords[iVertex].x,
                mTextureCoords[iVertex].y
            );
            
            vertices.push_back(vertex);
        }

        

        aiMaterial * cMat = scene->mMaterials[cMesh->mMaterialIndex];

        for(int iTex = 0; iTex < cMat->GetTextureCount(aiTextureType_DIFFUSE); iTex++) {
            aiString str;
            cMat->GetTexture(aiTextureType_DIFFUSE, i, &str);
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), "../assets/");
            texture.type = "texture_diffuse";
            texture.path = str;
            textures.push_back(texture);
            std::cout << "created texture!" << std::endl;
            
        }
    }

    
        
    
    Model model;
    model.vertices = vertices;
    model.indices = indices;
    model.textures = textures;
    

    return model;
}


Model merge(Model a, Model b) {

    // merge vertice arrays
    std::vector<Vertex> vertices = b.vertices;
    a.vertices.insert(a.vertices.end(), vertices.begin(), vertices.end());

    // merge indices
    
    

    unsigned int max = *std::max_element(a.indices.begin(), a.indices.end());
    std::vector<unsigned int> indices = b.indices;

    for(int i = 0; i < indices.size(); i++) {
        a.indices.push_back(indices[i] + max + 1);
    }

    // merge textures
    std::vector<Texture> textures = b.textures;
    a.textures.insert(a.textures.end(), textures.begin(), textures.end());


    // std::cout << "sizes: " << a.indices.size() << ", " << a.vertices.size() << std::endl;
    //done
    return a;
}