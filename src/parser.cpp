#include "Parser.h"

bool Parser::is_in_array(char p, char *array, int length) {
    for (int i = 0; i < length; ++i) {
        if ( array[i] == p) {
            return true;
        }
    }
    return false;
}

std::vector<Token> Parser::tokenizer(const char* string) {

    // remove any spaces in input string
    std::string temp;
    while (*string != '\0')
    {
        if (*string != ' ') { temp += *string; }
        ++string;
    }
    const char* temp_const = temp.c_str();
    // grab the first char
    char *p = (char *) &temp_const[0];

    // our list of Tokens to return to the user
    vector<Token> token_list;

    // stack of chars we use as we loop through the chars to keep track of entries
    stack<char> const_stack;
    
    // will store constants here as we loop through
    double const_val = 0;

    // looping through the chars
    while(*p != '\0') {
        if (is_in_array(*p, functions_char, function_length)) {

            // the current char p is in the functions_char array

            // create a Token "function_type" object, converting the char p into an int as the function_tag
        
            Token t = Token(*p, function_, (int)*p, 0);
            bool dyna_log = false;

            // trying to get the string for the function, parse up to the next '('
            std::string f;

            while (*p != '(') {
                
                if (*p == '_') {
                    // special condition of the base of the log is specified by the user
                    dyna_log = true;

                    ++p;
                    const_val = 0;
                    int i=0;
                    bool track_dots = false;
                    // variables used for tracking the constant value

                    while (isdigit((int)*p) != 0 || *p == '.') {
                        if (*p == '.') {
                            if (track_dots) { throw "More than one '.' in a constant value."; }
                            track_dots = true;
                        }
                        const_stack.push(*p);
                        ++p;
                    }
                    // we add digits for a number till we find another symbol

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
                    // for each digit scanned we mutltilpy it by it's respective power

                    Token base = Token('C', constant, 0, 0, const_val);

                    ++p;

                    Token val;

                    // here we check if the value inside of the log is a derivative or not
                    if (is_in_array(*p, variables, variables_length)) {
                        // seed the derivatives map, if the value is absent we will assume it's zero in the evaluate function_
                        map<char, double> deriv_seed;
                        if (this->seeds.count(*p) > 0){
                            deriv_seed.insert({*p, this->seeds[*p]});
                        } else {
                            deriv_seed.insert({*p, 1.0});
                        }
                        val = Token(*p, var, 0, 0, this->initial_values[*p], deriv_seed);

                    } else {
                        const_val = 0;
                        i=0;
                        track_dots = false;

                        while (isdigit((int)*p) != 0 || *p == '.') {
                            if (*p == '.') {
                                if (track_dots) { throw "More than one '.' in a constant value."; }
                                track_dots = true;
                            }
                            const_stack.push(*p);
                            ++p;
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

                        val = Token('C', constant, 0, 0, const_val);

                    }

                    // this large chunk of code is handle pushing tokens for the special log case
                    Token t0 = Token('(', binary_op, 0, precedence['(']);
                    token_list.push_back(t0);
                    t = Token('l', function_, (int)('l'), 0);
                    t.set_func_type("log"); // setting the name of the function f as the Token.func_type
                    token_list.push_back(t);
                    Token t1 = Token('(', binary_op, 0, precedence['(']);
                    token_list.push_back(t1);
                    token_list.push_back(val);
                    Token t2 = Token(')', binary_op, 0, precedence[')']);
                    token_list.push_back(t2);
                    Token t3 = Token('/', binary_op, 0, precedence['/']);
                    token_list.push_back(t3);
                    Token t4 = Token('l', function_, (int)('l'), 0);
                    t4.set_func_type("log");
                    token_list.push_back(t4);
                    Token t5 = Token('(', binary_op, 0, precedence['(']);
                    token_list.push_back(t5);
                    token_list.push_back(base);
                    Token t6 = Token(')', binary_op, 0, precedence[')']);
                    token_list.push_back(t6);
                    Token t7 = Token(')', binary_op, 0, precedence[')']);
                    token_list.push_back(t7);

                    // depending on what was in the log statement, we iterate respectively
                    if (val.type == constant) {
                        p +=1;
                    } else {
                        p += 2;
                    }

                    break;

                }
                if (!dyna_log) {
                    f += *p;
                    ++p;
                }
            }
            // try to find the string f in the functions array
            if (!dyna_log) {
                if (std::find(begin(functions), end(functions), f) != end(functions)) {
                    t.set_func_type(f); // setting the name of the function f as the Token.func_type
                    token_list.push_back(t);
                } else {
                    std::string msg = "Invalid equation input: ";
                    msg += f;
                    throw std::invalid_argument(msg);
                }
            }

        } else if (is_in_array(*p, variables, variables_length)) {
            // seed the derivatives map, if the value is absent we will assume it's zero in the evaluate function_
            map<char, double> deriv_seed;
            // if we find the variable, we relate it to the users input and seed it accordingly
            if (this->seeds.count(*p) > 0){
                deriv_seed.insert({*p, this->seeds[*p]});
            } else {
                deriv_seed.insert({*p, 1.0});
            }
            Token t = Token(*p, var, 0, 0, this->initial_values[*p], deriv_seed);
            token_list.push_back(t);
            ++p;

        } else if (is_in_array(*p, operators, operators_length)) {
            
            bool add_mul = false;

            if (*p == '-' && (token_list.empty() || 
                ((is_in_array(*(p - 1), operators, operators_length) && 
                    *(p-1) != ')')))) {
                // this statement only runs if we encounter a subtraction sign that 
                // is meant to be a negative sign
                ++p;
                const_val = 0;
                int i=0;
                bool track_dots = false;

                while (isdigit((int)*p) != 0 || *p == '.') {
                    if (*p == '.') {
                        if (track_dots) { throw "More than one '.' in a constant value."; }
                        track_dots = true;
                    }
                    const_stack.push(*p);
                    ++p;
                }

                while (!const_stack.empty()) {
                    if (const_stack.top() != '.') {
                        // this condition exists soley for decimal values

                        const_val += ((int)(const_stack.top() - '0')) * std::pow(10.0, i);
                        i++;
                    }
                    else {
                        const_val /= std::pow(10.0, i);
                        i = 0;
                    }

                    const_stack.pop();
                }

                if (const_val == 0) {
                    const_val = 1;
                    add_mul = true;
                }

                if (!add_mul) {
                    Token t = Token('C', constant, 0, 0, -const_val);
                    token_list.push_back(t);
                } else {
                    Token t = Token('C', constant, 0, 0, -const_val);
                    token_list.push_back(t);
                    token_list.push_back(Token('*', binary_op, 0, precedence['*']));
                } 

                // we add the negative sign and its respective digit back to the code

            } else {

                Token t = Token(*p, binary_op, 0, precedence[*p]);
                token_list.push_back(t);
                ++p;

                // otherwise we push normally
            }
        }
        else if (isdigit((int)*p) != 0 || (*p == '.' && isdigit((int)*(p+1)))) {
            // If the value is a constant, keep reading to get any additional digits of the constant
            // then create the token with the entire constant
            const_val = 0;
            int i=0;
            bool track_dots = false;
            
            // the following while loops follow the same logic as before.
            while (isdigit((int)*p) != 0 || *p == '.') {
                if (*p == '.') {
                    if (track_dots) { 
                        throw std::invalid_argument("More than one '.' in a constant value.");
                    }
                    track_dots = true;
                }
                const_stack.push(*p);
                ++p;
            }

            while (!const_stack.empty()) {
                if (const_stack.top() != '.') {
                    const_val += ((int) (const_stack.top() - '0')) * std::pow(10.0, i);
                    i++;
                } else {
                    const_val /= std::pow(10.0, i);
                    i = 0;
                }
                
                const_stack.pop();
            }

            // here we push back the tokens as a constant type
            Token t = Token('C', constant, 0, 0, const_val);
            token_list.push_back(t);
        }
        else {
            // error handling
            std::string msg = "unable to parse input character: ";
            msg += *p;
            throw std::invalid_argument(msg);
        }
    }

    return token_list;
}



std::vector<Token> Parser::formula_parser(std::vector<Token> input) {

    std::vector<Token> output_queue;
    std::stack<Token> operator_stack;
    // stack and queue used for sorting operations

    for (auto & it : input) {
        // shunting cases between variables, functions, operators and brackets

        if (it.type == var || it.type == constant) {

            output_queue.push_back(it);

        } else if (it.type == function_) {
            // functions go directly to the operator stack

            operator_stack.push(it);

        } else if (it.type == binary_op && it.value != '(' && it.value != ')') {
            // if we have a function / operator that has greater precedence, we reorder

            while (!operator_stack.empty() && operator_stack.top().value != '(' &&
                (operator_stack.top().precedence > it.precedence || 
                (operator_stack.top().precedence == it.precedence && it.value != '^'))) {
                
                output_queue.push_back(operator_stack.top());
                operator_stack.pop();

            }

            operator_stack.push(it);

        } else if (it.value == '(') {
            // left parentheses are binned

            operator_stack.push(it);

        } else if (it.value == ')') {

            // if we find a function, we look for it's end parentheses and order 
            // according to that

            while (!operator_stack.empty() && operator_stack.top().value != '(') {
                output_queue.push_back(operator_stack.top());
                operator_stack.pop();
            }

            if (!operator_stack.empty() && operator_stack.top().value == '(') {
                operator_stack.pop();
                if (!operator_stack.empty() && operator_stack.top().type == function_) {
                    output_queue.push_back(operator_stack.top());
                    operator_stack.pop();
                }
            }

        } else {
            std::string msg = "Mismatched Value: ";
            msg += it.value;
            throw std::invalid_argument(msg);
        }

    }

    while (!operator_stack.empty()) {
        // pushing remaining operators to the processing queue

        if (operator_stack.top().value != '(' && operator_stack.top().value != ')') {
            output_queue.push_back(operator_stack.top());
        }
        operator_stack.pop();
    }

    return output_queue;

}


Parser::Parser(const char *s, map<char, double> inits, map<char, double> seeds) {
    this->seeds = move(seeds);
    this->initial_values = move(inits);
    this->token_list = this->formula_parser(this->tokenizer(s));
}

Parser::Parser(const char *s, map<char, double> inits, map<char, double> seeds, int mode) {

    this->seeds = move(seeds);
    this->initial_values = move(inits);

    //mode 0 = forward, 1 = reverse
    if (mode == 0){
        this->token_list = this->formula_parser(this->tokenizer(s));
    } else {
        this->token_list = this->reverse_formula_parser(this->tokenizer(s));
    }

}