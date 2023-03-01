#ifndef BAYES_CLASSIFIER_H
#define BAYES_CLASSIFIER_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include "Eigen/Dense"


class BayesClassifier {
    private:
        
        int num_1;
        int num_2;

        double priors[2];

        // coordinates of the mean
        int means[2][2];

        //variances
        int vars[2][2][2];

        double bayes_i(int guess, double point[2]); //posterior p(Cn/x)
        double bayes_iii(int guess, double point[2]); //posterior p(Cn/x)
        double gauss_pdf_i(int guess, double point[2], int mu[2], int sigma[2][2]); //likelihood p(x/Cn)
        double gauss_pdf_iii(int guess, double point[2], int mu[2], int sigma[2][2]); //likelihood p(x/Cn)

        double risk(int action, int guess, double evidence);
        int loss(int action, int guess); //Zero-one loss

    public:
        BayesClassifier(int num_c1, int num_c2, int means[2][2], int vars[2][2][2]);

        int predict_case_i(double point[2]);
        int predict_case_iii(double point[2]);
};

#endif