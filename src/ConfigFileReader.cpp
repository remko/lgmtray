#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "ConfigFileReader.h"
#include "ConfigManager.h"
#include "exceptions.h"

ConfigFileReader::ConfigFileReader(const std::string& filename, ConfigManager* manager) : filename_(filename), manager_(manager)
{
}

void ConfigFileReader::read() 
{
	std::ifstream config_data(filename_.c_str());
	if (config_data.fail()) {
		std::cerr << "Cannot open " << filename_ << std::endl;
		exit(-1);
	}
	std::string config_string;
	while (!config_data.eof()) {
		std::string line;
		getline(config_data,line);
		config_string.append(line + "\n");
	}
	try {
		manager_->read(config_string);
	}
	catch (ConfigParseError e) {
		std::cerr << "Invalid input in " << filename_ << ": '" << e.getLine() << "'" << std::endl;
		exit(-1);
	}
}
