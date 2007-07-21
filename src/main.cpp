#include <iostream>
#include <cassert>
#include <sstream>

#include "ConfigFileReader.h"
#include "ConfigManager.h"
#include "GMailXmlParser.h"
#include "CURLMessageGetter.h"
#include "FreeDesktopTray.h"
#include "Message.h"
#include "MessageManager.h"
#include "MessageListener.h"
#include "Timer.h"
#include "TimerListener.h"
#include "exceptions.h"

#define CONFIG_FILE ".gmtray"

MessageGetter* getter_ = NULL;
MessageListener* listener_ = NULL;
MessageManager* manager_ = NULL;
TimerListener* timer_listener_ = NULL;

class MyTimerListener : public TimerListener
{
public:
	void alarm() {
		try {
			manager_->fetchMessages();
		}
		catch (ConnectionError e) {
			FreeDesktopTray::instance()->setIcon(FreeDesktopTray::Icon_Error);
			std::cerr << "A connection error occurred: " << e.getMessage() << std::endl;
			exit(-1);
		} 
		catch (UnauthorizedException) {
			FreeDesktopTray::instance()->setIcon(FreeDesktopTray::Icon_Error);
			std::cerr << "Invalid username/password" << std::endl;
			exit(-1);
		}
		catch (ParseError e) {
			FreeDesktopTray::instance()->setIcon(FreeDesktopTray::Icon_Error);
			std::cerr << "Parse error" << std::endl;
			exit(-1);
		}
		catch (...) {
			FreeDesktopTray::instance()->setIcon(FreeDesktopTray::Icon_Error);
			std::cout << "An unexpected error has occurred";
			exit(-1);
		}
	}
};

class MyListener : public MessageListener 
{
public:
	MyListener() : have_messages_(false) { }

	void newMessage(const Message& message) {
		if (!have_messages_) {
			FreeDesktopTray::instance()->setIcon(FreeDesktopTray::Icon_Messages);
			have_messages_ = true;
		}
		std::stringstream popup;
		popup << "New message" << std::endl;
		popup << "From: " << message.author_name << " <" << message.author_email << ">" << std::endl;
		popup << "Subject: " << message.title << std::endl;
		FreeDesktopTray::instance()->showPopup(popup.str());
	}

	void noMessages() {
		FreeDesktopTray::instance()->setIcon(FreeDesktopTray::Icon_NoMessages);
		have_messages_ = false;
	}

private:
	bool have_messages_;
};


class DummyMessageGetter : public MessageGetter
{
public:
	void setUserPass(const std::string& username, const std::string& password) {
	}

	std::vector<Message> getMessages() {
		return items_;
	}

	std::vector<Message> items_;
};

int main(int argc, char** argv)
{
	// Parser
	GMailXmlParser::initialize();

	// Read configuration
	ConfigManager config;
	std::string home(getenv("HOME"));
	assert(!home.empty());
	std::string config_file(home + "/" + CONFIG_FILE);
	ConfigFileReader config_reader(config_file, &config);
	config_reader.read();

	std::string username, password;
	int check_interval = 300;
	username = config.getValue("username");
	if (username.empty()) {
		std::cerr << "Missing username in " << config_file << std::endl;
		exit(-1);
	}
	password = config.getValue("password");
	if (password.empty()) {
		std::cerr << "Missing password in " << config_file << std::endl;
		exit(-1);
	}
	if (!config.getValue("dock").empty()) {
		FreeDesktopTray::instance()->setDock(config.getValue("dock") != "false");
	}
	if (!config.getValue("check_interval").empty()) {
		check_interval = atoi(config.getValue("check_interval").c_str());
		assert(check_interval > 0);
	}
	if (!config.getValue("check_popup_timeout").empty()) {
		int popup_timeout = atoi(config.getValue("check_popup_timeout").c_str());
		assert(popup_timeout >= 0);
		FreeDesktopTray::instance()->setPopupTimeout((unsigned int) popup_timeout);
	}

	// Getter
	if (config.getValue("dummy_get") == "true") {
		getter_ = new DummyMessageGetter();
	}
	else {
		getter_ = new CURLMessageGetter();
	}
	getter_->setUserPass(config.getValue("username"),config.getValue("password"));
	
	// Listener
	listener_ = new MyListener();

	// Manager
	manager_ = new MessageManager(getter_,listener_);

	// Alarm
	MyTimerListener* timer_listener_ = new MyTimerListener();
	Timer::instance()->addListener(timer_listener_);
	Timer::instance()->setInterval(check_interval);
	timer_listener_->alarm();

	FreeDesktopTray::instance()->start();

	GMailXmlParser::deinitialize();
	return 0;
}
