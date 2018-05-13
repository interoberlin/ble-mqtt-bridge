
MAIN = main

SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)


CPP = g++

CPPFLAGS += -Wall
CPPFLAGS += -g
CPPFLAGS += -std=gnu++17
CPPFLAGS += -fdiagnostics-color=auto
CPPFLAGS += -Iinclude/
LDFLAGS += -lpthread
LDFLAGS += -lmosquittopp


all: $(MAIN).elf

run: $(MAIN).elf
	./$^

$(MAIN).elf: $(OBJS)
	$(CPP) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $^ -o $@

clean:
	rm -f $(OBJS) $(MAIN).elf
