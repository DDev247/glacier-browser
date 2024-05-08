
#include "../include/texture.h"
#include "../include/logger.h"
#include "../include/glfw.hpp"
#include "../include/stb_image.h"
#include <fstream>

bool glacier::Texture::load(std::string path) {
    LOG(Log::Level::Debug, "glacier::Texture::load", "Loading texture (" + path + ")");

    stbi_set_flip_vertically_on_load(true);
    
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    
    if(!data) {
        LOG(Log::Level::Debug, "glacier::Texture::load", "Failed to load texture");
        return false;
    }

    LOG(Log::Level::Debug, "glacier::Texture::load", "Creating OpenGL texture");
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Write data and generate mipmaps
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind texture and free data
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    
    return true;
}

void glacier::Texture::bind() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

