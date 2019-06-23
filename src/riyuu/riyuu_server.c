
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
	// #define newsockfd (*((int *)_fd))
	// ssize_t n;
	// riyuu_obj *obj = (riyuu_obj *)malloc(sizeof(riyuu_obj) + 8096);

	// n = read(newsockfd, obj, sizeof(riyuu_obj) + 8096);

	// if (n < 0) {
	// 	error("ERROR reading from socket");
	// }

	// printf("Length: %ld\n", obj->len);
	// printf("Data: %s\n", obj->data);

	// n = write(newsockfd, "I got your message\n", 19);

	// if (n < 0) {
	// 	error("ERROR writing to socket");
	// }
	// close(newsockfd);
	// return NULL;
	// #undef newsockfd
}

void riyuu_tcp_bind(char *addr, uint16_t port)
{
	socklen_t clilen;
	int sockfd, *newsockfd;
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
		newsockfd = (int *)malloc(sizeof(int));
		*newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
		if (*newsockfd < 0) {
			error("ERROR on accept");
		}
		thpool_add_work(thpool, thread_worker, (void *)newsockfd);
	}
   
   return;
}
