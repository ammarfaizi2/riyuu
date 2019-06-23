
#ifndef riyuu__argv_parser_h
#define riyuu__argv_parser_h

#include <stdint.h>
#include <stdlib.h>
#include <riyuu/riyuu.h>

typedef struct _riyuu_opt {
	riyuu_opt opt;
	char *arg;
} riyuu_argv_opt;

typedef struct _riyuu_argv {
	riyuu_cmd cmd;
	char *appname;
	riyuu_argv_opt **opts;
	uint16_t opt_count;
} riyuu_argv;

riyuu_argv *riyuu_argv_parser(int argc, char *argv[], char *envp[], char **error);
void riyuu_opt_destroy(riyuu_argv *opt);
void show_help(char *appname);
void show_version();

#endif
