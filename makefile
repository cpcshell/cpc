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
CXXFLAGS:=-ICompat -ISources/CPinti/include

BAS:=fbc
BASFLAGS:=-i Sources/Cpcdos/Include

LD:=ld
LDFLAGS:= \
	/usr/lib/freebasic/linux-x86_64/libfb.a \
	/usr/lib/freebasic/linux-x86_64/libfbgfx.a \
	-lX11\
	-ldl \
	-lpthread \
	-lstdc++ \
	-lm \
	-lc

all: $(TARGET)

clean:
	rm -f $(TARGET)
	rm -f $(OBJ)

$(TARGET):$(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.cpp.o:%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

%.bas.o:%.bas
	$(BAS) -c -o $@ $< $(BASFLAGS)
