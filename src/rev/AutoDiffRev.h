#ifndef AUTODIFF_AUTODIFFREV_H
#define AUTODIFF_AUTODIFFREV_H

#include "../auto_diff.h"
#include "TokenRev.h"

using namespace std;

class AutoDiffRev  {

public:
    map<string, double> inits;
    explicit AutoDiffRev(vector<Token> toks, map<string, double> i);

    stack<Token*>* evalStack;
    stack<TokenRev*>* nodeStack;
    vector<Token> tokenVec;
    vector<TokenRev> node_vec;
    int index = 0;
    Token eval();
    void do_binary_op(Token* tmp);
    void do_function(Token* tmp);

    static map<string, double> do_derivs(TokenRev* parent);


};

#endif //AUTODIFF_AUTODIFFREV_H
