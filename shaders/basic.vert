#version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Texture;
layout (location = 2) in vec4 Color;

uniform mat4 MVP;

out vec3 UV;
out vec4 ColorOut;

void main() {
    gl_Position = MVP * vec4(Position.x, Position.y, Position.z, 1.0);
    UV = Texture;
    ColorOut = Color;
}
