#include "poll_wrapper.h"

#include <errno.h>
#include <stdlib.h>

static int pw_find(const pw_loop *loop, int fd) {
    nfds_t i;
    for (i = 0; i < loop->len; ++i) {
        if (loop->fds[i].fd == fd) {
            return (int)i;
        }
    }
    return -1;
}

int pw_init(pw_loop *loop, nfds_t cap) {
    if (!loop || cap == 0) {
        errno = EINVAL;
        return -1;
    }
    loop->fds = (struct pollfd *)calloc(cap, sizeof(struct pollfd));
    if (!loop->fds) {
        return -1;
    }
    loop->len = 0;
    loop->cap = cap;
    return 0;
}

void pw_free(pw_loop *loop) {
    if (!loop) {
        return;
    }
    free(loop->fds);
    loop->fds = NULL;
    loop->len = 0;
    loop->cap = 0;
}

int pw_add(pw_loop *loop, int fd, short events) {
    if (!loop || !loop->fds || fd < 0) {
        errno = EINVAL;
        return -1;
    }
    if (loop->len >= loop->cap) {
        errno = ENOSPC;
        return -1;
    }
    if (pw_find(loop, fd) >= 0) {
        errno = EEXIST;
        return -1;
    }
    loop->fds[loop->len].fd = fd;
    loop->fds[loop->len].events = events;
    loop->fds[loop->len].revents = 0;
    loop->len++;
    return 0;
}

int pw_mod(pw_loop *loop, int fd, short events) {
    if (!loop || !loop->fds || fd < 0) {
        errno = EINVAL;
        return -1;
    }
    int i = pw_find(loop, fd);
    if (i < 0) {
        errno = ENOENT;
        return -1;
    }
    loop->fds[i].events = events;
    loop->fds[i].revents = 0;
    return 0;
}

int pw_del(pw_loop *loop, int fd) {
    if (!loop || !loop->fds || fd < 0) {
        errno = EINVAL;
        return -1;
    }
    int i = pw_find(loop, fd);
    if (i < 0) {
        errno = ENOENT;
        return -1;
    }
    loop->len--;
    loop->fds[i] = loop->fds[loop->len];
    loop->fds[loop->len].fd = -1;
    loop->fds[loop->len].events = 0;
    loop->fds[loop->len].revents = 0;
    return 0;
}

int pw_wait(pw_loop *loop, int timeout_ms) {
    if (!loop || !loop->fds) {
        errno = EINVAL;
        return -1;
    }
    return poll(loop->fds, loop->len, timeout_ms);
}

short pw_revents(const pw_loop *loop, int fd) {
    if (!loop || !loop->fds || fd < 0) {
        return 0;
    }
    int i = pw_find(loop, fd);
    if (i < 0) {
        return 0;
    }
    return loop->fds[i].revents;
}
