#ifndef MESH_H
#define MESH_H

#include "Core.h"
#include "Texture.h"
#include "Shader.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
    public:
        // mesh data
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture>      textures;
        bool hasNormals;
        bool hasUVs;
        bool hasIndices;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, bool hasNormals, bool hasUVs);
        Mesh(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> uvs, 
            std::vector<unsigned int> indices, std::vector<Texture> textures);
        Mesh(std::vector<float> vertexPositions);
        void Draw(Shader &shader);

        void AddTexture(Texture tex) {
            textures.push_back(tex);
        }
    private:
        //  render data
        unsigned int VAO, VBO, EBO;

        void SetupMesh();
};  

#endif
