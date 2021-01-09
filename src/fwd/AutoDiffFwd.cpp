
#include "AutoDiffFwd.h"

#include <utility>

AutoDiffFwd::AutoDiffFwd(vector<Token> toks) {
    this->tokenVec = std::move(toks);
    this->evalStack = new stack<Token*>();

}

void AutoDiffFwd::do_binary_op(Token* tmp) {
    double const_val=0;
    map<string, double> der;
    Token *v1, *v2, *unit_tok, *zero_tok, *pow_base, *pow_exp, *pow_v1, *pow_v2;
    Token* top;

            v2 = evalStack->top();
            this->evalStack->pop();


            v1 = evalStack->top();
            this->evalStack->pop();

    switch (tmp->first_char) {
        case '+':
            const_val = v1->num_val + v2->num_val;
            der = DerivCalc::deriv_sum(v1, v2);
            break;
        case '-':
            const_val = v1->num_val - v2->num_val;
            der = DerivCalc::deriv_diff(v1, v2);
            break;
        case '*':
            const_val = v1->num_val * v2->num_val;
            der = DerivCalc::deriv_product(v1, v2);
            break;
        case '/':
            const_val = v1->num_val / v2->num_val;
            der = DerivCalc::deriv_quotient(v1, v2);
            break;
        case '^':
            // need to handle the case of a negative exponent value
            if (v2->num_val >= 0) {
                const_val = pow(v1->num_val, v2->num_val);
                der = DerivCalc::deriv_power(v1, v2);
            } else {

                // make new tokens 1/v1 and -1*v2
                unit_tok = new Token('C', constant, 0, 1);
                pow_v1 = new Token('T', var, 1 / v1->num_val, DerivCalc::deriv_quotient(unit_tok, v1));

                zero_tok = new Token('C', constant, 0, 0);
                pow_v2 = new Token('T', var, -1 * v2->num_val, DerivCalc::deriv_diff(zero_tok, v2));

                const_val = pow(v1->num_val, v2->num_val);
                der = DerivCalc::deriv_power(pow_v1, pow_v2);
            }

    }
    Token* t = new Token('T', var,const_val, der);
    this->evalStack->push(t);

}

void AutoDiffFwd::do_function(Token* tmp) {

    Token *v1 = evalStack->top();
    this->evalStack->pop();


    if (tmp->operation == "sin") this->evalStack->push(DerivCalc::do_sin(v1));
    if (tmp->operation == "cos") this->evalStack->push(DerivCalc::do_cos(v1));
    if (tmp->operation == "tan") this->evalStack->push(DerivCalc::do_tan(v1));

    if (tmp->operation == "sinh") this->evalStack->push(DerivCalc::do_sinh(v1));
    if (tmp->operation == "cosh") this->evalStack->push(DerivCalc::do_cosh(v1));
    if (tmp->operation == "tanh") this->evalStack->push(DerivCalc::do_tanh(v1));

    if (tmp->operation == "asin") this->evalStack->push(DerivCalc::do_asin(v1));
    if (tmp->operation == "acos") this->evalStack->push(DerivCalc::do_acos(v1));
    if (tmp->operation == "atan") this->evalStack->push(DerivCalc::do_atan(v1));

    if (tmp->operation == "sec") this->evalStack->push(DerivCalc::do_sec(v1));
    if (tmp->operation == "csc") this->evalStack->push(DerivCalc::do_csc(v1));
    if (tmp->operation == "cot") this->evalStack->push(DerivCalc::do_cot(v1));

    if (tmp->operation == "log") this->evalStack->push(DerivCalc::do_log(v1));
    if (tmp->operation == "exp") this->evalStack->push(DerivCalc::do_exp(v1));
    if (tmp->operation == "sqrt") this->evalStack->push(DerivCalc::do_sqrt(v1));

    if (tmp->operation == "lgs") this->evalStack->push(DerivCalc::do_lgs(v1));

}

Token AutoDiffFwd::eval() {
    int i;
    Token* tmp;
    for (i = 0; i < this->tokenVec.size(); ++i) {
        tmp = &this->tokenVec.at(i);
        switch (tmp->type) {
            case binary_op:
                do_binary_op(tmp);
                break;
            case func:
                do_function(tmp);
                break;
            case constant:
            case var:
                this->evalStack->push(tmp);
                break;
        }
    }
    return *this->evalStack->top();
}


