#ifndef MESSAGELISTENER_H
#define MESSAGELISTENER_H

class Message;

class MessageListener
{
public:
	virtual ~MessageListener() { }

	virtual void newMessage(const Message& message) = 0;
	virtual void noMessages() = 0;
};

#endif
