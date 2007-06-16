#include <sstream>
#include <iostream>

#include "ConfigManager.h"
#include "exceptions.h"

ConfigManager::ConfigManager()
{
}

void ConfigManager::clear()
{
	values_.clear();
}

void ConfigManager::read(const std::string& input)
{
	std::istringstream data(input);
	while (!data.eof()) {
		std::string line;
		getline(data,line);
		if (line.find_first_of('#') == 0) {
			continue;
		}
		else if (line.empty()) {
			continue;
		}
		else {
			int i = line.find_first_of('=');
			if (i > 0) {
				if ((unsigned int) i == line.size() - 1) {
					throw ConfigParseError(line);
				}
				values_[line.substr(0,i)] = line.substr(i+1,line.size()-i);
			}
			else {
				throw ConfigParseError(line);
			}
		}
	}
}

std::string ConfigManager::getValue(const std::string& key) const
{
	std::map<std::string,std::string>::const_iterator it = values_.find(key);
	if (it != values_.end()) {
		return (*it).second;
	}
	else {
		return "";
	}
}

std::vector<std::string> ConfigManager::keys() const
{
	std::vector<std::string> result;
	for (std::map<std::string,std::string>::const_iterator it = values_.begin(); it != values_.end(); ++it) {
		result.push_back(it->first);
	}
	return result;
}
