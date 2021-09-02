#ifndef NODE_H
#define NODE_H

class Node {
    public:
        Node* trueEdge = nullptr;
        Node* falseEdge = nullptr;
        virtual void printNode() {
            std::cout << "test" << std::endl;
        }

};

#endif
