#define GLM_FORCE_RADIANS

#include <iostream>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <tuple>
#include <optional>
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "Model.h"
#include "VertexArray.h"

bool show_test = true;

void error_callback(int error, const char *description) {
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		show_test = !show_test;
}

std::optional<std::tuple<int, int>> init_opengl(GLFWwindow **window) {
	if (!glfwInit()) {
		std::cerr << "Failed to init glfw!" << std::endl;
		return std::nullopt;
	}

	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
	*window = glfwCreateWindow(640, 480, "Smiley", NULL, NULL);
	if (*window == nullptr) {
		std::cerr << "Failed to create window!" << std::endl;
		return std::nullopt;
	}

	glfwMakeContextCurrent(*window);
	auto version = gladLoadGL(glfwGetProcAddress);
	std::cout << "OpenGL version: " << version << std::endl;
	if (version == 0) {
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		return std::nullopt;
	}

	glfwSwapInterval(1);

	glfwSetKeyCallback(*window, key_callback);

	int width, height;
	glfwGetFramebufferSize(*window, &width, &height);
	glViewport(0, 0, width, height);

	glEnable(GL_CULL_FACE);

	return std::make_tuple(width, height);
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

	auto dims = init_opengl(&window);
	if (!dims.has_value()) {
		return 1;
	}

	int width, height;
	std::tie(width, height) = dims.value();

	std::vector<glm::vec3> vertex_vec3s = {
			glm::vec3(-0.5f, -0.5f, 0.0f),
			glm::vec3(0.5f, -0.5f, 0.0f),
			glm::vec3(-0.0f, 0.5f, 0.0f),
	};
	std::vector<unsigned int> index_vec3s = {
			0, 1, 2
	};
	VertexArray va(vertex_vec3s, index_vec3s);

	auto test_model = Model::from_obj_file("../resources/test.obj");
	VertexArray test_va(test_model.vectors, test_model.faces);

	auto smiley_model = Model::from_obj_file("../resources/smiley.obj");
//	for (auto &v: smiley_model.vectors) {
//		v[2] = v[2] / 10;
//	}
	VertexArray smiley_va(smiley_model.vectors, smiley_model.faces);

	auto vertex_shader = compile_shader("../src/TriangleVertexShader.glsl", GL_VERTEX_SHADER);
	auto fragment_shader = compile_shader("../src/TriangleFragmentShader.glsl", GL_FRAGMENT_SHADER);

	auto program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	glUseProgram(program);
	while (GLenum error = glGetError()) {
		std::cout << "Program error: " << error << std::endl;
	}

	auto projection_loc = glGetUniformLocation(program, "u_projection");
	assert(projection_loc != -1);
	auto ident = glm::mat4(1.0f);
	glm::mat4 projection = glm::perspective(glm::radians(180.0f),
	                                        (GLfloat) width / (GLfloat) height,
	                                        0.1f,
	                                        100.0f);
	auto scale = glm::scale(ident, glm::vec3(0.5f));
	auto euler_angles = glm::vec3(glm::radians(270.0f), 0.0f, 0.0f);
	auto quat = glm::quat(euler_angles);
	auto rotate = glm::mat4(quat);
	while (GLenum error = glGetError()) {
		std::cout << "Uniform error: " << error << std::endl;
	}

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);
		if (show_test) {
			glFrontFace(GL_CCW);
			glUniformMatrix4fv(projection_loc, 1, GL_FALSE, &glm::identity<glm::mat4>()[0][0]);
			glBindVertexArray(test_va.vertex_array);
			glDrawElements(GL_TRIANGLES, test_va.num_vertices, GL_UNSIGNED_INT, nullptr);
			while (GLenum error = glGetError()) {
				std::cout << "Draw Error: " << error << std::endl;
			}
		} else {
			glFrontFace(GL_CW);
//			glUniformMatrix4fv(projection_loc, 1, GL_FALSE, &projection[0][0]);
//			glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(scale));
			glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(rotate * scale));

			glBindVertexArray(smiley_va.vertex_array);
			glDrawElements(GL_TRIANGLES, smiley_va.num_vertices, GL_UNSIGNED_INT, nullptr);
			while (GLenum error = glGetError()) {
				std::cout << "Draw Error: " << error << std::endl;
			}
		}

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

