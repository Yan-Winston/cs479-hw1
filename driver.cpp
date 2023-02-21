// By: Winston Yan
// On: Spring 2023
// For: CS479 HW 1


#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>

#include "classifier.h"


//means for         x  y
static int u1[2] = {1, 1};
static int u2[2] = {4, 4};

//covariance matrices
static int E1[2][2] = {{1, 0}, {0, 1}};
static int E2[2][2] = {{1, 0}, {0, 1}};



struct Coordinate{
    double x, y;
    int actual_class; //either 1 or 2
};

double ranf() {
    return (rand()/(double)RAND_MAX);
}

double box_muller(double m, double s) {
    double x1, x2, w, y1;
    static double y2;
    static bool use_last = false;

    if (use_last) {
        y1 = y2;
        use_last = false;
    } else {
        do {
            x1 = 2*ranf() - 1;
            x2 = 2*ranf() - 1;
            w = x1 * x1 + x2 * x2;
        } while (w >= 1.0);

        w = sqrt((-2*log(w))/w);

        y1 = x1 * w;
        y2 = x2 * w;
        use_last = true;
    }

    return(m+y1*s);
}

void generateData() {
    std::ofstream f;
    f.open("testData.csv", std::ios::out);

    Coordinate tempCoord;

    for (int i = 0; i < 60000; i++) {
        tempCoord.x = box_muller(u1[0], E1[0][0]);
        tempCoord.y = box_muller(u1[1], E1[1][1]);
        tempCoord.actual_class = 1;

        f << tempCoord.x << "," << tempCoord.y << "," << tempCoord.actual_class << std::endl;
    }

    for (int i = 0; i < 140000; i++) {
        tempCoord.x = box_muller(u2[0], E2[0][0]);
        tempCoord.y = box_muller(u2[1], E2[1][1]);
        tempCoord.actual_class = 2;

        f << tempCoord.x << "," << tempCoord.y << "," << tempCoord.actual_class << std::endl;
    }
    f.close();
}

void trainAndTest() {

}

void debugPlot() {
    //figure out smthn to plot
}

int menu() {
    std::string choice;

    std::cout << "Please select an option:" << std::endl;
    std::cout << "(1)     Generate New Data" << std::endl;
    std::cout << "(2)     Train and Test" << std::endl;
    std::cout << "(3)     Plot Data" << std::endl;
    std::cout << "(4)     Run All Above" << std::endl;
    std::cout << "(0)     Quit" << std::endl;
    std::cout << ">> ";
    std::cin >> choice;

    std::cout << std::endl << std::endl;

    return std::stoi(choice);
}



int main() {

    bool run = true;

    while(run) {
        switch(menu()) {
            case 1:
                generateData();
                break;

            case 2:
                trainAndTest();
                break;
            
            case 3:
                debugPlot();
                break;
            case 4:
                generateData();
                debugPlot();
                trainAndTest();
                break;

            default:
                run = false;
                break;
        }
    }

    return 0;
}