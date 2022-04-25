#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GLFW/glfw3.h>
  
class Shader {
public:
    Shader();
    Shader(const char* vertexPath, const char* fragmentPath);

    void Use() const;

    void SetBool(const std::string &name, bool value) const;  
    void SetInt(const std::string &name, int value) const;   
    void SetFloat(const std::string &name, float value) const;
    void SetMat4(const std::string &name, const glm::mat4 &value) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;

    unsigned int ID;
    bool depthTest = true;
    bool depthWrite = true;
    unsigned int depthFunc = GL_LESS;
};
  
#endif
