Source_Path = src/

Dependences = util.cpp syscall.cpp
PrefixedDependences = $(patsubst %.cpp, $(Source_Path)%.cpp, $(Dependences))

Encapsulations = ThreadPool.cpp Buffer.cpp Server.cpp Epoll.cpp Channel.cpp EventLoop.cpp InetAddress.cpp Socket.cpp Acceptor.cpp Connection.cpp
PrefixedEncapsulations = $(patsubst %.cpp, $(Source_Path)%.cpp, $(Encapsulations))

server:
	g++ -g -std=c++20 $(PrefixedDependences) $(PrefixedEncapsulations) server.cpp -o server

client:
	g++ -g -std=c++20 $(PrefixedDependences) $(PrefixedEncapsulations) client.cpp -o client

test_thread_pool:
	g++ -g -std=c++20 $(PrefixedDependences) $(PrefixedEncapsulations) ThreadPoolTest.cpp -o ThreadPoolTest

test:
	g++ -g -std=c++20 $(PrefixedDependences) $(PrefixedEncapsulations) test.cpp -o test	

clean:
	rm -f server
	rm -f client
	rm -f ThreadPoolTest
	rm -f test