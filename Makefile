
MAIN = main

CC = gcc
CPP = g++

CPPFLAGS += -Wall
CPPFLAGS += -std=gnu++17
CPPFLAGS += -fdiagnostics-color=auto
CPPFLAGS += -Iinclude/
LDFLAGS += -L../tinyb/build/src
LDFLAGS += -ltinyb
#LDFLAGS += -lthread
LDFLAGS += -lpthread

#SRCS = $(wildcard src/*.cpp)
SRCS = src/main.cpp
OBJS = $(SRCS:.cpp=.o)
ELF = $(MAIN).elf


all: $(ELF)

$(ELF): $(OBJS)
	$(CPP) $(CPPFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $^ -o $@

clean:
	rm -f $(OBJS) $(ELF)
