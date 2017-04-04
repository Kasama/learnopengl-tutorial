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
            }, 100.f
    };

    Polygon p2{
            {
                    {{-0.5f, -0.5f, +0.0f}, {+0.0f, +0.0f, +0.0f}},
                    {{+0.0f, +0.5f, +0.0f}, {+0.5f, +1.0f, +0.0f}},
                    {{+0.5f, -0.5f, +0.0f}, {+0.0f, +0.0f, +0.0f}}
            }, 500.f
    };
    p2.Position = {100.f, 100.f, 0.f};
    p2.Rotation = {Pi/2};

    Polygon p3{
            {
                    {{-0.5f, -0.5f, +0.0f}, {+0.0f, +0.0f, +0.0f}},
                    {{+0.0f, +0.5f, +0.0f}, {+0.5f, +1.0f, +0.0f}},
                    {{+0.5f, -0.5f, +0.0f}, {+0.0f, +0.0f, +0.0f}}
            }, 300.f
    };
    p3.Position = {400.f, 200.f, 0.f};
    p3.Rotation = {Pi/7};

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

    auto DrawPolygon = [shaderProgram, screenResolution](Polygon &p){

        gl::UseProgram(shaderProgram.id);
        GLint MVP_ID = gl::GetUniformLocation(shaderProgram.id, "MVP");
        Assert(MVP_ID >= 0);

        glm::mat4 projection = glm::ortho(0.f, (float) screenResolution.x, (float) screenResolution.y, 0.f);
        glm::mat4 view;
        glm::mat4 model;

        auto MakeModel = [&p]() {
            return glm::scale(
                    glm::translate(glm::mat4{}, p.Position) * // translation
                    glm::toMat4(glm::rotate(glm::quat{}, p.Rotation.angle, p.Rotation.rotAxis)), // rotation
                    p.Scale); // scale
        };

        model = MakeModel();

        glm::mat4 MVP = projection * view * model;

        gl::UniformMatrix4fv(MVP_ID, 1, gl::FALSE_, (const GLfloat *) glm::value_ptr(MVP));

        gl::BindVertexArray(p.VAO);
        gl::DrawArrays(gl::TRIANGLES, 0, p.vertices);
    };

    float lastTime = (float) glfwGetTime();
    // Gameloop
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();

        float currentDelta = (float) (glfwGetTime() - lastTime);
        lastTime = (float) glfwGetTime();

        if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
            shaderProgram.reloadShaders();
        }
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(window, 1);
        }
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS){
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            p1.setDirectionTo({x, y, 0.f});
            p2.setDirectionTo({x, y, 0.f});
            p3.setDirectionTo({x, y, 0.f});
        }

        p1.move(currentDelta);
        p2.move(currentDelta);
        p3.move(currentDelta);

        gl::Clear(gl::COLOR_BUFFER_BIT);

        DrawPolygon(p1);
        DrawPolygon(p2);
        DrawPolygon(p3);

        gl::BindVertexArray(0);

        glfwSwapBuffers(window);
    }

    return 0;
}
