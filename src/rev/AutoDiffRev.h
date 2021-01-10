#ifndef AUTODIFF_AUTODIFFREV_H
#define AUTODIFF_AUTODIFFREV_H

#include "../auto_diff.h"
using namespace std;

class AutoDiffRev  {

public:
    map<string, double> inits;
    explicit AutoDiffRev(vector<Token> toks, map<string, double> i);

    stack<Token*>* evalStack;
    vector<Token> tokenVec;

    Token eval();
    void do_binary_op(Token* tmp);
    void do_function(Token* tmp);
    double get_deriv(Token* tmp, string target, char op='x');
    double do_function_der(string operation, Token* l_child, string target_var);
};

#endif //AUTODIFF_AUTODIFFREV_H
