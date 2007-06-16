CXXFLAGS= \
	$(shell curl-config --cflags) \
	$(shell xml2-config --cflags) \
	-Wall
LDFLAGS= \
	-L/usr/X11R6/lib/ \
	$(shell curl-config --libs) \
	$(shell xml2-config --libs) \
	-lXpm -lX11
TARGET=gnotify
OBJECTS=main.o GMailXmlParser.o CURLMessageGetter.o MessageManager.o FreeDesktopTray.o Timer.o ConfigManager.o ConfigFileReader.o

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJECTS)

clean:
	-rm -rf $(OBJECTS) $(TARGET)
