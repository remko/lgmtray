#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <sstream>

#include "ConfigManager.h"
#include "exceptions.h"

class TestConfigReader : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(TestConfigReader);
	CPPUNIT_TEST(testRead);
	CPPUNIT_TEST_SUITE_END();

public:
	TestConfigReader();
	virtual ~TestConfigReader();

	void testRead();
};


TestConfigReader::TestConfigReader()
{
}

TestConfigReader::~TestConfigReader()
{
}

void TestConfigReader::testRead()
{
	ConfigManager manager;

	std::string config1;
	manager.read(config1);
	CPPUNIT_ASSERT_EQUAL(0U, (unsigned int) manager.keys().size());
	manager.clear();

	std::string config2("# Comments\n");
	manager.read(config2);
	CPPUNIT_ASSERT_EQUAL(0U, (unsigned int) manager.keys().size());
	manager.clear();

	std::string config3("\n");
	manager.read(config3);
	CPPUNIT_ASSERT_EQUAL(0U, (unsigned int) manager.keys().size());
	manager.clear();

	std::string config4("key1=value1\n");
	manager.read(config4);
	CPPUNIT_ASSERT_EQUAL(1U, (unsigned int) manager.keys().size());
	CPPUNIT_ASSERT_EQUAL(std::string("key1"),manager.keys()[0]);
	CPPUNIT_ASSERT_EQUAL(std::string("value1"),manager.getValue("key1"));
	manager.clear();

	std::string config5("key1 = value1 \n");
	manager.read(config5);
	CPPUNIT_ASSERT_EQUAL(1U, (unsigned int) manager.keys().size());
	CPPUNIT_ASSERT_EQUAL(std::string("key1 "),manager.keys()[0]);
	CPPUNIT_ASSERT_EQUAL(std::string(" value1 "),manager.getValue("key1 "));
	manager.clear();

	std::string config6("key1=value1");
	manager.read(config6);
	CPPUNIT_ASSERT_EQUAL(1U, (unsigned int) manager.keys().size());
	CPPUNIT_ASSERT_EQUAL(std::string("key1"),manager.keys()[0]);
	CPPUNIT_ASSERT_EQUAL(std::string("value1"),manager.getValue("key1"));
	manager.clear();

	std::string config7("key1=value1\nkey2=value2\n");
	manager.read(config7);
	CPPUNIT_ASSERT_EQUAL(2U, (unsigned int) manager.keys().size());
	CPPUNIT_ASSERT_EQUAL(std::string("key1"),manager.keys()[0]);
	CPPUNIT_ASSERT_EQUAL(std::string("key2"),manager.keys()[1]);
	CPPUNIT_ASSERT_EQUAL(std::string("value1"),manager.getValue("key1"));
	CPPUNIT_ASSERT_EQUAL(std::string("value2"),manager.getValue("key2"));
	manager.clear();

	std::string config8("# Comments\n\nkey1=value1\n\nkey2=value2\n");
	manager.read(config8);
	CPPUNIT_ASSERT_EQUAL(2U, (unsigned int) manager.keys().size());
	CPPUNIT_ASSERT_EQUAL(std::string("key1"),manager.keys()[0]);
	CPPUNIT_ASSERT_EQUAL(std::string("key2"),manager.keys()[1]);
	CPPUNIT_ASSERT_EQUAL(std::string("value1"),manager.getValue("key1"));
	CPPUNIT_ASSERT_EQUAL(std::string("value2"),manager.getValue("key2"));
	manager.clear();

	std::string config9("=value");
	CPPUNIT_ASSERT_THROW(manager.read(config9),ConfigParseError);
	std::string config10("key=");
	CPPUNIT_ASSERT_THROW(manager.read(config10),ConfigParseError);
}

CPPUNIT_TEST_SUITE_REGISTRATION(TestConfigReader);
