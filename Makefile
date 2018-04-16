
MAIN = hellotinyb

CC = gcc
CPP = g++

CPPFLAGS += -Wall
CPPFLAGS += -std=c++11
CPPFLAGS += -fdiagnostics-color
LDFLAGS += -L../tinyb/build/src
LDFLAGS += -ltinyb

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
ELF = $(MAIN).elf


all: $(ELF)

$(ELF): $(OBJS)
	$(CPP) $(CPPFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $^ -o $@

clean:
	rm -f $(OBJS) $(ELF)
