#ifndef TUTORIAL_POLYGON_HPP
#define TUTORIAL_POLYGON_HPP
#pragma once

struct Vertex {
    glm::vec3 PositionCoords;
    glm::vec3 TextureCoords;

    enum class attribute_location : GLuint {
        Position = 0,
        Texture,
        Size
    };
};

struct tRotation {
    float angle;
    glm::vec3 rotAxis = {0.f, 0.f, 1.f};
};

struct Polygon {
    GLuint VAO;
    GLsizei vertices;
    glm::vec3 Scale = glm::vec3{200.f, 200.f, 0.f};
    glm::vec3 Position = glm::vec3{200.f, 200.f, 0.f};
    tRotation Rotation{0};
    glm::vec3 Direction = glm::vec3{0.f};
    float Speed = 1.f;

    Polygon(std::vector<Vertex> vertexes, float speed, glm::vec3 direction){
        this->Direction = direction;
        this->Speed = speed;

        this->vertices = (GLsizei) vertexes.size();

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
            gl::BufferData(gl::ARRAY_BUFFER, sizeof(Vertex) * this->vertices, &vertexes[0], gl::STATIC_DRAW);

            // Set Position Stride
            auto PositionLocation = (GLuint) Vertex::attribute_location::Position;
            gl::EnableVertexAttribArray(PositionLocation);
            gl::VertexAttribPointer(
                    PositionLocation,
                    (GLint) Vertex::attribute_location::Size,
                    gl::FLOAT,
                    gl::FALSE_,
                    sizeof(Vertex),
                    (void*) offsetof(Vertex, PositionCoords)
            );

            // Set Texture Stride
            auto TextureLocation = (GLuint) Vertex::attribute_location::Texture;
            gl::EnableVertexAttribArray(TextureLocation);
            gl::VertexAttribPointer(
                    TextureLocation,
                    (GLint) Vertex::attribute_location::Size,
                    gl::FLOAT,
                    gl::FALSE_,
                    sizeof(Vertex),
                    (void*) offsetof(Vertex, TextureCoords)
            );

            LogError("Unbinding VAO");
            gl::BindVertexArray(0);
        }
    }

    Polygon(std::vector<Vertex> vertexes, float speed)
            : Polygon{vertexes, speed, glm::vec3{0.f}}{}

    Polygon(std::vector<Vertex> vertexes)
            : Polygon{vertexes, 100.f}{}

    void setDirectionTo(glm::vec3 dir){
        this->Direction = glm::normalize(dir - this->Position);
    }
    void move(float factor){
        LogError("Direction is: {%f, %f}", Direction.x, Direction.y);
        this->Position += (this->Direction * factor * this->Speed);
        LogError("new pos: {%f, %f}\n", Position.x, Position.y);
    }

    ~Polygon(){
        gl::DeleteVertexArrays(1, &this->VAO);
    }
};

#endif //TUTORIAL_POLYGON_HPP
