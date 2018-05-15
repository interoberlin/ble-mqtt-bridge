
MAIN = floorsensord

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

#SRCS = $(wildcard src/*.cpp)
SRCS = src/floorsensord.cpp src/MQTTClient.cpp src/BLEClient.cpp src/Bridge.cpp
OBJS = $(SRCS:.cpp=.o)
ELF = $(MAIN).elf


all: $(ELF)

run: $(ELF)
	./$<

$(ELF): $(OBJS)
	$(CPP) $^ $(CPPFLAGS) $(LDFLAGS) -o $@

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $^ -o $@

clean:
	rm -f $(TARGET1) $(OBJS1) $(TARGET2) $(OBJS2)
