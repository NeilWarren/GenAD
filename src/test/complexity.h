#ifndef COMPLEXITY_H
#define COMPLEXITY_H

#include "../auto_diff.h"
#include <chrono> 

class ComplexityEvaluator {

public:
    long input_length;
    long trials;
    string function;
    map<std::string, double> inits;


    ComplexityEvaluator(long input, long t) {
    	input_length = input;
    	trials = t;
    	function = "x0";
    	inits["x0"] = 1.0;

    	for (long i = 1; i < input_length; ++i) {
    		string var_name = "*x" + std::to_string(i);
    		inits[var_name] = 1.0;
    	}
    }

    float eval();

};

#endif //COMPLEXITY_H