#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

int main() {

    char cli_message[256] = "1";
    char cli_message2[256] = "2";

    char exit_message[256] = "EXIT";

    int net_socket;
    net_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    int conn = connect(net_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (conn != 0) {
        printf("ERROR CONNECTION\n");
    } else {
        printf("CONNECTION SUCCSESS\n");
    }


    send(net_socket, cli_message, sizeof(cli_message), 0);
    sleep(5);
    send(net_socket, cli_message2, sizeof(cli_message2), 0);
    sleep(5);
    send(net_socket, exit_message, sizeof(exit_message), 0);

    close(net_socket);
    return 0;
}
