#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <string>
#include <map>
#include <vector>

class ConfigManager
{
public:
	ConfigManager();

	void clear();
	void read(const std::string& input);
	std::string getValue(const std::string& key) const;
	std::vector<std::string> keys() const;

private:
	std::map<std::string,std::string> values_;
};

#endif
