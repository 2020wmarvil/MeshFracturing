#version 330 core

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D diffuse1;
uniform vec3 mainColor;

void main() {
    FragColor = texture(diffuse1, texCoords) * vec4(mainColor, 1.0);
}
