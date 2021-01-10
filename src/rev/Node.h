#ifndef MISHRA_WARREN_AUTODIFF_ADJOIN_NODE_H
#define MISHRA_WARREN_AUTODIFF_ADJOIN_NODE_H

#include "../auto_diff.h"

class Node {

public:
    double v;
    Node *child = nullptr;

    Node(double val, Node* c = nullptr){
      this->v = val;
      this->child = c;
    }



};


#endif //MISHRA_WARREN_AUTODIFF_ADJOIN_NODE_H
