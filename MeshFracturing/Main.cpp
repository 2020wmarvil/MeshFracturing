#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "SpeedRender/Core.h"
#include "SpeedRender/Camera.h"
#include "SpeedRender/Lighting.h"
#include "SpeedRender/Shader.h"
#include "SpeedRender/Model.h"
#include "SpeedRender/Mesh.h"
#include "SpeedRender/Texture.h"
#include "SpeedRender/Cubemap.h"
#include "SpeedRender/Skybox.h"

#include "ComputationalGeometry.h"

GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__) 

#define WIDTH 800
#define HEIGHT 600

void ProcessInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);  
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float lastTime = 0.0f, deltaTime = 0.0f;
bool mainWindowFocused = false;

Camera camera(
    glm::vec3(5.134f, 2.478f, 5.126f),
    glm::vec3(-0.7f, -0.276f, -0.659f),
    glm::vec3(0.0f, 1.0f, 0.0f), WIDTH, HEIGHT
);

int main() {
    // initialization
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // window creation
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Speed Render", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
    glfwSetCursorPosCallback(window, mouse_callback);  
    glfwSetMouseButtonCallback(window, mouse_button_callback);  
    glfwSetScrollCallback(window, scroll_callback); 

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // setup imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);

    // mesh data
    Model cube("assets/models/cube.obj");
    Model sphere("assets/models/sphere.obj"); 
    Model bunny("assets/models/bunny.obj");
    Model teapot("assets/models/teapot.obj");
    Model suzanne("assets/models/suzanne.obj");

    sphere.transform = {
        glm::vec3(0.0f, -1.703f, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(0.025f)
    };

    bunny.transform = {
        glm::vec3(0.566f, -0.778f, -0.106f),
        glm::vec3(0.0f, 0.556f, 0.0f),
        glm::vec3(1.0f)
    };

    teapot.transform = {
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(-1.533f, 0.067f, -2.632f),
        glm::vec3(0.153f)
    };

    suzanne.transform = {
        glm::vec3(0.0f),
        glm::vec3(-0.566f, 0.556f, 0.29f),
        glm::vec3(1.241f)
    };

    // textures
    Texture::SetFlipImageOnLoad(true);
    Texture diffuseMap("assets/images/container2.png");
    Texture specularMap("assets/images/container2_specular.png", "specular");
    cube.meshes[0].AddTexture(diffuseMap);
    cube.meshes[0].AddTexture(specularMap);

    // set up shaders
    Shader unlitShader("assets/shaders/MainVertex.vert", "assets/shaders/Unlit.frag");
    Shader litShader("assets/shaders/MainVertex.vert", "assets/shaders/LOGL_PBR.frag");
    Shader emShader("assets/shaders/MainVertex.vert", "assets/shaders/EM_Lit.frag");
    Shader normalsShader("assets/shaders/MainVertex.vert", "assets/shaders/TestNormals.frag");
    Shader uvsShader("assets/shaders/MainVertex.vert", "assets/shaders/TestUVs.frag");
    Material material = { diffuseMap, specularMap, 32.0f };

    // lights
    DirectionalLight dirLight(glm::vec3(-0.216f, -0.6f, -0.455f), Color(1.0f, 1.0f, 1.0f),
        { 
            glm::vec3(0.2f, 0.2f, 0.2f),
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        });

    enum ShaderState { SS_UNLIT, SS_LIT, SS_EM_LIT, SS_NORMALS, SS_UVS, SS_COUNT };
    int shaderState = SS_LIT;
    Shader* shader = &litShader;

    enum ModelState { MS_CUBE, MS_SPHERE, MS_BUNNY, MS_TEAPOT, MS_SUZANNE, MS_COUNT };
    int modelState = MS_BUNNY;
    Model* model = &bunny;

    enum FractureState { FS_ITERATIVE, FS_RECURSIVE, FS_TETRA, FS_VORONOI, FS_COUNT };
    int fractureState = FS_ITERATIVE;

    glm::vec3 albedo(1.0f, 0.0f, 0.0f);
    float metallic = 0.0f;
    float roughness = 0.385f;
    float ao = 0.511f;
    float refractionIndex = 1.022f;
    float reflectance = 0.519f;

    // skybox
    stbi_set_flip_vertically_on_load(false);
    std::vector<std::string> skyboxFaces = {
        "assets/skyboxes/water/right.jpg",
        "assets/skyboxes/water/left.jpg",
        "assets/skyboxes/water/top.jpg",
        "assets/skyboxes/water/bottom.jpg",
        "assets/skyboxes/water/front.jpg",
        "assets/skyboxes/water/back.jpg"
    }; Skybox skybox(skyboxFaces);

    std::vector<Model> toRender;
    std::vector<FractureComponent> fractures;
    bool beginFracture = false, fractureMode = false;

    int iterativeIterations = 6;
    int recursiveDepth = 4;
    int tetraPoints = 10;
    int voronoiPoints = 10;

    // render loop
    while(!glfwWindowShouldClose(window)) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        ProcessInput(window);

        if (beginFracture && !fractureMode) {
            if (fractureState == FS_ITERATIVE) {
                IterativeClippingFracture(model->meshes[0], fractures, iterativeIterations);
            } else if (fractureState == FS_RECURSIVE) {
                RecursivePlaneFracture(model->meshes[0], fractures, recursiveDepth);
            } else if (fractureState == FS_TETRA) {
                TetrahedralFracture(model->meshes[0], fractures, tetraPoints);
            } else if (fractureState == FS_VORONOI) {
                VoronoiFracture(model->meshes[0], fractures, voronoiPoints);
            }

            for (int i = 0; i < fractures.size(); i++) {
                toRender.push_back(fractures[i].GetMesh());
                toRender[i].transform = model->transform;
            }

            fractureMode = true;
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        {
            ImGui::PushStyleColor(ImGuiCol_ResizeGrip, 0);
            ImGui::Begin("Settings");
            ImGui::Text("Hold RMB+WASD to explore");

            const char* model_names[MS_COUNT] = { "Cube", "Sphere", "Bunny", "Teapot", "Suzanne" };
            const char* shader_names[SS_COUNT] = { "Unlit", "Lit", "Env Mapping", "Normals", "UVs" };
            const char* fracture_names[FS_COUNT] = { "Iterative Clipping", "Recursive Plane", "Tetrahedral", "Voronoi" };
            ImGui::Combo("Model", &modelState, model_names, IM_ARRAYSIZE(model_names));
            ImGui::Combo("Shader", &shaderState, shader_names, IM_ARRAYSIZE(shader_names));
            ImGui::Combo("Fracture Type", &fractureState, fracture_names, IM_ARRAYSIZE(fracture_names));

            if (fractureState == FS_ITERATIVE) {
                ImGui::InputInt("Iterations", &iterativeIterations, 1, 16);
            } else if (fractureState == FS_RECURSIVE) {
                ImGui::InputInt("Depth", &recursiveDepth, 1, 8);
            } else if (fractureState == FS_TETRA) {
                ImGui::InputInt("Points", &tetraPoints, 1, 100);
            } else if (fractureState == FS_VORONOI) {
                ImGui::InputInt("Points", &voronoiPoints, 1, 100);
            }

            if (ImGui::Button("Fracture")) { 
                if (!beginFracture) {
                    beginFracture = true;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Reset")) { 
                toRender.clear();
                fractures.clear();
                beginFracture = false;
                fractureMode = false;
            }


            if (shaderState == SS_UNLIT) shader = &unlitShader;
            else if (shaderState == SS_LIT) shader = &litShader;
            else if (shaderState == SS_EM_LIT) shader = &emShader;
            else if (shaderState == SS_NORMALS) shader = &normalsShader;
            else if (shaderState == SS_UVS) shader = &uvsShader;

            if (modelState == MS_CUBE) model = &cube;
            else if (modelState == MS_SPHERE) model = &sphere;
            else if (modelState == MS_BUNNY) model = &bunny;
            else if (modelState == MS_TEAPOT) model = &teapot;
            else if (modelState == MS_SUZANNE) model = &suzanne;

            ImGui::End();

            ImGui::Begin("FPS", (bool*)true, ImGuiWindowFlags_NoTitleBar);
            ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
            ImGui::End();
            ImGui::PopStyleColor();
        }

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix();

        shader->Use();
        shader->SetMat4("view", view);
        shader->SetMat4("projection", projection);

        if (shaderState == SS_UNLIT) {
            shader->SetVec3("mainColor", glm::vec3(1.0f, 1.0f, 1.0f)); 
        } else if (shaderState == SS_LIT) {
            // material
            shader->SetVec3("albedo", albedo);
            shader->SetFloat("metallic", metallic);
            shader->SetFloat("roughness", roughness);
            shader->SetFloat("ao", ao);
            // lights
            shader->SetVec3("lightPositions[0]", glm::vec3(1.0f, 5.0f, 0.0f));
            shader->SetVec3("lightPositions[1]", glm::vec3(-1.0f, -5.0f, 0.0f));
            shader->SetVec3("lightPositions[2]", glm::vec3(0.0f, 0.0f, 1.0f));
            shader->SetVec3("lightPositions[3]", glm::vec3(3.0f, 0.0f, -1.0f));
            shader->SetVec3("lightColors[0]", glm::vec3(1.0f, 1.0f, 1.0f));
            shader->SetVec3("lightColors[1]", glm::vec3(1.0f, 1.0f, 1.0f));
            shader->SetVec3("lightColors[2]", glm::vec3(1.0f, 1.0f, 1.0f));
            shader->SetVec3("lightColors[3]", glm::vec3(1.0f, 1.0f, 1.0f));
            shader->SetVec3("cameraPos", camera.position);
        } else if (shaderState == SS_EM_LIT) {
            shader->SetVec3("cameraPos", camera.position);
            shader->SetFloat("refractionIndex", refractionIndex);
            shader->SetFloat("reflectance", reflectance);
        }

        if (fractureMode) {
            for (int i = 0; i < toRender.size(); i++) {
                shader->SetMat4("model", toRender[i].GetModelMatrix());
                toRender[i].Draw(*shader);
            }
        } else {
            shader->SetMat4("model", model->GetModelMatrix());
            model->Draw(*shader);
        }

        skybox.Draw(view, projection);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();

        glCheckError();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void ProcessInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (mainWindowFocused) camera.ProcessWindowEvents(window, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}  

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (mainWindowFocused) camera.UpdateMousePosition(xpos, ypos);
}  

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        mainWindowFocused = true;
        camera.firstMouse = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        mainWindowFocused = false;
        camera.firstMouse = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (mainWindowFocused) camera.UpdateScrollPosition(xoffset, yoffset);
}
