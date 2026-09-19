CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2
CPPFLAGS = -Iinclude
TARGET = redis-lite
SRCS = src/main.cpp src/store.cpp src/protocol.cpp
OBJS = $(SRCS:.cpp=.o)

ifeq ($(OS),Windows_NT)
  LDLIBS = -lws2_32
else
  # Linux needs no extra libs for BSD sockets
  LDLIBS =
endif

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDLIBS)

src/%.o: src/%.cpp include/store.hpp include/protocol.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe
