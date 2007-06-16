#ifndef MESSAGEGETTER_H
#define MESSAGEGETTER_H

#include <vector>
#include <string>

#include "Message.h"

class MessageGetter
{
public:
	virtual ~MessageGetter() { }

	virtual void setUserPass(const std::string& username, const std::string& password) = 0;
	virtual std::vector<Message> getMessages() = 0;
};

#endif
