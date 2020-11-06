.SUFFIXES:

SRCDIR=Sources/
OBJDIR=Objects/
BINDIR=Binaries/

TARGET=cpcldr

SRCCXX=$(shell find $(SRCDIR) -name '*.cpp')
SRCBAS=$(shell find $(SRCDIR) -name '*.bas')

OBJ=$(SRCCXX:.cpp=.cpp.o) \
	$(SRCBAS:.bas=.bas.o)

CXX:=g++
CXXFLAGS:=-MD -IIncludes/ -ICompat -ISources/CPinti/include

BAS:=fbc
BASFLAGS:=-i Includes/ -i Sources/Cpcdos/Include -target linux-x86_64 

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
	-lstdc++

all: $(TARGET)

clean:
	rm -f $(TARGET)
	rm -f $(OBJ)
	rm -f $(OBJ:.o=.d)

$(TARGET):$(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.cpp.o:%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

%.bas.o:%.bas
	$(BAS) -c -o $@ $< $(BASFLAGS)

-include $(OBJ:.o=.d)
