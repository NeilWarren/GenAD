
#ifndef MISHRA_WARREN_AUTODIFF_TOKEN_H
#define MISHRA_WARREN_AUTODIFF_TOKEN_H

#include "sys.h"
using namespace std;

class token {

public:
    char type;             // constant "c", binary op "b", var/trace "t", adjoint "a"
    string operation;      // if unary op/function "f" token, this is the type, e.g., sin, cos, exp...
                           // if this is a binary operator type "b", this is add, sub, mul, div, or pow
    double num_val;        // numeric value of the node
    double adjoint_val;    // value of the token/node's adjoint
    vector<double> *deriv;  // vector of the derivatives indexed
    vector<token*> *children;  // pointers to children
    vector<token*> *parents;   // pointers to parents

    // num_vars is the length of the input vector
    token(char t, double n_val, unsigned int num_vars,  string op="none"){
        this->num_val = n_val;
        this->type = t;
        this->operation = move(op);
        this->adjoint_val = 0;
        if (t != 'a') {
            this->deriv = new vector<double>(num_vars);
        }
        this->children = nullptr;
        this->parents = nullptr;

    }

};


#endif //MISHRA_WARREN_AUTODIFF_TOKEN_H
