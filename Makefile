
MAIN = main

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)


CPP = g++

CPPFLAGS += -Wall
CPPFLAGS += -g


all: $(MAIN).elf

run: $(MAIN).elf
	./$^

%.elf: $(OBJS)
	$(CPP) $(CPPFLAGS) -o $@ $^

clean:
	rm -f $(OBJS) $(MAIN).elf
