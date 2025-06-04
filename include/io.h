#ifndef IO_H
#define IO_H

#include <sys/socket.h>

int read_from_socket(int socket, void *buffer, size_t size);
int write_to_socket(int socket, const void *buffer, size_t size);

#endif