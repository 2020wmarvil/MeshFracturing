#include "Lighting.h"

float vertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
};

DirectionalLight::DirectionalLight(const glm::vec3& direction, const Color &color, const LightProfile &lightProfile) {
    this->direction = direction;
    this->color = color;
    this->lightProfile = lightProfile;
}

PointLight::PointLight(const glm::vec3& position, const glm::vec3 &scale, const Color &color, const LightProfile &lightProfile, const AttenuationProfile &attenuationProfile) {
	this->position = position;
    this->scale = scale;
    this->color = color;
    this->lightProfile = lightProfile;
    this->attenuationProfile = attenuationProfile;

	// create shader
    shader = *(new Shader("assets/shaders/Light.vs", "assets/shaders/Light.fs"));

	// create vao, vbo
    glGenVertexArrays(1, &VAO);  
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void PointLight::Draw(const glm::mat4 &view, const glm::mat4 &projection) const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    
    shader.Use();
    shader.SetMat4("model", model);
    shader.SetMat4("view", view);
    shader.SetMat4("projection", projection);
    shader.SetVec3("color", color);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

SpotLight::SpotLight(const glm::vec3 &position, const glm::vec3 &direction, float cutoff, float innerCutoff, float outerCutoff, const glm::vec3 &scale, const Color &color, const LightProfile &lightProfile, const AttenuationProfile &attenuationProfile) {
	this->position = position;
	this->direction = direction;
	this->cutoff = cutoff;
	this->innerCutoff = innerCutoff;
	this->outerCutoff = outerCutoff;
    this->scale = scale;
    this->color = color;
    this->lightProfile = lightProfile;
    this->attenuationProfile = attenuationProfile;

	// create shader
    shader = *(new Shader("assets/shaders/Light.vs", "assets/shaders/Light.fs"));

	// create vao, vbo
    glGenVertexArrays(1, &VAO);  
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void SpotLight::Draw(const glm::mat4 &view, const glm::mat4 &projection) const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    
    shader.Use();
    shader.SetMat4("model", model);
    shader.SetMat4("view", view);
    shader.SetMat4("projection", projection);
    shader.SetVec3("color", color);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
