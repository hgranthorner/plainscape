//
// Created by Grant on 8/12/2022.
//

#include <iostream>
#include "VertexArray.h"

VertexArray::VertexArray(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices) {
	vertex_array = 0;
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	GLuint buffers[2];
	glGenBuffers(2, buffers);
	vertex_buffer = buffers[0];
	index_buffer = buffers[1];
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	while (GLenum error = glGetError()) {
		std::cout << "Bind Buffer 1 Error: " << error << std::endl;
	}
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_READ);
	while (GLenum error = glGetError()) {
		std::cout << "Buffer Data 1 Error: " << error << std::endl;
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	while (GLenum error = glGetError()) {
		std::cout << "Bind Buffer 2 Error: " << error << std::endl;
	}
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_READ);
	while (GLenum error = glGetError()) {
		std::cout << "Buffer Data 2 Error: " << error << std::endl;
	}


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindVertexArray(0);

	num_vertices = indices.size();
}