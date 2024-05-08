
#pragma once

#include <glm/glm.hpp>
#include <string>

namespace glacier {

    class Texture {
    public:
        Texture() {};

        // true if successful
        bool load(std::string path);
        unsigned int textureID;
        glm::ivec2 size;

        void bind();
    };

}
