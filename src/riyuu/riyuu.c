
#include <stdio.h>

#include <riyuu/riyuu.h>
#include <riyuu/server.h>
#include <riyuu/client.h>

uint8_t riyuu_exec_run(riyuu_run *run)
{
	switch (run->command) {
		case no_command:
		break;

		case run_server: {
			riyuu_server_config config;
			if (riyuu_server_config_parse(run, &config)) {
				return riyuu_server_run(&config);
			}
		}
		break;

		case run_client:
			riyuu_client_run();
		break;
	}

	return 1;
}
