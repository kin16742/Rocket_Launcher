#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
using namespace glm;

GLFWwindow* window;
int parachute = 0;
int launch = 0;
float charge = 0.0f;
float currentH = 0.0f;
int flag = 0, flag2 = 0;
float Pangle = 0;

GLfloat *makeCircleVertexData(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides);
GLfloat *makeCylinderVertexData(GLfloat x, GLfloat y, GLfloat z, GLfloat height, GLfloat radius, GLint numberOfSides);
GLfloat *makeConeVertexData(GLfloat x, GLfloat y, GLfloat z, GLfloat height, GLfloat radius, GLint numberOfSides);

int main(void) {
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1024, 768, "Rocket Launcher", NULL, NULL);  // 윈도우 설정
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);


	glClearColor(135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f, 0);	// 배경색

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// 로켓 날개
	static const GLfloat g_vertex_buffer_data[] = {
		1.0f, -6.5f, 0.0f,
		2.5f, -6.5f, 0.0f,
		1.0f, -2.5f, 0.0f,

		-1.0f, -6.5f, 0.0f,
		-2.5f, -6.5f, 0.0f,
		-1.0f,-2.5f, 0.0f,

		0.0f, -6.5f, 1.0f,
		0.0f, -6.5f, 2.5f,
		0.0f, -2.5f, 1.0f,

		0.0f, -6.5f, -1.0f,
		0.0f, -6.5f, -2.5f,
		0.0f, -2.5f, -1.0f
	};
	static const GLfloat g_color_buffer_data[] = {
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,

		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,

		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,

		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f
	};
	// 로켓 창문
	GLfloat *window_buffer_data = makeCircleVertexData(0, 0.5, 1.01, 0.3, 36);
	static const GLfloat windowColor_buffer_data[]{
		0.0f,  0.7f,  1.0f,
		0.0f,  0.7f,  1.0f,
		0.0f,  0.7f,  1.0f,

		0.0f,  0.7f,  1.0f,
		0.0f,  0.7f,  1.0f,
		0.0f,  0.7f,  1.0f,

		0.0f,  0.7f,  1.0f,
		0.0f,  0.7f,  1.0f,
		0.0f,  0.7f,  1.0f,

		0.0f,  0.7f,  1.0f,
		0.0f,  0.7f,  1.0f,
		0.0f,  0.7f,  1.0f,

		0.0f,  0.7f,  1.0f,
		0.0f,  0.7f,  1.0f,
		0.0f,  0.7f,  1.0f,

		0.0f,  0.7f,  1.0f,
		0.0f,  0.7f,  1.0f,
		0.0f,  0.7f,  1.0f,

		0.0f,  0.7f,  1.0f,
		0.0f,  0.7f,  1.0f,
		0.0f,  0.7f,  1.0f,

		0.0f,  0.7f,  1.0f,
		0.0f,  0.7f,  1.0f,
		0.0f,  0.7f,  1.0f,

		0.0f,  0.7f,  1.0f,
		0.0f,  0.7f,  1.0f,
		0.0f,  0.7f,  1.0f,

		0.0f,  0.7f,  1.0f,
		0.0f,  0.7f,  1.0f,
		0.0f,  0.7f,  1.0f,

		0.0f,  0.7f,  1.0f,
		0.0f,  0.7f,  1.0f,
		0.0f,  0.7f,  1.0f,

		0.0f,  0.7f,  1.0f,
		0.0f,  0.7f,  1.0f,
		0.0f,  0.7f,  1.0f
	};
	// 로켓 몸통
	GLfloat *cylinder_buffer_data = makeCylinderVertexData(0, -6.5, 0, 10, 1, 36);
	static const GLfloat cylinderColor_buffer_data[]{
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,

		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f,  1.0f,  0.9f,
		1.0f, 1.0f, 0.9f
	};
	// 로켓 뚜껑
	GLfloat *cone_buffer_data = makeConeVertexData(0, 3.5, 0, 3, 1, 36);
	static const GLfloat coneColor_buffer_data[]{
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f,
		1.0f,  0.3f,  0.3f
	};
	// 바닥
	static const GLfloat ground_buffer_data[] = {
		1000.0f, -6.5f, 1000.0f,
		1000.0f, -6.5f, -1000.0f,
		-1000.0f, -6.5f, 1000.0f,

		-1000.0f, -6.5f, -1000.0f,
		1000.0f, -6.5f, -1000.0f,
		-1000.0f, -6.5f, 1000.0f
	};
	static const GLfloat ground_color_data[] = {
		46.0f / 255.0f,  139.0f / 255.0f,  87.0f / 255.0f,
		46.0f / 255.0f,  139.0f / 255.0f,  87.0f / 255.0f,
		46.0f / 255.0f,  139.0f / 255.0f,  87.0f / 255.0f,

		46.0f / 255.0f,  139.0f / 255.0f,  87.0f / 255.0f,
		46.0f / 255.0f,  139.0f / 255.0f,  87.0f / 255.0f,
		46.0f / 255.0f,  139.0f / 255.0f,  87.0f / 255.0f
	};
	// 낙하산 줄
	static const GLfloat paraLine_buffer_data[] = {
		0.0f,0.0f,0.0f,
		3.0f,-2.0f,3.0f,

		0.0f,0.0f,0.0f,
		3.0f,-2.0f,-3.0f,

		0.0f,0.0f,0.0f,
		-3.0f,-2.0f,-3.0f,

		0.0f,0.0f,0.0f,
		-3.0f,-2.0f,3.0f,

		0.0f,0.0f,0.0f,
		4.242f,-2.0f,0.0f,

		0.0f,0.0f,0.0f,
		-4.242f,-2.0f,0.0f,

		0.0f,0.0f,0.0f,
		0.0f,-2.0f,4.242f,

		0.0f,0.0f,0.0f,
		0.0f,-2.0f,-4.242f
	};
	static const GLfloat paraLineColor_buffer_data[] = {
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f
	};
	// 낙하산
	static const GLfloat parachute_buffer_data[] = {
		0.0f,-4.5f,0.0f,
		3.0f,-2.0f,3.0f,
		0.0f,-2.0f,4.242f,

		0.0f,-4.5f,0.0f,
		0.0f,-2.0f,4.242f,
		-3.0f,-2.0f,3.0f,

		0.0f,-4.5f,0.0f,
		-3.0f,-2.0f,3.0f,
		-4.242f,-2.0f,0.0f,

		0.0f,-4.5f,0.0f,
		-4.242f,-2.0f,0.0f,
		-3.0f,-2.0f,-3.0f,

		0.0f,-4.5f,0.0f,
		-3.0f,-2.0f,-3.0f,
		0.0f,-2.0f,-4.242f,

		0.0f,-4.5f,0.0f,
		0.0f,-2.0f,-4.242f,
		3.0f,-2.0f,-3.0f,
		
		0.0f,-4.5f,0.0f,
		3.0f,-2.0f,-3.0f,
		4.242f,-2.0f,0.0f,

		0.0f,-4.5f,0.0f,
		4.242f,-2.0f,0.0f,
		3.0f,-2.0f,3.0f
	};
	static const GLfloat parachuteColor_buffer_data[] = {
		222.0f / 255.0f,184.0f / 255.0f,135.0f / 255.0f,
		222.0f / 255.0f,184.0f / 255.0f,135.0f / 255.0f,
		222.0f / 255.0f,184.0f / 255.0f,135.0f / 255.0f,
		233.0f / 255.0f,150.0f / 255.0f,122.0f / 255.0f,
		233.0f / 255.0f,150.0f / 255.0f,122.0f / 255.0f,
		233.0f / 255.0f,150.0f / 255.0f,122.0f / 255.0f,
		222.0f / 255.0f,184.0f / 255.0f,135.0f / 255.0f,
		222.0f / 255.0f,184.0f / 255.0f,135.0f / 255.0f,
		222.0f / 255.0f,184.0f / 255.0f,135.0f / 255.0f,
		233.0f / 255.0f,150.0f / 255.0f,122.0f / 255.0f,
		233.0f / 255.0f,150.0f / 255.0f,122.0f / 255.0f,
		233.0f / 255.0f,150.0f / 255.0f,122.0f / 255.0f,
		222.0f / 255.0f,184.0f / 255.0f,135.0f / 255.0f,
		222.0f / 255.0f,184.0f / 255.0f,135.0f / 255.0f,
		222.0f / 255.0f,184.0f / 255.0f,135.0f / 255.0f,
		233.0f / 255.0f,150.0f / 255.0f,122.0f / 255.0f,
		233.0f / 255.0f,150.0f / 255.0f,122.0f / 255.0f,
		233.0f / 255.0f,150.0f / 255.0f,122.0f / 255.0f,
		222.0f / 255.0f,184.0f / 255.0f,135.0f / 255.0f,
		222.0f / 255.0f,184.0f / 255.0f,135.0f / 255.0f,
		222.0f / 255.0f,184.0f / 255.0f,135.0f / 255.0f,
		233.0f / 255.0f,150.0f / 255.0f,122.0f / 255.0f,
		233.0f / 255.0f,150.0f / 255.0f,122.0f / 255.0f,
		233.0f / 255.0f,150.0f / 255.0f,122.0f / 255.0f
	};
	// 나무기둥
	GLfloat *tree_buffer_data = makeCylinderVertexData(0, -6.5, 0, 5.5, 0.5, 36);
	static const GLfloat tree_color_data[] = {
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,

		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f,
		139.0f / 255.0f, 69.0f / 255.0f, 19.0f / 255.0f
	};
	// 나뭇잎
	static const GLfloat leaf_buffer_data[] = {
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,

		-2.0f,-1.7f,-2.0f,
		-2.0f,-1.7f, 2.0f,
		-2.0f, 0.3f, 2.0f,
		2.0f, 0.3f,-2.0f,
		-2.0f,-1.7f,-2.0f,
		-2.0f, 0.3f,-2.0f,
		2.0f,-1.7f, 2.0f,
		-2.0f,-1.7f,-2.0f,
		2.0f,-1.7f,-2.0f,
		2.0f, 0.3f,-2.0f,
		2.0f,-1.7f,-2.0f,
		-2.0f,-1.7f,-2.0f,
		-2.0f,-1.7f,-2.0f,
		-2.0f, 0.3f, 2.0f,
		-2.0f, 0.3f,-2.0f,
		2.0f,-1.7f, 2.0f,
		-2.0f,-1.7f, 2.0f,
		-2.0f,-1.7f,-2.0f,
		-2.0f, 0.3f, 2.0f,
		-2.0f,-1.7f, 2.0f,
		2.0f,-1.7f, 2.0f,
		2.0f, 0.3f, 2.0f,
		2.0f,-1.7f,-2.0f,
		2.0f, 0.3f,-2.0f,
		2.0f,-1.7f,-2.0f,
		2.0f, 0.3f, 2.0f,
		2.0f,-1.7f, 2.0f,
		2.0f, 0.3f, 2.0f,
		2.0f, 0.3f,-2.0f,
		-2.0f, 0.3f,-2.0f,
		2.0f, 0.3f, 2.0f,
		-2.0f, 0.3f,-2.0f,
		-2.0f, 0.3f, 2.0f,
		2.0f, 0.3f, 2.0f,
		-2.0f, 0.3f, 2.0f,
		2.0f,-1.7f, 2.0f
	};
	static const GLfloat leaf_color_data[] = {
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f,
		0.0f, 100.0f / 255.0f, 0.0f
	};

	GLuint parachutebuffer;
	glGenBuffers(1, &parachutebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, parachutebuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(parachute_buffer_data), parachute_buffer_data, GL_STATIC_DRAW);

	GLuint parachuteColorbuffer;
	glGenBuffers(1, &parachuteColorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, parachuteColorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(parachuteColor_buffer_data), parachuteColor_buffer_data, GL_STATIC_DRAW);

	GLuint paraLinebuffer;
	glGenBuffers(1, &paraLinebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, paraLinebuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(paraLine_buffer_data), paraLine_buffer_data, GL_STATIC_DRAW);

	GLuint paraLineColorbuffer;
	glGenBuffers(1, &paraLineColorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, paraLineColorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(paraLineColor_buffer_data), paraLineColor_buffer_data, GL_STATIC_DRAW);

	GLuint conebuffer;
	glGenBuffers(1, &conebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, conebuffer);
	glBufferData(GL_ARRAY_BUFFER, 36 * 9 * sizeof(GLfloat), cone_buffer_data, GL_STATIC_DRAW);

	GLuint coneColorbuffer;
	glGenBuffers(1, &coneColorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, coneColorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coneColor_buffer_data), coneColor_buffer_data, GL_STATIC_DRAW);

	GLuint cylinderbuffer;
	glGenBuffers(1, &cylinderbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderbuffer);
	glBufferData(GL_ARRAY_BUFFER, 36 * 18 * sizeof(GLfloat), cylinder_buffer_data, GL_STATIC_DRAW);

	GLuint cylinderColorbuffer;
	glGenBuffers(1, &cylinderColorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderColorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cylinderColor_buffer_data), cylinderColor_buffer_data, GL_STATIC_DRAW);

	GLuint groundbuffer;
	glGenBuffers(1, &groundbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, groundbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground_buffer_data), ground_buffer_data, GL_STATIC_DRAW);

	GLuint groundcolorbuffer;
	glGenBuffers(1, &groundcolorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, groundcolorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground_color_data), ground_color_data, GL_STATIC_DRAW);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	GLuint wbuffer;
	glGenBuffers(1, &wbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, wbuffer);
	glBufferData(GL_ARRAY_BUFFER, 37 * 3 * sizeof(GLfloat), window_buffer_data, GL_STATIC_DRAW);

	GLuint wcbuffer;
	glGenBuffers(1, &wcbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, wcbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(windowColor_buffer_data), windowColor_buffer_data, GL_STATIC_DRAW);

	GLuint treebuffer;
	glGenBuffers(1, &treebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, treebuffer);
	glBufferData(GL_ARRAY_BUFFER, 36 * 18 * sizeof(GLfloat), tree_buffer_data, GL_STATIC_DRAW);

	GLuint treeCbuffer;
	glGenBuffers(1, &treeCbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, treeCbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tree_color_data), tree_color_data, GL_STATIC_DRAW);

	GLuint leafbuffer;
	glGenBuffers(1, &leafbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, leafbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(leaf_buffer_data), leaf_buffer_data, GL_STATIC_DRAW);

	GLuint leafCbuffer;
	glGenBuffers(1, &leafCbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, leafCbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(leaf_color_data), leaf_color_data, GL_STATIC_DRAW);

	glm::mat4 TMatrix[30] = {
		translate(mat4(), glm::vec3(-5.0f, 0.0f, -5.0f)),
		translate(mat4(), glm::vec3(5.0f, 0.0f, -10.0f)),
		translate(mat4(), glm::vec3(-10.0f, 0.0f, 0.0f)),
		translate(mat4(), glm::vec3(-10.0f, 0.0f, -20.0f)),
		translate(mat4(), glm::vec3(-20.0f, 0.0f, 0.0f)),
		translate(mat4(), glm::vec3(-20.0f, 0.0f, 10.0f)),
		translate(mat4(), glm::vec3(7.0f, 0.0f, -16.0f)),
		translate(mat4(), glm::vec3(-23.0f, 0.0f, 23.0f)),
		translate(mat4(), glm::vec3(-14.0f, 0.0f, -22.0f)),
		translate(mat4(), glm::vec3(-10.0f, 0.0f, 10.0f)), 
		translate(mat4(), glm::vec3(-25.0f, 0.0f, -25.0f)),
		translate(mat4(), glm::vec3(25.0f, 0.0f, -12.0f)),
		translate(mat4(), glm::vec3(-12.0f, 0.0f, -20.0f)),
		translate(mat4(), glm::vec3(-12.0f, 0.0f, -22.0f)),
		translate(mat4(), glm::vec3(-22.0f, 0.0f,-20.0f)),
		translate(mat4(), glm::vec3(-22.0f, 0.0f, -12.0f)),
		translate(mat4(), glm::vec3(27.0f, 0.0f, -26.0f)),
		translate(mat4(), glm::vec3(-23.0f, 0.0f, -23.0f)),
		translate(mat4(), glm::vec3(-14.0f, 0.0f, -22.0f)),
		translate(mat4(), glm::vec3(-10.0f, 0.0f, -12.0f)),
		translate(mat4(), glm::vec3(-5.0f, 0.0f, -25.0f)),
		translate(mat4(), glm::vec3(5.0f, 0.0f, -30.0f)),
		translate(mat4(), glm::vec3(-10.0f, 0.0f, -30.0f)),
		translate(mat4(), glm::vec3(-10.0f, 0.0f, -20.0f)),
		translate(mat4(), glm::vec3(-20.0f, 0.0f, 0.0f)),
		translate(mat4(), glm::vec3(-20.0f, 0.0f, -40.0f)),
		translate(mat4(), glm::vec3(27.0f, 0.0f, -16.0f)),
		translate(mat4(), glm::vec3(33.0f, 0.0f, -23.0f)),
		translate(mat4(), glm::vec3(14.0f, 0.0f, -22.0f)),
		translate(mat4(), glm::vec3(-10.0f, 0.0f, -30.0f))
	};
	glm::mat4 SMatrix[30] = {
		scale(mat4(), vec3(1.0f, 2.0f, 1.0f)),
		scale(mat4(), vec3(1.0f, 1.0f, 2.0f)),
		scale(mat4(), vec3(2.0f, 1.0f, 2.0f)),
		scale(mat4(), vec3(1.0f, 1.0f, 1.0f)),
		scale(mat4(), vec3(1.0f, 3.0f, 1.0f)),
		scale(mat4(), vec3(1.0f, 2.0f, 2.0f)),
		scale(mat4(), vec3(1.0f, 1.0f, 1.0f)),
		scale(mat4(), vec3(2.0f, 2.0f, 2.0f)),
		scale(mat4(), vec3(2.0f, 1.0f, 1.0f)),
		scale(mat4(), vec3(1.0f, 1.0f, 2.0f)),
		scale(mat4(), vec3(1.0f, 2.0f, 1.0f)),
		scale(mat4(), vec3(1.0f, 1.0f, 2.0f)),
		scale(mat4(), vec3(2.0f, 1.0f, 2.0f)),
		scale(mat4(), vec3(1.0f, 1.0f, 1.0f)),
		scale(mat4(), vec3(1.0f, 3.0f, 1.0f)),
		scale(mat4(), vec3(1.0f, 2.0f, 2.0f)),
		scale(mat4(), vec3(1.0f, 1.0f, 1.0f)),
		scale(mat4(), vec3(2.0f, 2.0f, 2.0f)),
		scale(mat4(), vec3(2.0f, 1.0f, 1.0f)),
		scale(mat4(), vec3(1.0f, 1.0f, 2.0f)),
		scale(mat4(), vec3(1.0f, 2.0f, 1.0f)),
		scale(mat4(), vec3(1.0f, 1.0f, 2.0f)),
		scale(mat4(), vec3(2.0f, 1.0f, 2.0f)),
		scale(mat4(), vec3(1.0f, 1.0f, 1.0f)),
		scale(mat4(), vec3(1.0f, 3.0f, 1.0f)),
		scale(mat4(), vec3(1.0f, 2.0f, 2.0f)),
		scale(mat4(), vec3(1.0f, 1.0f, 1.0f)),
		scale(mat4(), vec3(2.0f, 2.0f, 2.0f)),
		scale(mat4(), vec3(2.0f, 1.0f, 1.0f)),
		scale(mat4(), vec3(1.0f, 1.0f, 2.0f))
	};

	double lastTime = glfwGetTime();
	double lastFrameTime = lastTime;
	float Yangle = 0, angle = 0, flag2 = 0;
	float cnt = 0.0f;

	do {
		double currentTime = glfwGetTime();
		float deltaTime = (float)(currentTime - lastFrameTime);
		lastFrameTime = currentTime;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);

		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);

		glm::mat4 ScalingMatrix = scale(mat4(), vec3(cnt, cnt, cnt));
		glm::mat4 TranslationMatrix = translate(mat4(), glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat4 RotationMatrix = eulerAngleYXZ(0.0f, 0.0f, 0.0f);
		glm::mat4 TranslationMatrix2 = translate(mat4(), glm::vec3(0.0f, -6.5f, 0.0f));

		if (launch && charge > 40) {
			if (Yangle < 3.14159f)
				Yangle += deltaTime / 3.14159f * 2.0f;
			if (Yangle > 3.14159f / 10)
				flag2 = 1;
			if (Yangle > 3.14159f / 2)
				flag = 1;

			if (parachute && currentH > 0)
				currentH -= 0.003f;
			if (!parachute && glm::sin(Yangle) >= 0)
				currentH = float(charge * glm::sin(Yangle) / 3.0f) - 6.5f;

			if (currentH < 0)
				TranslationMatrix = translate(mat4(), glm::vec3(0.0f, 0.0f, -Pangle * 10 - Pangle * charge / 20));
			else if (parachute) {
				TranslationMatrix = translate(mat4(), glm::vec3(0.0f, currentH, -Pangle * 10 - Pangle * charge / 20));
				Pangle += deltaTime / 3.14159f / 2;
			}
			else {
				TranslationMatrix = translate(mat4(), glm::vec3(0.0f, currentH, -Yangle * 10 - Yangle * charge / 20));
				Pangle = Yangle;
			}

			if (flag2 == 1 && angle < 3.14159f) {
				angle += deltaTime / 3.14159f * 2.7f;
			}
			RotationMatrix = eulerAngleYXZ(0.0f, -angle, 0.0f);
		}

		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix  * TranslationMatrix * RotationMatrix;
		glm::mat4 MVPGround = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glm::mat4 MVPpara = ProjectionMatrix * ViewMatrix * ModelMatrix  * TranslationMatrix * RotationMatrix * ScalingMatrix * TranslationMatrix2;
		
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 정점 버퍼 그리기
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 색상 버퍼 그리기
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 18 * 3); // 삼각형 개수

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 정점 버퍼 그리기
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, conebuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 색상 버퍼 그리기
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, coneColorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 36 * 9); // 삼각형 개수
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, wbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 색상 버퍼 그리기
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, wcbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 37 * 3);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		// 정점 버퍼 그리기
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, cylinderbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 색상 버퍼 그리기
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, cylinderColorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 36 * 18); // 삼각형 개수

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		if (parachute) {
			if (cnt < 1.0f) cnt += 0.003f;
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPpara[0][0]);
			// 정점 버퍼 그리기
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, parachutebuffer);
			glVertexAttribPointer(
				0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);

			// 색상 버퍼 그리기
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, parachuteColorbuffer);
			glVertexAttribPointer(
				1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);

			glDrawArrays(GL_TRIANGLES, 0, 3 * 8); // 삼각형 개수

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);


			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPpara[0][0]);
			// 정점 버퍼 그리기
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, paraLinebuffer);
			glVertexAttribPointer(
				0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);

			// 색상 버퍼 그리기
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, paraLineColorbuffer);
			glVertexAttribPointer(
				1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);

			glDrawArrays(GL_LINES, 0, 2 * 8); // 삼각형 개수

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
		}
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPGround[0][0]);

		// 정점 버퍼 그리기
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, groundbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 색상 버퍼 그리기
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, groundcolorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 2 * 3); // 삼각형 개수

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		for (int i = 0; i < 30; i++) {
			glm::mat4 MVPtree = ProjectionMatrix * ViewMatrix * ModelMatrix  * TMatrix[i] * SMatrix[i];

			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPtree[0][0]);

			// 정점 버퍼 그리기
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, treebuffer);
			glVertexAttribPointer(
				0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);

			// 색상 버퍼 그리기
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, treeCbuffer);
			glVertexAttribPointer(
				1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);

			glDrawArrays(GL_TRIANGLES, 0, 36 * 18); // 삼각형 개수

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);

			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPtree[0][0]);

			// 정점 버퍼 그리기
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, leafbuffer);
			glVertexAttribPointer(
				0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);

			// 색상 버퍼 그리기
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, leafCbuffer);
			glVertexAttribPointer(
				1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);

			glDrawArrays(GL_TRIANGLES, 0, 24 * 3); // 삼각형 개수

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
		}

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);
	glfwTerminate();

	return 0;
}

GLfloat *makeCircleVertexData(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides)
{
	GLint numberOfVertices = numberOfSides + 1;

	GLfloat doublePi = 2.0f * 3.141592f;

	GLfloat *circleVerticesX = new GLfloat[numberOfVertices];
	GLfloat *circleVerticesY = new GLfloat[numberOfVertices];
	GLfloat *circleVerticesZ = new GLfloat[numberOfVertices];

	for (int i = 0; i < numberOfVertices; i++)
	{
		circleVerticesX[i] = x + (radius * cos(i * doublePi / numberOfSides));
		circleVerticesY[i] = y + (radius * sin(i * doublePi / numberOfSides));
		circleVerticesZ[i] = z;
	}
	GLfloat *allCircleVertices = new GLfloat[numberOfVertices * 3];

	for (int i = 0; i < numberOfVertices; i++)
	{
		allCircleVertices[i * 3] = circleVerticesX[i];
		allCircleVertices[(i * 3) + 1] = circleVerticesY[i];
		allCircleVertices[(i * 3) + 2] = circleVerticesZ[i];
	}

	return allCircleVertices;
}
GLfloat *makeConeVertexData(GLfloat x, GLfloat y, GLfloat z, GLfloat height, GLfloat radius, GLint numberOfSides) {
	GLint numberOfVertices = numberOfSides + 1;
	GLfloat doublePi = 2.0f * 3.141592f;
	GLfloat *circleVerticesX = new GLfloat[numberOfVertices * 3];
	GLfloat *circleVerticesY = new GLfloat[numberOfVertices * 3];
	GLfloat *circleVerticesZ = new GLfloat[numberOfVertices * 3];

	for (int i = 0; i < numberOfVertices; i++) {
		circleVerticesX[(i * 3) + 0] = x + (radius * cos(i * doublePi / numberOfSides));
		circleVerticesY[(i * 3) + 0] = y;
		circleVerticesZ[(i * 3) + 0] = z + (radius * sin(i * doublePi / numberOfSides));
		circleVerticesX[(i * 3) + 1] = x;
		circleVerticesY[(i * 3) + 1] = y + height;
		circleVerticesZ[(i * 3) + 1] = z;
		circleVerticesX[(i * 3) + 2] = x + (radius * cos((i + 1) * doublePi / numberOfSides));
		circleVerticesY[(i * 3) + 2] = y;
		circleVerticesZ[(i * 3) + 2] = z + (radius * sin((i + 1) * doublePi / numberOfSides));
	}
	GLfloat *allCylinderVertices = new GLfloat[numberOfVertices * 9];

	for (int i = 0; i < numberOfVertices * 3; i++) {
		allCylinderVertices[(i * 3) + 0] = circleVerticesX[i];
		allCylinderVertices[(i * 3) + 1] = circleVerticesY[i];
		allCylinderVertices[(i * 3) + 2] = circleVerticesZ[i];
	}
	return allCylinderVertices;
}
GLfloat *makeCylinderVertexData(GLfloat x, GLfloat y, GLfloat z, GLfloat height, GLfloat radius, GLint numberOfSides)
{
	GLint numberOfVertices = numberOfSides;

	GLfloat doublePi = 2.0f * 3.141592f;

	GLfloat *circleVerticesX = new GLfloat[numberOfVertices * 6];
	GLfloat *circleVerticesY = new GLfloat[numberOfVertices * 6];
	GLfloat *circleVerticesZ = new GLfloat[numberOfVertices * 6];

	for (int i = 0; i < numberOfVertices; i++)
	{
		circleVerticesX[(i * 6)] = x + (radius * cos(i * doublePi / numberOfSides));
		circleVerticesY[(i * 6)] = y;
		circleVerticesZ[(i * 6)] = z + (radius * sin(i * doublePi / numberOfSides));

		circleVerticesX[(i * 6) + 1] = x + (radius * cos((i + 1) * doublePi / numberOfSides));
		circleVerticesY[(i * 6) + 1] = y;
		circleVerticesZ[(i * 6) + 1] = z + (radius * sin((i + 1) * doublePi / numberOfSides));

		circleVerticesX[(i * 6) + 2] = x + (radius * cos(i * doublePi / numberOfSides));
		circleVerticesY[(i * 6) + 2] = y + height;
		circleVerticesZ[(i * 6) + 2] = z + (radius * sin(i * doublePi / numberOfSides));

		circleVerticesX[(i * 6) + 3] = x + (radius * cos(i * doublePi / numberOfSides));
		circleVerticesY[(i * 6) + 3] = y + height;
		circleVerticesZ[(i * 6) + 3] = z + (radius * sin(i * doublePi / numberOfSides));

		circleVerticesX[(i * 6) + 4] = x + (radius * cos((i + 1) * doublePi / numberOfSides));
		circleVerticesY[(i * 6) + 4] = y + height;
		circleVerticesZ[(i * 6) + 4] = z + (radius * sin((i + 1) * doublePi / numberOfSides));

		circleVerticesX[(i * 6) + 5] = x + (radius * cos((i + 1) * doublePi / numberOfSides));
		circleVerticesY[(i * 6) + 5] = y;
		circleVerticesZ[(i * 6) + 5] = z + (radius * sin((i + 1) * doublePi / numberOfSides));
	}
	GLfloat *allCircleVertices = new GLfloat[numberOfVertices * 18];

	for (int i = 0; i < numberOfVertices * 6; i++)
	{
		allCircleVertices[i * 3] = circleVerticesX[i];
		allCircleVertices[(i * 3) + 1] = circleVerticesY[i];
		allCircleVertices[(i * 3) + 2] = circleVerticesZ[i];
	}

	return allCircleVertices;
}
