#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

int main(int argc, char* argv[])
{
	CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();
	CppUnit::TextUi::TestRunner runner;
	runner.addTest( registry.makeTest() );
	return (runner.run("") ? 0 : 1);
}
