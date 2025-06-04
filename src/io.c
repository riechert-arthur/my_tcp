#include "io.h"

int read_from_socket(int socket, void *buffer, size_t size) {
    return recv(socket, buffer, size, 0);
}

int write_to_socket(int socket, const void *buffer, size_t size) {
    return send(socket, buffer, size, 0);
}