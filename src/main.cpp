#define GLM_FORCE_RADIANS

#include <iostream>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <tuple>
#include <optional>
#include <functional>
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "Model.h"
#include "VertexArray.h"

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

void error_callback(int error, const char *description);

struct State {
	bool show_test, enable_projection, enable_view;
	float x_rotation, y_rotation, z_rotation, fov, z_near, z_far, trans_x, trans_y, trans_z;
};

State state{
		.show_test =  false,
		.enable_projection = true,
		.enable_view = true,
		.x_rotation = 270,
		.y_rotation = 0,
		.z_rotation = 0,
		.fov = 45,
		.z_near = 0.1,
		.z_far = 100,
		.trans_x = 0,
		.trans_y = 0,
		.trans_z = 0,
};


GLuint getProgram(GLuint vertex_shader, GLuint fragment_shader);

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
	std::vector<float> smiley_vertices;
	smiley_vertices.reserve(smiley_model.vectors.size() * 6);
	for (auto &v: smiley_model.vectors) {
		smiley_vertices.push_back(v[0]);
		smiley_vertices.push_back(v[1]);
		smiley_vertices.push_back(v[2]);
		// double z for color
		smiley_vertices.push_back((v[0] + 1) / 2);
		smiley_vertices.push_back((v[1] + 1) / 2);
		smiley_vertices.push_back((v[2] + 1) / 2);
	}

	VertexArray smiley_va(smiley_vertices, smiley_model.faces);

	auto vertex_shader = compile_shader("../src/TriangleVertexShader.glsl", GL_VERTEX_SHADER);
	auto vertex_shader_with_color = compile_shader("../src/TriangleVertexShaderWithColor.glsl", GL_VERTEX_SHADER);
	auto fragment_shader = compile_shader("../src/TriangleFragmentShader.glsl", GL_FRAGMENT_SHADER);

	GLuint program_no_color = getProgram(vertex_shader, fragment_shader);
	GLuint program = getProgram(vertex_shader_with_color, fragment_shader);
	glUseProgram(program);
	while (GLenum error = glGetError()) {
		std::cout << "Program error: " << error << std::endl;
	}

	auto projection_loc = glGetUniformLocation(program, "u_MVP");
	assert(projection_loc != -1);

	while (!glfwWindowShouldClose(window)) {
		auto ident = glm::mat4(1.0f);
		auto euler_angles = glm::vec3(glm::radians(state.x_rotation),
		                              glm::radians(state.y_rotation),
		                              glm::radians(state.z_rotation));
		auto quat = glm::quat(euler_angles);
		glm::mat4 translation = glm::translate(ident, glm::vec3(state.trans_x, state.trans_y, state.trans_z));
		glm::mat4 rotate = glm::mat4(quat);
		glm::mat4 scale = glm::scale(ident, glm::vec3(0.5f));
		glm::mat4 model = translation * rotate * scale;

		glm::mat4 projection = glm::perspective(glm::radians(state.fov),
		                                        (GLfloat) width / (GLfloat) height,
		                                        state.z_near,
		                                        state.z_far);
		glm::mat4 view = glm::lookAt(
				glm::vec3(5, 0, 0),
				glm::vec3(0, 0, 0),
				glm::vec3(0, 1, 0)
		);

		glm::mat4 mvp =
				(state.enable_projection ? projection : glm::mat4(1))
				* (state.enable_view ? view : glm::mat4(1))
				* model;
		glfwPollEvents();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);
		if (state.show_test) {
			glFrontFace(GL_CCW);
			glUniformMatrix4fv(projection_loc, 1, GL_FALSE, &glm::identity<glm::mat4>()[0][0]);
			glBindVertexArray(test_va.vertex_array);
			glDrawElements(GL_TRIANGLES, test_va.num_vertices, GL_UNSIGNED_INT, nullptr);
			while (GLenum error = glGetError()) {
				std::cout << "Draw Error: " << error << std::endl;
			}
		} else {
			// Blender spits out triangles the wrong way
			glFrontFace(GL_CCW);

			glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(mvp));

			glBindVertexArray(smiley_va.vertex_array);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, smiley_va.index_buffer);
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

GLuint getProgram(GLuint vertex_shader, GLuint fragment_shader) {
	auto program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	return program;
}

void error_callback(int error, const char *description) {
	fprintf(stderr, "Error: %s\n", description);
}


void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	auto key_helper = [key](int in_key1, int in_key2, bool action_compare, float &state_value, float delta,
	                        const std::string &s) {
		if (key == in_key1 && action_compare) {
			state_value += delta;
			std::cout << s << state_value << std::endl;
		}
		if (key == in_key2 && action_compare) {
			state_value -= delta;
			std::cout << s << state_value << std::endl;
		}
	};

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		state.show_test = !state.show_test;

	key_helper(GLFW_KEY_UP, GLFW_KEY_DOWN,
						 action != GLFW_RELEASE,
	           state.y_rotation, 1, "New Y: ");

	key_helper(GLFW_KEY_RIGHT, GLFW_KEY_LEFT,
						 action != GLFW_RELEASE,
	           state.x_rotation, 1, "New X: ");

	key_helper(GLFW_KEY_SEMICOLON, GLFW_KEY_APOSTROPHE,
	           action != GLFW_RELEASE,
	           state.z_rotation, 1, "New Z: ");

	key_helper(GLFW_KEY_Q, GLFW_KEY_A,
	           action != GLFW_RELEASE,
	           state.z_near, 0.1, "New Z Near: ");

	key_helper(GLFW_KEY_W, GLFW_KEY_S,
	           action != GLFW_RELEASE,
	           state.z_far, 0.1, "New Z Far: ");

	key_helper(GLFW_KEY_E, GLFW_KEY_D,
	           action != GLFW_RELEASE,
	           state.fov, 1, "New FOV: ");

	key_helper(GLFW_KEY_U, GLFW_KEY_J,
	           action != GLFW_RELEASE,
	           state.trans_y, 0.5, "New Y Translation: ");

	key_helper(GLFW_KEY_Y, GLFW_KEY_I,
	           action != GLFW_RELEASE,
	           state.trans_x, 0.5, "New X Translation: ");

	key_helper(GLFW_KEY_H, GLFW_KEY_K,
	           action != GLFW_RELEASE,
	           state.trans_z, 0.5, "New Z Translation: ");

	if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		state.enable_projection = !state.enable_projection;
		std::cout << "New Projection Status: " << state.enable_projection << std::endl;
	}

	if (key == GLFW_KEY_X && action == GLFW_PRESS) {
		state.enable_view = !state.enable_view;
		std::cout << "New View Status: " << state.enable_view << std::endl;
	}
}