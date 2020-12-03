.SUFFIXES:

CXX			?= g++
CXXFLAGS	+= -std=c++17 -Wall -Wextra -Werror

CPCLDR_SRCS	= main.cpp
CPCSH_SRCS	= main.cpp

CPCLDR_OBJS	= $(addprefix src/cpcldr/, $(CPCLDR_SRCS:.c=.o))
CPCSH_OBJS	= $(addprefix src/cpcsh/, $(CPCSH_SRCS:.c=.o))

LD			?= ld
LDFLAGS		+= 

all: cpcldr cpcsh

debug: CXXFLAGS	+= -g -ggdb -fsanitize=undefined -fsanitize=address
debug: LDFLAGS	+= -g -ggdb -fsanitize=undefined -fsanitize=address
debug: cpcldr cpcsh

cpcldr: $(CPCLDR_OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

cpcsh: $(CPCSH_OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

run: cpcldr cpcsh
	xhost +
	sudo bash ./jail.sh
	sudo cp cpcldr jail/bin/
	sudo cp cpcsh jail/bin/
	DISPLAY=$$DISPLAY sudo chroot jail/ cpcldr
	xhost -

clean:
	rm -f $(CPCSH_OBJS)
	rm -f $(CPCSH_OBJS)

fclean:
	rm -f cpcldr cpcsh

re: fclean all

.PHONY: all debug run clean fclean re
