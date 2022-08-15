//
// Created by Grant on 8/12/2022.
//

#include <fstream>
#include <iostream>
#include "Model.h"

glm::vec3 read_vertices(const std::string &s) {
	glm::vec3 vec;
	unsigned int index = 0;
	size_t prev_pos = s.find(' ') + 1;
	size_t pos;
	while ((pos = s.find(' ', prev_pos)) != std::string::npos) {
		assert(index < 2);
		vec[index] = std::stof(s.substr(prev_pos, pos - prev_pos));
		prev_pos = pos + 1;
		index++;
	}

	vec[2] = std::stof(s.substr(prev_pos, s.size() - prev_pos));

	return vec;
}

void read_faces(const std::string &s, std::vector<unsigned int> &faces) {
	unsigned int index = 0;
	size_t prev_pos = s.find(' ') + 1;
	size_t pos;
	while ((pos = s.find(' ', prev_pos)) != std::string::npos) {
		auto slash_pos = s.find('/', prev_pos);
		assert(index < 2);
		faces.push_back(std::stoul(s.substr(prev_pos, slash_pos - prev_pos)) - 1);
		prev_pos = pos + 1;
		index++;
	}

	auto slash_pos = s.find('/', prev_pos);
	faces.push_back(std::stoul(s.substr(prev_pos, slash_pos - prev_pos)) - 1);
}

Model Model::from_obj_file(const std::string &path) {
	Model model;
	std::ifstream ifs(path);
	for (std::string line; std::getline(ifs, line);) {
		if (line.starts_with("v ")) {
			auto vec = read_vertices(line);
			model.vectors.push_back(vec);
		}
		if (line.starts_with("f ")) {
			read_faces(line, model.faces);
		}
	}
	return model;
}
