


CXX = g++
CXXFLAGS = -std=c++11
LDFLAGS = 


targets = text test server


SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
HDRS = $(wildcard *.hpp)


DEPS = $(OBJS:.o=.d)


text_SRCS = text.cpp compiler.cpp mudou2.cpp
test_SRCS = test.cpp
server_SRCS = compiler_server.cpp compiler.cpp mudou2.cpp compile_run.cpp


text_OBJS = $(text_SRCS:.cpp=.o)
test_OBJS = $(test_SRCS:.cpp=.o)
server_OBJS = $(server_SRCS:.cpp=.o)


all: $(targets)


%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -MMD -MP -c -o $@ $<


text: $(text_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

test: $(test_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

server: $(server_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^


run: text
	./text

run_test: test
	./test

run_server: server
	./server


clean:
	rm -f $(targets) $(OBJS) $(DEPS) *.stdout *.stderr *.stdin *.compiler_error *.exe
	rm -rf temp/ modou/ 2>/dev/null


-include $(DEPS)

.PHONY: all run run_test run_server clean