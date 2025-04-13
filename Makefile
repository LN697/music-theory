CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -Werror -Iincludes

TARGET = build/main
SRC = src/main.cpp

all: $(TARGET)
$(TARGET):
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) > logs/build.log 2>&1

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean