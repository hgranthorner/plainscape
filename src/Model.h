//
// Created by Grant on 8/12/2022.
//

#ifndef PLAINSCAPE_MODEL_H
#define PLAINSCAPE_MODEL_H


#include <string>
#include <vector>
#include <glm/vec3.hpp>

typedef glm::vec<3, unsigned int, glm::defaultp> vec3ui;

class Model {
public:
	std::vector<glm::vec3> vectors;
	std::vector<glm::vec3> vector_normals;
	std::vector<vec3ui> faces;
public:
	static Model from_obj_file(const std::string &path);
};


#endif //PLAINSCAPE_MODEL_H
