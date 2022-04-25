#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>

#include "Shader.h"
#include "Cubemap.h"

class Skybox {
public:
    Skybox(const std::vector<std::string>& faces);
	void Draw(glm::mat4 v, glm::mat4 p);
private:
	unsigned int VAO, VBO;
	Cubemap *cubemap;
	Shader* shader;
};

#endif