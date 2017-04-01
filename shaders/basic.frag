#version 330 core

in vec2 UV;

out vec4 Color;

void main() {
    float r = 0.4*((sin(UV.x + UV.y)+1)/2);
    float g = 0.2*((cos(UV.x + UV.y)+1)/2);
    float b = 0.9*((sin(UV.x + UV.y)+1)/2);
	Color = vec4(r, g, b, 1.0);
}
