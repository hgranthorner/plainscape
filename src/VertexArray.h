//
// Created by Grant on 8/12/2022.
//

#ifndef PLAINSCAPE_VERTEXARRAY_H
#define PLAINSCAPE_VERTEXARRAY_H

#include <vector>
#include "glad/gl.h"
#include "glm/vec3.hpp"

typedef glm::vec<3, unsigned int, glm::defaultp> vec3ui;

class VertexArray {
public:
	template <unsigned int N_Vertices, unsigned int N_Indices>
	VertexArray(float (&vertices)[N_Vertices], unsigned int (&indices)[N_Indices]);
	VertexArray(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices);

	unsigned int vertex_array;
	unsigned int vertex_buffer;
	unsigned int index_buffer;
	unsigned int num_vertices;
};

template <unsigned int N_Vertices, unsigned int N_Indices>
VertexArray::VertexArray(float (&vertices)[N_Vertices], unsigned int (&indices)[N_Indices]) {
	vertex_array = 0;
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	GLuint buffers[2];
	glGenBuffers(2, buffers);
	vertex_buffer = buffers[0];
	index_buffer = buffers[1];
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_READ);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_READ);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindVertexArray(0);

	num_vertices = sizeof indices / sizeof(indices[0]);
}


#endif //PLAINSCAPE_VERTEXARRAY_H
