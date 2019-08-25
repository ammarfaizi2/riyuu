
#ifndef RIYUU_SERVER_H
#define RIYUU_SERVER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct _riyuu_server_config {
	char *bind_addr;
	uint16_t bind_port;
} riyuu_server_config;

uint8_t riyuu_server_run(riyuu_server_config *config);
bool riyuu_server_config_parse(riyuu_run *run, riyuu_server_config *config);
void riyuu_tcp_bind(char *addr, uint16_t port);
void *riyuu_thread_worker(void *_fd);

#endif
