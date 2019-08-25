
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <riyuu/riyuu.h>
#include <riyuu/client.h>

uint8_t riyuu_client_run()
{
	riyuu_client_connect("127.0.0.1", 50000, NULL);
	return 0;
}

uint8_t riyuu_client_connect(char *host_addr, uint16_t port, char **error)
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    riyuu_data *data;
    size_t inlen;
    size_t reqlen;
    size_t reslen;

    signal(SIGPIPE, SIG_IGN);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)  { 
        printf("Socket creation error\n");
        return -1; 
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host_addr, &serv_addr.sin_addr) <= 0) {
        printf("Invalid address or address is not supported.\n");
        return 1;
    }

    printf("Connecting to %s:%d...\n", host_addr, port);
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection failed!\n");
        return 1;
    }
    printf("Connected!\n");

    data = (riyuu_data *)malloc(sizeof(riyuu_data));
    data->data = (char *)malloc(sizeof(char) * 2048);

    while (1) {
        char buff[10];
        printf(">> ");
        fgets(data->data, 2047, stdin);
        data->data_size = (uint32_t)strlen(data->data);

        send(sock, &(data->data_size), sizeof(uint32_t), 0);
        read(sock, buff, 2);
        buff[2] = 0;

        if (!strcmp("OK", buff)) {
            send(sock, data->data, data->data_size, 0);
        } else {
            printf("Response error\n");
        }
    }
    return 0;
}
