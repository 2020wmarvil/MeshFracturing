#ifndef CORE_H
#define CORE_H

#include <glm/glm.hpp>

#include "Texture.h"

#include <iostream>
#include <string>
#include <vector>
#include <list>


struct Transform {
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

typedef glm::vec3 Color;

struct Material {
    Texture diffuseMap;
    Texture specularMap;
    float shininess;
};

#endif
