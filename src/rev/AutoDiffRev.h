#ifndef AUTODIFF_AUTODIFFREV_H
#define AUTODIFF_AUTODIFFREV_H

#include "../auto_diff.h"
#include "Node.h"

using namespace std;

class AutoDiffRev  {

public:
    map<string, double> inits;
    explicit AutoDiffRev(vector<Token> toks, map<string, double> i);

    stack<Token*>* evalStack;
    stack<Node*>* nodeStack;
    vector<Token> tokenVec;
    vector<Node> node_vec;
    int index = 0;
    Token eval();
    void do_binary_op(Token* tmp);
    void do_function(Token* tmp);

    map<string, double> get_grad(Node* parent);


};

#endif //AUTODIFF_AUTODIFFREV_H
