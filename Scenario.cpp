#define STB_IMAGE_IMPLEMENTATION
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "camera.h"
#include "model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

unsigned int WIDTH = 800, HEIGHT = 600;
bool firstMouse = true;
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xoffIn, double yoffIn)
{
	float xoff = static_cast<float>(xoffIn);
	float yoff = static_cast<float>(yoffIn);
	if (firstMouse)
	{
		lastX = xoff;
		lastY = yoff;
		firstMouse = false;
	}

	float xoffset = xoff - lastX;
	float yoffset = lastY - yoff;
	lastX = xoff;
	lastY = yoff;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;


		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "ERROR::texture failed to load path:" << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}


unsigned int loadCubemap(vector<std::string>faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (int i = 0;i < 6;i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return textureID;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}
unsigned int quadVAO=0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
		 10.0f, -0.5f,  10.0f,  2.0f, 0.0f,
		-10.0f, -0.5f,  10.0f,  0.0f, 0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 2.0f,

		 10.0f, -0.5f,  10.0f,  2.0f, 0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 2.0f,
		 10.0f, -0.5f, -10.0f,  2.0f, 2.0f
		};

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glBindVertexArray(0); // 解绑 VAO
		glBindBuffer(GL_ARRAY_BUFFER, 0); // 解绑 VBO
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}


int main()
{

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Scenario", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "ERRROR::Failed to create a window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::Failed to initilize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	float skyboxVertices[] = {
		        -1.0f,  1.0f, -1.0f,
		        -1.0f, -1.0f, -1.0f,
		         1.0f, -1.0f, -1.0f,
		         1.0f, -1.0f, -1.0f,
		         1.0f,  1.0f, -1.0f,
		        -1.0f,  1.0f, -1.0f,
		
		        -1.0f, -1.0f,  1.0f,
		        -1.0f, -1.0f, -1.0f,
		        -1.0f,  1.0f, -1.0f,
		        -1.0f,  1.0f, -1.0f,
		        -1.0f,  1.0f,  1.0f,
		        -1.0f, -1.0f,  1.0f,
		
		         1.0f, -1.0f, -1.0f,
		         1.0f, -1.0f,  1.0f,
		         1.0f,  1.0f,  1.0f,
		         1.0f,  1.0f,  1.0f,
		         1.0f,  1.0f, -1.0f,
		         1.0f, -1.0f, -1.0f,
		
		        -1.0f, -1.0f,  1.0f,
		        -1.0f,  1.0f,  1.0f,
		         1.0f,  1.0f,  1.0f,
		         1.0f,  1.0f,  1.0f,
		         1.0f, -1.0f,  1.0f,
		        -1.0f, -1.0f,  1.0f,
		
		        -1.0f,  1.0f, -1.0f,
		         1.0f,  1.0f, -1.0f,
		         1.0f,  1.0f,  1.0f,
		         1.0f,  1.0f,  1.0f,
		        -1.0f,  1.0f,  1.0f,
		        -1.0f,  1.0f, -1.0f,
		
		        -1.0f, -1.0f, -1.0f,
		        -1.0f, -1.0f,  1.0f,
		         1.0f, -1.0f, -1.0f,
		         1.0f, -1.0f, -1.0f,
		        -1.0f, -1.0f,  1.0f,
		         1.0f, -1.0f,  1.0f
		    };

	unsigned int skyVBO, skyVAO;
	glGenVertexArrays(1, &skyVAO);
	glGenBuffers(1, &skyVBO);
	glBindVertexArray(skyVAO);
	glBindBuffer(GL_ARRAY_BUFFER,skyVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	Shader skyboxShader("E:\\vstudioproject\\OpenGLDemo\\OpenGLDemo\\vertexShaderSource_skybox.GLSL", "E:\\vstudioproject\\OpenGLDemo\\OpenGLDemo\\fragmentShaderSource_skybox.GLSL");

	Shader floorShader("E:\\vstudioproject\\OpenGLDemo\\OpenGLDemo\\vertexShaderSource_floor.GLSL","E:\\vstudioproject\\OpenGLDemo\\OpenGLDemo\\fragmentSjaderSource_floor.GLSL");


	vector<std::string>faces
    {
        "F:/OpenGLImage/skybox/right.jpg",
         "F:/OpenGLImage/skybox/left.jpg",
         "F:/OpenGLImage/skybox/top.jpg",
        
        "F:/OpenGLImage/skybox/bottom.jpg",
        "F:/OpenGLImage/skybox/front.jpg",
        "F:/OpenGLImage/skybox/back.jpg"
    };
	unsigned int cubemapTexture = loadCubemap(faces);
	unsigned int floorTexture = loadTexture("F:/OpenGLImage/metal.png");
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	floorShader.use();
	floorShader.setInt("floorTexture", 0);
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);


		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//trans data to skyboxshader;向着色器传送数据
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		floorShader.use();
		floorShader.setMat4("projection", projection);
		floorShader.setMat4("view", view);
		floorShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		renderQuad();

		

		glDepthFunc(GL_LEQUAL);

		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);

		glBindVertexArray(skyVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

}