#WARNINGS = -Wall -Wextra -Wno-switch -Wno-sign-compare -Wno-missing-braces -Wno-unused-parameter -Wno-char-subscripts
#DEBUG = -g
#PROFILE = -pg
OTHERS = -O0 -std=c++11

ODIR = obj
DDIR = .deps

TARGET = cmulti

OS  = $(shell uname -o)
CXX = g++

CFLAGS = $(WARNINGS) $(DEBUG) $(PROFILE) $(OTHERS)

LDFLAGS = -luv -lncursesw

SOURCES = $(wildcard *.cpp)
_OBJS = $(SOURCES:.cpp=.o)
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

all: $(TARGET)
	@

$(TARGET): $(ODIR) $(DDIR) $(OBJS)
	$(CXX) -o $(TARGET) $(CFLAGS) $(OBJS) $(LDFLAGS) 

$(ODIR):
	mkdir $(ODIR)

$(DDIR):
	@mkdir $(DDIR)

$(ODIR)/%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(ODIR)/*.o

-include $(SOURCES:%.cpp=$(DEPDIR)/%.P)
