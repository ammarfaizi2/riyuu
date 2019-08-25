
#ifndef RIYUU_ARGV_PARSER_H
#define RIYUU_ARGV_PARSER_H

#include <riyuu/riyuu.h>
#include <stdbool.h>

void show_help(char *app_name);
void show_version();
bool argv_parser(int argc, char *argv[], char *envp[], riyuu_run *run, uint8_t *exit_code);

#endif
