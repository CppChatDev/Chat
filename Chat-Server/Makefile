CXX := clang++
CXXFLAGS := -O2 -std=c++11 -stdlib=libstdc++ -march=native -ggdb -pipe -Wall -Wextra -Wshadow $(EXTRA_CXXFLAGS)
LDFLAGS := -lsqlite3 -lboost_system -lboost_date_time
LIBS := bcrypt.a

CPP_FILES := $(wildcard *.cpp)
OBJECTS := $(CPP_FILES:.cpp=.o)

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) $(LIBS) -o $@

format:
	clang-format-3.8 -i *.cpp *.h

clean:
	rm -f *.o
	rm -f main

.PHONY: clean format

CXXFLAGS += -MMD
-include $(OBJECTS:.o=.d)
