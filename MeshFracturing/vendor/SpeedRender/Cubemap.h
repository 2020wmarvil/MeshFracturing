#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <vector>
#include <string>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

class Cubemap {
public:
	Cubemap(const std::vector<std::string> &faces) {
		LoadCubeMap(faces);
	}

	void LoadCubeMap(const std::vector<std::string> &faces);

	unsigned int id;
};

#endif