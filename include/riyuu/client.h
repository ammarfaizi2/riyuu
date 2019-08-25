
#ifndef RIYUU_CLIENT_H
#define RIYUU_CLIENT_H

uint8_t riyuu_client_run();
uint8_t riyuu_client_connect(char *host_addr, uint16_t port, char **error);

#endif
