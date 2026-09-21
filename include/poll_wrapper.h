#ifndef POLL_WRAPPER_H
#define POLL_WRAPPER_H

#include <poll.h>

typedef struct {
    struct pollfd *fds;
    nfds_t len;
    nfds_t cap;
} pw_loop;

/* Initialize fixed-capacity loop. Returns 0 on success, -1 on error (errno set). */
int pw_init(pw_loop *loop, nfds_t cap);
/* Release resources owned by loop. Safe to call multiple times. */
void pw_free(pw_loop *loop);

/* Add fd with interest events. Fails if full or fd already exists. */
int pw_add(pw_loop *loop, int fd, short events);
/* Update interest events for an existing fd. */
int pw_mod(pw_loop *loop, int fd, short events);
/* Remove an existing fd from the loop. */
int pw_del(pw_loop *loop, int fd);

/* Call poll(); timeout_ms must be -1 or >=0. Returns poll() result. */
int pw_wait(pw_loop *loop, int timeout_ms);
/* Get revents for fd after pw_wait; returns 0 if fd not found/invalid loop. */
short pw_revents(const pw_loop *loop, int fd);

#endif
