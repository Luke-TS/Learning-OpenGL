// function pointer loading automation
#include "glad/gl.h"

// window & input manager for opengl context
#include <GLFW/glfw3.h>

// matrix library for OpenGL
#include <bits/types/struct_timeval.h>
#include <cstdlib>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <iostream>
#include <random>

#include "material.h"
#include "light.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 1200;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// rotation flag
bool rotate = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
float cube_vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  -5.0f, -15.0f), 
    glm::vec3(-1.5f, 2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, 0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, 2.0f, -2.5f),  
    glm::vec3( 1.5f,  -2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

glm::vec3 ambient = glm::vec3(0.24725f, 0.1995f,	0.0745f);
glm::vec3 diffuse = glm::vec3(0.75164f, 0.60648f, 0.22648);
glm::vec3 specular = glm::vec3(0.628281f, 0.555802f, 0.366065f);

Material cube_material{ambient, diffuse, specular, 0.4f};
Light light{glm::vec3(0.2f, 0.2f, 0.2f),
                     glm::vec3(0.5f, 0.5f, 0.5f),
                     glm::vec3(1.0f,1.0f,1.0f)};

int main(void)
{
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
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // mouse input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);  

    // keyboard input
    glfwSetKeyCallback(window, key_callback);

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    // vertex layout
    // ===========================================
    // | vertices[3] | normal[3] | textures[2] |
    // ===========================================
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // no need to re-copy
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // handles object positioning and lighting
    Shader obj_shader("vertex.glsl", "obj_fragment.glsl");

    // visualize light source
    Shader light_shader("vertex.glsl", "light_fragment.glsl");

    Texture diffuse_tex{"container2.jpg"};
    Texture specular_tex{"container2_specular.jpg"};
    Texture emissive_tex{"matrix.jpg"};
    glActiveTexture(GL_TEXTURE0);
    diffuse_tex.bind();
    glActiveTexture(GL_TEXTURE1);
    specular_tex.bind();
    glActiveTexture(GL_TEXTURE2);
    emissive_tex.bind();

    float cube_rotation_theta = 0.0f;
    glm::vec3 cube_axes[10];
    for (auto& ax : cube_axes) {
        float x = 2.0f * rand() / (float)RAND_MAX;
        float y = 2.0f * rand() / (float)RAND_MAX;
        float z = 2.0f * rand() / (float)RAND_MAX;

        ax = glm::vec3(x,y,z);
    }

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        processInput(window);

        // state-setting command
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float timeValue = glfwGetTime();

        auto lightPos = glm::vec3(2.0, 2.0f, -0.5f);
        auto lightColor = glm::vec3(1.0f);

        light.ambient = lightColor * 0.2f; 
        light.diffuse = lightColor * 0.6f; 
        light.specular =  lightColor; 

        obj_shader.use();
        glBindVertexArray(VAO_0);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // vertex shader uniforms
        obj_shader.setMat4("view", glm::value_ptr(view));
        obj_shader.setMat4("projection", glm::value_ptr(projection));

        // fragment shader uniforms
        obj_shader.setVec3("viewPos", glm::value_ptr(camera.Position));
        obj_shader.setInt("material.diffuse_tex", 0);
        obj_shader.setInt("material.specular_tex", 1);
        obj_shader.setInt("material.emissive_tex", 2);
        obj_shader.setFloat("material.shininess", 0.25f);
        light.use(obj_shader);
        obj_shader.setVec3("light.position", glm::value_ptr(camera.Position));
        obj_shader.setVec3("light.direction", glm::value_ptr(camera.Front));
        obj_shader.setFloat("light.cutoff", glm::cos(glm::radians(12.5f)));
        obj_shader.setFloat("light.outerCutoff", glm::cos(glm::radians(17.5f)));
        obj_shader.setFloat("light.constant", 1.0f);
        obj_shader.setFloat("light.linear", 0.09f);
        obj_shader.setFloat("light.quadratic", 0.032f);

        if (rotate) // update rotation theta
            cube_rotation_theta += deltaTime;

        for (int i = 0; i < 10; ++i) {

            // vertex shader transformation matrices
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, cube_rotation_theta, glm::normalize(cube_axes[i]));
            // vertex uniform
            obj_shader.setMat4("model", glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // render lights
        light_shader.use();
        glBindVertexArray(lightVAO);

        // different model matrix
        glm::mat4 light_model = glm::mat4(1.0f);
        light_model = glm::translate(light_model, lightPos);
        light_model = glm::scale(light_model, glm::vec3(0.5f, 0.5f, 0.5f));

        // vertex shader uniforms
        light_shader.setMat4("model", glm::value_ptr(light_model));
        light_shader.setMat4("view", glm::value_ptr(view));
        light_shader.setMat4("projection", glm::value_ptr(projection));

        // light fragment shader
        light_shader.setVec3("lightColor", glm::value_ptr(lightColor));

        glDrawArrays(GL_TRIANGLES, 0, 36);

        // check events and swap buffers
        glfwSwapBuffers(window);

        // update timing data
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  
    }
    glDeleteVertexArrays(1, &VAO_0);
    glDeleteBuffers( 1, &VBO);

    // clean resources
    glfwTerminate();

    return 0;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow *window) 
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
        rotate = !rotate;
}
