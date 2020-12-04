.SUFFIXES:

CC			?= gcc
CFLAGS		+= -std=c99 -Wall -Wextra -Werror -Iinclude
LDFLAGS		+= 

COMMON_SRCS	= logger.c
CPCLDR_SRCS	= main.c daemonize.c
CPCSH_SRCS	= main.c

COMMON_OBJS	= $(addprefix src/common/, $(COMMON_SRCS:.c=.o))
CPCLDR_OBJS	= $(addprefix src/cpcldr/, $(CPCLDR_SRCS:.c=.o)) $(COMMON_OBJS)
CPCSH_OBJS	= $(addprefix src/cpcsh/, $(CPCSH_SRCS:.c=.o)) $(COMMON_OBJS)

all: cpcldr cpcsh

debug: CFLAGS	+= -g -ggdb -fsanitize=undefined -fsanitize=address
debug: LDFLAGS	+= -g -ggdb -fsanitize=undefined -fsanitize=address
debug: cpcldr cpcsh

cpcldr: $(CPCLDR_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

cpcsh: $(CPCSH_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

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

fclean: clean
	rm -f cpcldr cpcsh

re: fclean all

.PHONY: all debug run clean fclean re
