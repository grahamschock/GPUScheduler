#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include "request.h"

int main() {
    int net_socket;
    int con_num = 0;
    net_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9005);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    int conn = connect(net_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (conn != 0) {
        printf("ERROR CONNECTION\n");
    } else {
        printf("CONNECTION SUCCSESS\n");
    }

    for(int n = 0; n < 50; n++) {
        struct request* req = generate_request();
        con_num = htonl(n);
        send(net_socket, &con_num, sizeof(con_num), 0);

        for(int i = 0; i < 512; i++) {
            for(int j = 0; j < 512; j++) {
                con_num = htonl(req->matrix[i][j]);
                send(net_socket, &con_num, sizeof(con_num), 0);
            }
        }
    }
    con_num = htonl(-1);
    send(net_socket, &con_num, sizeof(con_num), 0);
    close(net_socket);
    return 0;
}
