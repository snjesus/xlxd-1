CC=g++
CFLAGS=-c -ggdb3 -W -Wall -MMD -MD -std=c++11
LDFLAGS=-pthread
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
DEPENDS=$(SOUCESS:.cpp-.d)
EXECUTABLE=ambed

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -lftd2xx -Wl,-rpath,/usr/local/lib -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) $(EXECUTABLE) *.o *.d

-include $(DEPENDS)

install:
	mkdir -p /ambed
	cp $(EXECUTABLE) /ambed/
	cp ./run /ambed/
	cp ../systemd/ambed.service /lib/systemd/system/
	systemctl enable ambed
	# ambed is now controlled with systemd. The service has been enabled. You can start it
	# with systemctl start ambed.

