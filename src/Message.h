#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

struct Message 
{
	Message() {
	}

	Message(const std::string& title1, const std::string& summary1, const std::string& author_name1, const std::string& author_email1) : title(title1), summary(summary1), author_name(author_name1), author_email(author_email1) {
	}

	bool operator==(const Message& it) const {
		return (it.title == title) && (it.summary == summary) && (it.author_name == author_name) && (it.author_email == author_email);
	}

	std::string title;
	std::string summary;
	std::string author_name;
	std::string author_email;
};

//bool operator==(const Message& it1, const Message& it2) {
//	return (it1.title == it2.title) && (it1.summary == it2.summary) && (it1.author_name == it2.author_name) && (it1.author_email == it2.author_email);
//}


#endif
