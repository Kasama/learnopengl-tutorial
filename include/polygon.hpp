#ifndef TUTORIAL_POLYGON_HPP
#define TUTORIAL_POLYGON_HPP
#pragma once

struct Vertex {
    glm::vec3 PositionCoords;
    glm::vec2 TextureCoords;
    glm::vec4 Color;

    enum class attribute_location : GLuint {
        Position = 0,
        Texture,
        Color,
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
    glm::vec4 Color = {1.f, 1.f, 0.f, 1.f};
    float Speed;

    inline Polygon(std::vector<Vertex> vertexes, float speed, glm::vec3 direction){
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
                    (GLint) 3,
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
                    (GLint) 2,
                    gl::FLOAT,
                    gl::FALSE_,
                    sizeof(Vertex),
                    (void*) offsetof(Vertex, TextureCoords)
            );

            auto ColorLocation = (GLuint) Vertex::attribute_location::Color;
            gl::EnableVertexAttribArray(ColorLocation);
            gl::VertexAttribPointer(
                    ColorLocation,
                    (GLint) 4,
                    gl::FLOAT,
                    gl::FALSE_,
                    sizeof(Vertex),
                    (void *) offsetof(Vertex, Color)
            );

            LogError("Unbinding VAO");
            gl::BindVertexArray(0);
        }
    }

    inline Polygon(std::vector<Vertex> vertexes, float speed)
            : Polygon{vertexes, speed, glm::vec3{0.f}}{}

    inline Polygon(std::vector<Vertex> vertexes)
            : Polygon{vertexes, 100.f}{}

    inline void setDirectionTo(glm::vec3 position){
        this->Direction = glm::normalize(position - this->Position);
    }
    inline void move(float factor){
        this->Position += (this->Direction * factor * this->Speed);
    }

    inline ~Polygon(){
        gl::DeleteVertexArrays(1, &this->VAO);
    }
};

#endif //TUTORIAL_POLYGON_HPP
