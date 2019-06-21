
#include <stdio.h>
#include <string.h>
#include <riyuu/argv_parser.h>

// Free riyuu_opt heap.
void riyuu_opt_destroy(riyuu_plan *opt)
{
	if (opt == NULL) {
		return;
	}
	for (int i = 0; i < opt->opt_count; ++i) {
		if (opt->opt[i] != NULL) {
			free(opt->opt[i]->argopt);
			opt->opt[i]->argopt = NULL;
			free(opt->opt[i]);
		}
	}
	free(opt->opt);
	opt->opt = NULL;
	free(opt);
	opt = NULL;
}

// Parse argv
riyuu_plan *riyuu_argv_parser(int argc, char *argv[], char *envp[], char **error)
{
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
	ret->opt_count = 0;

	#define $arg (argv[i])
	#define SET_ARG_1(OPT_ENUM) \
		ret->opt[alloc_used] = (riyuu_opt *)malloc(sizeof(riyuu_opt)); \
		ret->opt[alloc_used]->opt = OPT_ENUM; \
		ret->opt[alloc_used]->argopt = argv[i + 1]; \
		alloc_used++; \
		continue;
	#define OPT2_NO_ARG(_opt, opt_name) \
		if (!strcmp($farg, _opt)) { \
			ret->opt[alloc_used] = (riyuu_opt *)malloc(sizeof(riyuu_opt)); \
			ret->opt[alloc_used]->opt = opt_name; \
			ret->opt[alloc_used]->argopt = NULL; \
			ret->opt_count++; \
			alloc_used++; \
			i++; \
			continue; \
		}
	#define OPT2_NEED_ARG(_opt, opt_name) \
		if (!strcmp($farg, _opt)) { \
			if ((i + 1) >= argc) { \
				*error = (char *)malloc(sizeof(error_need_arg) + sizeof(_opt) - 1); \
				sprintf(*error, error_need_arg, _opt); \
				goto err; \
			} \
			ret->opt[alloc_used] = (riyuu_opt *)malloc(sizeof(riyuu_opt)); \
			ret->opt[alloc_used]->opt = opt_name; \
			ret->opt[alloc_used]->argopt = argv[i + 1]; \
			ret->opt_count++; \
			alloc_used++; \
			i++; \
			continue; \
		}


	for (i = 1; i < argc; i++) {

		if ((alloc_used + 1) >= alloc_opt) {
			alloc_opt += 5;
			ret->opt = (riyuu_opt **)realloc(ret->opt, sizeof(riyuu_opt *) * alloc_opt);
		}

		len = strlen($arg);

		if ((*$arg) == '-') {
			if (len == 1) {
				#define _error_text "Invalid parameter \"-\""
				*error = (char *)malloc(sizeof(_error_text));
				strcpy(*error, _error_text);
				#undef _error_text
				goto err;
			}

			// Parse option that starts with "--"
			if ((*($arg + 1)) == '-') {
				#define $farg ($arg + 2)

				OPT2_NEED_ARG("bind-address", opt_bind_address) else
				OPT2_NEED_ARG("bind-port", opt_bind_port) else
				OPT2_NEED_ARG("nickname", opt_nickname) else
				{
					#define _error_text "Invalid parameter \"-\""
					*error = (char *)malloc(sizeof(_error_text));
					strcpy(*error, _error_text);
					#undef _error_text
					goto err;
				}

				alloc_used++;
				#undef OPT_CMP
				#undef $farg
				continue;
			} else 

			// Parse option that starts with "-"
			{
				#define $farg (*($arg + 1))

				{
					#define _error_text "Invalid parameter \"-%c\""
					*error = (char *)malloc(sizeof(_error_text));
					sprintf(*error, _error_text, $farg);
					#undef _error_text
					goto err;
				}

				#undef $farg
				continue;
			}
		}
	}
	#undef $arg
	#undef OPT_NEED_ARG
	return ret;

err:
	riyuu_opt_destroy(ret);
	return NULL;
}
