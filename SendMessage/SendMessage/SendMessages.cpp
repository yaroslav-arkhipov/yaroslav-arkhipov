//for clang function use
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <process.h>

#include <amqp.h>
#include <amqp_tcp_socket.h>

#include "utils.h"

//declare global variables
char const* hostname;
int port, status;
char const* routingkey;
char const* exchange;
amqp_socket_t* socket = NULL;
amqp_connection_state_t conn;

//new thread for produsing messages
void Thread() 
{
    char const* messagebody;
    int radix = 10;
    char buffer[20];
    while (true)
    {
        messagebody = _itoa(rand() % 10 + 9, buffer, radix ); //random numbers
        //declare props
        amqp_basic_properties_t props;
        //set basic props
        props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
        props.content_type = amqp_cstring_bytes("text/plain");
        props.delivery_mode = 2; //постоянная доставка
        //produsing messages
        amqp_basic_publish(conn, 1, amqp_cstring_bytes(exchange), amqp_cstring_bytes(routingkey), 0, 0, &props, amqp_cstring_bytes(messagebody));
        printf("Sent \n");
        //sleep
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}


int main(int argc, char const* const* argv) 
{
    //checking income parameters
    if (argc < 5) {
            printf("Usage: SendMessages host port exchange routingkey \n");
        return 1;
    }

    //parameters assignment
    hostname = argv[1];
    port = atoi(argv[2]);
    exchange = argv[3];
    routingkey = argv[4];

    conn = amqp_new_connection();

    socket = amqp_tcp_socket_new(conn);
    if (!socket) {
        die("creating TCP socket");
    }

    status = amqp_socket_open(socket, hostname, port);
    if (status) {
        die("opening TCP socket");
    }

    //loging in RabbitServer
   amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest");
    amqp_channel_open(conn, 1);
    amqp_get_rpc_reply(conn);
    //create thread
    std::thread thr(Thread);
    //join
    thr.join();
    return 0;
}
