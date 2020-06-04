all:tcp_srv select tcp_cli poll
poll:poll.cpp
	g++ -std=c++11 $^ -o $@
tcp_cli:tcp_cli.cpp
	g++ -std=c++11 $^ -o $@
tcp_srv:tcp_srv.cpp
	g++ -std=c++11 $^ -o $@
select:select.c
	gcc -std=c99  $^ -o $@
