#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <amqp.h>
#include <amqp_tcp_socket.h>
#include <json-c/json.h>

void sendMessage(amqp_connection_state_t conn, const char *message) {
    amqp_channel_open(conn, 1);
    amqp_get_rpc_reply(conn);

    amqp_basic_properties_t props;
    props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
    props.content_type = amqp_cstring_bytes("application/json");
    props.delivery_mode = 2;  // Persistent delivery mode

    amqp_bytes_t message_bytes = amqp_cstring_bytes(message);

    amqp_basic_publish(conn, 1, amqp_cstring_bytes(""), amqp_cstring_bytes("main"), 0, 0, &props, message_bytes);
}

int main() {
    const char *hostname = "195.214.235.212";
    int port = 5672;
    const char *username = "RgRabbit";
    const char *password = "ZSNVqEj9b2";
    const char *virtual_host = "/";

    amqp_connection_state_t conn;
    conn = amqp_new_connection();

    amqp_socket_t *socket = amqp_tcp_socket_new(conn);
    if (!socket) {
        fprintf(stderr, "Error creating TCP socket\n");
        exit(1);
    }

    int status = amqp_socket_open(socket, hostname, port);
    if (status) {
        fprintf(stderr, "Error opening TCP socket\n");
        exit(1);
    }

    amqp_login(conn, virtual_host, 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, username, password);
    
    sendMessage(conn, "{\"imei\":\"dfsf654fdsf\",\"version\":\"1\",\"battery\":\"4\",\"balance\":200,\"ZFI\":1,\"ZFA\":0,\"MFA\":0,\"AFA\":0,\"BFA\":0,\"SFA\":0}");

    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);

    return 0;
}
