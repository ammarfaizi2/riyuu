
#include <string.h>
#include <riyuu/riyuu.h>
#include <riyuu/argv_parser.h>
#include <riyuu/riyuu_socket.h>

char default_bind_addr[] = "0.0.0.0";

riyuu_server_config *riyuu_server_init(riyuu_argv *opt, char **error)
{
	riyuu_server_config *config;

	if (opt == NULL || (opt->opt_count < 1)) {
		goto err;
	}

	config = (riyuu_server_config *)malloc(sizeof(riyuu_server_config));
	config->bind_addr = default_bind_addr;
	config->bind_port = 54884;
	config->nickname = NULL;

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wswitch"
	for (uint16_t i = 0; i < opt->opt_count; ++i) {
		switch (opt->opts[i]->opt) {
			case opt_server_bind_address:
				config->bind_addr = opt->opts[i]->arg;
			break;

			case opt_server_bind_port:
				config->bind_port = (uint16_t)atol(opt->opts[i]->arg);
			break;

			case opt_server_nickname:
				config->nickname = opt->opts[i]->arg;
			break;

			case opt_server_daemonize:

			break;
		}
	}
	#pragma GCC diagnostic pop

	return config;

err:
	if (error != NULL) {
		#define _error_text "Cannot initialize riyuu server: Config is empty!"
		*error = (char *)malloc(sizeof(_error_text));
		strcpy(*error, _error_text);
		#undef _error_text
	}
	return NULL;
}

uint8_t riyuu_server_run(riyuu_server_config *config)
{
	riyuu_tcp_bind(config->bind_addr, config->bind_port);
	return 0;
}

void riyuu_config_destroy(riyuu_server_config *config)
{

}
