
#ifndef riyuu__argv_parser_h
#define riyuu__argv_parser_h

#include <stdint.h>
#include <stdlib.h>

typedef enum {
	opt_bind_address = 1,
	opt_bind_port = 2,
	opt_nickname = 3,
} riyuu_argv_opt;

typedef struct _riyuu_opt {
	riyuu_argv_opt opt;
	char *argopt;
} riyuu_opt;

typedef struct _riyuu_plan {
	char *appname;
	riyuu_opt **opt;
	uint16_t opt_count;
} riyuu_plan;

riyuu_plan *riyuu_argv_parser(int argc, char *argv[], char *envp[], char **error);
void riyuu_opt_destroy(riyuu_plan *opt);

#endif
