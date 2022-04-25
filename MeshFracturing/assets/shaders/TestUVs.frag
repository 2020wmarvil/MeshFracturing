#version 330 core

out vec4 FragColor;

in vec2 texCoords;

void main() {
    FragColor = vec4(texCoords.xy, 0.0, 1.0);
}
