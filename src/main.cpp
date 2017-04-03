#include <utils.hpp>
#include <iostream>
#include <gl_33.hpp>
#include <GLFW/glfw3.h>
#include <shader.hpp>

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

    struct Vertex {
        glm::vec3 PositionCoords;
        glm::vec3 TextureCoords;

        enum class attribute_location : GLuint {
            Position = 0,
            Texture = 1
        };
    };

    struct Polygon {
        GLuint VAO;
        glm::vec3 Direction = glm::vec3{0.f};
        float Speed = 100.f;

        Polygon(Vertex *vertexes, float speed, glm::vec3 direction){
            this->Direction = direction;
            this->Speed = speed;

            int vertices = 3;

            // Create Vertex Array
            gl::GenVertexArrays(1, &this->VAO);
            { // Vertex Array configuration
                // Bind VAO
                LogError("Binding VAO");
                gl::BindVertexArray(this->VAO);

                // Create Vertex Buffer
                GLuint VBO;
                gl::GenBuffers(1, &VBO);
                defer { gl::DeleteBuffers(1, &VBO); };
                gl::BindBuffer(gl::ARRAY_BUFFER, VBO);
                gl::BufferData(gl::ARRAY_BUFFER, sizeof(Vertex) * vertices, vertexes, gl::STATIC_DRAW);

                // Set Position Stride
                auto PositionLocation = (GLuint) Vertex::attribute_location::Position;
                gl::EnableVertexAttribArray(PositionLocation);
                gl::VertexAttribPointer( PositionLocation, 2, gl::FLOAT, gl::FALSE_, sizeof(Vertex), (void*) offsetof(Vertex, PositionCoords) );

                // Set Texture Stride
                auto TextureLocation = (GLuint) Vertex::attribute_location::Texture;
                gl::EnableVertexAttribArray(TextureLocation);
                gl::VertexAttribPointer( TextureLocation, 2, gl::FLOAT, gl::FALSE_, sizeof(Vertex), (void*) offsetof(Vertex, TextureCoords) );

                LogError("Unbinding VAO");
                gl::BindVertexArray(0);
            }
        }

        Polygon(Vertex *vertexes, float speed)
                : Polygon{vertexes, speed, glm::vec3{0.f}}{}

        Polygon(Vertex *vertexes)
                : Polygon{vertexes, 100.f}{}

        ~Polygon(){
            gl::DeleteVertexArrays(1, &this->VAO);
        }
    };

    Vertex v1 = {{-0.5f, -0.5f, +0.0f}, {+0.0f, +0.0f, +0.0f}};
    Vertex v2 = {{+0.0f, +0.5f, +0.0f}, {+0.5f, +1.0f, +0.0f}};
    Vertex v3 = {{+0.5f, -0.5f, +0.0f}, {+0.0f, +0.0f, +0.0f}};
    Vertex vs[] = {v1,v2,v3};
//    Polygon p1{vs};
    Polygon p1{vs, 100.f, glm::vec3{0.f}};

//    Vertex vertexes[] = {
//            {{-0.5f, -0.5f}},
//            {{+0.0f, +0.5f}},
//            {{+0.5f, -0.5f}}
//    };

//    LogError("Got to vertex initialization\n");

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

        gl::UseProgram(shaderProgram.id);
        GLint MVP_ID = gl::GetUniformLocation(shaderProgram.id, "MVP");
        Assert(MVP_ID >= 0);

        glm::mat4 projection = glm::ortho(0.f, (float) screenResolution.x, (float) screenResolution.y, 0.f);
        glm::mat4 view;
        glm::mat4 model;

        auto MakeModel = [](glm::vec3 scale, Rotation rotation, glm::vec3 translate) {
            return glm::scale(
                    glm::translate(glm::mat4{}, translate) * // translation
                            glm::toMat4(glm::rotate(glm::quat{}, rotation.angle, rotation.rotAxis)), // rotation
                    scale); // scale
        };

        glm::vec3 scale = {400.f, 400.f, 0.f};
        Rotation rotation = Rotation{Pi/4};
        glm::vec3 translate;

        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS){
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            LogError("Cursor pressed at: {%lf, %lf}\n", x, y);
            translate = {(float) x, (float) y, 0.f};
        }else{
            translate = {500.f, 300.f, 0.f};
        }

        model = MakeModel(scale, rotation, translate);

        glm::mat4 MVP = projection * view * model;

        gl::UniformMatrix4fv(MVP_ID, 1, gl::FALSE_, (const GLfloat *) glm::value_ptr(MVP));

        gl::Clear(gl::COLOR_BUFFER_BIT);
        gl::BindVertexArray(p1.VAO);
        gl::DrawArrays(gl::TRIANGLES, 0, 3);
        gl::BindVertexArray(0);

        glfwSwapBuffers(window);
    }

    return 0;
}
