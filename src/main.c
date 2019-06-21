
#include <stdio.h>
#include <riyuu/riyuu.h>
#include <riyuu/argv_parser.h>

int main(int argc, char *argv[], char *envp[])
{
   
   	char *error;
	riyuu_plan *opt;
	riyuu_server_config *config;
	uint8_t exit_code;

	opt = NULL;
	error = NULL;
	config = NULL;
	exit_code = 0;

	opt = riyuu_argv_parser(argc, argv, envp, &error);

	if (opt == NULL) {
		printf("Error: %s\n", error);
		goto err_clean;
	}

	config = riyuu_server_init(opt, &error);

	if (config == NULL) {
		printf("Error: %s\n", error);
		goto err_clean;
	}

	exit_code = riyuu_server_run(config);
	exit(exit_code);

err_clean:
	free(error);
	riyuu_opt_destroy(opt);
	riyuu_config_destroy(config);
	exit(1);
}
