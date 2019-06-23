
#ifndef riyuu__riyuu_h
#define riyuu__riyuu_h

#define RIYUU_VERSION "riyuu v0.0.1"

#include <stdint.h>

// Riyuu options.
typedef enum {
	/**
	 * Server option.
	 */
	opt_server_bind_address = 100,
	opt_server_bind_port = 101,
	opt_server_nickname = 102,
	opt_server_daemonize = 103,

	/**
	 * Client option.
	 */
	opt_client_target_host = 200,
	opt_client_target_port = 201,

	/**
	 * Global option.
	 */
	opt_version = 1,
	opt_help = 2,

} riyuu_opt;

// Riyuu commands.
typedef enum {
	cmd_no_cmd = 0,

	/**
	 * Server command.
	 */
	cmd_server_serve = 100,

	/**
	 * Client command.
	 */
	cmd_client_connect = 200
} riyuu_cmd;

// Server configuration.
typedef struct _riyuu_server_config {
	char *nickname;
	char *bind_addr;
	uint16_t bind_port;
} riyuu_server_config;

// Client configuration.
typedef struct _riyuu_client_config {
	char *target_host;
	uint16_t target_port;
} riyuu_client_config;

#include <riyuu/argv_parser.h>

riyuu_server_config *riyuu_server_init(riyuu_argv *opt, char **error);
uint8_t riyuu_server_run(riyuu_server_config *config);
void riyuu_config_destroy(riyuu_server_config *config);
void riyuu_serialize_cmd(char *file);

#endif
