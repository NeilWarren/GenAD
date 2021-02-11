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

    // Note doesn't work with cmake 3.17
    while ((cstr_it != input.end()) && (isdigit((int)*cstr_it) != 0 || *cstr_it == '.')) {
        if (*cstr_it == '.') {
            track_dots = true;
        }
        const_stack.push(*cstr_it);
        ++cstr_it;
        ++input_distance;
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

std::vector<Token> parser::tokenizer(std::string input) {

    remove(input.begin(), input.end(),' ');

    vector<Token> token_list;
    stack<char> const_stack;
    double const_val = 0;

    auto cstr_it = input.begin();

    while(cstr_it != input.end()) {
        //cout << *cstr_it << endl;
        if (is_in_array(*cstr_it, functions_char, function_length)) {
            Token t = Token(*cstr_it, func);
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
            std::pair<double, int> reader = decimal_reader(input, (int)std::distance(input.begin(), cstr_it) + 1);
            int start_index = (int)std::distance(input.begin(), cstr_it);
            std::string sub_string = input.substr(start_index, reader.second - start_index);
            Token t = Token(*cstr_it, var, 0.0, this->initial_values[sub_string]);
            t.derivs.insert({sub_string, 1});
            t.set_var_name(sub_string);
            token_list.push_back(t);
            cstr_it = input.begin() + reader.second;

        } else if (is_in_array(*cstr_it, operators, operators_length)) {
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
                    Token t = Token('C', constant, 0, -const_val);
                    token_list.push_back(t);
                } else {
                    Token t = Token('C', constant, 0, -const_val);
                    token_list.push_back(t);
                    token_list.push_back(Token('*', binary_op, precedence['*']));
                } 

            } else {

                Token t = Token(*cstr_it, binary_op, precedence[*cstr_it]);
                token_list.push_back(t);
                cstr_it++;
            }
        }
        else if (isdigit((int)*cstr_it) != 0 || (*cstr_it == '.' && isdigit((int)*(cstr_it+1)))) {
            const_val = decimal_reader(input, (int)std::distance(input.begin(), cstr_it)).first;
            cstr_it = input.begin() + decimal_reader(input, (int)std::distance(input.begin(), cstr_it)).second;

            Token t = Token('C', constant, 0, const_val);
            token_list.push_back(t);
        }
        else {
            std::string msg = "unable to parse input character: ";
            msg += *cstr_it;
            cstr_it++;
            //throw std::invalid_argument(msg);
        }
    }

    return token_list;
}


std::vector<Token> parser::parse(std::vector<Token> input) {

    std::vector<Token> output;

    std::stack<Token> op_stack;

    for (auto & token_it : input) {
        if (token_it.type == var || token_it.type == constant) {
            output.push_back(token_it);
        } else if (token_it.type == func) {
            op_stack.push(token_it);
        } else if (token_it.type == binary_op && token_it.first_char != '(' && token_it.first_char != ')') {
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
                if (!op_stack.empty() && op_stack.top().type == func) {
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

