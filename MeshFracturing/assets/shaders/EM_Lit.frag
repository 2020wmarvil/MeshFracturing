#version 330 core
out vec4 FragColor;

in vec3 normal;
in vec3 worldPosition;
in vec3 pos;

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform float refractionIndex;
uniform float reflectance;

uniform mat4 model;

void main() {             
    vec3 wPos = vec3(model * vec4(pos, 1.0));

    vec3 I = normalize(wPos - cameraPos);
    vec3 R1 = reflect(I, normalize(normal));
    vec3 R2 = refract(I, normalize(normal), refractionIndex);
    vec4 reflectColor = texture(skybox, R1);
    vec4 refractColor = texture(skybox, R2);
    vec4 color = (reflectColor * reflectance) + refractColor * (1 - reflectance);
    FragColor = vec4(color.xyz, 1.0);
}

