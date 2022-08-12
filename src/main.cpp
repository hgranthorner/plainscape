#include <iostream>
#include <fstream>
#include <string>
#include "glad/gl.h"
#include "GLFW/glfw3.h"

void error_callback(int error, const char *description) {
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int init_opengl(GLFWwindow **window) {
	if (!glfwInit()) {
		std::cerr << "Failed to init glfw!" << std::endl;
		return 1;
	}

	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	*window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
	if (*window == nullptr) {
		std::cerr << "Failed to create window!" << std::endl;
		return 1;
	}

	glfwMakeContextCurrent(*window);
	auto version = gladLoadGL(glfwGetProcAddress);
	std::cout << "OpenGL version: " << version << std::endl;
	if (version == 0) {
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		return 1;
	}

	glfwSwapInterval(1);

	glfwSetKeyCallback(*window, key_callback);

	int width, height;
	glfwGetFramebufferSize(*window, &width, &height);
	glViewport(0, 0, width, height);

	return 0;
}

GLuint compile_shader(const std::string &path, int shader_type) {
	auto shader = glCreateShader(shader_type);
	std::ifstream ifs(path);
	std::string content((std::istreambuf_iterator<char>(ifs)),
	                    (std::istreambuf_iterator<char>()));
	const char *content_cstr = content.c_str();
	glShaderSource(shader, 1, &content_cstr, NULL);
	glCompileShader(shader);
	GLint params;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &params);
	if (params != GL_TRUE) {
		GLint logSize = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
		char log[logSize];
		glGetShaderInfoLog(shader, logSize, &logSize, log);
		std::cerr << "Failed to compile shader (" << path << "): " << log << std::endl;
		glDeleteShader(shader);
		return -1;
	}

	return shader;
}

int main() {
	GLFWwindow *window = nullptr;

	if (init_opengl(&window) != 0) {
		return 1;
	}

	float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f,
	};

	GLuint vertex_array;
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices, GL_STATIC_READ);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	auto vertex_shader = compile_shader("../src/TriangleVertexShader.glsl", GL_VERTEX_SHADER);
	auto fragment_shader = compile_shader("../src/TriangleFragmentShader.glsl", GL_FRAGMENT_SHADER);

	auto program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

