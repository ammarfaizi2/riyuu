
#include <string.h>
#include <riyuu/riyuu.h>
#include <riyuu/argv_parser.h>

riyuu_server_config *riyuu_server_init(riyuu_plan *opt, char **error)
{
	riyuu_server_config *config;

	if (opt == NULL || (opt->opt_count < 1)) {
		goto err;
	}

	config = (riyuu_server_config *)malloc(sizeof(riyuu_server_config));
	config->bind_addr = NULL;
	config->bind_port = 0;
	config->nickname = NULL;

	for (uint16_t i = 0; i < opt->opt_count; ++i) {
		switch (opt->opt[i]->opt) {
			case opt_bind_address:
				config->bind_addr = opt->opt[i]->argopt;
			break;

			case opt_bind_port:
				config->bind_port = (uint16_t)atol(opt->opt[i]->argopt);
			break;

			case opt_nickname:
				config->nickname = opt->opt[i]->argopt;
			break;
		}
	}

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

	
	
	return 1;
}

void riyuu_config_destroy(riyuu_server_config *config)
{

}
