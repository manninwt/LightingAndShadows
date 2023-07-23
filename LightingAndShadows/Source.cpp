#define _USE_MATH_DEFINES
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "stb_image.h"
#include <C:\Development\VSRepos\LightingAndShadows\LightingAndShadows\Shader.h>

class Triangle {
public:
    glm::vec3 k1;
    glm::vec3 k2;
    glm::vec3 k3;
};

class Square {
public:
    glm::vec3 k1;
    glm::vec3 k2;
    glm::vec3 k3;
    glm::vec3 k4;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
std::vector<std::vector<glm::vec3>> CalculateVerticies(GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat radius, unsigned int sectorCount, unsigned int stackCount);
std::vector<glm::vec3> CalculateIndicies(std::vector<std::vector<glm::vec3>> vertices, unsigned int sectorCount, unsigned int stackCount);
std::vector<glm::vec3> CalculateNormals(std::vector<glm::vec3> vertices);
glm::vec3 Normalize(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
std::vector<float> CombineIndsandNormsUntilImSmarter(std::vector<glm::vec3> norms, std::vector<glm::vec3> inds);
void LoadIndices(glm::vec3 k1, glm::vec3 k2, glm::vec3 k3, std::vector<glm::vec3>& indicesList);
void LoadIndicesFromTriangle(Triangle triangle, std::vector<glm::vec3>& indicesList);
void EvaluateSquareSection(glm::vec3 k1, glm::vec3 k2, glm::vec3 k3, glm::vec3 k4, std::vector<glm::vec3>& indicesList);

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1440;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -70.0f;
float pitch = 15.0f;
float lastX = 1920.0f / 2.0;
float lastY = 1440.0f / 2.0;
float fov = 45.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
#pragma region Path/Shader Stuff
    const char* basePathImages = "C:\\Development\\OpenGL\\Images\\";
    const char* basePathProj = "C:\\Development\\VSRepos\\LightingAndShadows\\";

    char lightCubeVsShader[100];
    char lightCubeFsShader[100];
    char sphereVsShader[100];
    char sphereFsShader[100];

    strcpy_s(lightCubeVsShader, basePathProj);
    strcpy_s(sphereVsShader, basePathProj);
    strcpy_s(lightCubeFsShader, basePathProj);
    strcpy_s(sphereFsShader, basePathProj);

    strcat_s(lightCubeVsShader, "LightingAndShadows\\shader.vs");
    strcat_s(lightCubeFsShader, "LightingAndShadows\\shader.fs");

    strcat_s(sphereVsShader, "LightingAndShadows\\shader2.vs");
    strcat_s(sphereFsShader, "LightingAndShadows\\shader2.fs");

    char containerPath[100];
    strcpy_s(containerPath, basePathImages);
    strcat_s(containerPath, "container.jpg");

    char facePath[100];
    strcpy_s(facePath, basePathImages);
    strcat_s(facePath, "awesomeface.png");
#pragma endregion

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);


#pragma region Verticies
    float vertices[] = {
        // positions        // texture coords
      -7.0f, -7.0f, -57.0f,//  0.0f, 0.0f,
       7.0f, -7.0f, -57.0f,//  1.0f, 0.0f,
       7.0f,  7.0f, -57.0f,//  1.0f, 1.0f,
       7.0f,  7.0f, -57.0f,//  1.0f, 1.0f,
      -7.0f,  7.0f, -57.0f,//  0.0f, 1.0f,
      -7.0f, -7.0f, -57.0f,//  0.0f, 0.0f,
                          //
      -7.0f, -7.0f,  -43.0f,//  0.0f, 0.0f,
       7.0f, -7.0f,  -43.0f,//  1.0f, 0.0f,
       7.0f,  7.0f,  -43.0f,//  1.0f, 1.0f,
       7.0f,  7.0f,  -43.0f,//  1.0f, 1.0f,
      -7.0f,  7.0f,  -43.0f,//  0.0f, 1.0f,
      -7.0f, -7.0f,  -43.0f,//  0.0f, 0.0f,
                          //
      -7.0f,  7.0f,  -43.0f,//  1.0f, 0.0f,
      -7.0f,  7.0f, -57.0f,//  1.0f, 1.0f,
      -7.0f, -7.0f, -57.0f,//  0.0f, 1.0f,
      -7.0f, -7.0f, -57.0f,//  0.0f, 1.0f,
      -7.0f, -7.0f,  -43.0f,//  0.0f, 0.0f,
      -7.0f,  7.0f,  -43.0f,//  1.0f, 0.0f,
                          //
       7.0f,  7.0f,  -43.0f,//  1.0f, 0.0f,
       7.0f,  7.0f, -57.0f,//  1.0f, 1.0f,
       7.0f, -7.0f, -57.0f,//  0.0f, 1.0f,
       7.0f, -7.0f, -57.0f,//  0.0f, 1.0f,
       7.0f, -7.0f,  -43.0f,//  0.0f, 0.0f,
       7.0f,  7.0f,  -43.0f,//  1.0f, 0.0f,
                          //
      -7.0f, -7.0f, -57.0f,//  0.0f, 1.0f,
       7.0f, -7.0f, -57.0f,//  1.0f, 1.0f,
       7.0f, -7.0f, -43.0f,//  1.0f, 0.0f,
       7.0f, -7.0f, -43.0f,//  1.0f, 0.0f,
      -7.0f, -7.0f, -43.0f,//  0.0f, 0.0f,
      -7.0f, -7.0f, -57.0f,//  0.0f, 1.0f,
                          //
      -7.0f,  7.0f, -57.0f,//  0.0f, 1.0f,
       7.0f,  7.0f, -57.0f,//  1.0f, 1.0f,
       7.0f,  7.0f, -43.0f,//  1.0f, 0.0f,
       7.0f,  7.0f, -43.0f,//  1.0f, 0.0f,
      -7.0f,  7.0f, -43.0f,//  0.0f, 0.0f,
      -7.0f,  7.0f, -57.0f//,  0.0f, 1.0f
    };

    std::vector<std::vector<glm::vec3>> sphereVertices = CalculateVerticies(15.0f, 12.0f, -10.0f, 7.0f, 20, 20);
    std::vector<glm::vec3> verts = CalculateIndicies(sphereVertices, 20, 20);
    std::vector<glm::vec3> normals = CalculateNormals(verts);
    std::vector<float> indices = CombineIndsandNormsUntilImSmarter(normals, verts);

#pragma endregion

#pragma region Vertex Buffer
    unsigned int VBO;
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(float), &indices.front(), GL_STATIC_DRAW);

    unsigned int VBO1;
    glGenBuffers(1, &VBO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
#pragma endregion

#pragma region Texture Buffer
    //Texture element generation
    unsigned int texture1, texture2;

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // Set the texture wrapping options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set the texture filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load and generate the texture
    int width, height, nrChannels;

    unsigned char* data = stbi_load(containerPath, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // Set the texture wrapping options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set the texture filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    data = stbi_load(facePath, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
#pragma endregion

#pragma region Vertex Array
    // Configure the Vertex Array Object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(float), &indices.front(), GL_DYNAMIC_DRAW);
    
    // Position Attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int VAO1;
    glGenVertexArrays(1, &VAO1);
    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Position Attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int LightVAO;
    glGenVertexArrays(1, &LightVAO);
    glBindVertexArray(LightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position Attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
#pragma endregion

    // Texture Attributes
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);

    Shader sphereShader(sphereVsShader, sphereFsShader);
    Shader lightCubeShader(lightCubeVsShader, lightCubeFsShader);
    /*ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);*/

    glm::vec3 cubePositions[] = {
    glm::vec3(-5.0f,  20.0f,  -43.0f),
    glm::vec3(-5.0f,  -40.0f, -43.0f),
    glm::vec3(-35.0f, -10.0f, -43.0f),
    glm::vec3(25.0f, -10.0f, -43.0f),
   /* glm::vec3(-2.4f, -0.4f, -50.0f),
    glm::vec3(-1.7f,  3.0f, -50.0f),*/
    //glm::vec3(-1.3f, -2.0f, -2.5f),
    //glm::vec3(1.5f,  2.0f, -2.5f),
    //glm::vec3(-1.5f,  0.9f, -1.5f),
    //glm::vec3(-1.3f,  1.0f, -1.5f)*/
    };

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_POLYGON);

        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, texture1);
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, texture2);

        sphereShader.use();


        sphereShader.setVec3("objectColor", 1.0f, 1.0f, 0.2f);
        sphereShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        sphereShader.setVec3("lightPos", lightPos);

        // Camera Work/View Matrix
        glm::mat4 view = glm::mat4(1.0f);
        const float radius = 10.0f;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        sphereShader.setMat4("view", view);

        // Projection Matrix
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(fov), 1920.0f / 1440.0f, 0.1f, 500.0f);
        sphereShader.setMat4("projection", projection);

        glBindVertexArray(VAO);

        float angle = 7.0f;

        for (unsigned int i = 0; i < std::size(cubePositions); i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            if (i % 3 == 0)  // every 3rd iteration (including the first) we set the angle using GLFW's time function.
                angle = glfwGetTime() * 25.0f;
            sphereShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, indices.size() * sizeof(glm::vec3));
        }

        lightCubeShader.use();

        glm::mat4 viewb = glm::mat4(1.0f);
        const float radiusb = 10.0f;
        viewb = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        lightCubeShader.setMat4("view", viewb);

        // Projection Matrix
        glm::mat4 projectionb;
        projectionb = glm::perspective(glm::radians(fov), 1920.0f / 1440.0f, 0.1f, 500.0f);
        lightCubeShader.setMat4("projection", projectionb);

        glm::mat4 modelb = glm::mat4(1.0f);
        modelb = glm::translate(modelb, lightPos);
        modelb = glm::scale(modelb, glm::vec3(0.2f));
        lightCubeShader.setMat4("model", modelb);

        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    const float cameraSpeed = 45.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

std::vector<std::vector<glm::vec3>> CalculateVerticies(GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat radius, unsigned int sectorCount, unsigned int stackCount)
{
    std::vector<std::vector<glm::vec3>> vertices;
    float pi = M_PI;

    vertices.push_back(std::vector<glm::vec3>());

    glm::vec3 vert;
    vert[0] = centerPosX;
    vert[1] = centerPosY;
    vert[2] = centerPosZ + radius;

    vertices[0].push_back(vert);

    for (float i = 1.0; i <= stackCount; i++)
    {
        vertices.push_back(std::vector<glm::vec3>());
        float sigma = (pi / 2) - (pi * (i / stackCount));
        for (float j = 0.0; j <= sectorCount; j++)
        {
            float theta = (2 * pi) * (j / sectorCount);
            glm::vec3 vert;
            vert[0] = centerPosX + (radius * cosf(sigma)) * cosf(theta);
            vert[1] = centerPosY + (radius * cosf(sigma)) * sinf(theta);
            vert[2] = centerPosZ + radius * sinf(sigma);

            vertices[(int)i].push_back(vert);
        }
    }
    return vertices;
}

std::vector<glm::vec3> CalculateIndicies(std::vector<std::vector<glm::vec3>> vertices, unsigned int sectorCount, unsigned int stackCount)
{
    glm::vec3 k1, k2, k3, k4, k5, k6;
    std::vector<glm::vec3> indices;

    for (float i = 0.0; i <= stackCount - 1; ++i)
    {
        for (float j = 0.0; j <= sectorCount; ++j)
        {
            // First passthrough in which we are dealing with the very top point of the circle
            if (i == 0) 
            {
                k1 = vertices[0][0];
                k2 = vertices[1][j];
                // Conditional to wrap around to the first point for the final sector triangle
                if (j != sectorCount)
                {
                    k3 = vertices[1][j + 1];
                }
                else
                {
                    k3 = vertices[1][0];
                }

                LoadIndices(k1, k2, k3, indices);
            }
            else if (j != sectorCount && i != 0 && i != stackCount - 1)
            {
                k1 = vertices[i][j];
                k2 = vertices[i + 1][j];
                k3 = vertices[i][j + 1];
                k4 = vertices[i + 1][j + 1];

                EvaluateSquareSection(k1, k2, k3, k4, indices);
            }
            else if (j == sectorCount && i != stackCount) 
            {
                k1 = vertices[i][j];
                k2 = vertices[i + 1][j];
                k3 = vertices[i][0];
                k4 = vertices[i + 1][0];

                EvaluateSquareSection(k1, k2, k3, k4, indices);
            }
            else
            {
                k1 = vertices[i][0];
                k2 = vertices[i - 1][j];
                // Conditional to wrap around to the first point for the final sector triangle
                if (j != sectorCount)
                {
                    k3 = vertices[i - 1][j + 1];
                }
                else
                {
                    k3 = vertices[i - 1][0];
                }

                LoadIndices(k1, k2, k3, indices);
            }
        }
    }

    return indices;
}

void LoadIndices(glm::vec3 k1, glm::vec3 k2, glm::vec3 k3, std::vector<glm::vec3>& indicesList)
{
    indicesList.push_back(k1);
    indicesList.push_back(k2);
    indicesList.push_back(k3);
}

void LoadIndicesFromTriangle(Triangle triangle, std::vector<glm::vec3>& indicesList)
{
    indicesList.push_back(triangle.k1);
    indicesList.push_back(triangle.k2);
    indicesList.push_back(triangle.k3);
}

void EvaluateSquareSection(glm::vec3 k1, glm::vec3 k2, glm::vec3 k3, glm::vec3 k4, std::vector<glm::vec3>& indicesList)
{
    Square section;
    Triangle triangle1, triangle2;

    section.k1 = k1;
    section.k2 = k2;
    section.k3 = k3;
    section.k4 = k4;

    triangle1.k1 = section.k1;
    triangle1.k2 = section.k2;
    triangle1.k3 = section.k3;
    LoadIndicesFromTriangle(triangle1, indicesList);

    triangle2.k1 = section.k3;
    triangle2.k2 = section.k2;
    triangle2.k3 = section.k3;
    LoadIndicesFromTriangle(triangle2, indicesList);
}

std::vector<glm::vec3> CalculateNormals(std::vector<glm::vec3> vertices)
{
    std::vector<glm::vec3> normals;    
    for (int i = 0; i < vertices.size() - 3; i += 3)
    {
        glm::vec3 p1 = vertices[i];
        glm::vec3 p2 = vertices[i + 1];
        glm::vec3 p3 = vertices[i + 2];

        normals.push_back(Normalize(p1, p2, p3));
    }

    return normals;
}

glm::vec3 Normalize(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
    glm::vec3 vecDiff1 = p2 - p1;
    glm::vec3 vecDiff2 = p3 - p1;

    glm::vec3 normal;
    normal[0] = (vecDiff1.y * vecDiff2.z) - (vecDiff1.z * vecDiff2.y);
    normal[1] = (vecDiff1.z * vecDiff2.x) - (vecDiff1.x * vecDiff2.z);
    normal[2] = (vecDiff1.x * vecDiff2.y) - (vecDiff1.y * vecDiff2.x);

    return normal;
}

std::vector<float> CombineIndsandNormsUntilImSmarter(std::vector<glm::vec3> norms, std::vector<glm::vec3> inds) 
{
    std::vector<float> floatArray;

    for (int i = 0, j = 0; i < inds.size(); i += 1)
    {
        floatArray.push_back(inds[i].x);
        floatArray.push_back(inds[i].y);
        floatArray.push_back(inds[i].z);
        floatArray.push_back(norms[j].x);
        floatArray.push_back(norms[j].y);
        floatArray.push_back(norms[j].z);
        
        if (i % 3 == 0)
        {
            if (j != 797) 
            {
                j += 1;
            }
            else 
            {
                j = 0;
            }
            
        }
    }

    return floatArray;
}