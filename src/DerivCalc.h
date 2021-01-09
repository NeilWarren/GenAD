#ifndef FINALPROJECT_TRACE_DERIVCALC_H
#define FINALPROJECT_TRACE_DERIVCALC_H

#include "auto_diff.h"

class DerivCalc {

public:

    // binary operations
    static map<string, double> deriv_sum(Token *v1, Token *v2);
    static map<string, double> deriv_diff(Token *v1, Token *v2);
    static map<string, double> deriv_product(Token *v1, Token *v2);
    static map<string, double> deriv_quotient(Token *v1, Token *v2);
    static map<string, double> deriv_power(Token *v1, Token *v2);

    // trig functions
    static Token* do_sin(Token *v1);
    static Token* do_cos(Token *v1);
    static Token* do_tan(Token *v1);

    // reciprocol trig functions
    static Token* do_csc(Token *v1);
    static Token* do_sec(Token *v1);
    static Token* do_cot(Token *v1);

    // inverse trig functions
    static Token* do_asin(Token *v1);
    static Token* do_acos(Token *v1);
    static Token* do_atan(Token *v1);

    //hyperbolic trig functions
    static Token* do_sinh(Token *v1);
    static Token* do_cosh(Token *v1);
    static Token* do_tanh(Token *v1);

    // exponentials
    static Token* do_exp(Token *v1);
    static Token* do_log(Token *v1);

    // misc
    static Token* do_sqrt(Token *v1);
    static Token* do_lgs(Token *v1);

    // helper functions
    static set<string> get_key_set(Token *v1, Token *v2);
};


#endif //FINALPROJECT_TRACE_DERIVCALC_H
