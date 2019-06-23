
#include <stdio.h>
#include <string.h>
#include <riyuu/riyuu.h>
#include <riyuu/argv_parser.h>

int main(int argc, char *argv[], char *envp[])
{
	if (argc == 1) {
		show_help(argv[0]);
		return 0;
	}

   	char *error;
	riyuu_argv *opt;
	uint8_t exit_code;

	opt = NULL;
	error = NULL;
	exit_code = 0;

	opt = riyuu_argv_parser(argc, argv, envp, &error);

	if (opt == NULL) {
		printf("Error: %s\n", error);
		goto err_clean;
	}

	// Server command.
	if ((opt->cmd >= 100) && (opt->cmd < 200)) {
		riyuu_server_config *config;
		config = NULL;

		config = riyuu_server_init(opt, &error);
		if (config == NULL) {
			printf("Error: %s\n", error);
			riyuu_config_destroy(config);
			goto err_clean;
		}
		exit_code = riyuu_server_run(config);
		goto _exit;
	} else 

	// Client command.
	if ((opt->cmd >= 200) && (opt->cmd < 300)) {

	}


err_clean:
	free(error);
	riyuu_opt_destroy(opt);
	exit_code = 1;

_exit:
	return exit_code;
}
