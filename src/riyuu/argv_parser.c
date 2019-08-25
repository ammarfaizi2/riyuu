
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <riyuu/riyuu.h>
#include <riyuu/argv_parser.h>

void show_help(char *app_name)
{
	printf("Usage: %s [command] [options]\n", app_name);
}

void show_version()
{
	printf("Riyuu: %s\n", RIYUU_VERSION);
}

static const char *invalid_parameter = "Invalid parameter \"%s\" at offset %d\n";
static const char *invalid_command = "Invalid command \"%s\" at offset %d\n";

#define COMMAND(cmd, value) \
	if (!strcmp(cmd, argv[i])) { \
		run->command = value; \
	}\

bool argv_parser(int argc, char *argv[], char *envp[], riyuu_run *run, uint8_t *exit_code)
{
	int i;
	size_t len;
	run->command = no_command;
	run->config_file = NULL;

	for (i = 1; i < argc; i++) {

		len = strlen(argv[i]);

		if (len < 1) {
			printf("argv has been corrupted!\n");
			*exit_code = 1;
			return false;
		}

		if (argv[i][0] == '-') {
			if (len == 1) {
				printf(invalid_parameter, argv[i], i);
				*exit_code = 1;
				return false;
			}

			if (len == 2) {
				switch (argv[i][1]) {
					case 'v':
						show_version();
						return false;
					break;
				}
			}

		} else {
			COMMAND("server", run_server) else 
			COMMAND("connect", run_client) else {
				printf(invalid_command, argv[i], i);
			}
		}
	}

	return true;
}
