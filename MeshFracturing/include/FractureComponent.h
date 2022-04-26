#ifndef FRACTURE_COMPONENT_H
#define FRACTURE_COMPONENT_H

#include <glm/glm.hpp>

#include "SpeedRender/Model.h"
#include "SpeedRender/Mesh.h"

class FractureComponent {
public:
	FractureComponent() { }

	FractureComponent(const Mesh& mesh) { 
        for (int i = 0; i < mesh.indices.size(); i += 3) {
            AddTriangle(
                mesh.vertices[i + 0].position,
                mesh.vertices[i + 1].position,
                mesh.vertices[i + 2].position,
                mesh.vertices[i + 0].normal,
                mesh.vertices[i + 1].normal,
                mesh.vertices[i + 2].normal,
                mesh.vertices[i + 0].texCoords,
                mesh.vertices[i + 1].texCoords,
                mesh.vertices[i + 2].texCoords);
        }
    }

    void AddTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 n1, glm::vec3 n2, glm::vec3 n3, glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3) {
        int baseIndex = positions.size();
        triangles.push_back(baseIndex + 0);
        triangles.push_back(baseIndex + 1);
        triangles.push_back(baseIndex + 2);
        positions.push_back(p1);
        positions.push_back(p2);
        positions.push_back(p3);
        normals.push_back(n1);
        normals.push_back(n2);
        normals.push_back(n3);
        uvs.push_back(uv1);
        uvs.push_back(uv2);
        uvs.push_back(uv3);
    }

    Mesh GetMesh() {
        return Mesh(positions, normals, uvs, triangles, std::vector<Texture>());
	}
private:
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<unsigned int> triangles;
};

#endif