#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>

#include <netinet/in.h>

int* proc_table;
int  i = 0;

int get_response(int client_socket) {
    char server_message[256];
    char exit_message[256] = "EXIT";
    for(;;) {
        recv(client_socket, &server_message, sizeof(server_message), 0);
        if (strncmp(server_message, exit_message, 4) == 0) {
            for(int j = 0; j < i; j++) {
                printf("%d\n", proc_table[j]);
            }
            return 0;
        }
        int val = atoi(server_message);
        proc_table[i] = val;
        i++;
    }

    return 0;
}

void* scheduler(void* arg) {
    for(;;) {
        sleep(2);
        printf("HELLO FROM THREAD");
        for(int j = 0; j < i; j++) {
            printf("%d\n", proc_table[j]);
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
    server_address.sin_port = htons(8080);
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
    while(1);
    close(client_socket);

    return 0;
}
