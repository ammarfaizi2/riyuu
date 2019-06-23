
#include <stdio.h>
#include <string.h>
#include <riyuu/riyuu.h>
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
	char *cmdptr;
	riyuu_plan *ret;
	size_t len;
	bool got_cmd;
	register uint8_t alloc_opt;
	register uint8_t alloc_used;
	const char error_need_arg[] = "Option %s needs an argument!";

	got_cmd = false;
	alloc_used = 0;
	alloc_opt = 5;
	cmdptr = NULL;
	ret = (riyuu_plan *)malloc(sizeof(riyuu_plan));
	ret->appname = argv[0];
	ret->cmd = cmd_no_cmd;
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
	#define RCMD(_cmd, cmd_name) \
		if (!strcmp($farg, _cmd)) { \
			ret->cmd = cmd_name; \
			got_cmd = true; \
			cmdptr = $farg; \
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
				OPT2_NEED_ARG("sfile", opt_serialize_target_file) else
				OPT2_NEED_ARG("serialize-file", opt_serialize_target_file) else

				/**
				 * @todo Make riyuu daemon.
				 */
				// OPT2_NO_ARG("daemon", opt_daemonize) else
				// OPT2_NO_ARG("daemonize", opt_daemonize) else

				OPT2_NO_ARG("version", opt_version) else
				OPT2_NO_ARG("help", opt_help) else
				{
					#define _error_text "Invalid parameter \"--"
					*error = (char *)malloc(sizeof(_error_text) + len);
					strcpy(*error, _error_text);
					strcat(*error, $farg);
					*((*error) + len + sizeof(_error_text) - 3) = '"';
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
		} else {

			#define $farg ($arg)

			if (got_cmd) {
				#define _error_text "Invalid parameter \"%s\" for \"%s\" command"
				*error = (char *)malloc(sizeof(_error_text) + len + strlen(cmdptr));
				sprintf(*error, _error_text, $farg, cmdptr);
				#undef _error_text
				goto err;
			}

			RCMD("serve", cmd_serve) else
			RCMD("serialize", cmd_serialize) else
			{
				#define _error_text "Invalid command \""
				*error = (char *)malloc(sizeof(_error_text) + len);
				strcpy(*error, _error_text);
				strcat(*error, $farg);
				*((*error) + sizeof(_error_text) + len - 1) = '"';
				#undef _error_text
				goto err;
			}

			#undef $farg
		}
	}

	#undef $arg
	#undef OPT2_NO_ARG
	#undef OPT_NEED_ARG
	return ret;

err:
	riyuu_opt_destroy(ret);
	return NULL;
}

void show_help(char *appname)
{
	printf("Usage: %s [options]\n\n", appname);

	// Commands
	printf("Commands:\n");
	printf("  serve\t\t\tRun riyuu server\n\n");

	// Options
	printf("Options:\n");
	printf("  --help\t\tShow this message\n");
	printf("  --version\t\tShow riyuu version\n");
	printf("  --bind-address\tSpecify bind address (default: 0.0.0.0)\n");
	printf("  --bind-port\t\tSpecify bind port (default: 54884)\n");
}

void show_version()
{
	printf("%s\n", RIYUU_VERSION);
}
