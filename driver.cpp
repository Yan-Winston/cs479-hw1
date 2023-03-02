// By: Winston Yan and Ford Bailey
// On: Spring 2023
// For: CS479 HW 1



#include "bayes_classifier.h"



//set A
//means of features x  y
static int u_A[2][2] = {{1, 1}, {4, 4}};

//covariance matrix
static int E_A[2][2][2] = {{{1, 0}, {0, 1}}, {{1, 0}, {0, 1}}};



//set B
//means of features x  y
static int u_B[2][2] = {{1, 1}, {4, 4}};

//covariance matrices
static int E_B[2][2][2] = {{{1, 0}, {0, 1}}, {{4, 0}, {0, 8}}};





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
    std::string set;
    std::cout << "[Re]generate Data Set A (1) or B (2): ";
    std::cin >> set;
    
    if (std::stoi(set) != 2) {
        f.open("testData_A.txt", std::ios::out);

        Coordinate tempCoord;

        for (int i = 0; i < 60000; i++) {
            tempCoord.x = box_muller(u_A[0][0], E_A[0][0][0]);
            tempCoord.y = box_muller(u_A[0][1], E_A[0][1][1]);
            tempCoord.actual_class = 1;

            f << tempCoord.x << " " << tempCoord.y << " " << tempCoord.actual_class << std::endl;
        }

        for (int i = 0; i < 140000; i++) {
            tempCoord.x = box_muller(u_A[1][0], E_A[1][0][0]);
            tempCoord.y = box_muller(u_A[1][1], E_A[1][1][1]);
            tempCoord.actual_class = 2;

            f << tempCoord.x << " " << tempCoord.y << " " << tempCoord.actual_class << std::endl;
        }
        
        f.close();
    } else {
        f.open("testData_B.txt", std::ios::out);

        Coordinate tempCoord;

        for (int i = 0; i < 60000; i++) {
            tempCoord.x = box_muller(u_B[0][0], E_B[0][0][0]);
            tempCoord.y = box_muller(u_B[0][1], E_B[0][1][1]);
            tempCoord.actual_class = 1;

            f << tempCoord.x << " " << tempCoord.y << " " << tempCoord.actual_class << std::endl;
        }

        for (int i = 0; i < 140000; i++) {
            tempCoord.x = box_muller(u_B[1][0], E_B[1][0][0]);
            tempCoord.y = box_muller(u_B[1][1], E_B[1][1][1]);
            tempCoord.actual_class = 2;

            f << tempCoord.x << " " << tempCoord.y << " " << tempCoord.actual_class << std::endl;
        }
        
        f.close();
    }
}

void trainAndTest(int i) {

    std::ifstream f;
    std::ofstream out;

    std::string temp;

    double data[200000][3];
    double point[2];

    int counter = 0;
    int tests = 0;
    
    double x, y, c;

    if (i == 1) {
        f.open("testData_A.txt");
    } else {
        f.open("testData_B.txt");
    }



    std::string line;

    while (std::getline(f, line)) {
        
        std::istringstream iss(line);

        if (!(iss >> x >> y >> c)) {
            std::cout << "Unable to read file." << std::endl;
            break;
        }        

        data[counter][0] = x;
        data[counter][1] = y;
        data[counter][2] = c;
        
        counter++;
    }

    counter = 0;
    f.close();



    std::random_shuffle(std::begin(data), std::end(data));

    std::cout <<  "Would you like to train/test using Case I (1), Case III (2), or Euclidean (3)? ";
    std::cin >> temp;

    if (temp == "2") {
        //run test for case III

        if (i == 1) {
            //data set A
            BayesClassifier* classifier = new BayesClassifier(60000, 140000, u_A, E_A);

            int correct;
            int attempted;
            int guess;

            //   classed 1 as 2, 2 as 1, no class 
            int incorrect[3] = {0, 0, 0};

            out.open("Results/results_A_case_iii.txt");
            out << "File of results for Data set A using Case III" << std::endl << std::endl;

            while (tests < 10) {

                correct = 0;
                attempted = 0;
                incorrect[0] = 0;
                incorrect[1] = 0;
                incorrect[2] = 0;

                while (counter-(tests*20000) < 20000) {
                    point[0] = data[counter][0];
                    point[1] = data[counter][1];

                    guess = classifier->predict_case_iii(point);
                    
                    if (data[counter][2] == guess) {
                        //is right
                        correct++;
                    } else if (data[counter][2] < guess) {
                        //guessed 2, but was 1
                        incorrect[0]++;
                    } else if (guess != 0) {
                        //guessed 1, but was 2
                        incorrect[1]++;
                    } else {
                        //no class
                        incorrect[2]++;
                    }

                    attempted++;                    
                    counter++;
                }
                tests++;

                out << "Trial " << tests << " Results: " << std::endl;
                out << "Accuracy: " << (double) correct/attempted << std::endl;
                out << "Errors: " << std::endl;
                out << "    Classified 1 as 2: " << incorrect[0] << std::endl;
                out << "    Classified 2 as 1: " << incorrect[1] << std::endl;
                out << "    Did not classify: " << incorrect[2] << std::endl << std::endl << std::endl;
            }

            out.close();
            std::cout << "Results Saved Successfully." << std::endl << std::endl;

        } else {
            //data set B
            BayesClassifier* classifier = new BayesClassifier(60000, 140000, u_B, E_B);

            int correct;
            int attempted;
            int guess;

            //   classed 1 as 2, 2 as 1, no class 
            int incorrect[3] = {0, 0, 0};

            out.open("Results/results_B_case_iii.txt");
            out << "File of results for Data set B using Case III" << std::endl << std::endl;

            while (tests < 10) {

                correct = 0;
                attempted = 0;
                incorrect[0] = 0;
                incorrect[1] = 0;
                incorrect[2] = 0;

                while (counter-(tests*20000) < 20000) {
                    point[0] = data[counter][0];
                    point[1] = data[counter][1];

                    guess = classifier->predict_case_iii(point);
                    
                    if (data[counter][2] == guess) {
                        //is right
                        correct++;
                    } else if (data[counter][2] < guess) {
                        //guessed 2, but was 1
                        incorrect[0]++;
                    } else if (guess != 0) {
                        //guessed 1, but was 2
                        incorrect[1]++;
                    } else {
                        //no class
                        incorrect[2]++;
                    }

                    attempted++;                    
                    counter++;
                }
                tests++;

                out << "Trial " << tests << " Results: " << std::endl;
                out << "Accuracy: " << (double) correct/attempted << std::endl;
                out << "Errors: " << std::endl;
                out << "    Classified 1 as 2: " << incorrect[0] << std::endl;
                out << "    Classified 2 as 1: " << incorrect[1] << std::endl;
                out << "    Did not classify: " << incorrect[2] << std::endl << std::endl << std::endl;
            }

            out.close();
            std::cout << "Results Saved Successfully." << std::endl << std::endl;
        }

    } else if (temp == "3") {
        //run test for the euclidean distance classifier
        
        if (i == 1) {
            //data set A
            BayesClassifier* classifier = new BayesClassifier(60000, 140000, u_A, E_A);

            int correct;
            int attempted;
            int guess;

            //   classed 1 as 2, 2 as 1, no class 
            int incorrect[3] = {0, 0, 0};

            out.open("Results/results_A_case_e.txt");
            out << "File of results for Data set A using the Euclidean Distance Classifier" << std::endl << std::endl;

            while (tests < 10) {

                correct = 0;
                attempted = 0;
                incorrect[0] = 0;
                incorrect[1] = 0;
                incorrect[2] = 0;

                while (counter-(tests*20000) < 20000) {
                    point[0] = data[counter][0];
                    point[1] = data[counter][1];

                    guess = classifier->predict_case_euclid(point);
                    
                    if (data[counter][2] == guess) {
                        //is right
                        correct++;
                    } else if (data[counter][2] < guess) {
                        //guessed 2, but was 1
                        incorrect[0]++;
                    } else if (guess != 0) {
                        //guessed 1, but was 2
                        incorrect[1]++;
                    } else {
                        //no class
                        incorrect[2]++;
                    }

                    attempted++;                    
                    counter++;
                }
                tests++;

                out << "Trial " << tests << " Results: " << std::endl;
                out << "Accuracy: " << (double) correct/attempted << std::endl;
                out << "Errors: " << std::endl;
                out << "    Classified 1 as 2: " << incorrect[0] << std::endl;
                out << "    Classified 2 as 1: " << incorrect[1] << std::endl;
                out << "    Did not classify: " << incorrect[2] << std::endl << std::endl << std::endl;
            }

            out.close();
            std::cout << "Results Saved Successfully." << std::endl << std::endl;

        } else {
            //data set B
            BayesClassifier* classifier = new BayesClassifier(60000, 140000, u_B, E_B);

            int correct;
            int attempted;
            int guess;

            //   classed 1 as 2, 2 as 1, no class 
            int incorrect[3] = {0, 0, 0};

            out.open("Results/results_B_case_euclid.txt");
            out << "File of results for Data set B using the Euclidean Distance Classifier" << std::endl << std::endl;

            while (tests < 10) {

                correct = 0;
                attempted = 0;
                incorrect[0] = 0;
                incorrect[1] = 0;
                incorrect[2] = 0;

                while (counter-(tests*20000) < 20000) {
                    point[0] = data[counter][0];
                    point[1] = data[counter][1];

                    guess = classifier->predict_case_euclid(point);
                    
                    if (data[counter][2] == guess) {
                        //is right
                        correct++;
                    } else if (data[counter][2] < guess) {
                        //guessed 2, but was 1
                        incorrect[0]++;
                    } else if (guess != 0) {
                        //guessed 1, but was 2
                        incorrect[1]++;
                    } else {
                        //no class
                        incorrect[2]++;
                    }

                    attempted++;                    
                    counter++;
                }
                tests++;

                out << "Trial " << tests << " Results: " << std::endl;
                out << "Accuracy: " << (double) correct/attempted << std::endl;
                out << "Errors: " << std::endl;
                out << "    Classified 1 as 2: " << incorrect[0] << std::endl;
                out << "    Classified 2 as 1: " << incorrect[1] << std::endl;
                out << "    Did not classify: " << incorrect[2] << std::endl << std::endl << std::endl;
            }

            out.close();
            std::cout << "Results Saved Successfully." << std::endl << std::endl;
        }

    } else {
        //run test for case I

        if (i == 1) {
            //data set A
            BayesClassifier* classifier = new BayesClassifier(60000, 140000, u_A, E_A);

            int correct;
            int attempted;
            int guess;

            //   classed 1 as 2, 2 as 1, no class 
            int incorrect[3] = {0, 0, 0};

            out.open("Results/results_A_case_i.txt");
            out << "File of results for Data set A using Case I" << std::endl << std::endl;

            while (tests < 10) {

                correct = 0;
                attempted = 0;
                incorrect[0] = 0;
                incorrect[1] = 0;
                incorrect[2] = 0;

                while (counter-(tests*20000) < 20000) {
                    point[0] = data[counter][0];
                    point[1] = data[counter][1];

                    guess = classifier->predict_case_i(point);
                    
                    if (data[counter][2] == guess) {
                        //is right
                        correct++;
                    } else if (data[counter][2] < guess) {
                        //guessed 2, but was 1
                        incorrect[0]++;
                    } else if (guess != 0) {
                        //guessed 1, but was 2
                        incorrect[1]++;
                    } else {
                        //no class
                        incorrect[2]++;
                    }

                    attempted++;                    
                    counter++;
                }
                tests++;

                out << "Trial " << tests << " Results: " << std::endl;
                out << "Accuracy: " << (double) correct/attempted << std::endl;
                out << "Errors: " << std::endl;
                out << "    Classified 1 as 2: " << incorrect[0] << std::endl;
                out << "    Classified 2 as 1: " << incorrect[1] << std::endl;
                out << "    Did not classify: " << incorrect[2] << std::endl << std::endl << std::endl;
            }

            out.close();
            std::cout << "Results Saved Successfully." << std::endl << std::endl;

        } else {
            //data set B
            BayesClassifier* classifier = new BayesClassifier(60000, 140000, u_B, E_B);

            int correct;
            int attempted;
            int guess;

            //   classed 1 as 2, 2 as 1, no class 
            int incorrect[3] = {0, 0, 0};

            out.open("Results/results_B_case_i.txt");
            out << "File of results for Data set B using Case I" << std::endl << std::endl;

            while (tests < 10) {

                correct = 0;
                attempted = 0;
                incorrect[0] = 0;
                incorrect[1] = 0;
                incorrect[2] = 0;

                while (counter-(tests*20000) < 20000) {
                    point[0] = data[counter][0];
                    point[1] = data[counter][1];

                    guess = classifier->predict_case_i(point);
                    
                    if (data[counter][2] == guess) {
                        //is right
                        correct++;
                    } else if (data[counter][2] < guess) {
                        //guessed 2, but was 1
                        incorrect[0]++;
                    } else if (guess != 0) {
                        //guessed 1, but was 2
                        incorrect[1]++;
                    } else {
                        //no class
                        incorrect[2]++;
                    }

                    attempted++;                    
                    counter++;
                }
                tests++;

                out << "Trial " << tests << " Results: " << std::endl;
                out << "Accuracy: " << (double) correct/attempted << std::endl;
                out << "Errors: " << std::endl;
                out << "    Classified 1 as 2: " << incorrect[0] << std::endl;
                out << "    Classified 2 as 1: " << incorrect[1] << std::endl;
                out << "    Did not classify: " << incorrect[2] << std::endl << std::endl << std::endl;
            }

            out.close();
            std::cout << "Results Saved Successfully." << std::endl << std::endl;
        }
    }

}

void debugPlot() {
    std::cout << "Run \"python plot.py\" to visualize the data." << std::endl << std::endl;
}

int menu() {
    std::string choice;

    std::cout << "Please select an option:" << std::endl;
    std::cout << "(1)     Generate New Data" << std::endl;
    std::cout << "(2)     Train and Test" << std::endl;
    std::cout << "(3)     Plot Data" << std::endl;
    std::cout << "(0)     Quit" << std::endl;
    std::cout << ">> ";
    std::cin >> choice;

    std::cout << std::endl << std::endl;

    return std::stoi(choice);
}



int main() {

    std::string temp = "";
    bool run = true;

    while(run) {
        switch(menu()) {
            case 1:
                generateData();
                break;

            case 2:

                std::cout << "Data set A(1) or B(2)? ";
                std::cin >> temp;
                std::cout << std::endl;
                if (std::stoi(temp) != 1) {
                    trainAndTest(2);
                } else {
                    trainAndTest(1);
                }

                break;
            
            case 3:
                debugPlot();
                break;

            default:
                run = false;
                break;
        }
    }

    return 0;
}