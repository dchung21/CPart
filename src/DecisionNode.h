#ifndef DECISION_NODE_H
#define DECISION_NODE_H

#include <Rcpp.h>

class Rule;

class DecisionNode : public Node {
    private:
        Rule* rule;

    public:
        DecisionNode(Rule* rule) {
            this->rule = rule;
        }

        Rule* getRule() {
            return rule;
        }

        void printNode() {
            std::cout << "Decision Node " << rule->getRule() << std::endl;
        }
};

#endif
