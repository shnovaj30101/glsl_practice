#include<glad/glad.h> // configure the OpenGL function (find the function location)
#include<GLFW/glfw3.h> // provide the window API
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <vector>
#include <cmath>

#include "Shader.h"
#include "camera.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
float sphere_center_distance(float phi_deg, float theta_deg);
float sphere_gau_bias(float phi_deg, float theta_deg);
float mod(float a,float b);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
unsigned int theta_scale = 240;
unsigned int phi_scale = 480;
unsigned int pre_theta_scale = 0;
unsigned int pre_phi_scale = 0;
bool jKeyRelease = true;
bool kKeyRelease = true;
float radius = 2.0;
float gau_center_phi = 0.0;
float gau_center_theta = 90.0;
float gau_sigma = 0.3;
float gau_a = 1.0;
glm::vec3 lightPos;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool mouse_disable = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};


int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // assign version: 3.3 core

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
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

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("sphere.vs", "sphere.fs");
    Shader lightShader("light.vs", "light.fs");

    // set up the sphere vertext data
    vector<Vertex> sphere_vertices;
    vector<unsigned int> sphere_indices;
    float theta_deg, phi_deg, theta_unit, phi_unit, quadrangle_count=0;
    glm::vec3 tmp_vector1,tmp_vector2,tmp_vector3,tmp_vector4;
    glm::vec3 tmp_normal;
    Vertex tmp_vertex;
    

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // normal           // texture coords
         0.5f,  0.5f,-0.5f,   0.0f, 0.0f,-1.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f,-0.5f,   0.0f, 0.0f,-1.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f,-0.5f,   0.0f, 0.0f,-1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f,-0.5f,   0.0f, 0.0f,-1.0f,   0.0f, 1.0f,  // top left 

         0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // bottom right
         0.5f, -0.5f,-0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
         0.5f,  0.5f,-0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,  // top left 

        -0.5f,  0.5f, 0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
        -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f,-0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f,-0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,  // top left 

         0.5f,  0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f,  0.5f,-0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f,  0.5f,-0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // top left 

         0.5f, -0.5f, 0.5f,   0.0f,-1.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f,-0.5f,   0.0f,-1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f,-0.5f,   0.0f,-1.0f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f, -0.5f, 0.5f,   0.0f,-1.0f, 0.0f,   0.0f, 1.0f,  // top left 

         0.5f,  0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f   // top left 

    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
        0+4, 1+4, 3+4, // first triangle
        1+4, 2+4, 3+4, // second triangle
        0+8, 1+8, 3+8, // first triangle
        1+8, 2+8, 3+8, // second triangle
        0+12, 1+12, 3+12, // first triangle
        1+12, 2+12, 3+12, // second triangle
        0+16, 1+16, 3+16, // first triangle
        1+16, 2+16, 3+16, // second triangle
        0+20, 1+20, 3+20, // first triangle
        1+20, 2+20, 3+20 // second triangle
    };


    unsigned int lightVAO, lightVBO, lightEBO;
    glGenVertexArrays(1,&lightVAO);
    glGenBuffers(1,&lightVBO);
    glGenBuffers(1,&lightEBO);

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int texture,texture2,texture3;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    ourShader.use();
    glEnable(GL_DEPTH_TEST);

    glm::mat4 proj;
    glm::mat4 view;
    glm::mat4 model;
    model = glm::translate(model,glm::vec3(0,0,0));

    ourShader.setMat4("model",model);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        lightPos.x = 4*sin(glfwGetTime());
        lightPos.z = 4*cos(glfwGetTime());
        lightPos.y = 0;

        ourShader.use();

        view = camera.GetViewMatrix();

        proj = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH/(float)SCR_HEIGHT,0.1f,100.0f);

        ourShader.setMat4("view",view);
        ourShader.setMat4("proj",proj);

        ourShader.setVec3("point_light.position", lightPos);
        ourShader.setVec3("viewPos",camera.Position);

		// point_light 參數設定
        ourShader.setVec3("point_light.ambient",  0.1f, 0.1f, 0.1f);
        ourShader.setVec3("point_light.diffuse",  0.4f, 0.4f, 0.4f); // 将光照调暗了一些以搭配场景
        ourShader.setVec3("point_light.specular", 1.0f, 1.0f, 1.0f); 
        ourShader.setFloat("point_light.constant", 1.0f); 
        ourShader.setFloat("point_light.linear", 0.09f); 
        ourShader.setFloat("point_light.quadratic", 0.032f); 
        ourShader.setFloat("shininess", 64.0f);

		// dir_light 參數設定
        ourShader.setVec3("dir_light.direction",  -1.0f, -1.0f, -1.0f);
        ourShader.setVec3("dir_light.ambient",  0.1f, 0.1f, 0.1f);
        ourShader.setVec3("dir_light.diffuse",  0.4f, 0.4f, 0.4f); // 将光照调暗了一些以搭配场景
        ourShader.setVec3("dir_light.specular", 1.0f, 1.0f, 1.0f); 

		// dir_light2 參數設定
        ourShader.setVec3("dir_light2.direction",  1.0f, 1.0f, 1.0f);
        ourShader.setVec3("dir_light2.ambient",  0.1f, 0.1f, 0.1f);
        ourShader.setVec3("dir_light2.diffuse",  0.4f, 0.4f, 0.4f); // 将光照调暗了一些以搭配场景
        ourShader.setVec3("dir_light2.specular", 1.0f, 1.0f, 1.0f); 



        if (theta_scale != pre_theta_scale && phi_scale != pre_phi_scale) {
            pre_theta_scale = theta_scale;
            pre_phi_scale = phi_scale;

            sphere_vertices.clear();
            sphere_indices.clear();
            quadrangle_count=0;

            theta_unit =  180.0/theta_scale;
            phi_unit =  360.0/phi_scale;
            for (int t=1 ; t<theta_scale-1 ; t++) {
                theta_deg = t * theta_unit;
                for (int p=0 ; p<phi_scale ; p++) {
                    float gau_bias;
                    phi_deg = p * phi_unit;
                    gau_bias = sphere_gau_bias(phi_deg,theta_deg);
                    tmp_vector1.x = (radius+gau_bias)*sin(glm::radians(theta_deg))*cos(glm::radians(phi_deg));
                    tmp_vector1.y = (radius+gau_bias)*sin(glm::radians(theta_deg))*sin(glm::radians(phi_deg));
                    tmp_vector1.z = (radius+gau_bias)*cos(glm::radians(theta_deg));

                    gau_bias = sphere_gau_bias(phi_deg+phi_unit,theta_deg);
                    tmp_vector2.x = (radius+gau_bias)*sin(glm::radians(theta_deg))*cos(glm::radians(phi_deg+phi_unit));
                    tmp_vector2.y = (radius+gau_bias)*sin(glm::radians(theta_deg))*sin(glm::radians(phi_deg+phi_unit));
                    tmp_vector2.z = (radius+gau_bias)*cos(glm::radians(theta_deg));

                    gau_bias = sphere_gau_bias(phi_deg+phi_unit,theta_deg+theta_unit);
                    tmp_vector3.x = (radius+gau_bias)*sin(glm::radians(theta_deg+theta_unit))*cos(glm::radians(phi_deg+phi_unit));
                    tmp_vector3.y = (radius+gau_bias)*sin(glm::radians(theta_deg+theta_unit))*sin(glm::radians(phi_deg+phi_unit));
                    tmp_vector3.z = (radius+gau_bias)*cos(glm::radians(theta_deg+theta_unit));

                    gau_bias = sphere_gau_bias(phi_deg,theta_deg+theta_unit);
                    tmp_vector4.x = (radius+gau_bias)*sin(glm::radians(theta_deg+theta_unit))*cos(glm::radians(phi_deg));
                    tmp_vector4.y = (radius+gau_bias)*sin(glm::radians(theta_deg+theta_unit))*sin(glm::radians(phi_deg));
                    tmp_vector4.z = (radius+gau_bias)*cos(glm::radians(theta_deg+theta_unit));

                    tmp_normal = glm::normalize(glm::cross(tmp_vector3-tmp_vector4,tmp_vector1-tmp_vector4));

                    tmp_vertex.position = tmp_vector1;
                    tmp_vertex.normal = tmp_normal;
                    sphere_vertices.push_back(tmp_vertex); // left up
                    tmp_vertex.position = tmp_vector2;
                    tmp_vertex.normal = tmp_normal;
                    sphere_vertices.push_back(tmp_vertex); // right up
                    tmp_vertex.position = tmp_vector3;
                    tmp_vertex.normal = tmp_normal;
                    sphere_vertices.push_back(tmp_vertex); // right down
                    tmp_vertex.position = tmp_vector4;
                    tmp_vertex.normal = tmp_normal;
                    sphere_vertices.push_back(tmp_vertex); // left down

                    sphere_indices.push_back(0+4*quadrangle_count);
                    sphere_indices.push_back(1+4*quadrangle_count);
                    sphere_indices.push_back(3+4*quadrangle_count);
                    sphere_indices.push_back(1+4*quadrangle_count);
                    sphere_indices.push_back(2+4*quadrangle_count);
                    sphere_indices.push_back(3+4*quadrangle_count);
                    quadrangle_count++;
                }
            }

            printf("%d\n",phi_scale);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*sphere_vertices.size(),&sphere_vertices[0],GL_DYNAMIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*sphere_indices.size(),&sphere_indices[0],GL_DYNAMIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);

        }
        
        glBindVertexArray(VAO);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, sphere_indices.size(),GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        // setting light box
        // ----------------------------------

        lightShader.use();

        glm::mat4 lightModel;
        lightModel = glm::translate(lightModel,lightPos);


        lightShader.setMat4("Model",lightModel);
        lightShader.setMat4("View",view);
        lightShader.setMat4("Proj",proj);

        glBindVertexArray(lightVAO);
        glDrawElements(GL_TRIANGLES, sphere_indices.size(),GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // swap buffer and pollevent
        // ----------------------------------
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);


    glfwTerminate();
    return 0;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
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
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && jKeyRelease) {
        //jKeyRelease = false;
        if (theta_scale>3) {
            theta_scale -= 1;
            phi_scale -= 2;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && kKeyRelease) {
        //kKeyRelease = false;
        if (theta_scale<240) {
            theta_scale += 1;
            phi_scale += 2;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_RELEASE) {
        jKeyRelease = true;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE) {
        kKeyRelease = true;
    }
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

float sphere_center_distance(float phi_deg, float theta_deg) {
    return sqrt(pow(min(abs(mod(phi_deg,360.0)-mod(gau_center_phi,360.0)),360-abs(mod(phi_deg,360.0)-mod(gau_center_phi,360.0))),2.0)+pow(theta_deg-gau_center_theta,2.0))/20.0;
}
float sphere_gau_bias(float phi_deg, float theta_deg) {
    return gau_a*exp(-pow(sphere_center_distance(phi_deg,theta_deg),2.0)/(2.0*pow(gau_sigma,2.0)));
}
float mod(float a, float b) {
    int c = a/b;
    return a-b*c;
}
