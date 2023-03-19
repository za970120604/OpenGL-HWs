#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Object.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void loadMaterialLight();
unsigned int createShader(const char* filename, const char* type);
unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader);
void loadTexture(unsigned int& texture, const char* tFileName);
void DrawModel(glm::mat4 M, glm::mat4 V, glm::mat4 P, unsigned int program , unsigned int VAO , unsigned int texture);
glm::mat4 getPerspective();
glm::mat4 getView();

Object* catModel = new Object("obj/cat_l25.obj");

Material material;
Light light;
glm::vec3 cameraPos = glm::vec3(0, 3.5, 3.5);

int windowWidth = 800, windowHeight = 600;

int Effect_num = 1; 

void DrawModel(glm::mat4 M, glm::mat4 V, glm::mat4 P, unsigned int program , unsigned int VAO , unsigned int texture) {
	glUseProgram(program);

	unsigned int Modelloc, Viewloc, Perspectiveloc;
	Modelloc = glGetUniformLocation(program, "Model");
	Viewloc = glGetUniformLocation(program, "View");
	Perspectiveloc = glGetUniformLocation(program, "Perspective");
	glUniformMatrix4fv(Modelloc, 1, GL_FALSE, glm::value_ptr(M));
	glUniformMatrix4fv(Viewloc, 1, GL_FALSE, glm::value_ptr(V));
	glUniformMatrix4fv(Perspectiveloc, 1, GL_FALSE, glm::value_ptr(P));
	glUniform1i(glGetUniformLocation(program, "Texture"), 0);

	if (Effect_num == 1 | Effect_num == 2) {
		unsigned int mambientloc, mdiffuseloc, mspecularloc, mglossloc;
		mambientloc = glGetUniformLocation(program, "ambient_m");
		mdiffuseloc = glGetUniformLocation(program, "diffuse_m");
		mspecularloc = glGetUniformLocation(program, "specular_m");
		mglossloc = glGetUniformLocation(program, "gloss");
		glUniform3fv(mambientloc, 1 , glm::value_ptr(material.ambient));
		glUniform3fv(mdiffuseloc, 1 , glm::value_ptr(material.diffuse));
		glUniform3fv(mspecularloc, 1, glm::value_ptr(material.specular));
		glUniform1f(mglossloc , material.gloss);

		unsigned int lambientloc, ldiffuseloc, lspecularloc, lposloc;
		lambientloc = glGetUniformLocation(program, "ambient_l");
		ldiffuseloc = glGetUniformLocation(program, "diffuse_l");
		lspecularloc = glGetUniformLocation(program, "specular_l");
		lposloc = glGetUniformLocation(program, "light_pos");
		glUniform3fv(lambientloc, 1, glm::value_ptr(light.ambient));
		glUniform3fv(ldiffuseloc, 1, glm::value_ptr(light.diffuse));
		glUniform3fv(lspecularloc, 1, glm::value_ptr(light.specular));
		glUniform3fv(lposloc, 1, glm::value_ptr(light.position));

		unsigned int cameraloc;
		cameraloc = glGetUniformLocation(program, "camera_pos");
		glUniform3fv(cameraloc, 1, glm::value_ptr(cameraPos));
	}
	else if (Effect_num == 3) {
		unsigned int lposloc, mdiffuseloc;
		lposloc = glGetUniformLocation(program, "light_pos");
		glUniform3fv(lposloc, 1, glm::value_ptr(light.position));
		mdiffuseloc = glGetUniformLocation(program, "diffuse_m");
		glUniform3fv(mdiffuseloc, 1, glm::value_ptr(material.diffuse));
	}
	else {
		unsigned int cameraloc;
		cameraloc = glGetUniformLocation(program, "camera_pos");
		glUniform3fv(cameraloc, 1, glm::value_ptr(cameraPos));
	}


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, catModel->positions.size());
	glBindVertexArray(0);

	glUseProgram(0);
}
int main()
{
	// Initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFE_OPENGL_FORWARD_COMPACT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "HW3", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// TODO:
	// Create shaders
	unsigned int vertexShader_Phong, fragmentShader_Phong;
	unsigned int shaderProgram_Phong;
	vertexShader_Phong = createShader("shaders/Phong.vert", "vert");
	fragmentShader_Phong = createShader("shaders/Phong.frag", "frag");
	shaderProgram_Phong = createProgram(vertexShader_Phong, fragmentShader_Phong);

	unsigned int vertexShader_Gouraud, fragmentShader_Gouraud;
	unsigned int shaderProgram_Gouraud;
	vertexShader_Gouraud = createShader("shaders/Gouraud.vert", "vert");
	fragmentShader_Gouraud = createShader("shaders/Gouraud.frag", "frag");
	shaderProgram_Gouraud = createProgram(vertexShader_Gouraud, fragmentShader_Gouraud);

	unsigned int vertexShader_Toon, fragmentShader_Toon;
	unsigned int shaderProgram_Toon;
	vertexShader_Toon = createShader("shaders/Toon.vert", "vert");
	fragmentShader_Toon = createShader("shaders/Toon.frag", "frag");
	shaderProgram_Toon = createProgram(vertexShader_Toon, fragmentShader_Toon);

	unsigned int vertexShader_Edge, fragmentShader_Edge;
	unsigned int shaderProgram_Edge;
	vertexShader_Edge = createShader("shaders/Edge.vert", "vert");
	fragmentShader_Edge = createShader("shaders/Edge.frag", "frag");
	shaderProgram_Edge = createProgram(vertexShader_Edge, fragmentShader_Edge);


	// Texture
	unsigned int catTexture;
	loadTexture(catTexture, "obj/Cat_diffuse.jpg");

	// VAO, VBO
	unsigned int VAO, VBO[3];
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(3, VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (catModel->positions.size()), &(catModel->positions[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (catModel->normals.size()), &(catModel->normals[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (catModel->texcoords.size()), &(catModel->texcoords[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, 0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// Display loop
	loadMaterialLight();
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.4f, 0.2f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// TODO:
		// Draw the cat with current active shader
		glm::mat4 M = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(0, 1, 0));
		glm::mat4 V = getView(); 
		glm::mat4 P = getPerspective();
		if (Effect_num == 1) {
			DrawModel(M, V, P, shaderProgram_Phong , VAO, catTexture);
		}
		else if (Effect_num == 2) {
			DrawModel(M, V, P, shaderProgram_Gouraud, VAO, catTexture);
		}
		else if (Effect_num == 3) {
			DrawModel(M, V, P, shaderProgram_Toon, VAO, catTexture);
		}
		else {
			DrawModel(M, V, P, shaderProgram_Edge, VAO, catTexture);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// TODO:
// Add key callback to switch between shaders
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		Effect_num = 1; 
	}
		
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		Effect_num = 2;
	}
		
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		Effect_num = 3;
	}
		
	if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
		Effect_num = 4; 
	}
		
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
}

unsigned int createShader(const char* filename, const char* type)
{
	unsigned int shader;
	if (strcmp(type, "vert") == 0)
		shader = glCreateShader(GL_VERTEX_SHADER);
	else if (strcmp(type, "frag") == 0)
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	else
	{
		cout << "Unknown Shader Type.\n";
		return 0;
	}

	FILE* fp = fopen(filename, "rb");
	fseek(fp, 0, SEEK_END);
	long fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);  //same as rewind(fp);

	char* source = (char*)malloc(sizeof(char) * (fsize + 1));
	fread(source, fsize, 1, fp);
	fclose(fp);

	source[fsize] = 0;

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::" << filename << "::COMPLIATION_FAILED\n" << infoLog << endl;
		return -1;
	}

	return shader;
}

unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
	unsigned int program = glCreateProgram();

	//Attach our shaders to our program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	//Link our program
	glLinkProgram(program);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	int success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		int maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		char* infoLog = (char*)malloc(sizeof(char) * (maxLength));
		glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);

		//We don't need the program anymore.
		glDeleteProgram(program);
		//Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		puts(infoLog);
		free(infoLog);

		return -1;
	}

	//Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	return program;
}

void loadMaterialLight() {
	material.ambient = glm::vec3(1.0, 1.0, 1.0);
	material.diffuse = glm::vec3(1.0, 1.0, 1.0);
	material.specular = glm::vec3(0.7, 0.7, 0.7);
	material.gloss = 10.5;

	light.ambient = glm::vec3(0.2, 0.2, 0.2);
	light.diffuse = glm::vec3(0.8, 0.8, 0.8);
	light.specular = glm::vec3(0.5, 0.5, 0.5);
	light.position = glm::vec3(10, 10, 10);
}

void loadTexture(unsigned int& texture, const char* tFileName) {
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(tFileName, &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

glm::mat4 getPerspective()
{
	return glm::perspective(
		glm::radians(45.0f),
		(float)windowWidth / (float)windowHeight, 
		1.0f, 100.0f);
}

glm::mat4 getView()
{
	return glm::lookAt(
		cameraPos,
		glm::vec3(0, 0.5, 0),
		glm::vec3(0, 1, 0));
}
