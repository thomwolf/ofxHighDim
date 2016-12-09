/*
 *  tsne.h
 *  Header file for t-SNE.
 *
 *  Created by Laurens van der Maaten.
 *  Copyright 2012, Delft University of Technology. All rights reserved.
 *
 *  Multicore version by Dmitry Ulyanov, 2016. dmitry.ulyanov.msu@gmail.com
 */


#ifndef TSNE_H
#define TSNE_H


static inline double sign(double x) { return (x == .0 ? .0 : (x < .0 ? -1.0 : 1.0)); }


class TSNE
{
public:
    void run(double* X, int N, int D, double* Y, int *fixedPointsIndexes, int no_dims, double perplexity, double theta, int max_iter, bool runManually=false);
    void runIteration(int N_fixed_points = 0);
    bool load_data(double** data, int* n, int* d, double* theta, double* perplexity);
    void save_data(double* data, int* landmarks, double* costs, int n, int d);
    void symmetrizeMatrix(int** row_P, int** col_P, double** val_P, int N);
    void finish();
private:
//    int num_threads;
    void computeGradient(int* inp_row_P, int* inp_col_P, double* inp_val_P, double* Y, int N, int D, double* dC, double theta);
    double evaluateError(int* row_P, int* col_P, double* val_P, double* Y, int N, double theta);
    void zeroMean(double* X, int N, int D);
    void computeGaussianPerplexity(double* X, int N, int D, int** _row_P, int** _col_P, double** _val_P, double perplexity, int K);
    double randn();

    bool exact;
    float total_time;
    time_t start, end;
    int iter, max_iter, stop_lying_iter, mom_switch_iter;
    double momentum, final_momentum;
    double eta;
    double* dY, *uY, *gains;
    double* P, *val_P;
    int *row_P, *col_P;
    int *fixedPointsIndexes;
    
    double *X, *Y;
    int N, D, no_dims, N_fixed_points;
    double perplexity, theta;
};

#endif

