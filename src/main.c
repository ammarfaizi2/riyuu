
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <riyuu/argv_parser.h>

int main(int argc, char *argv[], char *envp[])
{
	uint8_t exit_code = 0;
	riyuu_run run;

	if (argc == 1) {
		show_help(argv[0]);
		goto out;
	}

	if (!argv_parser(argc, argv, envp, &run, &exit_code)) {
		goto out;
	}

	exit_code = riyuu_exec_run(&run);

out:
	exit(exit_code);
}
