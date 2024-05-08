
#include "../include/shader.h"
#include "../include/logger.h"
#include "../include/glfw.hpp"
#include <fstream>
#include <sstream>

bool glacier::Shader::compile(std::string vertexPath, std::string fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        LOG(Log::Level::Debug, "glacier::Shader::Shader", "Opening shader files (" + vertexPath + ", " + fragmentPath + ")");
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        LOG(Log::Level::Debug, "glacier::Shader::Shader", "Reading shader files");
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e) {
        LOG(Log::Level::Error, "glacier::Shader::Shader", "std::ifstream::failure '" + std::string(e.what()) + "'");
        return false;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];


    // vertex Shader
    LOG(Log::Level::Debug, "glacier::Shader::Shader", "Compiling vertex shader");
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        LOG(Log::Level::Error, "glacier::Shader::Shader", "Failed to compile vertex shader. OpenGL info log: " + std::string(infoLog));
        return false;
    }

    // fragment shader
    LOG(Log::Level::Debug, "glacier::Shader::Shader", "Compiling fragment shader");
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        LOG(Log::Level::Error, "glacier::Shader::Shader", "Failed to compile fragment shader. OpenGL info log: " + std::string(infoLog));
        return false;
    }

    // shader Program
    LOG(Log::Level::Debug, "glacier::Shader::Shader", "Creating shader program");
    programID = glCreateProgram();
    glAttachShader(programID, vertex);
    glAttachShader(programID, fragment);
    LOG(Log::Level::Debug, "glacier::Shader::Shader", "Linking shader program");
    glLinkProgram(programID);
    // print linking errors if any
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        LOG(Log::Level::Debug, "glacier::Shader::Shader", "Linking shader program failed. OpenGL info log: " + std::string(infoLog));
        return false;
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return true;
}

void glacier::Shader::use()
{
    glUseProgram(programID);
}

void glacier::Shader::setBool(std::string name, bool value) const
{
    glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}

void glacier::Shader::setInt(std::string name, int value) const
{
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void glacier::Shader::setFloat(std::string name, float value) const
{
    glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}
