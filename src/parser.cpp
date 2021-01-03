#include "parser.h"

void parser::dynamic_log_handler(std::string input) {}

bool parser::is_in_array(char p, char *array, int length) {
    for (int i = 0; i < length; ++i) {
        if ( array[i] == p) {
            return true;
        }
    }
    return false;
}

std::pair<double, int> parser::decimal_reader(std::string input, int input_distance) {

    double const_val = 0;
    stack<char> const_stack;
    int i=0;
    bool track_dots = false;

    auto cstr_it = input.begin() + input_distance;

    while (isdigit((int)*cstr_it) != 0 || *cstr_it == '.') {
        if (*cstr_it == '.') {
            const_stack.push(*cstr_it);
            ++cstr_it;
            ++input_distance;
        }
    }

    while (!const_stack.empty()) {
        if (const_stack.top() != '.') {
            const_val += ((int)(const_stack.top() - '0')) * std::pow(10.0, i);
            i++;             
        } else {
            const_val /= std::pow(10.0, i);
            i = 0;
        }
    const_stack.pop();
    }

    return std::pair<double, int>(const_val, input_distance);
}

std::vector<token> parser::tokenizer(std::string input) {

    remove(input.begin(), input.end(),' ');

    vector<token> token_list;
    stack<char> const_stack;
    
    // will store constants here as we loop through
    double const_val = 0;

    auto cstr_it = input.begin();

    while(cstr_it != input.end()) {
        if (is_in_array(*cstr_it, functions_char, function_length)) {
            std::cout << 1 << std::endl;


            token t = token(*cstr_it, token::func);

            std::string f;

            while (*cstr_it != '(') {
                 f += *cstr_it;
                ++cstr_it;
            }

            if (std::find(begin(functions), end(functions), f) != end(functions)) {    // setting the name of the function f as the Token.func_type                    if (std::find(begin(functions), end(functions), f) != end(functions)) {
                t.set_func_type(f);
                token_list.push_back(t);
            }    

        } else if (is_in_array(*cstr_it, variables, variables_length)) {
            std::cout << 2 << std::endl;

            std::pair<float, int> reader = decimal_reader(input, (int)std::distance(input.begin(), cstr_it));
            std::cout << reader.second << std::endl;
            std::cout << (int)std::distance(input.begin(), cstr_it) << std::endl;
            std::string sub_string = input.substr((int)std::distance(input.begin(), cstr_it) , reader.second + 1);
            token t = token(*cstr_it, token::var, 0.0, this->initial_values[sub_string]);
            token_list.push_back(t);
            cstr_it += reader.second + 1;
            

        } else if (is_in_array(*cstr_it, operators, operators_length)) {
            std::cout << 3 << std::endl;

            
            bool add_mul = false;

            if (*cstr_it == '-' && (token_list.empty() || 
                ((is_in_array(*(cstr_it - 1), operators, operators_length) && 
                    *(cstr_it-1) != ')')))) {

                cstr_it++;
                
                const_val = decimal_reader(input, (int)std::distance(input.begin(), cstr_it)).first;
                cstr_it = input.begin() + decimal_reader(input, (int)std::distance(input.begin(), cstr_it)).second + 1;

                if (const_val == 0) {
                    const_val = 1;
                    add_mul = true;
                }

                if (!add_mul) {
                    token t = token('C', token::constant, 0, -const_val);
                    token_list.push_back(t);
                } else {
                    token t = token('C', token::constant, 0, -const_val);
                    token_list.push_back(t);
                    token_list.push_back(token('*', token::binary_op, precedence['*']));
                } 

            } else {

                token t = token(*cstr_it, token::binary_op, 0, precedence[*cstr_it]);
                token_list.push_back(t);
                cstr_it++;
            }
        }
        else if (isdigit((int)*cstr_it) != 0 || (*cstr_it == '.' && isdigit((int)*(cstr_it+1)))) {
            std::cout << 4 << std::endl;

            const_val = decimal_reader(input, (int)std::distance(input.begin(), cstr_it)).first;
            cstr_it = input.begin() + decimal_reader(input, (int)std::distance(input.begin(), cstr_it)).second + 1;

            token t = token('C', token::constant, 0, const_val);
            token_list.push_back(t);
        }
        else {
            std::cout << 5 << std::endl;
            // error handling
            std::string msg = "unable to parse input character: ";
            msg += *cstr_it;
            throw std::invalid_argument(msg);
        }
    }

    return token_list;
}


std::vector<token> parser::parse(std::vector<token> input) {

    std::vector<token> output;

    std::stack<token> op_stack;

    for (auto & token_it : input) {
        if (token_it.type == token::var || token_it.type == token::constant) {
            output.push_back(token_it);
        } else if (token_it.type == token::func) {
            op_stack.push(token_it);
        } else if (token_it.type == token::binary_op && token_it.first_char!= '(' && token_it.first_char != ')') {
            while (!op_stack.empty() && op_stack.top().first_char != '(' &&
                (op_stack.top().precedence > token_it.precedence || 
                (op_stack.top().precedence == token_it.precedence && token_it.first_char != '^'))) {
                
                output.push_back(op_stack.top());
                op_stack.pop();
            }
            op_stack.push(token_it);
        } else if (token_it.first_char == '(') {
            op_stack.push(token_it);
        } else if (token_it.first_char == ')') {
            while (!op_stack.empty() && op_stack.top().first_char != '(') {
                output.push_back(op_stack.top());
                op_stack.pop();
            }
            if (!op_stack.empty() && op_stack.top().first_char == '(') {
                op_stack.pop();
                if (!op_stack.empty() && op_stack.top().type == token::func) {
                    output.push_back(op_stack.top());
                    op_stack.pop();
                }
            }
        }
    }

    while (!op_stack.empty()) {
        if (op_stack.top().first_char != '(' && op_stack.top().first_char != ')') {
            output.push_back(op_stack.top());
        }
        op_stack.pop();
    }
    return output;
}


parser::parser(std::string s, map<std::string, double> inits) {
    this->initial_values = inits;
    this->token_list = this->parse(this->tokenizer(s));
}

