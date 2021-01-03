#ifndef PARSER_H
#define PARSER_H

#include "sys.h"
#include "token.h"

using namespace std;

class parser {

public:

    map<char, int> precedence  =  {{'+', 10},
                                    {'-', 10},
                                    {'/', 25},
                                    {'*', 20},
                                    {'^', 30},
                                    {'(', 0},
                                    {')', 0}};

    const char* functions[16] = {"cos", "sin", "tan",
                                 "csc", "sec", "cot", 
                                 "sinh", "cosh", "tanh",
                                 "acos", "asin", "atan", 
                                 "log", "exp", "sqrt", "lgs"};

    char functions_char[7] = { 'c', 's', 't', 'l', 's', 'a', 'e' };
    const int function_length = 7;

    // known function_ identifier constants

    char operators[7] = {'+', '-', '/', '*', '^', '(', ')'};
    const int operators_length = 7;


    char variables[10] = {'w', 'x', 'y', 'z', 'k', 'm', 'd', 'v', 'u', 'o'};
    const int variables_length = 10;

    vector<token> token_list;
    map<std::string, double> initial_values;

    parser(std::string s, map<std::string, double> inits);
    
    static bool is_in_array(char p, char* array, int length);
    std::vector<token> tokenizer(std::string string); 
    std::vector<token> parse(std::vector<token> input);
    void dynamic_log_handler(std::string input);
    std::pair<double, int> decimal_reader(std::string input, int input_distance);
};


#endif //FINALPROJECT_TRACE_PARSER_H