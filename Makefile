CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2
CPPFLAGS = -Iinclude
TARGET = redis-lite
SRCS = src/main.cpp src/store.cpp src/protocol.cpp
LIB_SRCS = src/store.cpp src/protocol.cpp
OBJS = $(SRCS:.cpp=.o)

ifeq ($(OS),Windows_NT)
  LDLIBS = -lws2_32
else
  LDLIBS =
endif

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDLIBS)

src/%.o: src/%.cpp include/store.hpp include/protocol.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

store_test: tests/store_test.cpp src/store.cpp include/store.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ tests/store_test.cpp src/store.cpp

protocol_test: tests/protocol_test.cpp $(LIB_SRCS) include/store.hpp include/protocol.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ tests/protocol_test.cpp $(LIB_SRCS)

test: store_test protocol_test
	./store_test
	./protocol_test

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe store_test store_test.exe protocol_test protocol_test.exe
