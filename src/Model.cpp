//
// Created by Grant on 8/12/2022.
//

#include <fstream>
#include <iostream>
#include <glm.hpp>
#include "Model.h"

Model Model::from_obj_file(const std::string &path) {
	std::ifstream ifs(path);
	for (std::string line; std::getline(ifs, line); ) {
		std::cout << line << std::endl;
	}
	return {};
}
