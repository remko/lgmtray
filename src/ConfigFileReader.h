#ifndef CONFIGFILEREADER_H
#define CONFIGFILEREADER_H

class ConfigManager;

class ConfigFileReader
{
public:
	ConfigFileReader(const std::string& filename, ConfigManager *);

	void read();

private:
	std::string filename_;
	ConfigManager* manager_;
};

#endif
