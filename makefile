.SUFFIXES:

SRCDIR=Sources/
OBJDIR=Objects/
BINDIR=Binaries/

TARGET=cpcldr

SRCCXX=$(shell find $(SRCDIR) -name '*.cpp')
SRCBAS=$(shell find $(SRCDIR) -name '*.bas')

OBJ=$(SRCCXX:.cpp=.cpp.o) \
	$(SRCBAS:.bas=.bas.o)

CXX?=g++
CXXFLAGS:= \
	-g3 \
	-MD \
	-IIncludes/ \
	-ICompat \
	-ISources/CPinti/include \
	-std=c++17 \
	-Wall \
	-Wextra \
	-Werror

BAS:=fbc
BASFLAGS:=-gen gcc -i Includes/ -i Sources/Cpcdos/Include -target linux-x86_64 -g

LD:=ld
LDFLAGS:= \
	/usr/lib/freebasic/linux-x86_64/libfbgfx.a \
	/usr/lib/freebasic/linux-x86_64/libfb.a \
	-lX11 \
	-lXrandr \
	-lXext \
	-lXpm \
	-lncurses \
	-ldl \
	-lz \
	-lzip \
	-lpng \
	-lpthread \
	-lstdc++ \

all: $(TARGET)

debug: CXXFLAGS	+= -fsanitize=undefined -fsanitize=address
debug: LDFLAGS	+= -fsanitize=undefined -fsanitize=address
debug: BASFLAGS += -Wc -fsanitize=undefined -Wc -fsanitize=address

clean:
	rm -f $(TARGET)
	rm -f $(OBJ)
	rm -f $(OBJ:.o=.d)

run: $(TARGET)
	xhost +
	sudo bash ./jail.sh
	sudo cp $(TARGET) jail/bin/$(TARGET)
	DISPLAY=$$DISPLAY sudo chroot jail/ cpcldr
	xhost -

debug : $(TARGET)

re: clean all

$(TARGET):$(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.cpp.o:%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

%.bas.o:%.bas
	$(BAS) -c -o $@ $< $(BASFLAGS)

-include $(OBJ:.o=.d)

.PHONY: all clean run re debug
