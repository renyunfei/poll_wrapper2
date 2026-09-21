#ifndef POLL_WRAPPER_H
#define POLL_WRAPPER_H

#include <poll.h>

typedef struct {
    struct pollfd *fds;
    nfds_t len;
    nfds_t cap;
} pw_loop;

int pw_init(pw_loop *loop, nfds_t cap);
void pw_free(pw_loop *loop);

int pw_add(pw_loop *loop, int fd, short events);
int pw_mod(pw_loop *loop, int fd, short events);
int pw_del(pw_loop *loop, int fd);

int pw_wait(pw_loop *loop, int timeout_ms);
short pw_revents(const pw_loop *loop, int fd);

#endif
