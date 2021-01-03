#include "auto_diff.h"
#include "parser.h"

int main() {

	std::cout << "Hello, World!" << std::endl;
	std::string input = "x1+exp(x0)";
	map<std::string, double> inits;
	inits["x0"] = 1.0;
	inits["x1"] = 2.0;

   std::vector<token> token_list = parser(input, inits).token_list;

	for (auto i : token_list) {
		std::cout << i.first_char << std::endl;
	}

    return 0;
}
