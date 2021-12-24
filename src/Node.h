#ifndef NODE_H
#define NODE_H

class Rule;

class Node {
    private:
        Rule* rule = nullptr;
        double val = 0;

    public:
        // for decision nodes
        Node(Rule* rule) {
            this->rule = rule;
        }

        // overloaded constructor for leaf nodes
        Node(Rcpp::NumericVector& vec, const std::vector<int>& rows) {
            for (const auto& i : rows) {
                val += vec[i];
            }
            val /= rows.size();
        }

        // temp constructor just for testing
        Node(double x) {
            this->val = x;
        }

        ~Node() {
            if (rule)
                delete rule;
        }

        double getVal() {
            return val;
        }

        Rule* getRule() {
            return rule;
        }

        Node* trueEdge = nullptr;
        Node* falseEdge = nullptr;
        virtual void printNode() {
            if (this->rule) {
                //std::cout << std::to_string(rule->getColumn()) << std::endl;
                std::cout << "X" << this->rule->getColumn() << " < " << this->rule->getValue() << std::endl;
            }

            else {
                std::cout << "Leaf Node: " << std::to_string(val) << std::endl;
            }
        }

};

#endif
