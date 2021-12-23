#include <Rcpp.h>
#include <DecisionTree.h>
#include <format.h>
#include "Rule.h"

class Node;

using namespace Rcpp;

// [[Rcpp::export]]
Rcpp::NumericMatrix test(const DataFrame& df, const DataFrame& X) {

    DecisionTree* dt = new DecisionTree(df, "Y");
    dt->printTree();
    const unsigned int numLeaves = dt->getNumLeaves();
    Node* root = dt->getRoot();
    Rcpp::NumericMatrix res = extractPartition(X, root, numLeaves);
    delete dt;

    printf("a\n");
    return res;
}
