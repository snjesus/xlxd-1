CC=g++
#CFLAGS=-c -ggdb3 -W -Wall -MMD -MD -std=c++11
CFLAGS=-c -W -Wall -MMD -MD -std=c++11
LDFLAGS=-pthread
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
DEPENDS=$(SOURCES:.cpp=.d)
EXECUTABLE=xlxd

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o :
	$(CC) $(CFLAGS) $< -o $@

clean :
	$(RM) $(EXECUTABLE) *.o *.d

-include $(DEPENDS)

install :
	mkdir -p /xlxd
	cp -f $(EXECUTABLE) /xlxd/
	[ -f /xlxd/xlxd.blacklist ] && \
		cp ../config/xlxd.blacklist /xlxd/xlxd.blacklist.sample || \
		cp ../config/xlxd.blacklist /xlxd/xlxd.blacklist
	[ -f /xlxd/xlxd.whitelist ] && \
		cp ../config/xlxd.whitelist /xlxd/xlxd.whitelist.sample || \
		cp ../config/xlxd.whitelist /xlxd/xlxd.whitelist
	[ -f /xlxd/xlxd.interlink ] && \
		cp ../config/xlxd.interlink /xlxd/xlxd.interlink.sample || \
		cp ../config/xlxd.interlink /xlxd/xlxd.interlink
	cp -f ../systemd/xlxd.service /lib/systemd/system/
	systemctl enable xlxd
	# xlxd is now controlled with systemd. The service has been enabled. You can start it
	# with systemctl start xlxd
