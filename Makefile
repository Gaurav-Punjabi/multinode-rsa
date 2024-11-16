CXX=g++
CXXFLAGS = -Wall -Wextra -02

TARGET = test

SRCS = test.cpp

OBJS = $(SRCS.cpp=.o)

all: $(TARGET)

$(TARGET): $(SRCS)
  $(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
  rm -rf $(TARGET) $(OBJS)
