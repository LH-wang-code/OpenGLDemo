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

//Model Mountain("F:\\OpenGLImage\\shanshi\\shanshi\\ShanShi.obj");
Camera camera(glm::vec3(0.0f, 0.0f, 100.0f));

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

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}


unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			 // bottom face
			 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			  1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 // top face
			 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			  1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			  1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
unsigned int floorVAO;


void renderScene(Shader &shader)
{
	glm::mat4 model = glm::mat4(1.0f);
	shader.setMat4("model", model);
	glBindVertexArray(floorVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	

	//model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
	//model = glm::scale(model, glm::vec3(0.5f));
	//shader.setMat4("model", model);
	//renderCube();
	//model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
	//model = glm::scale(model, glm::vec3(0.5f));
	//shader.setMat4("model", model);
	//renderCube();
	//model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
	//model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	//model = glm::scale(model, glm::vec3(0.25));
	//shader.setMat4("model", model);
	//renderCube();
}

unsigned wallVAO=0, wallVBO = 0;
void renderWall()
{
	float vertices[] = {
		// 位置              // 法线           // 纹理坐标
		// 前面 (2个三角形)
		-5.0f, -5.0f,  5.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
		 5.0f, -5.0f,  5.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
		 5.0f,  5.0f,  5.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,

		 5.0f,  5.0f,  5.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		-5.0f,  5.0f,  5.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
		-5.0f, -5.0f,  5.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

		// 后面 (2个三角形)
		-5.0f, -5.0f, -5.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 5.0f, -5.0f, -5.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 5.0f,  5.0f, -5.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

		 5.0f,  5.0f, -5.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-5.0f,  5.0f, -5.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-5.0f, -5.0f, -5.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,

		// 左面 (2个三角形)
		-5.0f, -5.0f, -5.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-5.0f, -5.0f,  5.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-5.0f,  5.0f,  5.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,

		-5.0f,  5.0f,  5.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-5.0f,  5.0f, -5.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-5.0f, -5.0f, -5.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

		// 右面 (2个三角形)
		 5.0f, -5.0f,  5.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 5.0f, -5.0f, -5.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 5.0f,  5.0f, -5.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,

		 5.0f,  5.0f, -5.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 5.0f,  5.0f,  5.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 5.0f, -5.0f,  5.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

		 // 顶面 (2个三角形)
		 -5.0f,  5.0f,  5.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		  5.0f,  5.0f,  5.0f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		  5.0f,  5.0f, -5.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,

		  5.0f,  5.0f, -5.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 -5.0f,  5.0f, -5.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		 -5.0f,  5.0f,  5.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
		glGenVertexArrays(1, &wallVAO);
		glGenBuffers(1, &wallVBO);
	
		glBindBuffer(GL_ARRAY_BUFFER, wallVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindVertexArray(wallVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	
	// render Cube
	glBindVertexArray(wallVAO);
	glDrawArrays(GL_TRIANGLES, 0, 30);
	glBindVertexArray(0);
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

int main()
{
	glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Scenario", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "ERRROR::Failed to create a window" << std::endl;
		glfwTerminate();
		return -1;
	}
//
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
//
//
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::Failed to initilize GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);



	Shader shader("E:\\vstudioproject\\OpenGLDemo\\OpenGLDemo\\vertexShaderSource_shadow.GLSL", "E:\\vstudioproject\\OpenGLDemo\\OpenGLDemo\\fragmentShaderSource_shadow.GLSL");
	Shader simpleDepthShader("E:\\vstudioproject\\OpenGLDemo\\OpenGLDemo\\vertexShaderSource_depthmap.GLSL", "E:\\vstudioproject\\OpenGLDemo\\OpenGLDemo\\fragmentShadow_depthmap.GLSL");
	Shader debugDepthQuad("E:\\vstudioproject\\OpenGLDemo\\OpenGLDemo\\vertexShaderSource_shadowmap.GLSL", "E:\\vstudioproject\\OpenGLDemo\\OpenGLDemo\\fragmentShaderSource_shadowmap.GLSL");


	Shader skyboxShader("E:\\vstudioproject\\OpenGLDemo\\OpenGLDemo\\vertexShaderSource_skybox.GLSL", "E:\\vstudioproject\\OpenGLDemo\\OpenGLDemo\\fragmentShaderSource_skybox.GLSL");
	Shader TreeShader("E:\\vstudioproject\\OpenGLDemo\\OpenGLDemo\\vertexShaderSource_tree.GLSL", "E:\\vstudioproject\\OpenGLDemo\\OpenGLDemo\\fragmentShaderSource_tree.GLSL");
	Shader wallShader("E:\\vstudioproject\\OpenGLDemo\\OpenGLDemo\\vertexShaderSource_wall.GLSL", "E:\\vstudioproject\\OpenGLDemo\\OpenGLDemo\\fragmentShaderSource_wall.GLSL");
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
//
	unsigned int skyVBO, skyVAO;
	glGenVertexArrays(1, &skyVAO);
	glGenBuffers(1, &skyVBO);
	glBindVertexArray(skyVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	float floorVertices[] = {
		// positions            // normals         // texcoords
		 100.0f, -0.5f,  100.0f,  0.0f, 1.0f, 0.0f,  100.0f,  0.0f,
		-100.0f, -0.5f,  100.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-100.0f, -0.5f, -100.0f,  0.0f, 1.0f, 0.0f,   0.0f, 100.0f,

		 100.0f, -0.5f,  100.0f,  0.0f, 1.0f, 0.0f,  100.0f,  0.0f,
		-100.0f, -0.5f, -100.0f,  0.0f, 1.0f, 0.0f,   0.0f, 100.0f,
		 100.0f, -0.5f, -100.0f,  0.0f, 1.0f, 0.0f,  100.0f, 100.0f
	};
	vector<std::string>faces
	{
		"F:\\OpenGLImage\\skybox\\right.jpg",
		 "F:\\OpenGLImage\\skybox\\left.jpg",
		 "F:\\OpenGLImage\\skybox\\top.jpg",

		"F:\\OpenGLImage\\skybox\\bottom.jpg",
		"F:\\OpenGLImage\\skybox\\front.jpg",
		"F:\\OpenGLImage\\skybox\\back.jpg"
	};
	unsigned int cubemapTexture = loadCubemap(faces);
	unsigned int floorVBO;
	glGenVertexArrays(1, &floorVAO);
	glGenBuffers(1, &floorVBO);
	glBindVertexArray(floorVAO);
	glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);
	Model models("F:\\OpenGLImage\\shanshi\\shanshi\\ShanShi.obj");

	Model rock("F:\\OpenGLImage\\rock\\rock.obj");
	Model tree("F:\\OpenGLImage\\tree\\tree.obj");
	unsigned int floorTexture = loadTexture("F:\\OpenGLImage\\grass.jpg");
	unsigned int testTexture = loadTexture("F:\\OpenGLImage\\metal.png");
	//墙，实现法线贴图，视差贴图的应用
	unsigned int walldiffuseMap = loadTexture("F:/OpenGLImage/bricks2.jpg");
    unsigned int wallnormalMap = loadTexture("F:/OpenGLImage/bricks2_normal.jpg");
    unsigned int walldispMap = loadTexture("F:/OpenGLImage/bricks2_disp.jpg");
	
	
	//实例化树模型
	unsigned int amount = 10;
	glm::mat4* treeMatrices = new glm::mat4[amount];
	srand(static_cast<unsigned int>(glfwGetTime()));
	float radius = 100.0f;
	float offset = 15.0f;
	for (unsigned int i = 0;i < amount;i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		float y = 0.0f;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));
		float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
		model = glm::scale(model, glm::vec3(scale));
		treeMatrices[i] = model;
	}
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &treeMatrices[0], GL_STATIC_DRAW);
	for (unsigned int i = 0; i < tree.meshes.size(); i++)
	{
		unsigned int VAO = tree.meshes[i].VAO;
		glBindVertexArray(VAO);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}

	//帧缓冲
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);
	shader.use();
	shader.setInt("diffuseTexture", 0);
	shader.setInt("shadowMap", 1);
	debugDepthQuad.use();
	debugDepthQuad.setInt("depthMap", 0);

	wallShader.use();
	wallShader.setInt("diffuseMap", 0);
	wallShader.setInt("normalMap", 0);
	wallShader.setInt("dispMap", 0);

	glm::vec3 lightPos(0.0f, 100.0f, 100.0f);
	//glm::vec3 lightPos(0.5f, 1.0f, 0.3f);
	//std::cout << "walldiffuseMap ID: " << walldiffuseMap << std::endl;
	//std::cout << "wallnormalMap ID: " << wallnormalMap << std::endl;
	//std::cout << "walldispMap ID: " << walldispMap << std::endl;
	while (!glfwWindowShouldClose(window))
	{
		bool isIstanced = false;
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//画深度贴图
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 0.1f, far_plane = 200.0f;
		lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 200.0f);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		simpleDepthShader.use();
		simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//draw the floor
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		renderScene(simpleDepthShader);
		//moutain
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		simpleDepthShader.setMat4("model", model);
		models.Draw(simpleDepthShader);
		//isIstanced = true;//这里最后画的的实例化数组
		//simpleDepthShader.setBool("isIstanced", isIstanced);
		//
		//draw the wall
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 4.5f, 80.0f));
		simpleDepthShader.setMat4("model", model);
		renderWall();



		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//画完之后重置窗口
		isIstanced = false;
		glViewport(0, 0, WIDTH, HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();
		//光空间矩阵
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setVec3("viewPos", camera.Position);
		shader.setVec3("lightPos", lightPos);
		shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		//moutain
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		shader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, testTexture);

		models.Draw(shader);
		//floor
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		renderScene(shader);
		

		//wall
		wallShader.use();
		wallShader.setVec3("lightPos", lightPos);
		wallShader.setVec3("viewPos", camera.Position);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 4.5f, 80.0f));
		wallShader.setMat4("model", model);
		wallShader.setMat4("projection", projection);
		wallShader.setMat4("view", view);
		wallShader.setFloat("height_scale",0.1f);
		wallShader.setBool("parallax", true);
		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, walldiffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, wallnormalMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, walldispMap);
		renderWall();

		// 
		//TreeShader.use();
		//TreeShader.setMat4("projection", projection);
		//TreeShader.setMat4("view", view);

		//TreeShader.use();
		//TreeShader.setInt("texture_diffuse1",0);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, tree.textures_loaded[0].id);
		//for (unsigned int i = 0; i < tree.meshes.size(); i++)
		//{
		//	glBindVertexArray(tree.meshes[i].VAO);
		//	glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(tree.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, amount);
		//	glBindVertexArray(0);
		//}
		//isIstanced = true;
		//shader.setBool("isIstanced", isIstanced);
		//shader.setMat4("model", glm::mat4(1.0f));

		//if (!tree.textures_loaded.empty()) {
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, tree.textures_loaded[0].id);
		//	shader.setInt("texture_diffuse1", 0);
		//}

		// 确保实例化矩阵缓冲区绑定
	/*	glBindBuffer(GL_ARRAY_BUFFER, buffer);*/

		// 渲染所有网格
		//for (unsigned int i = 0; i < tree.meshes.size(); i++) {
		//	glBindVertexArray(tree.meshes[i].VAO);
		//	glDrawElementsInstanced(
		//		GL_TRIANGLES,
		//		static_cast<unsigned int>(tree.meshes[i].indices.size()),
		//		GL_UNSIGNED_INT,
		//		0,
		//		amount
		//	);
		//	glBindVertexArray(0);
		//}
		//画天空盒
		glDepthFunc(GL_LEQUAL);
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); 
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		glBindVertexArray(skyVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);
		debugDepthQuad.use();
		debugDepthQuad.setFloat("near_plane", near_plane);
		debugDepthQuad.setFloat("far_plane", far_plane);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		//renderQuad();


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &floorVAO);
	glDeleteBuffers(1, &floorVBO);
	glfwTerminate();
	return 0;
}