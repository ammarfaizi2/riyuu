
#ifndef riyuu__riyuu_h
#define riyuu__riyuu_h

#define RIYUU_VERSION "riyuu v0.0.1"

#include <stdint.h>
#include <riyuu/argv_parser.h>

typedef struct _riyuu_server_config {
	char *bind_addr;
	char *nickname;
	uint16_t bind_port;
} riyuu_server_config;

riyuu_server_config *riyuu_server_init(riyuu_plan *opt, char **error);
uint8_t riyuu_server_run(riyuu_server_config *config);
void riyuu_config_destroy(riyuu_server_config *config);

#endif
