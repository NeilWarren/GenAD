
#include "AutoDiffRev.h"


AutoDiffRev::AutoDiffRev(vector<Token> toks, map<string, double> i) {
    this->tokenVec = toks;
    this->evalStack = new stack<Token*>();
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
            case var:
                this->evalStack->push(tmp);
                break;
        }
    }
    
    map<string, double> der;
    for (auto &it : inits) {
        der.insert({it.first, get_deriv(evalStack->top(), it.first)});
    }

    Token t = Token('T', var, evalStack->top()->num_val, der);
    return t;
   
}

double AutoDiffRev::get_deriv(Token *tmp, string target, char op) {

    switch (tmp->type) {
        case binary_op:
            switch (tmp->first_char) {
                case '+':
                    return get_deriv(tmp->l_c, target, tmp->first_char) + get_deriv(tmp->r_c, target, tmp->first_char);
                case '-':
                    return get_deriv(tmp->l_c, target, tmp->first_char) - get_deriv(tmp->r_c, target, tmp->first_char);
                case '*':
                    return get_deriv(tmp->l_c, target, tmp->first_char) * get_deriv(tmp->r_c, target, tmp->first_char);
                case '/':
                    return get_deriv(tmp->l_c, target, tmp->first_char) / get_deriv(tmp->r_c, target, tmp->first_char);
                case '^':
                    return pow(get_deriv(tmp->l_c, target, tmp->first_char), get_deriv(tmp->r_c, target, tmp->first_char));
            }
            break;
        case func:
            return do_function_der(tmp->operation, tmp->l_c, target);

        case var:
            if (tmp->var_name == target) return 1;
            switch (op) {
                case '+':
                case '-':
                case 'x':
                    return 0;
                case '*':
                    return inits[tmp->var_name];
                case '/':
                    return inits[tmp->var_name];
            }
        case constant:
            switch (op) {
                case '+':
                case '-':
                case 'x':
                    return 0;
                case '*':
                case '/':
                    return tmp->num_val;
            }


    }

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

    tmp->set_lc(v1);
    tmp->set_rc(v2);

    switch (tmp->first_char) {
        case '+':
            const_val = v1->num_val + v2->num_val;
            break;
        case '-':
            const_val = v1->num_val - v2->num_val;
            break;
        case '*':
            const_val = v1->num_val * v2->num_val;
            break;
        case '/':
            const_val = v1->num_val / v2->num_val;
            break;
        case '^':
            const_val = pow(v1->num_val, v2->num_val);

    }
    tmp->set_n_val(const_val);
    //cout << tmp->first_char << endl;
    this->evalStack->push(tmp);

}

void AutoDiffRev::do_function(Token *tmp) {
    double const_val=0;
    Token *v1;

    v1 = evalStack->top();
    this->evalStack->pop();

    tmp->set_lc(v1);

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

    // Todo
    if (tmp->operation == "sqrt") const_val = pow(v1->num_val, 0.5);

    if (tmp->operation == "lgs") const_val = 1 / (1 + exp((-1 * v1->num_val) ));

    tmp->set_n_val(const_val);
    this->evalStack->push(tmp);


}

double AutoDiffRev::do_function_der(string operation, Token *v1, string target_var) {

    if (operation == "sin") return cos(v1->num_val) *  get_deriv(v1, target_var);
    if (operation == "cos") return -1*sin(v1->num_val) *  get_deriv(v1, target_var);
    if (operation == "tan") return (1/cos(v1->num_val))*(1/cos(v1->num_val))* get_deriv(v1, target_var);

    if (operation == "sinh") return cosh(v1->num_val)*get_deriv(v1, target_var);
    if (operation == "cosh") return sinh(v1->num_val)*get_deriv(v1, target_var);
    if (operation == "tanh") return (1-tanh(v1->num_val)*tanh(v1->num_val))*get_deriv(v1, target_var);

    if (operation == "asin") return get_deriv(v1, target_var)/pow(v1->num_val*v1->num_val, 0.5);
    if (operation == "acos") return -1*get_deriv(v1, target_var)/pow(1 - v1->num_val*v1->num_val, 0.5);
    if (operation == "atan") return get_deriv(v1, target_var) / (1 + v1->num_val * v1->num_val);

    if (operation == "sec") return (1/cos(v1->num_val))*(tan(v1->num_val))*get_deriv(v1, target_var);
    if (operation == "csc") return (-1/sin(v1->num_val))*(1/tan(v1->num_val))*get_deriv(v1, target_var);
    if (operation == "cot") return (-1/sin(v1->num_val))*(1/sin(v1->num_val))*get_deriv(v1, target_var);

    if (operation == "log") return (1/v1->num_val)*get_deriv(v1, target_var);
    if (operation == "exp") return exp(v1->num_val)*get_deriv(v1, target_var);
    // Todo
    if (operation == "sqrt") return 0;

    if (operation == "lgs") return ( exp(-v1->num_val) / pow((1 + exp(-v1->num_val) ), 2) )*get_deriv(v1, target_var);




}



