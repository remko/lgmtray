#include <algorithm>

#include "MessageManager.h"
#include "MessageListener.h"
#include "MessageGetter.h"

MessageManager::MessageManager(MessageGetter* getter, MessageListener* listener) : getter_(getter), listener_(listener)
{
}

void MessageManager::fetchMessages()
{
	std::vector<Message> previous_messages = messages_;
	messages_ = getter_->getMessages();

	// New messages
	for(std::vector<Message>::const_iterator i = messages_.begin(); i != messages_.end(); ++i) {
		if (std::find(previous_messages.begin(),previous_messages.end(),*i) == previous_messages.end()) {
			listener_->newMessage(*i);
		}
	}
	
	// No new messages
	if (messages_.size() == 0 && previous_messages.size() > 0) {
		listener_->noMessages();
	}
}
