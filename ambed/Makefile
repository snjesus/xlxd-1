#copyright (C) by Thomas A. Early, N7TAE

# If you are going to change this path, you will
# need to update the systemd service script
BINDIR=/usr/local/bin

GCC=g++
CFLAGS=-c -W -Wall -MMD -MD -std=c++11
# uncomment the next line for gdb debugging support
#CFLAGS=-c -ggdb3 -W -Wall -MMD -MD -std=c++11
LDFLAGS=-pthread
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
DEPENDS=$(SOUCESS:.cpp=.d)
EXECUTABLE=ambed

all : $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(GCC) $(LDFLAGS) $(OBJECTS) -lftd2xx -Wl,-rpath,/usr/local/lib -o $@

.cpp.o :
	$(GCC) $(CFLAGS) $< -o $@

clean :
	$(RM) $(EXECUTABLE) *.o *.d

-include $(DEPENDS)

# The install and uninstall targets need to be run by root
install :
	cp $(EXECUTABLE) $(BINDIR)
	cp ../systemd/ambed.service /lib/systemd/system/
	systemctl enable ambed
	systemctl daemon-reload
	systemctl start ambed

uninstall :
	systemctl stop ambed
	systemctl disable ambed
	rm -f $(BINDIR)/ambed
