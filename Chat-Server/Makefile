CXX := clang++
CXXFLAGS := -O2 -std=c++14 -stdlib=libc++ -march=native -ggdb -pipe -Wall -Wextra -Wshadow
LDFLAGS := -lsqlite3 -lboost_system -lboost_date_time

CPP_FILES := $(wildcard *.cpp)
OBJECTS := $(CPP_FILES:.cpp=.o)

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

clean:
	rm -f *.o
	rm -f main

.PHONY: clean

CXXFLAGS += -MMD
-include $(OBJECTS:.o=.d)
