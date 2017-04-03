#include <utils.hpp>
#include <iostream>
#include <gl_33.hpp>
#include <GLFW/glfw3.h>
#include <shader.hpp>
#include <polygon.hpp>

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
        glfwWindowHint(GLFW_SAMPLES, 8);
    }

    glm::ivec2 screenResolution = {800, 600};

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
    gl::Enable(gl::MULTISAMPLE);

    glfwGetFramebufferSize(window, &screenResolution.x, &screenResolution.y);
    gl::Viewport(0, 0, screenResolution.x, screenResolution.y);

    glfwSwapInterval(1); //vsync

    gl::ClearColor(0.2,0.7,1,1);

    LogError("Got to vertex initialization\n");

    Polygon p1{
            {
                    {{-0.5f, -0.5f, +0.0f}, {+0.0f, +0.0f, +0.0f}},
                    {{+0.0f, +0.5f, +0.0f}, {+0.5f, +1.0f, +0.0f}},
                    {{+0.5f, -0.5f, +0.0f}, {+0.0f, +0.0f, +0.0f}}
            }
    };

    Polygon p2{
            {
                    {{-0.5f, -0.5f, +0.0f}, {+0.0f, +0.0f, +0.0f}},
                    {{-0.5f, +0.5f, +0.0f}, {+0.0f, +1.0f, +0.0f}},
                    {{+0.5f, +0.5f, +0.0f}, {+1.0f, +1.0f, +0.0f}},
                    {{+0.5f, -0.5f, +0.0f}, {+1.0f, +0.0f, +0.0f}}
            }
    };

    LogError("Got to shader loading\n");

    std::vector<std::pair<ShaderProgram::Stage, std::string>> vec{
            {
                    {ShaderProgram::Stage::Vertex, "basic"},
                    {ShaderProgram::Stage::Fragment, "basic"}
            }
    };
    ShaderProgram shaderProgram{vec};
    shaderProgram.reloadShaders();

    LogError("got to gameloop\n");

    struct Rotation {
        float angle;
        glm::vec3 rotAxis = {0.f, 0.f, 1.f};
    };

    // Gameloop
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();

        if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
            shaderProgram.reloadShaders();
        }
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(window, 1);
        }

        auto DrawPolygon = [shaderProgram, screenResolution](
                Polygon &p,
                glm::vec3 scale,
                Rotation rotation,
                glm::vec3 translation
        ){

            gl::UseProgram(shaderProgram.id);
            GLint MVP_ID = gl::GetUniformLocation(shaderProgram.id, "MVP");
            Assert(MVP_ID >= 0);

            glm::mat4 projection = glm::ortho(0.f, (float) screenResolution.x, (float) screenResolution.y, 0.f);
            glm::mat4 view;
            glm::mat4 model;

            auto MakeModel = [scale, rotation, translation]() {
                return glm::scale(
                        glm::translate(glm::mat4{}, translation) * // translation
                        glm::toMat4(glm::rotate(glm::quat{}, rotation.angle, rotation.rotAxis)), // rotation
                        scale); // scale
            };

            model = MakeModel();

            glm::mat4 MVP = projection * view * model;

            gl::UniformMatrix4fv(MVP_ID, 1, gl::FALSE_, (const GLfloat *) glm::value_ptr(MVP));

            gl::BindVertexArray(p.VAO);
            gl::DrawArrays(gl::TRIANGLES, 0, p.vertices);
        };

        gl::Clear(gl::COLOR_BUFFER_BIT);

        DrawPolygon(p1, {200.f, 200.f, 0.f}, Rotation{Pi}, {200.f, 300.f, 0.f});
        DrawPolygon(p1, {200.f, 200.f, 0.f}, Rotation{Pi}, {400.f, 300.f, 0.f});
        DrawPolygon(p1, {200.f, 200.f, 0.f}, Rotation{Pi}, {300.f, 100.f, 0.f});

        gl::BindVertexArray(0);

        glfwSwapBuffers(window);
    }

    return 0;
}
