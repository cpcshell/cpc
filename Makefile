.SUFFIXES:

CC			?= gcc
CFLAGS		+= -std=c99 -Wall -Wextra -Werror -Iinclude \
				`pkg-config --cflags x11`
LDFLAGS		+= `pkg-config --libs x11` -lzmq -lczmq

COMMON_SRCS	= logger.c
WM_SRCS		= wm.c
CPCLDR_SRCS	= main.c daemonize.c
CPCSH_SRCS	= main.c

COMMON_OBJS	= $(addprefix src/common/, $(COMMON_SRCS:.c=.o))
WM_OBJS		= $(addprefix window_manager/, $(WM_SRCS:.c=.o))
CPCLDR_OBJS	= $(addprefix src/cpcldr/, $(CPCLDR_SRCS:.c=.o) $(WM_OBJS)) \
				$(COMMON_OBJS)
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
	rm -f $(CPCLDR_OBJS)
	rm -f $(CPCSH_OBJS)

fclean: clean
	rm -f cpcldr cpcsh

re: fclean all

make-pot:
	xgettext --keyword=_ --language=C --add-comments --sort-output -o po/cpcdos.pot $(CPCLDR_OBJS:.o=.c) $(CPCSH_OBJS:.o)

.PHONY: all debug run clean fclean re
