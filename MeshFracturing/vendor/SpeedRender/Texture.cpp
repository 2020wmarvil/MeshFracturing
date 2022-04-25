#include "Texture.h"

Texture::Texture(std::string filepath, std::string type) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    this->type = type;

    // TODO: wrap and filter parameters
    // TODO: handle file formats (rgb, rgba, etc.)

    //// set the texture wrapping/filtering options (on the currently bound texture object)
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate the texture
    unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &numChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture at " << filepath << std::endl;
    }
    stbi_image_free(data);
}