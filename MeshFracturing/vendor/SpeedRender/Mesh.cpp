#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, bool hasNormals, bool hasUVs) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->hasNormals = hasNormals;
    this->hasUVs = hasUVs;

    this->hasIndices = true;

    SetupMesh();
    RecalculateBounds();
}

Mesh::Mesh(std::vector<float> vertexPositions) {
    for (int i = 0; i < vertexPositions.size(); i+=3) {
        vertices.push_back( {
            glm::vec3(vertexPositions[i + 0], vertexPositions[i + 1], vertexPositions[i + 2]),
            glm::vec3(0.0f),
            glm::vec2(0.0f),
        });

        indices.push_back(i);
        indices.push_back(i+1);
        indices.push_back(i+2);
    }

    hasNormals = false;
    hasUVs = false;
    hasIndices = true;

    SetupMesh();
    RecalculateBounds();
}

Mesh::Mesh(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> uvs,
    std::vector<unsigned int> indices, std::vector<Texture> textures) {

    for (int i = 0; i < positions.size(); i++) {
        this->vertices.push_back({ positions[i], normals[i], uvs[i] });
    }

    this->indices = indices;
    this->textures = textures;

    this->hasNormals = true;
    this->hasUVs = true;

    this->hasIndices = true;

    SetupMesh();
    RecalculateBounds();
}

void Mesh::SetupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    if (hasIndices) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    }

    int attribArray = 0;
    glEnableVertexAttribArray(attribArray);
    glVertexAttribPointer(attribArray, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    attribArray++;

    if (hasNormals) {
        glEnableVertexAttribArray(attribArray);	
        glVertexAttribPointer(attribArray, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        attribArray++;
    }

    if (hasUVs) {
        glEnableVertexAttribArray(attribArray);	
        glVertexAttribPointer(attribArray, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
        attribArray++;
    } 

    glBindVertexArray(0);
}

void Mesh::RecalculateBounds() {
    bounds = { vertices[0].position, vertices[0].position, vertices[0].position };

    for (int i = 0; i < vertices.size(); i++) {
        float x = vertices[i].position.x;
        float y = vertices[i].position.y;
        float z = vertices[i].position.z;

        if (x < bounds.min.x) {
            bounds.min.x = x;
        }
        if (x > bounds.max.x) {
            bounds.max.x = x;
        }

        if (y < bounds.min.y) {
            bounds.min.y = y;
        }
        if (y > bounds.max.y) {
            bounds.max.y = y;
        }

        if (z < bounds.min.z) {
            bounds.min.z = z;
        }
        if (z > bounds.max.z) {
            bounds.max.z = z;
        }
    }

    bounds.center = (bounds.max + bounds.min) * 0.5f;
}

void Mesh::Draw(Shader& shader) {
    shader.Use();

    //if (shader.depthTest) glEnable(GL_DEPTH_TEST);
    //else glDisable(GL_DEPTH_TEST);

    //if (shader.depthWrite) glDepthMask(GL_TRUE); 
    //else glDepthMask(GL_FALSE); 

    //glDepthFunc(shader.depthFunc);

    //unsigned int diffuseNr = 1;
    //unsigned int specularNr = 1;
    //for(unsigned int i = 0; i < textures.size(); i++) {
    //    glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
    //    // retrieve texture number (the N in diffuse_textureN)
    //    std::string number;
    //    std::string name = textures[i].type;
    //    if(name == "texture_diffuse")
    //        number = std::to_string(diffuseNr++);
    //    else if(name == "texture_specular")
    //        number = std::to_string(specularNr++);

    //    shader.SetFloat(("material." + name + number).c_str(), i);
    //    glBindTexture(GL_TEXTURE_2D, textures[i].id);
    //}
    //glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    if (hasIndices) {
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }
    glBindVertexArray(0);
}