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
	std::string input = "cos(x0)^sin(x1)";
	map<std::string, double> inits;
	inits["x0"] = 1.2;
	inits["x1"] = 1.3;
	inits["x2"] = 2.0;

   std::vector<Token> token_list = parser(input, inits).token_list;

    cout << parser_print(token_list) << endl;

    AutoDiffFwd ad = AutoDiffFwd(token_list);
    Token result = ad.eval();
    cout << result.num_val << endl;

    for (auto & it2 : result.derivs) {
        cout << "df/d" << it2.first << " = " << it2.second << endl;
    }

    AutoDiffRev ad2 = AutoDiffRev(token_list, inits);
    Token result2 = ad2.eval();
    cout << result2.num_val << endl;

    for (auto & it2 : result2.derivs) {
        cout << "df/d" << it2.first << " = " << it2.second << endl;
    }


    return 0;
}
