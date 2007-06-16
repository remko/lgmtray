CXXFLAGS=$(shell curl-config --cflags) -I/usr/include/libxml2  -I.. -Wall -g
LDFLAGS=$(shell curl-config --libs) -lxml2 -lcppunit
TESTSUITE=testsuite
OBJECTS= \
	../GMailXmlParser.o TestGMailXmlParser.o \
	../MessageManager.o TestMessageManager.o \
	../ConfigManager.o TestConfigManager.o \
	testsuite.o

$(TESTSUITE): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $(TESTSUITE) $(OBJECTS)

clean:
	-rm -rf $(OBJECTS) $(TESTSUITE)

check: $(TESTSUITE)
	./$(TESTSUITE)
