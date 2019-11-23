// opt
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// std
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <array>
// local
#include "Shader.h"

void subdivideTriangle(
    const std::array<float, 9>& tri,
    const int level,
    std::vector<float>& output
)
{
    if (level <= 0)
    {
        output.insert(output.end(), tri.begin(), tri.end());
    }
    else
    {
        glm::vec3 v12{
            tri[0] + tri[3],
            tri[1] + tri[4],
            tri[2] + tri[5]
        };
        v12 /= 2.0f;

        v12 = glm::normalize(v12);

        glm::vec3 v23{
            tri[0] + tri[6],
            tri[1] + tri[7],
            tri[2] + tri[8]
        };
        v23 /= 2.0f;

        v23 = glm::normalize(v23);

        glm::vec3 v13{
            tri[3] + tri[6],
            tri[4] + tri[7],
            tri[5] + tri[8]
        };
        v13 /= 2.0f;

        v13 = glm::normalize(v13);

        subdivideTriangle(
            {
                tri[0], tri[1], tri[2],
                v23.x, v23.y, v23.z,
                v12.x, v12.y, v12.z
            },
            level - 1,
            output
        );
        subdivideTriangle(
            {
                tri[3], tri[4], tri[5],
                v13.x, v13.y, v13.z,
                v12.x, v12.y, v12.z
            },
            level - 1,
            output
        );
        subdivideTriangle(
            {
                tri[6], tri[7], tri[8],
                v23.x, v23.y, v23.z,
                v13.x, v13.y, v13.z
            },
            level - 1,
            output
        );
        subdivideTriangle(
            {
                v12.x, v12.y, v12.z,
                v23.x, v23.y, v23.z,
                v13.x, v13.y, v13.z
            },
            level - 1,
            output
        );
    }
}

void processInput(GLFWwindow* window, float dt, glm::mat4& view)
{
    static float yaw = 0.0f;
    static float pitch = glm::radians(45.0f);
    constexpr float speed = glm::radians(90.0f);
    constexpr float radius = 3.0f;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        yaw -= speed * dt;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        yaw += speed * dt;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        pitch -= speed * dt;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        pitch += speed * dt;
    view = glm::mat4{ 1.0f };
    view = glm::lookAt(
        glm::vec3{ cos(yaw) * sin(pitch) * radius, cos(pitch) * radius, sin(pitch) * sin(yaw) * radius },
        glm::vec3{ 0.0f, 0.0f, 0.0f },
        glm::vec3{ 0.0f, 1.0f, 0.0f }
    );
}

int main()
{
    constexpr unsigned int SCR_WIDTH = 600;
    constexpr unsigned int SCR_HEIGHT = 600;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Sphere", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << '\n';
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(
        window,
        [](GLFWwindow* window, int size, int _) 
        { 
            glfwSetWindowSize(window, size, size);
            glViewport(0, 0, size, size);
            
        }
    );

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader shaderProgram{ "shaders/player.vert", "shaderes/player.frag" };

    std::vector<float> octahedron{
         0.0f,  1.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         0.0f,  0.0f,  1.0f,
         0.0f, -1.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         0.0f,  0.0f,  1.0f,
         0.0f,  1.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
         0.0f,  0.0f,  1.0f,
         0.0f, -1.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
         0.0f,  0.0f,  1.0f,
         0.0f,  1.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         0.0f,  0.0f, -1.0f,
         0.0f, -1.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         0.0f,  0.0f, -1.0f,
         0.0f, -1.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
         0.0f,  0.0f, -1.0f,
         0.0f,  1.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
         0.0f,  0.0f, -1.0f,
    };
    std::vector<float> circle{};
    for (int64_t i = 0; i < static_cast<int64_t>(octahedron.size()); i += 9)
    {
        std::array<float, 9> triangle{};
        std::copy(octahedron.begin() + i, octahedron.begin() + i + 9, triangle.begin());
        subdivideTriangle(            
            triangle,
            3,
            circle
        );
    }

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, circle.size() * sizeof(float), circle.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0); 

    glUseProgram(shaderProgram);
    glm::mat4 view{ 1.0f };

    const int viewLoc = glGetUniformLocation(shaderProgram, "view");

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.0f, 2.0f);
    glUniformMatrix4fv(
        glGetUniformLocation(shaderProgram, "proj"),
        1, GL_FALSE,
        glm::value_ptr(proj)
    );

    glm::mat4 model{ glm::scale(glm::mat4{ 1.0f }, glm::vec3{ 0.5f, 0.5f, 0.5f }) };
    glUniformMatrix4fv(
        glGetUniformLocation(shaderProgram, "model"),
        1, GL_FALSE,
        glm::value_ptr(model)
    );    
    
    double lastTime = glfwGetTime();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        float dt = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;
        processInput(window, dt, view);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniformMatrix4fv(
            viewLoc,
            1, GL_FALSE,
            glm::value_ptr(view)
        );

        // draw our first triangle
        glBindVertexArray(VAO); 
        glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(circle.size()));
        glBindVertexArray(0);
 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return 0;
}