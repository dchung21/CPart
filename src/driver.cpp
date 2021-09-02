#include <Rcpp.h>
#include <DecisionTree.h>

using namespace Rcpp;

// [[Rcpp::export]]
void test(const DataFrame& df, const std::string col, int threshold) {
    printf("a\n");
    DecisionTree* dt = new DecisionTree(df, col, threshold);
    dt->printTree();
    delete dt;
}
