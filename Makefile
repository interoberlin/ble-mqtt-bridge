
#
# Toolchain
#
CC = gcc
CPP = g++

CPPFLAGS += -Wall
CPPFLAGS += -g
CPPFLAGS += -std=gnu++17
CPPFLAGS += -fdiagnostics-color=auto
CPPFLAGS += -Iinclude/
LDFLAGS += -ltinyb
LDFLAGS += -lpthread
LDFLAGS += -lmosquittopp

#
# Source files
#
COMMON_SRCS = src/BLEClient.cpp src/MQTTClientRobby.cpp src/Bridge.cpp
TARGET1 = floorsensord
SRCS1 = $(COMMON_SRCS) src/floorsensord.cpp
OBJS1 = $(SRCS1:.cpp=.o)

TARGET2 = robbyd
SRCS2 = $(COMMON_SRCS) src/robbyd.cpp
OBJS2 = $(SRCS2:.cpp=.o)

#
# make targets
#
all: $(TARGET1) $(TARGET2) $(TARGET3)

run: $(TARGET1)
	./$<

$(TARGET1): $(OBJS1)
	$(CPP) $^ $(CPPFLAGS) $(LDFLAGS) -o $@

$(TARGET2): $(OBJS2)
	$(CPP) $^ $(CPPFLAGS) $(LDFLAGS) -o $@

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $^ -o $@

clean:
	rm -f $(TARGET1) $(OBJS1) $(TARGET2) $(OBJS2)
