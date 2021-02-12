#include <iostream>
#include <fstream>

#include "auto_diff.h"

using namespace std;

std::string parser_print(vector<Token> tokenVec) {
    string output ="";
    int index =0;
    for (auto & it : tokenVec) {
        switch(it.type) {
            case 0:
                cout << "Token #" << index << " is a binary_op with first_char: " << it.first_char;
                break;
            case 1:
                cout << "Token #" << index << " is a func with operation: " << it.operation;
                break;
            case 2:
                cout << "Token #" << index << " is a var with var_name: " << it.var_name << " and value " << it.num_val;
                break;
            case 3:
                cout << "Token #" << index << " is a constant with value: " << it.num_val;
                break;
        }
       cout << endl;
        index++;
    }
    return output;
}

int main() {


	std::cout << "Hello, World!" << std::endl;

    
	std::pair<string, map<std::string, double>> file_output = autodiff_file_reader("example.txt");
	auto vm = var_map(file_output);

    string input = vm.out_func;
    cout << input << endl;
    map<std::string, double> inits = vm.out_inits;

    std::vector<Token> token_list = parser(input, inits).token_list;

    cout << parser_print(token_list) << endl;

    /*
    AutoDiffFwd ad = AutoDiffFwd(token_list);
    Token result = ad.eval();
    cout << result.num_val << endl;

    for (auto & it2 : result.derivs) {
        cout << "df/d" << it2.first << " = " << it2.second << endl;
    }
    */

    AutoDiffRev ad2 = AutoDiffRev(token_list, inits);
    Token result2 = ad2.eval();
    cout << result2.num_val << endl;

    for (auto & it2 : vm.swap(result2.derivs)) {
        cout << "df/d" << it2.first << " = " << it2.second << endl;
    }
    /*

    ofstream file;

    file.open("data_parser.txt");
    for (int i=1; i < 8; ++i) {
        cout << "i: " << i << endl;
        file << std::to_string(ComplexityEvaluator((long)pow(2.0, (double)i), 100).eval_parse());
        file << "\n";
    }
    file.close();


    file.open("data_rev.txt");
    for (int i=1; i < 8; ++i) {
        cout << "i: " << i << endl;
        file << std::to_string(ComplexityEvaluator((long)pow(2.0, (double)i), 100).eval_ad(1));
        file << "\n";
    }
    file.close();

    file.open("data_fwd.txt");
    for (int i=1; i < 8; ++i) {
        cout << "i: " << i << endl;
        file << std::to_string(ComplexityEvaluator((long)pow(2.0, (double)i), 100).eval_ad(0));
        file << "\n";
    }
    file.close();
    */

    return 0;
}
