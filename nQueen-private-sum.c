#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<omp.h>

#define MAXN 50


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

int
main(int argc, char **argv)
{
    assert(argc == 2);
    int n = atoi(argv[1]);
    assert(n <= MAXN);

    int pos[MAXN], solution = 0;
    #pragma omp parallel for private (pos) reduction(+ : solution)
    for (int i = 0; i < n; i++) {
        pos[0] = i;
        solution += nQueen(pos, 1, n);
    }
    printf("%d\n", solution);

    return 0;
}