#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<pthread.h>
#include <sys/time.h>
#define MAXN 50

int n;
int solution;
pthread_mutex_t solutionLock;

int isValid(int *pos, int cur, int cur_pos)
{
    for (int i = 0; i < cur; i++) {
        if (pos[i] == cur_pos)
            return 0;
        if (cur - i == cur_pos - pos[i])
            return 0;
        if (cur + cur_pos == i + pos[i])
            return 0;
    }
    return 1;
}

void
nQueen(int *pos, int cur, int n)
{
    if (cur == n) {
        pthread_mutex_lock(&solutionLock);
        solution++;
        pthread_mutex_unlock(&solutionLock);
    }
    for (int i = 0; i < n; i++) {
        if (cur == 0 || isValid(pos, cur, i)) {
            pos[cur] = i;
            nQueen(pos, cur + 1, n);
        }
    }
}

void *go_nQueen(void *pos)
{
    nQueen((int *)pos, 1, n);
    pthread_exit(NULL);
}

int
main(int argc, char **argv)
{
    assert(argc == 2);
    n = atoi(argv[1]);
    assert(n <= MAXN);

    pthread_t threads[MAXN];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_mutex_init(&solutionLock, NULL);

    for (int i = 0; i < n; i++) {
        int *pos = (int *) alloca(n * sizeof(int));
        pos[0] = i;
        if (pthread_create(&threads[i], NULL, go_nQueen, (void *)pos))
            exit(1);
    }
    for (int i = 0; i < n; i++)
        pthread_join(threads[i], NULL);
    printf("%d\n", solution);

    pthread_exit(NULL);

    return 0;
}