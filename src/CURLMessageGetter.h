#ifndef CURLMESSAGEGETTER_H
#define CURLMESSAGEGETTER_H

#include <string>
#include <curl/curl.h>

#include "Message.h"
#include "MessageGetter.h"

class CURLMessageGetter : public MessageGetter
{
public:
	CURLMessageGetter();
	virtual ~CURLMessageGetter();

	void setUserPass(const std::string& username, const std::string& password);
	std::vector<Message> getMessages();

private:
	// CURL interface
	static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);

private:
	char error_buffer_[CURL_ERROR_SIZE];
	CURL* handle_;
	std::string data_;
	std::string username_;
	std::string password_;
};

#endif
