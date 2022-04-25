#ifndef MODEL_H
#define MODEL_H

#include "cy/cyTriMesh.h"

#include "Core.h"
#include "Shader.h"
#include "Mesh.h"

class Model {
public:
    Model(std::string path);
    Model(Mesh mesh);
    void Draw(Shader &shader);	

    Transform transform = { glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f) };

    glm::mat4 GetModelMatrix() {
        glm::mat4 model(1.0f);
        model = glm::translate(model, transform.position);
        model = glm::rotate(model, transform.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, transform.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, transform.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, transform.scale);
        return model;
    }

    std::vector<Mesh> meshes;
private:
    void LoadModel(std::string path);
};

#endif
