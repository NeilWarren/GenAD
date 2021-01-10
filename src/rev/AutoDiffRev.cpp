
#include "AutoDiffRev.h"


AutoDiffRev::AutoDiffRev(vector<Token> toks, map<string, double> i) {
    this->tokenVec = toks;
    this->evalStack = new stack<Token*>();
    this->nodeStack = new stack<Node*>();
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
                this->nodeStack->push(new Node(tmp->num_val, "const_"+to_string(index)));
                index++;
                break;
            case var:
                this->evalStack->push(tmp);
                this->nodeStack->push(new Node(tmp->num_val, tmp->var_name));
                break;
        }
    }
    
    map<string, double> der = this->get_grad(nodeStack->top());
    map<string, double> der_;

    for (auto &it : inits) {
        der_.insert({it.first, der[it.first]});
    }

    Token t = Token('T', var, evalStack->top()->num_val, der_);
    return t;
   
}

map<string, double> AutoDiffRev::get_grad(Node* parent){

    map<string, double> return_dict;
    stack< pair<Node*, double> > s;
    if (parent->grad != nullptr){
        pair<Node *, double> l = *parent->grad->first;
        s.push(l);
        
        if (parent->grad->second != nullptr){
            pair<Node*, double> r = *parent->grad->second;
            s.push(r);
        }

        while (!s.empty()){
            pair<Node*, double> tmp = s.top();
            s.pop();
            Node* node = tmp.first;
            double route_value = tmp.second;
            if (return_dict.find(tmp.first->var_name) == return_dict.end()){
                return_dict[tmp.first->var_name] = route_value;
                //cout << "setting " << tmp.first->var_name << " to " << route_value << endl;
            } else {
                return_dict[tmp.first->var_name] += route_value;
                //cout << "adding " << tmp.first->var_name << " of " << route_value << endl;
            }
            if (node->t != leaf){
                Node *child_node1 = node->grad->first->first;
                double child_route_value1 = node->grad->first->second;
                pair<Node*, double> tmp_l = pair<Node*, double>(child_node1,child_route_value1*route_value);
                s.push(tmp_l);

                if (node->grad->second != nullptr) {
                    Node *child_node2 = node->grad->second->first;
                    double child_route_value2 = node->grad->second->second;
                    pair<Node *, double> tmp_r = pair<Node *, double>(child_node2, child_route_value2 * route_value);
                    s.push(tmp_r);
                }

            }

        }

    }
    return return_dict;

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

    Node* v2_ = nodeStack->top();
    this->nodeStack->pop();

    Node* v1_ = nodeStack->top();
    this->nodeStack->pop();

    tmp->set_lc(v1);
    tmp->set_rc(v2);

    switch (tmp->first_char) {
        case '+':
            const_val = v1->num_val + v2->num_val;
            nodeStack->push(new Node(add, v1_, v2_, index));
            index++;
            break;
        case '-':
            const_val = v1->num_val - v2->num_val;
            nodeStack->push(new Node(add, v1_,new Node(neg, v2_, ++index), ++index));
            break;
        case '*':
            const_val = v1->num_val * v2->num_val;
            nodeStack->push(new Node(mul, v1_, v2_, index));
            index++;
            break;
        case '/':
            const_val = v1->num_val / v2->num_val;
            nodeStack->push(new Node(mul, v1_,new Node(inv, v2_, ++index), ++index));
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

    Node* v1_ = nodeStack->top();
    this->nodeStack->pop();

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

    nodeStack->push(new Node(unary_op, v1_, ++index, tmp->operation));


}




