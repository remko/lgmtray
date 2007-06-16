#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <string>

#include "GMailXmlParser.h"

class TestGMailXmlParser : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(TestGMailXmlParser);
	CPPUNIT_TEST(testNoItem);
	CPPUNIT_TEST(testMultipleItems);
	CPPUNIT_TEST_SUITE_END();

public:
	TestGMailXmlParser();
	virtual ~TestGMailXmlParser();

	void setUp();
	void tearDown();

	void testNoItem();
	void testMultipleItems();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestGMailXmlParser);

TestGMailXmlParser::TestGMailXmlParser()
{
}

TestGMailXmlParser::~TestGMailXmlParser()
{
}

void TestGMailXmlParser::setUp()
{
	GMailXmlParser::initialize();
}

void TestGMailXmlParser::tearDown()
{
	GMailXmlParser::deinitialize();
}

void TestGMailXmlParser::testNoItem()
{
	std::string test_xml =
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<feed>\n"
		"</feed>\n";
	CPPUNIT_ASSERT(GMailXmlParser::parse(test_xml));
	CPPUNIT_ASSERT_EQUAL((size_t) 0, GMailXmlParser::getItems().size());
}

void TestGMailXmlParser::testMultipleItems()
{
	std::string test_xml =
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<feed>\n"
		"  <entry>\n"
		"    <title>Title 1</title>\n"
		"    <summary>Summary 1</summary>\n"
		"    <author>\n"
		"      <name>Author 1</name>\n"
		"      <email>Email 1</email>\n"
		"    </author>\n"
		"  </entry>\n"
		"  <entry>\n"
		"    <title>Title 2</title>\n"
		"    <summary>Summary 2</summary>\n"
		"    <author>\n"
		"      <name>Author 2</name>\n"
		"      <email>Email 2</email>\n"
		"    </author>\n"
		"  </entry>\n"
		"</feed>\n";
	CPPUNIT_ASSERT(GMailXmlParser::parse(test_xml));
	
	const std::vector<Message>& items = GMailXmlParser::getItems();
	CPPUNIT_ASSERT_EQUAL((size_t) 2,items.size());
	CPPUNIT_ASSERT_EQUAL(std::string("Title 1"), items[0].title);
	CPPUNIT_ASSERT_EQUAL(std::string("Summary 1"), items[0].summary);
	CPPUNIT_ASSERT_EQUAL(std::string("Author 1"), items[0].author_name);
	CPPUNIT_ASSERT_EQUAL(std::string("Email 1"), items[0].author_email);
	CPPUNIT_ASSERT_EQUAL(std::string("Title 2"), items[1].title);
	CPPUNIT_ASSERT_EQUAL(std::string("Summary 2"), items[1].summary);
	CPPUNIT_ASSERT_EQUAL(std::string("Author 2"), items[1].author_name);
	CPPUNIT_ASSERT_EQUAL(std::string("Email 2"), items[1].author_email);
}
