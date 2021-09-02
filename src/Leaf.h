#ifndef LEAF_H
#define LEAF_H

#include <Rcpp.h>
#include "Node.h"

class Leaf : public Node {
    private:
        float val = 0;

    public:
        Leaf(Rcpp::NumericVector& vec, const std::vector<int>& rows) {
            for (const auto& i : rows)
                val += vec[i]; 

            val /= rows.size();
        }

        float getVal() {
            return val;
        }

        void printNode() {
            std::cout << "Leaf Node " << val << std::endl;
        }

};

#endif
