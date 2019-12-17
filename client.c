#include <zmq.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include "zmq.hpp"
#include <thread>
#include <functional>

std::vector<std::string> address{"tcp://10.1.18.194:5555", "tcp://10.1.18.236:5555", "tcp://10.1.18.253:5555"};

void func(zmq::context_t& context, int index);

int main (void)
{
    int child = 3;
    zmq::context_t context(1);
//    zmq::socket_t socket(context, ZMQ_REQ);
//    socket.connect("tcp://10.1.18.194:5555");

	std::vector<std::thread> pool;
	for(int i = 0; i < child; i++){
		pool.emplace_back(std::bind(&func, std::ref(context), i));
	}

/*
    int request_nbr;
    for (request_nbr = 0; request_nbr != 10; request_nbr++) {
        char buffer [10] = {'\0'};
        printf ("Sending Hello %d…\n", request_nbr);
        zmq_send (socket, "Hello", 5, 0);
        zmq_recv (socket, buffer, 10, 0);
        printf ("Received %s\n", buffer);
    }
*/
    for(auto & t : pool)
    {
	t.join();   
    }
    pool.clear();

    return 0;
}

void func(zmq::context_t& context, int index){
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect(address[index].c_str());
	
        char buffer [10] = {'\0'};
        printf ("Hello %d…\n", index);
        zmq_send (socket, "Hello", 5, 0);
        zmq_recv (socket, buffer, 10, 0);
        printf ("connected %d\n", index);
}

