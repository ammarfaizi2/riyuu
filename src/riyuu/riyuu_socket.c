
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <riyuu/riyuu.h>
#include <thpool/thpool.h>
#include <riyuu/riyuu_socket.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void *thread_worker(void *_fd) {

	#define newsockfd (*((int *)_fd))

	ssize_t n;
	char buffer[256];
	bzero(buffer, 256);
	n = read(newsockfd, buffer, 255);
	if (n < 0) {
		error("ERROR reading from socket");
	}
	printf("Here is the message: %s\n", buffer);
	sleep(3);
	n = write(newsockfd, "I got your message", 18);
	if (n < 0)
	error("ERROR writing to socket");
	close(newsockfd);	
	return NULL;

	#undef newsockfd
}

void riyuu_tcp_bind(char *addr, uint16_t port)
{
	socklen_t clilen;
	int sockfd, newsockfd;
    struct sockaddr_in serv_addr, cli_addr;
    threadpool thpool;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
    	error("ERROR opening socket");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(addr);
    serv_addr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    printf("Listening on %s:%d...\n", addr, port);
    thpool = thpool_init(5);

    while (1) {
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
		if (newsockfd < 0) {
			error("ERROR on accept");
		}
		thpool_add_work(thpool, thread_worker, (void *)&newsockfd);
		close(newsockfd);
    }
   
   return;
}
