#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include <vector>

#include "Message.h"

class MessageGetter;
class MessageListener;

class MessageManager
{
public:
	MessageManager(MessageGetter* getter, MessageListener* listener);

	void fetchMessages();

private:
	MessageGetter* getter_;
	MessageListener* listener_;
	std::vector<Message> messages_;
};

#endif
