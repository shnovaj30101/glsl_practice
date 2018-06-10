#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "camera.h"
#include "model.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 5.0f, 8.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool mouse_disable = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("model.vs", "model.fs");
    Shader lightShader("light.vs", "light.fs");
    
    // creat a cube light source
    // -------------------------
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

    glm::vec3 lightPos, lightPos2;

    unsigned int lightVAO,lightVBO,lightEBO;
	glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1,&lightVBO);
    glGenBuffers(1,&lightEBO);
	glBindVertexArray(lightVAO);
	// 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


    // load models
    // -----------
    Model ourModel("./nanosuit/nanosuit.obj");

    
    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        lightPos = glm::vec3(5*cos(glfwGetTime()),7.0f,5*sin(glfwGetTime()));
        lightPos2 = glm::vec3(5*(cos(glfwGetTime()-glm::radians(180.0))),7.0f,5*(sin(glfwGetTime()-glm::radians(180.0))));

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        ourShader.setVec3("point_light.position", lightPos);
        ourShader.setFloat("point_light.constant", 1.0f);
        ourShader.setFloat("point_light.linear", 0.05f);
        ourShader.setFloat("point_light.quadratic", 0.018f);
        ourShader.setFloat("point_light.ambient", 0.1f);
        ourShader.setFloat("point_light.diffuse", 0.4f);
        ourShader.setFloat("point_light.specular", 1.0f);
        ourShader.setVec3("point_light2.position", lightPos2);
        ourShader.setFloat("point_light2.constant", 1.0f);
        ourShader.setFloat("point_light2.linear", 0.05f);
        ourShader.setFloat("point_light2.quadratic", 0.018f);
        ourShader.setFloat("point_light2.ambient", 0.1f);
        ourShader.setFloat("point_light2.diffuse", 0.4f);
        ourShader.setFloat("point_light2.specular", 1.0f);
        
        ourShader.setVec3("ViewPos",camera.Position);
        ourShader.setFloat("shininess", 32.0f);


        // render the loaded model
        glm::mat4 model;
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down
        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);

        // draw the lightsource cube
        lightShader.use();
        glm::mat4 lightModel,lightModel2;
        lightModel = glm::scale(lightModel, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down
        lightModel = glm::translate(lightModel, lightPos); // translate it down so it's at the center of the scene
        lightShader.setMat4("Model",lightModel);
        lightShader.setMat4("View",view);
        lightShader.setMat4("Proj",projection);
        
        glBindVertexArray(lightVAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        lightModel2 = glm::scale(lightModel2, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down
        lightModel2 = glm::translate(lightModel2, lightPos2); // translate it down so it's at the center of the scene
        lightShader.setMat4("Model",lightModel2);
        lightShader.setMat4("View",view);
        lightShader.setMat4("Proj",projection);
        
        glBindVertexArray(lightVAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
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
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        if (mouse_disable) {
            mouse_disable=false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            mouse_disable=true;
        }
    }
    
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
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
