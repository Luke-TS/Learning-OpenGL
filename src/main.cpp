// function pointer loading automation
#include "glad/gl.h"

// window & input manager for opengl context
#include <GLFW/glfw3.h>

// matrix library for OpenGL
#include <bits/types/struct_timeval.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <iostream>
#include <random>

#include "shader.h"
#include "texture.h"

#define WIDTH  1000
#define HEIGHT 1000

struct InputFlags {
    bool rotate = true;
};

// global time tracking
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// global camera status
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

float lastX = 400, lastY = 300;
float pitch, yaw;
bool  firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
  
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}  

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

float fov = 45.0f;
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f; 
}

void processInput(GLFWwindow *window, struct InputFlags *flags) {
    // escape to close
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    static bool backspace_was_down = false; // previous backspace state

    bool backspace_is_down = glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS;
    if (!backspace_was_down && backspace_is_down) { // state is different
        flags->rotate = !flags->rotate;             // flip flag
    }

    backspace_was_down = backspace_is_down; // update state
   
    const float cameraSpeed = 5.0f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};


int main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // glfwGetProcAddress queries OpenGL driver for supported functions
    // glad automates this process and defines function pointers in gl.h
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // Query GPU information
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* version = glGetString(GL_VERSION);

    std::cout << "OpenGL Renderer: " << renderer << std::endl;
    std::cout << "OpenGL Vendor: " << vendor << std::endl;
    std::cout << "OpenGL Version: " << version << std::endl;

    // set dimensions and resize callback
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // mouse input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);  

    // scroll input
    glfwSetScrollCallback(window, scroll_callback);
    
    // enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // init VAO
    unsigned int VAO_0;
    glGenVertexArrays(1, &VAO_0);
    // init buffers
    unsigned int VBO; // vertex buffer
    unsigned int EBO; // element buffer
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // copy data
    glBindVertexArray(VAO_0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    // set attribute pointers
    // ===========================================
    // | vertices[3] | colors[3] | tex_coords[2] |
    // ===========================================
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    Shader shader("vertex.glsl", "fragment.glsl");
    Texture texture("wall.jpg");

    struct InputFlags flags = {
        .rotate = true,
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis01(0.0f, 1.0f);

    float rand_axes[10];
    for (size_t i = 0; i < 10; ++i) {
        rand_axes[i] = dis01(gen);
    }

    // OR create LookAt view matrix with glm
    glm::mat4 view;
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),  // cam position
                       glm::vec3(0.0f, 0.0f, 0.0f),  // target position
                       glm::vec3(0.0f, 1.0f, 0.0f)); // look up position


    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window, &flags);

        // state-setting command
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // state-using command
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO_0);
        texture.bind();
        
        float timeValue = glfwGetTime();
        // fragment shader uniforms
        float greenValue = (sin(timeValue* 1.08)/ 4.0f) + 0.25f;
        float redValue = (sin(timeValue* 1.23)/ 4.0f) + 0.25f;
        float blueValue = (sin(timeValue* 1.35) / 4.0f) + 0.25f;
        shader.setFloat("greenValue", greenValue);
        shader.setFloat("redValue", redValue);
        shader.setFloat("blueValue", blueValue);

        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(-55.0f) * timeValue, glm::vec3(0.4f, rand_axes[i], 1.0f)); 
            model = glm::rotate(model, glm::radians(-55.0f) * timeValue, glm::vec3(rand_axes[i], 0.7f, 0.3f)); 
            float angle = 20.0f * i; 
            shader.setMat4("model", glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        shader.setMat4("view", glm::value_ptr(view));

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(fov), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        shader.setMat4("projection", glm::value_ptr(projection));

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // use EBO
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // check events and swap buffers
        glfwSwapBuffers(window);
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO_0);
    glDeleteBuffers( 1, &VBO);

    // clean resources
    glfwTerminate();

    return 0;
}
