#include "DerivCalc.h"

map<string, double> DerivCalc::deriv_sum(Token *v1, Token *v2) {

    set<string> keys = get_key_set(v1, v2);
    map<string, double> sum;

    for (auto &key : keys) {
        sum.insert({key,v1->derivs[key] + v2->derivs[key]});
    }
    return sum;
}

map<string, double> DerivCalc::deriv_diff(Token *v1, Token *v2) {

    set<string> keys = get_key_set(v1, v2);
    map<string, double> diff;

    for (auto &key : keys) {
        diff.insert({key, v1->derivs[key] - v2->derivs[key]});
    }
    return diff;
}

map<string, double> DerivCalc::deriv_product(Token *v1, Token *v2) {

    set<string> keys = get_key_set(v1, v2);
    map<string, double> prod;
    double v1_val, v1_der, v2_val, v2_der;

    // product rule is der = (v1)(v2)' + (v1)'(v2)
    for (auto &key : keys) {
        v1_val = v1->num_val;
        v1_der =v1->derivs[key];
        v2_val = v2->num_val;
        v2_der =v2->derivs[key];
        prod.insert({key, v1_val*v2_der + v2_val*v1_der});
    }
    return prod;
}

map<string, double> DerivCalc::deriv_quotient(Token *v1, Token *v2) {

    set<string> keys = get_key_set(v1, v2);
    map<string, double> q;
    double v1_val, v1_der, v2_val, v2_der, q_;

    // quotient rule for (v1/v2)' = ((v1)'(v2) - (v1)(v2)') / (v2*v2)
    for (auto &key : keys) {
        v1_val = v1->num_val;
        v1_der =v1->derivs[key];
        v2_val = v2->num_val;
        v2_der =v2->derivs[key];
        q_ = (v1_der * v2_val - v1_val * v2_der) / (v2_val * v2_val);
        q.insert({key, q_});
    }
    return q;
}



map<string, double> DerivCalc::deriv_power(Token *v1, Token *v2) {

    set<string> keys = get_key_set(v1, v2);
    map<string, double> p;
    double v1_val, v1_der, v2_val, v2_der, p_;

    // power rule
    //  d/dx( f(x)^g(x) ) = f(x)^g(x) * d/dx( g(x) ) * ln( f(x) )
    //                     + f(x)^( g(x)-1 ) * g(x) * d/dx( f(x) )
    for (auto &key : keys) {
        v1_val = v1->num_val;
        v1_der =v1->derivs[key];
        v2_val = v2->num_val;
        v2_der =v2->derivs[key];
        p_ = pow(v1_val, v2_val) * v2_der * log(abs(v1_val)) + pow(v1_val,v2_val-1) * v2_val * v1_der;
        p.insert({key, p_});
    }
    return p;
}


Token* DerivCalc::do_sin(Token *v1) {
    map<string, double> der;

    // calculate the numeric value as sin
    double num_val = sin(v1->num_val);

    // calculate the derivative as cos of val, * prev partial
    for (auto &it : v1->derivs) {
        double tmp_cos = cos(v1->num_val)*it.second;
        der.insert({it.first, tmp_cos});
    }
    return new Token('T', var, num_val, der);
}

Token* DerivCalc::do_cos(Token *v1) {
    map<string, double> der;

    double num_val = cos(v1->num_val);
    for (auto &it : v1->derivs) {
        double tmp_cos = -1*sin(v1->num_val)*it.second;
        der.insert({it.first, tmp_cos});
    }
    return new Token('T', var, num_val, der);
}

Token* DerivCalc::do_log(Token *v1) {
    map<string, double> der;

    double num_val = log(v1->num_val);
    for (auto &it : v1->derivs) {
        double tmp = (1/v1->num_val)*it.second;
        der.insert({it.first, tmp});
    }
    return new Token('T', var, num_val, der);
}

Token* DerivCalc::do_tan(Token *v1) {
    map<string, double> der;

    double num_val = tan(v1->num_val);
    for (auto &it : v1->derivs) {
        double tmp = (1/cos(v1->num_val))*(1/cos(v1->num_val))*it.second;
        der.insert({it.first, tmp});
    }
    return new Token('T', var, num_val, der);
}

Token* DerivCalc::do_sec(Token *v1) {
    map<string, double> der;

    // sec x tan x
    double num_val = 1/cos(v1->num_val);
    for (auto &it : v1->derivs) {
        double tmp = (1/cos(v1->num_val))*(tan(v1->num_val))*it.second;
        der.insert({it.first, tmp});
    }
    return new Token('T', var, num_val, der);
}

Token* DerivCalc::do_csc(Token *v1) {
    map<string, double> der;

    //  -csc x cot x
    double num_val = 1/sin(v1->num_val);
    for (auto &it : v1->derivs) {
        double tmp = (-1/sin(v1->num_val))*(1/tan(v1->num_val))*it.second;
        der.insert({it.first, tmp});
    }
    return new Token('T', var, num_val, der);
}

Token* DerivCalc::do_cot(Token *v1) {
    map<string, double> der;

    //  - csc2 x
    double num_val = 1/tan(v1->num_val);
    for (auto &it : v1->derivs) {
        double tmp = (-1/sin(v1->num_val))*(1/sin(v1->num_val))*it.second;
        der.insert({it.first, tmp});
    }
    return new Token('T', var, num_val, der);
}

Token* DerivCalc::do_asin(Token *v1) {
    map<string, double> der;

    double num_val = asin(v1->num_val);

    // 1/sqrt(1-x^2)
    for (auto &it : v1->derivs) {
        double tmp = 1 - v1->num_val*v1->num_val;
        tmp = it.second/pow(tmp, 0.5);
        der.insert({it.first, tmp});
    }
    return new Token('T', var, num_val, der);
}

Token* DerivCalc::do_acos(Token *v1) {
    map<string, double> der;

    double num_val = acos(v1->num_val);

    // -1/sqrt(1-x^2)
    for (auto &it : v1->derivs) {
        double tmp = 1 - v1->num_val*v1->num_val;
        tmp = -1*it.second/pow(tmp, 0.5);
        der.insert({it.first, tmp});
    }
    return new Token('T', var, num_val, der);
}

Token* DerivCalc::do_atan(Token *v1) {
    map<string, double> der;

    double num_val = atan(v1->num_val);

    // 1/(1+x^2)
    for (auto &it : v1->derivs) {
        double tmp = 1 + v1->num_val * v1->num_val;
        tmp = it.second / tmp;
        der.insert({it.first, tmp});
    }
    return new Token('T', var, num_val, der);
}

Token* DerivCalc::do_sinh(Token *v1) {
    map<string, double> der;

    // calculate the numeric value as sin
    double num_val = sinh(v1->num_val);

    // calculate the derivative as cos of val, * prev partial
    for (auto &it : v1->derivs) {
        double tmp_cos = cosh(v1->num_val)*it.second;
        der.insert({it.first, tmp_cos});
    }
    return new Token('T', var, num_val, der);
}

Token* DerivCalc::do_cosh(Token *v1) {
    map<string, double> der;

    double num_val = cosh(v1->num_val);
    for (auto &it : v1->derivs) {
        double tmp_cos = sinh(v1->num_val)*it.second;
        der.insert({it.first, tmp_cos});
    }
    return new Token('T', var, num_val, der);
}

Token* DerivCalc::do_tanh(Token *v1) {
    map<string, double> der;

    double num_val = tanh(v1->num_val);
    for (auto &it : v1->derivs) {
        double tmp = (1-tanh(v1->num_val)*tanh(v1->num_val))*it.second;
        der.insert({it.first, tmp});
    }
    return new Token('T', var, num_val, der);
}

Token* DerivCalc::do_exp(Token *v1) {
    map<string, double> der;

    double num_val = exp(v1->num_val);
    for (auto &it : v1->derivs) {
        double tmp = (exp(v1->num_val))*it.second;
        der.insert({it.first, tmp});
    }
    return new Token('T', var, num_val, der);
}

Token* DerivCalc::do_sqrt(Token *v1) {
    map<string, double> der, return_der;
    double num_val;
    Token exp_tok = Token('C', constant, 0,0.5);

    num_val = pow(v1->num_val, 0.5);
    return_der = deriv_power(v1, &exp_tok);

    return new Token('T', var, num_val, return_der);
}

Token* DerivCalc::do_lgs(Token *v1) {

    //  f(x) = 1 / (1 + exp(-1*x))
    // f'(x) = exp(x) / ( 1 + exp(x) )^2
    Token *t, *unit_tok;
    map<string, double> der, return_der;

    double num_val = 1 / (1 + exp((-1 * v1->num_val) ));
    for (auto &it : v1->derivs) {
        double tmp = ( exp(-v1->num_val) / pow((1 + exp(-v1->num_val) ), 2) )*it.second;
        der.insert({it.first, tmp});
    }
    return new Token('T', var, num_val, der);
}



set<string> DerivCalc::get_key_set(Token *v1, Token *v2) {
    set<string> return_set;
    if (!v1->derivs.empty()){
        for (auto &it : v1->derivs) {
            return_set.insert(it.first);
        }
    }
    if (!v2->derivs.empty()) {
        for (auto &it : v2->derivs) {
            return_set.insert(it.first);
        }
    }
    return return_set;
}


