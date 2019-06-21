
#include <stdio.h>
#include <riyuu/argv_parser.h>

int main(int argc, char *argv[], char *envp[]) {
   
   	char *error;
	riyuu_plan *opt;

	opt = NULL;
	error = NULL;

	opt = riyuu_argv_parser(argc, argv, envp, &error);

	if (opt == NULL) {
		printf("Error: %s\n", error);
		free(error);
		exit(1);
	}

	riyuu_opt_destroy(opt);
}
