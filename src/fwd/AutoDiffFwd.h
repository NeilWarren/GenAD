#ifndef MISHRA_WARREN_AUTODIFF_AUTODIFFFWD_H
#define MISHRA_WARREN_AUTODIFF_AUTODIFFFWD_H

#include "../auto_diff.h"

using namespace std;

class AutoDiffFwd {

public:
    stack<Token*>* evalStack;
    vector<Token> tokenVec;

    void do_binary_op(Token* tmp);
    void do_function(Token* tmp);
    Token eval();
    explicit AutoDiffFwd(vector<Token> toks);


};


#endif //MISHRA_WARREN_AUTODIFF_AUTODIFFFWD_H
