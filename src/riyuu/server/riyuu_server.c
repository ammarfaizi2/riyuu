
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <riyuu/riyuu.h>
#include <riyuu/server.h>
#include <thpool/thpool.h>
#include <riyuu/argv_parser.h>

#define RIYUU_CONFIG_ACT(name, action) \
	if (!strncmp(name, strptr, oft_a)) action

#ifdef RIYUU_DEBUG
#define PRINTF_DEBUG(...) printf(__VA_ARGS__)
#else
#define PRINTF_DEBUG(...)
#endif

bool riyuu_server_config_parse(riyuu_run *run, riyuu_server_config *config)
{
	FILE *h;
	char stream[1024];
	char *strptr = stream;
	char *strptr2 = stream;
	size_t len, i, oft, oft_a, oft_b;
	bool got_oft;

	if (run->config_file == NULL) {
		h = fopen("riyuu.conf", "rb");
	} else {
		h = fopen(run->config_file, "rb");
	}

	if (h == NULL) return false;

	while (fgets(stream, 1023, h)) {

		got_oft = false;
		len = strlen(stream);

		oft_a = oft_b = 0;

		for (i = 0; i < len; i++) {
			if ((*strptr2) == '=') {
				oft = i;
				oft_b = i + 1;
				got_oft = true;
				break;
			}
			strptr2++;
		}

		if (got_oft) {
			oft_a = oft - 1;
			while (
				((*strptr) == ' ') || ((*strptr) == '\t') || 
				((*strptr) == '\r') || ((*strptr) == '\n')
			) {
				strptr++;
			}
			while (
				(oft_a > 0) && 
				((stream[oft_a] == ' ') || (stream[oft_a] == '\t') || 
				(stream[oft_a] == '\r') || (stream[oft_a] == '\n'))
			) {
				oft_a--;
			}
			oft_a++;

			strptr2++;
			while (
				((*strptr2) == ' ') || ((*strptr2) == '\t') ||
				((*strptr2) == '\r') || ((*strptr2) == '\n')
			) {
				strptr2++;
			}

			len--;
			while ((len > 0) && 
				((stream[len] == ' ') || (stream[len] == '\t') ||
				(stream[len] == '\r') || (stream[len] == '\n'))
			) {
				len--;
			}
			len = len - oft_b + 1;

			RIYUU_CONFIG_ACT("bind_addr", {
				config->bind_addr = (char *)malloc(len + 1);
				strncpy(config->bind_addr, strptr2, len);
				PRINTF_DEBUG("Parsed bind_addr: %s\n", config->bind_addr);
			}) else
			RIYUU_CONFIG_ACT("bind_port", {
				char tmp[6];
				strncpy(tmp, strptr2, 5);
				config->bind_port = (uint16_t)atoi(tmp);
				PRINTF_DEBUG("Parsed bind_port: %d\n", config->bind_port);
			})
		}

		strptr = stream;
		strptr2 = stream;
	}

	fclose(h);
	return true;
}

uint8_t riyuu_server_run(riyuu_server_config *config)
{
	riyuu_tcp_bind(config->bind_addr, config->bind_port);
	return 0;
}

bool riyuu_socket_setopt(int sock_fd)
{
	struct timeval timeout;

	timeout.tv_sec = 30;
	timeout.tv_usec = 0;

	if (setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, (void *)&timeout, sizeof(timeout)) < 0) {
		perror("setsockopt failed\n");
		return false;
	}

	if (setsockopt(sock_fd, SOL_SOCKET, SO_SNDTIMEO, (void *)&timeout, sizeof(timeout)) < 0) {
		perror("setsockopt failed\n");
		return false;
	}

	return true;
}

void *riyuu_thread_worker(void *_fd)
{
	#define sock_fd (*((int *)_fd))

	riyuu_data *req;
	riyuu_data *tmp;
	bool exec_return = false;

	if (!riyuu_socket_setopt(sock_fd)) {
		goto out;
	}

	req = (riyuu_data *)malloc(sizeof(riyuu_data));
	if (req == NULL) {
		printf("Cannot allocate memory!\n");
		goto out;
	}

	printf("A new client connected with fd: %d\n", sock_fd);

	while (1) {
		ssize_t n;

		// Read data size.
		n = read(sock_fd, &(req->data_size), sizeof(uint32_t));
		if (n < 0) {
			perror("Error read");
			break;
		}

		write(sock_fd, "OK", 2);

		PRINTF_DEBUG("Got data length: %d\n", req->data_size);

		// Read data.
		req->data = (char *)malloc(sizeof(char) * req->data_size);

		n = read(sock_fd, req->data, req->data_size);
		if (n < 0) {
			perror("Error read");
			break;
		}
		
		PRINTF_DEBUG("Got data: %s\n", req->data);

		// // Process the request.
		// exec_return = riyuu_exec_request(sock_fd, req);
		// free(req);

		// if (!exec_return) break;
	}

	out:
	close(sock_fd);
	free(_fd);

	#undef sock_fd

	return NULL;
}

void riyuu_tcp_bind(char *addr, uint16_t port)
{
	socklen_t clilen;
	int sockfd, *new_sock_fd;
	struct sockaddr_in serv_addr, cli_addr;
	threadpool thpool;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
		perror("Error opening socket");
		exit(1);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(addr);
	serv_addr.sin_port = htons(port);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("Error on binding");
		exit(1);
	}
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);

	printf("Listening on %s:%d...\n", addr, port);

	thpool = thpool_init(5);

	// signal(SIGPIPE, SIG_IGN);

	while (1) {
		new_sock_fd = (int *)malloc(sizeof(int));

		*new_sock_fd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
		if (*new_sock_fd < 0) {
			perror("Error on accept");
		}

		thpool_add_work(thpool, riyuu_thread_worker, (void *)new_sock_fd);
	}

	return;
}
