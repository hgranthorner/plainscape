//
// Created by Grant on 8/12/2022.
//

#include <fstream>
#include <iostream>
#include "Model.h"

glm::vec3 read_vertices(const std::string& s) {
	glm::vec3 vec;
	unsigned int index = 0;
	size_t prev_pos = s.find(' ') + 1;
	size_t pos;
	while ((pos = s.find(' ', prev_pos)) != std::string::npos) {
		assert(index < 3);
		vec[index] = std::stof(s.substr(prev_pos, pos - prev_pos));
		prev_pos = pos + 1;
	}

	return vec;
}

vec3ui read_faces(const std::string& s) {
	vec3ui vec;
	unsigned int index = 0;
	size_t prev_pos = s.find(' ') + 1;
	size_t pos;
	while ((pos = s.find(' ', prev_pos)) != std::string::npos) {
		auto slash_pos = s.find('/', prev_pos);
		assert(index < 3);
		vec[index] = std::stoul(s.substr(prev_pos, slash_pos - prev_pos));
		prev_pos = pos + 1;
	}

	return vec;
}

Model Model::from_obj_file(const std::string &path) {
	Model model;
	std::ifstream ifs(path);
	for (std::string line; std::getline(ifs, line); ) {
		if (line.starts_with("v ")) {
			auto vec = read_vertices(line);
			model.vectors.push_back(vec);
		}
		if (line.starts_with("f ")) {
			auto face = read_faces(line);
			model.faces.push_back(face);
		}
	}
	return model;
}
