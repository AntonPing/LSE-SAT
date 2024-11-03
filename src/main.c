#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "../include/solver.h"

void test() {
    struct matrix mat;
    int sign[25] = {
        0, 0, 0, 1, 1,
        1, 0, 0, -1, 0,
        0, 1, -1, -1, 0,
        0, -1, 0, 0, -1,
        0, 0, 1, 0, -1,
    };
    mat.row = 5;
    mat.col = 5;
    mat.sign_mat = sign;
    srand((unsigned) time(NULL));
    solve_sat(&mat);
}

int main() {
    printf("hello\n");
    test();
}