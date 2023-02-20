// By: Winston Yan
// On: Spring 2023
// For: CS479 HW 1


#include <iostream>
#include "classifier.h"



int menu() {
    std::string choice;

    std::cout << "Please select an option:" << std::endl;
    std::cout << "(1)     Generate New Data" << std::endl;
    std::cout << "(2)     Train and Test" << std::endl;
    std::cout << "(3)     Generate New Data, Train, and Test" << std::endl;
    std::cout << "(0)     Quit" << std::endl;
    std::cout << ">> ";
    std::cin >> choice;

    std::cout << std::endl << std::endl;

    return std::stoi(choice);
}

void generateData() {

}

void trainAndTest() {

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
                generateData();
                trainAndTest();
                break;

            default:
                run = false;
                break;
        }
    }

    return 0;
}