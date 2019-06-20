
#include <stdio.h>
#include <string.h>
#include <riyuu/argv_parser.h>

riyuu_plan *riyuu_argv_parser(int argc, char *argv[], char *envp[], char **error) {
	int i;
	riyuu_plan *ret;
	size_t len;
	register uint8_t alloc_opt;
	register uint8_t alloc_used;
	const char error_need_arg[] = "Option %s needs an argument!";

	alloc_used = 0;
	alloc_opt = 5;
	ret = (riyuu_plan *)malloc(sizeof(riyuu_plan));
	ret->appname = argv[0];
	ret->opt = (riyuu_opt **)malloc(sizeof(riyuu_opt *) * alloc_opt);

	#define $arg (argv[i])
	#define SET_ARG_1(OPT_ENUM) \
		ret->opt[alloc_used] = (riyuu_opt *)malloc(sizeof(riyuu_opt)); \
		ret->opt[alloc_used]->opt = OPT_ENUM; \
		ret->opt[alloc_used]->argopt = argv[i + 1]; \
		alloc_used++; \
		continue;

	for (i = 1; i < argc; i++) {

		if ((alloc_used + 1) >= alloc_opt) {
			alloc_opt += 5;
			ret->opt = (riyuu_opt **)malloc(sizeof(riyuu_opt *) * alloc_opt);
		}

		len = strlen($arg);

		if ((*$arg) == '-') {
			if (len == 1) {

				#define _error_text "Invalid parameter \"-\""
				*error = (char *)malloc(sizeof(_error_text));
				strcpy(*error, _error_text);
				#undef _error_text

				free(ret);
				return NULL;
			}

			if ((*($arg + 1)) == '-') {
				#define $farg ($arg + 2)
				#define OPT_CMP(A, B) if ((!strcmp(A, B)))

				#define _opt "bind-address"
				OPT_CMP($farg, _opt) {
					if ((i + 1) >= argc) {
						*error = (char *)malloc(sizeof(_error_text) + sizeof(_opt) - 1);
						sprintf(*error, error_need_arg, _opt);
						goto error_need_arg;
					}
				} else 
				#undef _opt

				#define _opt "bind-port"
				OPT_CMP($farg, _opt) {
					if ((i + 1) >= argc) {
						*error = (char *)malloc(sizeof(_error_text) + sizeof(_opt) - 1);
						sprintf(*error, error_need_arg, _opt);
						goto error_need_arg;
					}
					ret->opt[alloc_used] = (riyuu_opt *)malloc(sizeof(riyuu_opt));
					ret->opt[alloc_used]->opt = opt_bind_port;
					ret->opt[alloc_used]->argopt = argv[i + 1];
				} else {
					// Error.
				}

				alloc_used++;
				#undef OPT_CMP
				#undef $farg
				continue;
			} else {
				#define $farg (*($arg + 1))
				printf("%c\n", $farg);
				#undef $farg
				continue;
			}
		}
	}
	#undef $arg

	return ret;

error_need_arg:

	free(ret);
	return NULL;
}
