#ifndef NODE_H
#define NODE_H

class Rule;

class Node {
    private:
        Rule* rule;
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

        double getVal() {
            return val;
        }

        Rule* getRule() {
            return rule;
        }

        Node* trueEdge = nullptr;
        Node* falseEdge = nullptr;
        virtual void printNode() {
            std::cout << "test" << std::endl;
        }

};

#endif
