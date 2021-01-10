
#include "AutoDiffRev.h"


AutoDiffRev::AutoDiffRev(vector<Token> toks, map<string, double> i) {
    this->tokenVec = toks;
    this->evalStack = new stack<Token*>();
    this->nodeStack = new stack<TokenRev*>();
    this->inits = std::move(i);
}


Token AutoDiffRev::eval() {
    
    Token *tmp;
    for (int i = 0; i < this->tokenVec.size(); ++i) {

        tmp = &this->tokenVec.at(i);

        switch (tmp->type) {
            case binary_op:
                do_binary_op(tmp);
                break;
            case func:
                do_function(tmp);
                break;
            case constant:
                this->evalStack->push(tmp);
                this->nodeStack->push(new TokenRev(tmp->num_val, "const_" + to_string(index)));
                index++;
                break;
            case var:
                this->evalStack->push(tmp);
                this->nodeStack->push(new TokenRev(tmp->num_val, tmp->var_name));
                break;
        }
    }
    
    map<string, double> der = this->do_derivs(nodeStack->top());
    map<string, double> der_;

    for (auto &it : inits) {
        der_.insert({it.first, der[it.first]});
    }

    Token t = Token('T', var, evalStack->top()->num_val, der_);
    return t;
   
}

map<string, double> AutoDiffRev::do_derivs(TokenRev* parent){

    map<string, double> ret_map;
    stack< pair<TokenRev*, double> > s;
    if (parent->derivs != nullptr){
        pair<TokenRev *, double> l = *parent->derivs->first;
        s.push(l);

        if (parent->derivs->second != nullptr){
            pair<TokenRev*, double> r = *parent->derivs->second;
            s.push(r);
        }

        while (!s.empty()){
            pair<TokenRev*, double> tmp = s.top();
            s.pop();
            TokenRev* cur_tok = tmp.first;
            double cur_val = tmp.second;
            if (ret_map.find(tmp.first->var_name) == ret_map.end()){
                ret_map[tmp.first->var_name] = cur_val;

            } else {
                ret_map[tmp.first->var_name] += cur_val;

            }
            if (cur_tok->t != leaf){
                TokenRev *left_child_tok = cur_tok->derivs->first->first;
                double left_child_val = cur_tok->derivs->first->second;
                pair<TokenRev*, double> tmp_l = pair<TokenRev*, double>(left_child_tok, left_child_val * cur_val);
                s.push(tmp_l);

                if (cur_tok->derivs->second != nullptr) {
                    TokenRev *right_child_tok = cur_tok->derivs->second->first;
                    double right_child_val = cur_tok->derivs->second->second;
                    pair<TokenRev *, double> tmp_r = pair<TokenRev *, double>(right_child_tok, right_child_val * cur_val);
                    s.push(tmp_r);
                }

            }

        }

    }
    return ret_map;

}



void AutoDiffRev::do_binary_op(Token *tmp) {

    double const_val=0;
    double der = 0;
    Token *v1, *v2;
    Token* top;

    v2 = evalStack->top();
    this->evalStack->pop();

    v1 = evalStack->top();
    this->evalStack->pop();

    TokenRev* v2_ = nodeStack->top();
    this->nodeStack->pop();

    TokenRev* v1_ = nodeStack->top();
    this->nodeStack->pop();

    switch (tmp->first_char) {
        case '+':
            const_val = v1->num_val + v2->num_val;
            nodeStack->push(new TokenRev(add, v1_, v2_, index));
            index++;
            break;
        case '-':
            const_val = v1->num_val - v2->num_val;
            nodeStack->push(new TokenRev(add, v1_, new TokenRev(neg, v2_, ++index), ++index));
            break;
        case '*':
            const_val = v1->num_val * v2->num_val;
            nodeStack->push(new TokenRev(mul, v1_, v2_, index));
            index++;
            break;
        case '/':
            const_val = v1->num_val / v2->num_val;
            nodeStack->push(new TokenRev(mul, v1_, new TokenRev(divis, v2_, ++index), ++index));
            break;
        case '^':
            const_val = pow(v1->num_val, v2->num_val);
            nodeStack->push(new TokenRev(power, v1_, v2_, index));

    }
    tmp->set_n_val(const_val);
    this->evalStack->push(tmp);

}

void AutoDiffRev::do_function(Token *tmp) {
    double const_val=0;
    Token *v1;

    v1 = evalStack->top();
    this->evalStack->pop();

    TokenRev* v1_ = nodeStack->top();
    this->nodeStack->pop();

    if (tmp->operation == "sin") const_val = sin(v1->num_val);
    if (tmp->operation == "cos") const_val = cos(v1->num_val);
    if (tmp->operation == "tan") const_val = tan(v1->num_val);

    if (tmp->operation == "sinh") const_val = sinh(v1->num_val);
    if (tmp->operation == "cosh") const_val = cosh(v1->num_val);
    if (tmp->operation == "tanh") const_val = tanh(v1->num_val);

    if (tmp->operation == "asin") const_val = asin(v1->num_val);
    if (tmp->operation == "acos") const_val = acos(v1->num_val);
    if (tmp->operation == "atan") const_val = atan(v1->num_val);

    if (tmp->operation == "sec") const_val = 1/cos(v1->num_val);
    if (tmp->operation == "csc") const_val = 1/sin(v1->num_val);
    if (tmp->operation == "cot") const_val = 1/tan(v1->num_val);

    if (tmp->operation == "log") const_val = 1/log(v1->num_val);
    if (tmp->operation == "exp") const_val = exp(v1->num_val);

    if (tmp->operation == "sqrt") const_val = pow(v1->num_val, 0.5);

    if (tmp->operation == "lgs") const_val = 1 / (1 + exp((-1 * v1->num_val) ));

    tmp->set_n_val(const_val);
    this->evalStack->push(tmp);

    if (tmp->operation != "sqrt"){
        nodeStack->push(new TokenRev(unary_op, v1_, ++index, tmp->operation));
    } else {
        nodeStack->push(new TokenRev(square_root, v1_, ++index));
    }





}




