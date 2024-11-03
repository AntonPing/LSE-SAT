#ifndef SOLVER_H
#define SOLVER_H

struct matrix {
    int row; // number of variables
    int col; // number of clauses
    int* sign_mat;
};

void solve_sat(const struct matrix* mat);

#endif