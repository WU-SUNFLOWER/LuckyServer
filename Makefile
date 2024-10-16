Source_Path = src/

Dependences = util.cpp syscall.cpp
PrefixedDependences = $(patsubst %.cpp, $(Source_Path)%.cpp, $(Dependences))

Encapsulations = thread_pool.cpp buffer.cpp server.cpp epoll.cpp channel.cpp event_loop.cpp inet_address.cpp socket.cpp acceptor.cpp connection.cpp
PrefixedEncapsulations = $(patsubst %.cpp, $(Source_Path)%.cpp, $(Encapsulations))

server:
	g++ -g -std=c++20 $(PrefixedDependences) $(PrefixedEncapsulations) server.cpp -o server
	g++ -g -std=c++20 $(PrefixedDependences) client.cpp -o client

clean:
	rm -f server
	rm -f client