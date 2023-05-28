SYS := $(shell g++ -dumpmachine)
ifneq (, $(findstring apple, $(SYS)))
CFLAGS	= -Wall -g -std=c++11 
LFLAGS	= -lpthread
else
CFLAGS	= -Wall -g -std=c++11
LFLAGS	= -lpthread
endif

CC		= g++

srcs = $(wildcard *.cpp)
objs = $(srcs:.cpp=.o)
deps = $(srcs:.cpp=.d)

a.out: $(objs)
	$(CC) $^ -g -o $@ $(LFLAGS)

%.o: %.cpp
	$(CC) -MMD -MP $(CFLAGS) -c $< -o $@

.PHONY: clean

# $(RM) is rm -f by default
clean:
	$(RM) $(objs) $(deps) a.out core

-include $(deps)
