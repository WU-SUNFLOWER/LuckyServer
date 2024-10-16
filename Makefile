Source_Path = src
Include_Path = include

Include_Paths = -I$(Include_Path)

Source_Files = $(wildcard $(Source_Path)/*.cpp)

CFlags = -g -Wall -Wextra

server:
	g++ $(CFlags) $(Include_Paths) $(Source_Files) server.cpp -o server
client:
	g++ $(CFlags) $(Include_Paths) $(Source_Files) client.cpp -o client
test_thread_pool:
	g++ $(CFlags) $(Include_Paths) $(Source_Files) ThreadPoolTest.cpp -o ThreadPoolTest
test:
	g++ $(CFlags) $(Include_Paths) $(Source_Files) test.cpp -o test	
clean:
	rm -f server
	rm -f client
	rm -f ThreadPoolTest
	rm -f test