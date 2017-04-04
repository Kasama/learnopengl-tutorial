#version 330 core

in vec3 UV;
in vec4 Color;

out vec4 ColorOut;

void main() {
    float r = 0.4*((sin(UV.x + UV.y)+1)/2);
    float g = 0.2*((cos(UV.x + UV.y)+1)/2);
    float b = 0.9*((sin(UV.x + UV.y)+1)/2);
//    float r = 0.4*((sin(UV.x + UV.y)+1)/2);
//    float g = 0.7*((cos(UV.x + UV.y)+1)/2);
//    float b = 0.3*((sin(UV.x + UV.y)+1)/2);
	ColorOut = vec4(r, g, b, 1.0);
//    ColorOut = Color;
}
