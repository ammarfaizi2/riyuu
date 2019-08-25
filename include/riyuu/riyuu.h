
#ifndef RIYUU_RIYUU_H
#define RIYUU_RIYUU_H

#include <stdint.h>

#define RIYUU_VERSION "0.0.1"

typedef enum {
	no_command = 0,
	run_server = 1,
	run_client = 2,
} riyuu_command;

typedef struct _riyuu_run {
	riyuu_command command;
	char *config_file;
} riyuu_run;

typedef struct _riyuu_data {
	uint32_t data_size;
	char *data;
} riyuu_data;

uint8_t riyuu_exec_run(riyuu_run *run);

#endif
