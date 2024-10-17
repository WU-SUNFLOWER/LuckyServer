APPS = echo_server http_server single_client multiply_client test_thread_pool

Source_Path = src
Include_Path = include

Include_Paths = -I$(Include_Path)

Source_Files = $(wildcard $(Source_Path)/*.cpp)

CFlags = -g -Wall -Wextra

CXX = g++

.PHONY: all clean $(APPS)

all: $(APPS)

$(APPS): %: dist/%

dist/%: app/%.cpp $(Source_Files) | dist
	$(CXX) $(CFlags) $(Include_Paths) $(Source_Files) $< -o $@

dist:
	mkdir -p dist

clean:
	rm -rf dist