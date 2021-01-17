#include "complexity.h"

using namespace std;
using namespace std::chrono; 


float ComplexityEvaluator::eval_ad(int mode) {

    std::vector<Token> token_list = parser(this->function, this->inits).token_list;

	auto start = high_resolution_clock::now();
	for (long i = 0; i < this->trials; ++i) {

        if (mode == 1) {
            AutoDiffRev ad = AutoDiffRev(token_list, this->inits);
            Token result = ad.eval();
        } if (mode == 0) {
            AutoDiffFwd ad = AutoDiffFwd(token_list);
            Token result = ad.eval();
        }

		if (i == 9) {
			start = high_resolution_clock::now(); 
		}
	}
	auto stop = high_resolution_clock::now(); 

	auto duration = duration_cast<microseconds>(stop - start); 

	long result = (long)duration.count();

	return result / (this->trials - 10);

}

float ComplexityEvaluator::eval_parse() {

    std::vector<Token> token_list;
    auto start = high_resolution_clock::now();
    for (long i = 0; i < this->trials; ++i) {
        std::vector<Token> token_list = parser(this->function, this->inits).token_list;
        if (i == 9) {
            start = high_resolution_clock::now();
        }
    }
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);

    long result = (long)duration.count();

    return result / (this->trials - 10);

}
