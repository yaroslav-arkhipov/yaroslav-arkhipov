#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <amqp.h>
#include <amqp_tcp_socket.h>

#include <assert.h>

#include "utils.h"

int main(int argc, char const* const* argv) {
    //variables
    char const* hostname;
    int port, status;
    char const* queuename;
    amqp_socket_t* socket = NULL;
    amqp_connection_state_t conn;

    //checking income parameters
    if (argc < 3) {
        fprintf(stderr, "Usage: Consumer host port queuename\n");
        return 1;
    }

    //parameters assignment
    hostname = argv[1];
    port = atoi(argv[2]);
    queuename = argv[3];

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

    //consume to queue
    amqp_basic_consume(conn, 1, amqp_cstring_bytes(queuename), amqp_empty_bytes, 0, 0, 0, amqp_empty_table);
    amqp_get_rpc_reply(conn);

    for (;;) {
        amqp_rpc_reply_t res;
        amqp_envelope_t envelope;

        //release buffer
        amqp_maybe_release_buffers(conn);

        res = amqp_consume_message(conn, &envelope, NULL, 0);

        if (AMQP_RESPONSE_NORMAL != res.reply_type)  break;
       
        //print the message
        printf("----\n");

        amqp_dump(envelope.message.body.bytes, envelope.message.body.len);

        amqp_destroy_envelope(&envelope);
    }

    //destroy connection
    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);

    return 0;
}
