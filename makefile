bayesianClassifier: driver.o classifier.o
	g++ -o bayesianClassifier driver.o classifier.o -std=c++11

driver.o: driver.cpp classifier.cpp classifier.h
	g++ -c driver.cpp

classifier.o: classifier.cpp classifier.h
	g++ -c classifier.cpp

clear: 
	rm *.o