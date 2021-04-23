#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<pthread.h>
#include <sys/time.h>
#define MAXN 50

int n;

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

int
nQueen(int *pos, int cur, int n)
{
    if (cur == n)
        return 1;
    int ways = 0;
    for (int i = 0; i < n; i++) {
        if (cur == 0 || isValid(pos, cur, i)) {
            pos[cur] = i;
            ways += nQueen(pos, cur + 1, n);
        }
    }
    return ways;
}

void *go_nQueen(void *pos)
{
    int *sum = (int *) malloc(sizeof(int));
    *sum = nQueen((int *)pos, 1, n);
    pthread_exit(sum);
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

    for (int i = 0; i < n; i++) {
        int *pos = (int *) alloca(n * sizeof(int));
        pos[0] = i;
        if (pthread_create(&threads[i], NULL, go_nQueen, (void *)pos))
            exit(1);
    }
    int solution = 0;
    int *result;
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], (void **)&result);
        solution += *result;
        free(result);
    }
    printf("%d\n", solution);

    pthread_attr_destroy(&attr);
    pthread_exit(NULL);

    return 0;
}