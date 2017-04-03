#ifndef TUTORIAL_SHADER_HPP
#define TUTORIAL_SHADER_HPP
#pragma once

#include <gl_33.hpp>
#include <map>
#include <vector>

struct ShaderProgram{
    enum class Stage : GLuint{
        Vertex = 0,
        Fragment,
        Total
    };

    static const unsigned int INVALID = (const unsigned int) -1;
    static const std::string ShaderDirectory;
    static const std::array<std::string, (unsigned int) Stage::Total> Extensions;
    static const std::array<GLuint, (unsigned int) Stage::Total> GLShaderStage;

    GLuint id;
    GLint success = 0;
    std::array<std::string, (unsigned int) Stage::Total> shaderFiles;
    std::array<GLuint, (unsigned int) Stage::Total> shaderIds;

    ShaderProgram();
    ShaderProgram(std::vector<std::pair<Stage, std::string>> shaders);

    void setFile(Stage stage, std::string string);
    void deleteShaders();
    GLint reloadShaders();

    ~ShaderProgram();
};

#endif //TUTORIAL_SHADER_HPP
