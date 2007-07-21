include ../../Makefile.config

LDFLAGS+=-lcppunit
CPPFLAGS+=-I..

TESTSUITE=testsuite
OBJECTS=testsuite.o TestGMailXmlParser.o  TestMessageManager.o TestConfigManager.o
EXTRA_OBJECTS=../liblgmtray.a

$(TESTSUITE): $(OBJECTS) $(EXTRA_OBJECTS)
	$(CXX) $(LDFLAGS) -o $(TESTSUITE) $(OBJECTS) $(EXTRA_OBJECTS)

clean:
	-rm -rf $(OBJECTS) $(TESTSUITE)

check: $(TESTSUITE)
	./$(TESTSUITE)
