
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <riyuu/riyuu.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void riyuu_tcp_bind(char *addr, uint16_t port)
{

	ssize_t n;
	char buffer[256];
	socklen_t clilen;
	int sockfd, newsockfd;
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
    	error("ERROR opening socket");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    // serv_addr.sin_addr.s_addr = addr;
    serv_addr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    int pid;

    while (1) {
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
		if (newsockfd < 0) {
			error("ERROR on accept");
		}

		pid = fork();
		if (pid < 0) {
			error("ERROR in new process creation");
		}

		if (pid == 0) {
			bzero(buffer, 256);
			n = read(newsockfd, buffer, 255);
			if (n < 0) {
				error("ERROR reading from socket");
			}

			printf("Here is the message: %s\n", buffer);
			n = write(newsockfd, "I got your message", 18);
			if (n < 0)
			error("ERROR writing to socket");
			close(newsockfd);
		} else {
			close(newsockfd);
		}
    }
   
   return;
}
