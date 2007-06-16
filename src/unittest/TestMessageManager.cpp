#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include "MessageManager.h"
#include "MessageListener.h"
#include "MessageGetter.h"
#include "MessageListener.h"

// -----------------------------------------------------------------------------

class DummyMessageListener : public MessageListener
{
public:
	DummyMessageListener() : nb_nomessages_called_(0) {
	}

	virtual void newMessage(const Message& message) {
		items_.push_back(message);
	}

	virtual void noMessages() {
		nb_nomessages_called_++;
	}

	std::vector<Message> items_;
	unsigned int nb_nomessages_called_;
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

// -----------------------------------------------------------------------------

class TestMessageManager : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(TestMessageManager);
	CPPUNIT_TEST(testNotify);
	CPPUNIT_TEST_SUITE_END();

public:
	TestMessageManager();
	virtual ~TestMessageManager();

	void testNotify();
};


TestMessageManager::TestMessageManager()
{
}

TestMessageManager::~TestMessageManager()
{
}

void TestMessageManager::testNotify()
{
	DummyMessageListener listener;
	DummyMessageGetter getter;
	MessageManager manager(&getter, &listener);

	Message item1("Title 1","Summary 1","Author 1","Email 1");
	getter.items_.push_back(item1);
	manager.fetchMessages();
	CPPUNIT_ASSERT_EQUAL(0U, listener.nb_nomessages_called_);
	CPPUNIT_ASSERT_EQUAL(1U, (unsigned int) listener.items_.size());
	CPPUNIT_ASSERT(item1 == listener.items_[0]);

	listener.items_.clear();
	manager.fetchMessages();
	CPPUNIT_ASSERT_EQUAL(0U, listener.nb_nomessages_called_);
	CPPUNIT_ASSERT_EQUAL(0U, (unsigned int) listener.items_.size());

	Message item2("Title 2","Summary 2","Author 2","Email 2");
	Message item3("Title 3","Summary 3","Author 3","Email 3");
	getter.items_.push_back(item2);
	getter.items_.push_back(item3);
	manager.fetchMessages();
	CPPUNIT_ASSERT_EQUAL(0U, listener.nb_nomessages_called_);
	CPPUNIT_ASSERT_EQUAL(2U, (unsigned int) listener.items_.size());
	CPPUNIT_ASSERT(item2 == listener.items_[0]);
	CPPUNIT_ASSERT(item3 == listener.items_[1]);

	getter.items_.clear();
	listener.items_.clear();
	manager.fetchMessages();
	CPPUNIT_ASSERT_EQUAL(1U, listener.nb_nomessages_called_);
	CPPUNIT_ASSERT_EQUAL(0U, (unsigned int) listener.items_.size());

	manager.fetchMessages();
	CPPUNIT_ASSERT_EQUAL(1U, listener.nb_nomessages_called_);
	CPPUNIT_ASSERT_EQUAL(0U, (unsigned int) listener.items_.size());
}

CPPUNIT_TEST_SUITE_REGISTRATION(TestMessageManager);
