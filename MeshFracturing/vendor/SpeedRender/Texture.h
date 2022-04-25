#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stb_image.h>

#include <string>
#include <iostream>

class Texture {
public:
	Texture(std::string filepath, std::string type="diffuse");

	static void SetFlipImageOnLoad(bool flip) {
		stbi_set_flip_vertically_on_load(flip);
	}

    unsigned int id;
	std::string type;
	int width, height, numChannels;
};
#endif
