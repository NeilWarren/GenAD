#ifndef MISHRA_WARREN_AUTODIFF_ADJOIN_NODE_H
#define MISHRA_WARREN_AUTODIFF_ADJOIN_NODE_H

#include "../auto_diff.h"

enum node_type {
    leaf,
    add,
    mul,
    inv,
    neg,
    unary_op
};

class Node {

public:
    double v;
    string var_name = "";
    node_type t;
    pair< pair<Node*, double>*, pair<Node*, double>* > *grad = nullptr;

    Node(double val, string var){
        t = leaf;
        var_name = var;
        v = val;
    }

    Node(node_type t, Node* l, int i) {
        this->var_name =  "unary_" + to_string(i);
        pair<Node*, double> *lc;
        this->t = t;
        switch (t){
            case neg:
                v =  -1*l->v;
                lc = new pair<Node*, double>(l,-1);
                break;
            case inv:
                v = 1 / l->v;
                lc = new pair<Node*, double>(l,-1*pow(l->v,-2));
                break;




        }
        grad =  new pair< pair<Node*, double>*, pair<Node*, double>* >(lc,nullptr);
    }


    Node(node_type t, Node* l, Node* r, int i){
        this->t = t;
        pair<Node*, double> *lc, *rc;
        this->var_name =  "trace_" + to_string(i);
        switch (t){
            case add:
                v = l->v + r->v;
                lc = new pair<Node*, double>(l,1);
                rc = new  pair<Node*, double>(r,1);
                break;
            case mul:
                v = l->v * r->v;
                lc = new pair<Node*, double>(l,r->v);
                rc = new  pair<Node*, double>(r,l->v);
                break;

        }
        grad =  new pair< pair<Node*, double>*, pair<Node*, double>* >(lc,rc);

    }

    Node(node_type t, Node* l, int i, string op) {
        this->var_name =  "unary_" + to_string(i);
        pair<Node*, double> *lc;
        this->t = t;

        v =  get_v(l,op);
        lc = do_function_der(op,l);


        grad =  new pair< pair<Node*, double>*, pair<Node*, double>* >(lc,nullptr);
    }

    double get_v(Node* v1, string op){
        double const_val;
        if (op == "sin") const_val = sin(v1->v);
        if (op == "cos") const_val = cos(v1->v);
        if (op == "tan") const_val = tan(v1->v);

        if (op == "sinh") const_val = sinh(v1->v);
        if (op == "cosh") const_val = cosh(v1->v);
        if (op == "tanh") const_val = tanh(v1->v);

        if (op == "asin") const_val = asin(v1->v);
        if (op == "acos") const_val = acos(v1->v);
        if (op == "atan") const_val = atan(v1->v);

        if (op == "sec") const_val = 1/cos(v1->v);
        if (op == "csc") const_val = 1/sin(v1->v);
        if (op == "cot") const_val = 1/tan(v1->v);

        if (op == "log") const_val = 1/log(v1->v);
        if (op == "exp") const_val = exp(v1->v);

        if (op == "sqrt") const_val = pow(v1->v, 0.5);

        if (op == "lgs") const_val = 1 / (1 + exp((-1 * v1->v) ));
        return const_val;

    }

    pair<Node*, double>* do_function_der(string operation, Node *v1) {

        double der_val;

        if (operation == "sin") der_val =  cos(v1->v);
        if (operation == "cos") der_val =  -1*sin(v1->v);
        if (operation == "tan") der_val =  (1/cos(v1->v))*(1/cos(v1->v));

        if (operation == "sinh") der_val =  cosh(v1->v);
        if (operation == "cosh") der_val =  sinh(v1->v);
        if (operation == "tanh") der_val =  (1-tanh(v1->v)*tanh(v1->v));

        if (operation == "asin") der_val =  1/pow(v1->v*v1->v, 0.5);
        if (operation == "acos") der_val =  -1/pow(1 - v1->v*v1->v, 0.5);
        if (operation == "atan") der_val =  1 / (1 + v1->v * v1->v);

        if (operation == "sec") der_val =  (1/cos(v1->v))*(tan(v1->v));
        if (operation == "csc") der_val =  (-1/sin(v1->v))*(1/tan(v1->v));
        if (operation == "cot") der_val =  (-1/sin(v1->v))*(1/sin(v1->v));

        if (operation == "log") der_val =  (1/v1->v);
        if (operation == "exp") der_val =  exp(v1->v);
        // Todo
        if (operation == "sqrt") der_val =  0;

        if (operation == "lgs") der_val =  ( exp(-v1->v) / pow((1 + exp(-v1->v) ), 2) );

        return new pair<Node*, double>(v1,der_val);

    }




};


#endif //MISHRA_WARREN_AUTODIFF_ADJOIN_NODE_H
