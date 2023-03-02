#include "bayes_classifier.h"



using namespace Eigen;



BayesClassifier::BayesClassifier(int num_c1, int num_c2, int m[2][2], int v[2][2][2]) {
    num_1 = num_c1;
    num_2 = num_c2;
    
    // i know there's gotta be a better way but its 1 am :)
    means[0][0] = m[0][0];
    means[0][1] = m[0][1];
    means[1][0] = m[1][0];
    means[1][1] = m[1][1];

    vars[0][0][0] = v[0][0][0];
    vars[0][0][1] = v[0][0][1];
    vars[0][1][0] = v[0][1][0];
    vars[0][1][1] = v[0][1][1];
    vars[1][0][0] = v[1][0][0];
    vars[1][0][1] = v[1][0][1];
    vars[1][1][0] = v[1][1][0];
    vars[1][1][1] = v[1][1][1];

    priors[0] = num_1 / (double) (num_1+num_2);
    priors[1] = num_2 / (double) (num_1+num_2);
}


int BayesClassifier::predict_case_i(double point[2]) {
    double evidence_1 = bayes_i(1, point);
    double evidence_2 = bayes_i(2, point);

    double r1 = risk(1, 1, evidence_2); //risk of classing 1
    double r2 = risk(2, 2, evidence_1); //risk of classing 2

    if (r1 < r2) {
        return 1;
    } else if (r1 > r2) {
        return 2;
    } else {
        return 0;
    }
}

int BayesClassifier::predict_case_iii(double point[2]) {
    double evidence_1 = bayes_iii(1, point);
    double evidence_2 = bayes_iii(2, point);


    double r1 = risk(1, 1, evidence_2); //risk of classing 1
    double r2 = risk(2, 2, evidence_1); //risk of classing 2
    
    if (r1 < r2) {
        return 1;
    } else if (r1 > r2) {
        return 2;
    } else {
        return 0;
    }
}

int BayesClassifier::predict_case_euclid(double point[2]) {
    double evidence_1 = euclid(1, point);
    double evidence_2 = euclid(2, point);


    double r1 = risk(1, 1, evidence_2); //risk of classing 1
    double r2 = risk(2, 2, evidence_1); //risk of classing 2
    
    if (r1 < r2) {
        return 1;
    } else if (r1 > r2) {
        return 2;
    } else {
        return 0;
    }
}



double BayesClassifier::bayes_i(int guess, double point[2]) {
    return gauss_pdf_i(guess, point, means[guess-1], vars[guess-1]) * priors[guess-1];
}

double BayesClassifier::bayes_iii(int guess, double point[2]) {
    return gauss_pdf_iii(guess, point, means[guess-1], vars[guess-1]) * priors[guess-1];
}

double BayesClassifier::euclid(int guess, double point[2]) {
    return gauss_pdf_euclid(guess, point, means[guess-1], vars[guess-1]) * priors[guess-1];
}

double BayesClassifier::gauss_pdf_i(int guess, double p[2], int m[2], int s[2][2]) {
    Vector2d point;
    Vector2d mean;
    Matrix2d variance;

    point << p[0], p[1];
    mean << m[0], m[1];
    variance << s[0][0], s[0][1],
                s[1][0], s[1][1];

    double x = -(point - mean).transpose() * (point - mean);
    double y = (2 * sqrt(variance(0,0)));
    double z = log(priors[guess-1]); 

    return (x/y) + y;
}

double BayesClassifier::gauss_pdf_iii(int guess, double p[2], int m[2], int s[2][2]) {
    Vector2d point;
    Vector2d mean;
    Matrix2d variance;

    point << p[0], p[1];
    mean << m[0], m[1];
    variance << s[0][0], s[0][1],
                s[1][0], s[1][1];

    double xtWix = (point.transpose() * (-0.5 * variance.inverse()) * point);
    double wix = ((variance.inverse() * mean).transpose() * point);
    double wio = (-0.5 * mean.transpose() * variance.inverse() * mean) - (0.5 * log(variance.determinant())) + log(priors[guess-1]);

    return xtWix + wix + wio;
}

double BayesClassifier::gauss_pdf_euclid(int guess, double p[2], int m[2], int s[2][2]) {
    Vector2d point;
    Vector2d mean;
    Matrix2d variance;

    point << p[0], p[1];
    mean << m[0], m[1];
    variance << s[0][0], s[0][1],
                s[1][0], s[1][1];

    return -(point-mean).transpose() * (point-mean);
}



double BayesClassifier::risk(int action, int guess, double evidence) {
    return (double) loss(action, guess) * evidence;
}

int BayesClassifier::loss(int action, int guess) {
    if (action == guess) {
        return 1;
    }
    return 0;
}