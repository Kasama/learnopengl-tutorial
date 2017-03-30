#include <utils.hpp>
#include <stdio.h>
#include <iostream>
#include <gl_33.hpp>
#include <GLFW/glfw3.h>
#include <file.hpp>

int main() {
    LogError("Started\n");
    glfwInit();
    defer { glfwTerminate(); } ;
    auto GLCallBack = [](int Error, const char* text){
            LogError("[GLFW Error: %d:\n %s", Error, text);
            Assert(!"[GLFW] Fatal Error.");
        };
    glfwSetErrorCallback(GLCallBack);

    { // GLFW Windows hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, false);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    }

    ivec2 screenResolution = {800, 600};

    GLFWwindow *window = glfwCreateWindow(screenResolution.x, screenResolution.y, "Tutorial", nullptr, nullptr);
    Assert(window);

    LogError("Initialized Window\n");

    glfwMakeContextCurrent(window);
    Assert(gl::sys::LoadFunctions());


    if(gl::exts::var_KHR_debug) {
        int ContextFlags;
        gl::GetIntegerv(gl::CONTEXT_FLAGS, &ContextFlags);
        if (ContextFlags & gl::CONTEXT_FLAG_DEBUG_BIT) {
            gl::DebugMessageCallback(
                    [](auto, auto, auto, GLenum Severity,
                       auto, const GLchar *Message, auto) {
                        LogError("[OpenGL] %x: %s\n", Severity, Message);
                    }, nullptr
            );
        } else { Assert(!"No debugging context"); }
    } else { Assert(!"Could not debug"); }

    gl::Enable(gl::DEBUG_OUTPUT_SYNCHRONOUS);

    glfwGetFramebufferSize(window, &screenResolution.x, &screenResolution.y);
    gl::Viewport(0, 0, screenResolution.x, screenResolution.y);

    glfwSwapInterval(1); //vsync

    gl::ClearColor(0.2,0.7,1,1);

    struct Vertex {
        vec2 Position;
        vec2 TextureCoords;

        enum class attribute_location : GLuint {
            Position = 0,
            Texture = 1
        };
    };

    Vertex vertexes[] = {
            {{-0.5f, -0.5f}, {+0.0f, +0.0f}},
            {{+0.0f, +0.5f}, {+0.5f, +1.0f}},
            {{+0.5f, -0.5f}, {+0.0f, +0.0f}}
    };

//    Vertex vertexes[] = {
//            {{-0.5f, -0.5f}},
//            {{+0.0f, +0.5f}},
//            {{+0.5f, -0.5f}}
//    };

    LogError("Got to vertex initialization\n");

    // Vertex Buffers
    GLuint VAO;
    // Create Vertex Array
    gl::GenVertexArrays(1, &VAO);
    defer { gl::DeleteVertexArrays(1, &VAO); };
    { // Vertex Array configuration
        // Bind VAO
        gl::BindVertexArray(VAO);

        // Create Vertex Buffer
        GLuint VBO;
        gl::GenBuffers(1, &VBO);
        defer { gl::DeleteBuffers(1, &VBO); };
        gl::BindBuffer(gl::ARRAY_BUFFER, VBO);
        gl::BufferData(gl::ARRAY_BUFFER, sizeof(Vertex) * 3, vertexes, gl::STATIC_DRAW);

        // Set Position Stride
        auto PositionLocation = (GLuint) Vertex::attribute_location::Position;
        gl::EnableVertexAttribArray(PositionLocation);
        gl::VertexAttribPointer(
                PositionLocation,
                2,
                gl::FLOAT,
                gl::FALSE_,
                sizeof(Vertex),
                (void*) offsetof(Vertex, Position)
        );

        // Set Texture Stride
        auto TextureLocation = (GLuint) Vertex::attribute_location::Texture;
        gl::EnableVertexAttribArray(TextureLocation);
        gl::VertexAttribPointer(
                TextureLocation,
                2,
                gl::FLOAT,
                gl::FALSE_,
                sizeof(Vertex),
                (void*) offsetof(Vertex, TextureCoords)
        );

        gl::BindVertexArray(0);
    }

    LogError("Got to shader loading\n");

    GLuint ShaderProgram;
    { // Shaders
        string VertexShaderCode = ReadFile("shaders/basic.vert");
        string FragmentShaderCode = ReadFile("shaders/basic.frag");

        ShaderProgram = gl::CreateProgram();
        auto MakeShader = [](auto Program, auto Type, const GLchar* const Text, const GLint Size){
            auto Shader = gl::CreateShader(Type);
            gl::ShaderSource(Shader, 1, &Text, &Size);
            gl::CompileShader(Shader);

            GLint success;
            gl::GetShaderiv(Shader, gl::COMPILE_STATUS, &success);
            char log[512];
            if(!success && log != nullptr){
                gl::GetShaderInfoLog(Shader, (GLsizei) sizeof(log) -1, nullptr, log);
                LogError("Shader compilation failed:\n%s", log);
            }

            gl::AttachShader(Program, Shader);
            return Shader;
        };

        MakeShader(ShaderProgram, gl::VERTEX_SHADER, VertexShaderCode.c_str(), (const GLint) VertexShaderCode.length());;
        MakeShader(ShaderProgram, gl::FRAGMENT_SHADER, FragmentShaderCode.c_str(), (const GLint) FragmentShaderCode.length());;

        gl::LinkProgram(ShaderProgram);

        GLint Success;
        gl::GetProgramiv(ShaderProgram, gl::LINK_STATUS, &Success);
        char log[512];
        if (!Success && log != nullptr) {
            gl::GetProgramInfoLog(ShaderProgram, (GLsizei) (sizeof(log) - 1), nullptr, log);
            LogError("%s", log);
        }

        gl::ValidateProgram(ShaderProgram);
    }

    LogError("got to gameloop\n");

    // Gameloop
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();

        gl::Clear(gl::COLOR_BUFFER_BIT);
        gl::UseProgram(ShaderProgram);
        gl::BindVertexArray(VAO);
        gl::DrawArrays(gl::TRIANGLES, 0, 3);
        gl::BindVertexArray(0);

        glfwSwapBuffers(window);
    }

    return 0;
}