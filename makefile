bayesianClassifier: driver.o bayes_classifier.o
	g++ -o bayesianClassifier driver.o bayes_classifier.o -std=c++11

driver.o: driver.cpp bayes_classifier.cpp bayes_classifier.h
	g++ -c driver.cpp

bayes_classifier.o: bayes_classifier.cpp bayes_classifier.h
	g++ -c bayes_classifier.cpp

clear: 
	rm *.o