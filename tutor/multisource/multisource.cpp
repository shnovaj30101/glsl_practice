#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Shader.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window); 
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float lastX = 400, lastY = 300;

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
float yaw = -90.0f, pitch = 0.0f, fov = 45.0f;
bool firstMouse=true;

glm::vec3 lightPos;
float LastStopTime;
float LastStopAngle;
bool TimeInitial = true;
bool LightPosRotate = true;

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

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader zprogram
    // ------------------------------------
    Shader objShader("color.vs", "color.fs");
    Shader lampShader("light.vs", "light.fs");

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

    unsigned int VBO, VAO, EBO; 

	glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
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
	// 加载并生成纹理
	int width, height, nrChannels;
	unsigned char *data = stbi_load("container2.png", &width, &height, &nrChannels, 0);
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

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width2, height2, nrChannels2;
	unsigned char *data2 = stbi_load("container2_specular.png", &width2, &height2, &nrChannels2, 0);
	if (data2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data2);
    
    glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    objShader.use(); // don't forget to activate/use the shader before setting uniforms!

    glm::mat4 rotate_model[]={
        glm::mat4(1.0f),
        glm::mat4(1.0f),
        glm::mat4(1.0f),
        glm::mat4(1.0f),
        glm::mat4(1.0f),
        glm::mat4(1.0f),
        glm::mat4(1.0f),
        glm::mat4(1.0f),
        glm::mat4(1.0f),
        glm::mat4(1.0f)
    };

    glm::mat4 proj;
    proj = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    glm::vec3 cube_pos[] = {
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

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (TimeInitial) {
            TimeInitial = false;
            LastStopTime = glfwGetTime();
            LastStopAngle = 0;
        }
        if (LightPosRotate) {
            lightPos.x = 4*sin(glfwGetTime()-LastStopTime+LastStopAngle)+2;
            lightPos.z = 4*cos(glfwGetTime()-LastStopTime+LastStopAngle)-4;
        } else {
            lightPos.x = 4*sin(LastStopAngle)+2;
            lightPos.z = 4*cos(LastStopAngle)-4;
        }

		glActiveTexture(GL_TEXTURE1); // 在绑定纹理之前先激活纹理单元
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE2); // 在绑定纹理之前先激活纹理单元
		glBindTexture(GL_TEXTURE_2D, texture2);


        objShader.use();
        objShader.setVec3("dir_light.direction", -0.2f, -1.0f, -0.3f);
        objShader.setVec3("point_light.position", lightPos);
        objShader.setVec3("spot_light.position", cameraPos);
        objShader.setVec3("spot_light.direction", cameraFront);
        objShader.setVec3("viewPos",cameraPos);

		// material 參數設定
		objShader.setInt("material.diffuse", 1);
        objShader.setInt("material.specular", 2);
        objShader.setFloat("material.shininess", 64.0f);

		// dir_light 參數設定
        objShader.setVec3("dir_light.ambient",  0.1f, 0.1f, 0.1f);
        objShader.setVec3("dir_light.diffuse",  0.4f, 0.4f, 0.4f); // 将光照调暗了一些以搭配场景
        objShader.setVec3("dir_light.specular", 1.0f, 1.0f, 1.0f); 

		// point_light 參數設定
        objShader.setVec3("point_light.ambient",  0.1f, 0.1f, 0.1f);
        objShader.setVec3("point_light.diffuse",  0.4f, 0.4f, 0.4f); // 将光照调暗了一些以搭配场景
        objShader.setVec3("point_light.specular", 1.0f, 1.0f, 1.0f); 
        objShader.setFloat("point_light.constant", 1.0f); 
        objShader.setFloat("point_light.linear", 0.09f); 
        objShader.setFloat("point_light.quadratic", 0.032f); 

		// point_light 參數設定
        objShader.setVec3("spot_light.ambient",  0.1f, 0.1f, 0.1f);
        objShader.setVec3("spot_light.diffuse",  0.4f, 0.4f, 0.4f); // 将光照调暗了一些以搭配场景
        objShader.setVec3("spot_light.specular", 1.0f, 1.0f, 1.0f); 
        objShader.setFloat("spot_light.innerCutOff", glm::cos(glm::radians(12.5f))); 
        objShader.setFloat("spot_light.outerCutOff", glm::cos(glm::radians(17.5f))); 

        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glm::mat4 proj;
		proj = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        objShader.setMat4("View", view);
        objShader.setMat4("Proj", proj);

        for (int i=1 ; i<10 ; i++) {
            // render container
            glm::mat4 trans_model;
            trans_model = glm::translate(trans_model,cube_pos[i]);
            rotate_model[i]= glm::rotate(rotate_model[i], (float)glm::radians(sin(glfwGetTime()))*(i+1), glm::vec3(1.0f, 0.0f, 0.0f));
            rotate_model[i]= glm::rotate(rotate_model[i], (float)glm::radians(sin(glfwGetTime()))*(i+1), glm::vec3(0.0f, 1.0f, 0.0f));
			objShader.setMat4("TransModel", trans_model);
			objShader.setMat4("RotateModel", rotate_model[i]);
            //objShader.setMat4("TransMat", trans);

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
		lampShader.use();
        lampShader.setMat4("Proj", proj);
        lampShader.setMat4("View", view);
		glm::mat4 trans_model;
		trans_model = glm::translate(trans_model,lightPos);
        lampShader.setMat4("Model", trans_model);

        glBindVertexArray(lightVAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

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
    float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        if (LightPosRotate) {
            LastStopAngle = glfwGetTime()-LastStopTime+LastStopAngle;
            LightPosRotate = false;
        } else {
            LightPosRotate = true;
            LastStopTime = glfwGetTime();
        }
    
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
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

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);

    //cameraFront = glm::rotate(glm::mat4(1.0f),(float)glm::radians(sensitivity*xoffset),cameraUp)*cameraFront;
    //cameraUp    = glm::rotate(glm::mat4(1.0f),(float)glm::radians(sensitivity*yoffset),cameraFront)*cameraUp;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  if(fov >= 1.0f && fov <= 45.0f)
    fov -= yoffset;
  if(fov <= 1.0f)
    fov = 1.0f;
  if(fov >= 45.0f)
    fov = 45.0f;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
