#include <iostream>

#include "GMailXmlParser.h"
#include "CURLMessageGetter.h"
#include "exceptions.h"

CURLMessageGetter::CURLMessageGetter()
{
	curl_global_init(CURL_GLOBAL_SSL | CURL_GLOBAL_WIN32);

	handle_ = curl_easy_init();
	//curl_easy_setopt(handle, CURLOPT_VERBOSE, 1);
	curl_easy_setopt(handle_, CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt(handle_, CURLOPT_WRITEFUNCTION, CURLMessageGetter::write_data);
	curl_easy_setopt(handle_, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(handle_, CURLOPT_ERRORBUFFER, error_buffer_);
	curl_easy_setopt(handle_, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(handle_, CURLOPT_URL, "https://mail.google.com/mail/feed/atom");
}

CURLMessageGetter::~CURLMessageGetter()
{
	curl_easy_cleanup(handle_);
}

void CURLMessageGetter::setUserPass(const std::string& username, const std::string& password)
{
	username_ = username;
	password_ = password;
}

size_t CURLMessageGetter::write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
	CURLMessageGetter* getter = static_cast<CURLMessageGetter*>(userp);
	getter->data_ += std::string((const char*) buffer, size*nmemb);
	return size*nmemb;
}


std::vector<Message> CURLMessageGetter::getMessages()
{
	// Fetch the XML data
	std::string auth = username_ + ":" + password_;
	curl_easy_setopt(handle_, CURLOPT_USERPWD, auth.c_str());
	CURLcode result = curl_easy_perform(handle_);
	if (result) {
		throw ConnectionError(error_buffer_);
	}
	long retcode;
	curl_easy_getinfo(handle_, CURLINFO_HTTP_CODE, &retcode);
	if (retcode != 200) {
		if (retcode == 401) {
			throw UnauthorizedException();
		}
		else {
			throw ConnectionError(retcode);
		}
	}

	// Parse the XML stream
	if (!GMailXmlParser::parse(data_)) {
		throw ParseError();
	}

	data_.clear();

	return GMailXmlParser::getItems();
}
