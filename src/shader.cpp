#include <shader.hpp>
#include <file.hpp>

const std::string ShaderProgram::ShaderDirectory = "shaders/";
const std::array<std::string, (unsigned int) ShaderProgram::Stage::Total> ShaderProgram::Extensions{
        ".vert",
        ".frag"
};
const std::array<GLuint, (unsigned int) ShaderProgram::Stage::Total> ShaderProgram::GLShaderStage{
        gl::VERTEX_SHADER,
        gl::FRAGMENT_SHADER
};

ShaderProgram::ShaderProgram() {
    this->id = gl::CreateProgram();
    for (auto& file : shaderFiles){ file = ""; }
    for (auto& shader : shaderIds){ shader = INVALID; }
}

ShaderProgram::ShaderProgram(std::vector<std::pair<Stage, std::string>> shaders) : ShaderProgram{} {
    for (const auto& shader : shaders){
        this->setFile(shader.first, shader.second);
    }
}

ShaderProgram::~ShaderProgram() {
    deleteShaders();
    gl::DeleteProgram(this->id);
}

void ShaderProgram::setFile(Stage stage, std::string string) {
    this->shaderFiles[(unsigned int) stage] = string;
}

GLint ShaderProgram::reloadShaders() {
    this->deleteShaders();
    for (int i = 0; i < (int) Stage::Total; i++){
        std::string fullShaderPath = ShaderDirectory + shaderFiles[i] + Extensions[i];
        std::string shaderCode = ReadFile(fullShaderPath);
        const char * ShaderText = shaderCode.c_str();
        const GLint ShaderTextSize = (const GLint) shaderCode.length();

        auto glShader = gl::CreateShader(GLShaderStage[i]);
        gl::ShaderSource(glShader, 1, &ShaderText, &ShaderTextSize);
        gl::CompileShader(glShader);

        GLint success;
        gl::GetShaderiv(glShader, gl::COMPILE_STATUS, &success);
        char log[512];
        if (!success && log != nullptr) {
            gl::GetShaderInfoLog(glShader, (GLsizei) sizeof(log) - 1, nullptr, log);
            LogError("glShader compilation failed:\n%s", log);
        }
        gl::AttachShader(this->id, glShader);
    }
    gl::LinkProgram(this->id);

    gl::GetProgramiv(this->id, gl::LINK_STATUS, &this->success);
    char log[512];
    if (!this->success && log != nullptr) {
        gl::GetProgramInfoLog(this->id, (GLsizei) (sizeof(log) - 1), nullptr, log);
        LogError("%s", log);
    }

    gl::ValidateProgram(this->id);

    return this->success;
}

void ShaderProgram::deleteShaders() {
    gl::DeleteProgram(this->id);
    this->id = gl::CreateProgram();
    for (auto& shader : shaderIds){
        if (shader != INVALID){
            gl::DetachShader(this->id, shader);
            gl::DeleteShader(shader);
            shader = INVALID;
        }
    }
}


