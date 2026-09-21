#include "poll_wrapper.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    int sv[2] = {-1, -1};
    pw_loop loop = {0};
    pw_loop tiny = {0};
    char ch = 'x';
    int n;

    errno = 0;
    if (pw_init(&loop, 0) == 0 || errno != EINVAL) {
        fprintf(stderr, "pw_init should fail when cap is 0\n");
        pw_free(&loop);
        return 1;
    }

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) != 0) {
        perror("socketpair");
        return 1;
    }
    if (pw_init(&loop, 4) != 0) {
        perror("pw_init");
        close(sv[0]);
        close(sv[1]);
        return 1;
    }
    if (pw_add(&loop, sv[0], POLLIN) != 0) {
        perror("pw_add");
        pw_free(&loop);
        close(sv[0]);
        close(sv[1]);
        return 1;
    }
    errno = 0;
    if (pw_add(&loop, sv[0], POLLIN) == 0 || errno != EEXIST) {
        fprintf(stderr, "duplicate pw_add should fail\n");
        pw_free(&loop);
        close(sv[0]);
        close(sv[1]);
        return 1;
    }
    if (pw_init(&tiny, 1) != 0) {
        perror("pw_init tiny");
        pw_free(&loop);
        close(sv[0]);
        close(sv[1]);
        return 1;
    }
    if (pw_add(&tiny, sv[0], POLLIN) != 0) {
        perror("pw_add tiny");
        pw_free(&tiny);
        pw_free(&loop);
        close(sv[0]);
        close(sv[1]);
        return 1;
    }
    errno = 0;
    if (pw_add(&tiny, sv[1], POLLIN) == 0 || errno != ENOSPC) {
        fprintf(stderr, "pw_add should fail when loop is full\n");
        pw_free(&tiny);
        pw_free(&loop);
        close(sv[0]);
        close(sv[1]);
        return 1;
    }
    pw_free(&tiny);
    if (write(sv[1], &ch, sizeof(ch)) != (ssize_t)sizeof(ch)) {
        perror("write");
        pw_free(&loop);
        close(sv[0]);
        close(sv[1]);
        return 1;
    }

    n = pw_wait(&loop, 1000);
    if (n <= 0) {
        fprintf(stderr, "pw_wait failed or timed out: %d\n", n);
        pw_free(&loop);
        close(sv[0]);
        close(sv[1]);
        return 1;
    }
    if ((pw_revents(&loop, sv[0]) & POLLIN) == 0) {
        fprintf(stderr, "missing POLLIN event\n");
        pw_free(&loop);
        close(sv[0]);
        close(sv[1]);
        return 1;
    }

    if (read(sv[0], &ch, sizeof(ch)) != (ssize_t)sizeof(ch)) {
        perror("read");
        pw_free(&loop);
        close(sv[0]);
        close(sv[1]);
        return 1;
    }

    if (pw_mod(&loop, sv[0], POLLOUT) != 0) {
        perror("pw_mod");
        pw_free(&loop);
        close(sv[0]);
        close(sv[1]);
        return 1;
    }
    n = pw_wait(&loop, 1000);
    if (n <= 0 || (pw_revents(&loop, sv[0]) & POLLOUT) == 0) {
        fprintf(stderr, "missing POLLOUT event\n");
        pw_free(&loop);
        close(sv[0]);
        close(sv[1]);
        return 1;
    }

    if (pw_del(&loop, sv[0]) != 0) {
        perror("pw_del");
        pw_free(&loop);
        close(sv[0]);
        close(sv[1]);
        return 1;
    }
    errno = 0;
    if (pw_del(&loop, sv[0]) == 0 || errno != ENOENT) {
        fprintf(stderr, "deleting missing fd should fail\n");
        pw_free(&loop);
        close(sv[0]);
        close(sv[1]);
        return 1;
    }
    errno = 0;
    if (pw_mod(&loop, sv[0], POLLIN) == 0 || errno != ENOENT) {
        fprintf(stderr, "modifying missing fd should fail\n");
        pw_free(&loop);
        close(sv[0]);
        close(sv[1]);
        return 1;
    }
    errno = 0;
    if (pw_wait(&loop, -2) != -1 || errno != EINVAL) {
        fprintf(stderr, "invalid timeout should fail\n");
        pw_free(&loop);
        close(sv[0]);
        close(sv[1]);
        return 1;
    }

    pw_free(&loop);
    close(sv[0]);
    close(sv[1]);
    puts("ok");
    return 0;
}
