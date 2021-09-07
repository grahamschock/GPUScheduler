#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>

#include <netinet/in.h>
#include "request.h"

struct request** proc_table;
int  i = 0;

int get_response(int client_socket) {
    int recv_int = 0;
    for(;;) {
        recv(client_socket, &recv_int, sizeof(recv_int), 0);
        if(ntohl(recv_int) == (uint32_t)-1) {
            return 0;
        }
        struct request* recive = calloc(1, sizeof(struct request));
        for(int j = 0; j < 512; j++) {
            for (int k = 0; k < 512; k++) {
                recv(client_socket, &recv_int, sizeof(recv_int), 0);
                recive->matrix[j][k] = ntohl(recv_int);
            }
        }
        proc_table[i] = recive;
        i++;
    }
    return 0;
}

uint get_sum(struct request* req) {
    uint sum = 0;
    for(int i = 0; i < 512; i++) {
        for(int j = 0; j < 512; j++) {
            sum += req->matrix[i][j] % 4;
        }
    }
    return sum;
}

void* scheduler(void* arg) {
    int n = 0;
    uint sum = 0;
    for(;;) {
        for(int j = n; j < i; j++) {
            sum = get_sum(proc_table[j]);
            printf("SUM: %d\n", sum);
            n++;
        }
    }
    return NULL;
}

int main() {
    int serv_socket;
    pthread_t thread_id;
    serv_socket = socket(AF_INET, SOCK_STREAM, 0);

    proc_table = calloc(256, sizeof(int));

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9005);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int bind_val = bind(serv_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    if (bind_val != 0) {
        printf("ERROR BIND\n");
    } else {
        printf("BIND SUCCSESS\n");
    }

    listen(serv_socket, 5);

    int client_socket = accept(serv_socket, NULL, NULL);
    pthread_create(&thread_id, NULL, scheduler, NULL);
    get_response(client_socket);
    while(1)
        ;
    close(client_socket);

    return 0;
}
