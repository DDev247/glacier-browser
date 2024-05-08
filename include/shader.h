
#pragma once

#include <string>

namespace glacier {

    class Shader
    {
    public:
        Shader() {};

        // true if successful
        bool compile(std::string vertexPath, std::string fragmentPath);
        unsigned int programID;

        void use();
        void setBool(std::string name, bool value) const;
        void setInt(std::string name, int value) const;
        void setFloat(std::string name, float value) const;
    };

}
