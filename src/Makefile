include ../Makefile.config

LIB_TARGET=libgmtray.a
LIB_OBJECTS=GMailXmlParser.o CURLMessageGetter.o MessageManager.o FreeDesktopTray.o Timer.o ConfigManager.o ConfigFileReader.o

TARGET=gmtray
TARGET_OBJECTS=main.o libgmtray.a

$(TARGET): $(TARGET_OBJECTS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(TARGET_OBJECTS)

$(LIB_TARGET): $(LIB_OBJECTS)
	ar cr $@ $(LIB_OBJECTS)

clean:
	-rm -rf $(TARGET_OBJECTS) $(TARGET) $(LIB_OBJECTS) $(LIB_TARGET)
	$(MAKE) -C unittest clean

check: $(LIB_TARGET)
	$(MAKE) -C unittest check
