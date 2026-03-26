# Makefile for mudou project

# 编译器和编译选项
CXX = g++
CXXFLAGS = -std=c++17
LDFLAGS = 

# 可执行文件目标
targets = text test server

# 源文件和头文件
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
HDRS = $(wildcard *.hpp)

# 依赖关系
DEPS = $(OBJS:.o=.d)

# 特定目标的源文件
text_SRCS = text.cpp compiler.cpp mudou2.cpp
test_SRCS = test.cpp
server_SRCS = compiler_server.cpp compiler.cpp mudou2.cpp

# 特定目标的对象文件
text_OBJS = $(text_SRCS:.cpp=.o)
test_OBJS = $(test_SRCS:.cpp=.o)
server_OBJS = $(server_SRCS:.cpp=.o)

# 默认目标
all: $(targets)

# 编译规则
%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -MMD -MP -c -o $@ $<

# 链接规则
text: $(text_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

test: $(test_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

server: $(server_OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

# 运行规则
run: text
	./text

run_test: test
	./test

run_server: server
	./server

# 清理目标
clean:
	rm -f $(targets) $(OBJS) $(DEPS) *.stdout *.stderr *.stdin *.compiler_error *.exe
	rm -rf temp/ modou/ 2>/dev/null

# 包含依赖文件
-include $(DEPS)

.PHONY: all run run_test run_server clean