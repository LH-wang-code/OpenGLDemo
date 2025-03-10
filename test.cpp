//#define STB_IMAGE_IMPLEMENTATION
//#include<iostream>
//#include<glad/glad.h>
//#include<GLFW/glfw3.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include "shader.h"
//#include "camera.h"
//#include "model.h"
//
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//const int WIDTH = 800;
//const int HEIGHT = 600;
//bool firstMouse = true;
//float lastX = WIDTH / 2.0f;
//float lastY = HEIGHT / 2.0f;
//
//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
//
//
//float deltaTime = 0.0f;
//float lastFrame = 0.0f;
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//	glViewport(0, 0, width, height);
//}
//void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
//{
//	float xpos = static_cast<float>(xposIn);
//	float ypos = static_cast<float>(yposIn);
//
//	if (firstMouse)
//	{
//		lastX = xpos;
//		lastY = ypos;
//		firstMouse = false;
//	}
//	float xoffset = xpos - lastX;
//	float yoffset = lastY - ypos;
//	lastX = xpos;
//	lastY = ypos;
//	camera.ProcessMouseMovement(xoffset, yoffset);
//
//}
//void scroll_back(GLFWwindow* window, double xoffset, double yoffset)
//{
//	camera.ProcessMouseScroll(static_cast<float>(yoffset));
//}
//
//
//void processInput(GLFWwindow* window)
//{
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, true);
//	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//		camera.ProcessKeyboard(FORWARD, deltaTime);
//	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//		camera.ProcessKeyboard(BACKWARD, deltaTime);
//	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//		camera.ProcessKeyboard(LEFT, deltaTime);
//	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//		camera.ProcessKeyboard(RIGHT, deltaTime);
//
//}
//int main()
//{
//
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE;
//#endif
//	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Learn Assimp", nullptr, nullptr);
//	if (window == nullptr)
//	{
//		std::cout << "ERROR::Failed to create a window" << std::endl;
//		glfwTerminate();
//		return -1;
//
//	}
//	glfwMakeContextCurrent(window);
//	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//	glfwSetCursorPosCallback(window, mouse_callback);
//	glfwSetScrollCallback(window, scroll_back);
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//
//
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//	{
//		std::cout << "ERROR::Failed to initilize GLAD" << std::endl;
//		return -1;
//	}
//
//	stbi_set_flip_vertically_on_load(true);
//	glEnable(GL_DEPTH_TEST);
//	
//	
//	
//	Shader ourShader("E:\\vstudioproject\\LearnOpenGL\\LearnOpenGL\\vertexShaderSource_model.GLSL", "E:\\vstudioproject\\LearnOpenGL\\LearnOpenGL\\fragmentShaderSource_model.GLSL");
//	Shader lightShader("E:\\vstudioproject\\LearnOpenGL\\LearnOpenGL\\vertexShaderSource_AssimpLight.GLSL","E:\\vstudioproject\\LearnOpenGL\\LearnOpenGL\\fragmentShaderSource_AssimpLight.GLSL");
//	Model ourModel("F:/OpenGLImage/backpack/backpack.obj");
//
//	float vertices[] = {
//		// positions          // normals           // texture coords
//		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
//		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
//		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
//		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
//		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
//		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
//
//		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
//		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
//		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
//		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
//		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
//		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
//
//		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
//		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
//		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
//		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
//		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
//		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
//
//		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
//		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
//		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
//		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
//		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
//		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
//
//		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
//		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
//		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
//		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
//		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
//		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
//
//		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
//		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
//		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
//		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
//		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
//		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
//	};
//
//
//
//	glm::vec3 pointLightPositions[] = {
//		 glm::vec3(0.7f,  0.2f,  2.0f),
//		glm::vec3(2.3f, -3.3f, -4.0f)
//};
//
//
//	unsigned int lightCubeVAO,VBO;
//	glGenVertexArrays(1, &lightCubeVAO);
//	
//	glGenBuffers(1, &VBO);
//	glBindVertexArray(lightCubeVAO);
//	
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//
//	while (!glfwWindowShouldClose(window))
//	{
//		
//		float currentFrame = static_cast<float>(glfwGetTime());
//		deltaTime = currentFrame - lastFrame;
//		lastFrame = currentFrame;
//		pointLightPositions[0] = glm::vec3(4.0 * sin(currentFrame),4.0*cos(currentFrame),0.0f);
//		pointLightPositions[1] = glm::vec3(0.0f, 4.0 * sin(currentFrame), 4.0 * cos(currentFrame));
//		processInput(window);
//		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		ourShader.use();
//
//		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
//		glm::mat4 view = camera.GetViewMatrix();
//
//		ourShader.setMat4("projection", projection);
//		ourShader.setMat4("view", view);
//
//
//
//		glm::mat4 model = glm::mat4(1.0f);
//		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
//		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
//		ourShader.setMat4("model", model);
//
//
//		ourShader.setVec3("lights[0].position", pointLightPositions[0]);
//		ourShader.setVec3("lights[0].ambient", 0.05f, 0.05f, 0.05f);
//
//		ourShader.setVec3("lights[0].diffuse", 0.8f, 0.8f, 0.8f);
//		ourShader.setVec3("lights[0].specular", 1.0f, 1.0f, 1.0f);
//		ourShader.setFloat("lights[0].constant",1.0f);
//		ourShader.setFloat("lights[0].linear",0.09f);
//		ourShader.setFloat("lights[0].quadratic",0.032f);
//
//		ourShader.setVec3("lights[1].position", pointLightPositions[1]);
//		ourShader.setVec3("lights[1].ambient", 0.05f, 0.05f, 0.05f);
//
//		ourShader.setVec3("lights[1].diffuse", 0.8f, 0.8f, 0.8f);
//		ourShader.setVec3("lights[1].specular", 1.0f, 1.0f, 1.0f);
//		ourShader.setFloat("lights[1].constant", 1.0f);
//		ourShader.setFloat("lights[1].linear", 0.09f);
//		ourShader.setFloat("lights[1].quadratic", 0.032f);
//
//		ourModel.Draw(ourShader);
//
//
//
//		lightShader.use();
//		lightShader.setMat4("projection", projection);
//		lightShader.setMat4("view", view);
//		glBindVertexArray(lightCubeVAO);
//		for (int i = 0;i < 2;i++)
//		{
//			glm::mat4 model = glm::mat4(1.0f);
//			model = glm::translate(model,pointLightPositions[i]);
//			model = glm::scale(model, glm::vec3(0.2f));
//			lightShader.setMat4("model", model);
//			glDrawArrays(GL_TRIANGLES, 0, 36);
//
//		}
//
//		
//
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//
//
//	}
//	glfwTerminate();
//	return 0;
//}