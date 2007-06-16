#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>

class UnauthorizedException
{
};

class ParseError
{
};

class ConnectionError
{
public:
	ConnectionError(int code) : code_(code) { }
	ConnectionError(std::string message) : message_(message) { }

	std::string getMessage() const {
		if (message_.size() > 0) {
			return message_;
		}
		if (code_ > 0) {
			// FIXME
			return std::string("Error code: "); // + std::string(code_);
		}
		return "";
	}

private:
	int code_;
	std::string message_;
};

class ConfigParseError
{
public:
	ConfigParseError(std::string line) : line_(line) { }

	std::string getLine() const {
		return line_;
	}

private:
	std::string line_;
};

#endif
