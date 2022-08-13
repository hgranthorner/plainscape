//
// Created by Grant on 8/12/2022.
//

#include <iostream>
#include "VertexArray.h"

void create_vertex_array_and_buffers(unsigned int &vertex_array, unsigned int &vertex_buffer, unsigned int &index_buffer) {
	vertex_array = 0;
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	GLuint buffers[2];
	glGenBuffers(2, buffers);
	vertex_buffer = buffers[0];
	index_buffer = buffers[1];
}

void bind_and_load_index_buffer(unsigned int index_buffer_id, std::vector<unsigned int> indices) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
	while (GLenum error = glGetError()) {
		std::cout << "Bind Buffer 2 Error: " << error << std::endl;
	}
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_READ);
	while (GLenum error = glGetError()) {
		std::cout << "Buffer Data 2 Error: " << error << std::endl;
	}
}

VertexArray::VertexArray(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices) {
	create_vertex_array_and_buffers(vertex_array, vertex_buffer, index_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	while (GLenum error = glGetError()) {
		std::cout << "Bind Buffer 1 Error: " << error << std::endl;
	}
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_READ);
	while (GLenum error = glGetError()) {
		std::cout << "Buffer Data 1 Error: " << error << std::endl;
	}

	bind_and_load_index_buffer(index_buffer, indices);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindVertexArray(0);

	num_vertices = indices.size();
}


VertexArray::VertexArray(std::vector<float> vertices, std::vector<unsigned int> indices) {
	create_vertex_array_and_buffers(vertex_array, vertex_buffer, index_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	while (GLenum error = glGetError()) {
		std::cout << "Bind Buffer 1 Error: " << error << std::endl;
	}
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_READ);
	while (GLenum error = glGetError()) {
		std::cout << "Buffer Data 1 Error: " << error << std::endl;
	}

	bind_and_load_index_buffer(index_buffer, indices);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, nullptr);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (const void *)(sizeof(float) * 3));
	while (GLenum error = glGetError()) {
		std::cout << "Binding Vertex Attrib Pointer Error: " << error << std::endl;
	}
	glBindVertexArray(0);

	num_vertices = indices.size();
}