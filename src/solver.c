#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

struct matrix {
    int row; // number of variables
    int col; // number of clauses
    int* sign_mat;
};

int get_sign(const struct matrix* mat, int i, int j) {
    assert(i >= 0 && i < mat->row);
    assert(j >= 0 && j < mat->col);
    return mat->sign_mat[i * mat->col + j];
}

struct state {
    double scale;
    double* xs; // vector of (row)

    double* exp_val; // matrix of (row, col)
    double* col_sum; // vector of (col)
    double y;
};

double x_rand()
{
    double x = (double)rand() / (double)RAND_MAX;
    return 2 * x - 1;
}

double full_update(const struct matrix* mat, struct state* state) {
    int row = mat->row;
    int col = mat->col;
    double scale = state->scale;

    for(int j = 0; j < col; j ++) {
        state->col_sum[j] = 0;
    }

    for(int i = 0; i < row; i ++) {
        double x = state->xs[i];
        for(int j = 0; j < col; j ++) {
            int sign = get_sign(mat, i, j);
            if(sign == 0) {
                state->exp_val[i * col + j] = 0;
            } else {
                state->exp_val[i * col + j] = exp(sign * scale * x);
                state->col_sum[j] += state->exp_val[i * col + j];
            }
        }
    }

    double sum = 0;
    for(int j = 0; j < col; j ++) {
        sum += 1 / state->col_sum[j];
    }

    state->y = -1 / scale * log(sum);
    return state->y;
}

double update_x(const struct matrix* mat, struct state* state, int i, double new_x) {
    int row = mat->row;
    int col = mat->col;
    double scale = state->scale;

    state->xs[i] = new_x;
    for(int j = 0; j < col; j ++) {
        int sign = get_sign(mat, i, j);
        if(sign == 0) {
            assert(state->exp_val[i * col + j] == 0);
            // state->exp_val[i * col + j] = 0;
        } else {
            state->col_sum[j] -= state->exp_val[i * col + j];
            state->exp_val[i * col + j] = exp(sign * scale * new_x);
            state->col_sum[j] += state->exp_val[i * col + j];
        }
    }

    double sum = 0;
    for(int j = 0; j < col; j ++) {
        sum += 1 / state->col_sum[j];
    }
    state->y = -1 / scale * log(sum);
    return state->y;
}

double try_random_update_x(const struct matrix* mat, struct state* state, int i) {
    double old_x = state->xs[i];
    double old_y = state->y;
    double new_x = x_rand();
    double new_y = update_x(mat, state, i, new_x);

    if(new_y > old_y) {
        return new_y;
    } else {
        update_x(mat, state, i, old_x);
        return old_y;
    }
};

#define BIG_ITER 10000
#define SMALL_ITER 10000
#define SCALE_START 0.1
#define SCALE_STEP 0.01

void solve_sat(const struct matrix* mat) {
    int row = mat->row;
    int col = mat->col;

    struct state state;
    state.xs = malloc(row * sizeof(double));
    state.exp_val = malloc(row * col * sizeof(double));
    state.col_sum = malloc(col * sizeof(double));
    state.y = 0;

    state.scale = SCALE_START;

    for(int bi = 0; bi < BIG_ITER; bi ++) {
        full_update(mat, &state);
        for(int si = 0; si < SMALL_ITER; si ++) {
            int i = rand() % row;
            // printf("%lf, scale=%lf, change %d\n", state.y, state.scale, i + 1);
            try_random_update_x(mat, &state, i);
        }
        printf("%lf, scale=%lf\n", state.y, state.scale);
        state.scale += SCALE_STEP;
    }

    free(state.xs);
    free(state.exp_val);
    free(state.col_sum);
}