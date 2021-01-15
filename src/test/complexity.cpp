#include "complexity.h"

using namespace std;
using namespace std::chrono; 


float ComplexityEvaluator::eval() {

	auto start = high_resolution_clock::now(); 
	for (long i = 0; i < this->trials; ++i) {
		std::vector<Token> token_list = parser(this->function, this->inits).token_list;
		if (i == 10) {
			start = high_resolution_clock::now(); 
		}
	}
	auto stop = high_resolution_clock::now(); 

	auto duration = duration_cast<microseconds>(stop - start); 

	long result = (long)duration.count();

	return result / this->trials;

}