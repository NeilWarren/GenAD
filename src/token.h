
#ifndef MISHRA_WARREN_AUTODIFF_TOKEN_H
#define MISHRA_WARREN_AUTODIFF_TOKEN_H

#include "sys.h"
using namespace std;

class token {

public:

    enum token_type {
        binary_op,
        func, 
        var,
        constant
    };

    token_type type;            // constant "c", binary op "b", var/trace "t", adjoint "a"
    string operation;           // if unary op/function "f" token, this is the type, e.g., sin, cos, exp...
                                // if this is a binary operator type "b", this is add, sub, mul, div, or pow
    double num_val;             // numeric value of the node
    double adjoint_val;         // value of the token/node's adjoint
    vector<double> *deriv;      // vector of the derivatives indexed
    vector<token*> *children;   // pointers to children
    vector<token*> *parents;    // pointers to parents
    int precedence;
    char first_char;
    string var_name;


    // num_vars is the length of the input vector
    token(char t, token_type type, int precedence=0.0, double n_val=0.0, unsigned int num_vars=0.0,  string op="none"){
        this->first_char = t;
        this->num_val = n_val;
        this->type = type;
        this->precedence = precedence;
        this->operation = move(op);
        this->adjoint_val = 0;
        if (t != 'a') {
            this->deriv = new vector<double>(num_vars);
        }
        this->children = nullptr;
        this->parents = nullptr;

    }

    void set_func_type(std::string f){
        this->operation = move(f);
    }

    void set_var_name(std::string var) {
        this->var_name = var;
    }

};


#endif //MISHRA_WARREN_AUTODIFF_TOKEN_H
