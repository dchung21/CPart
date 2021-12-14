#include <Rcpp.h>
#include <DecisionTree.h>
#include <format.h>

using namespace Rcpp;

// [[Rcpp::export]]
void test(const DataFrame& df, const std::string col, int threshold) {
    /*
    DecisionTree* dt = new DecisionTree(df, col, threshold);
    dt->printTree();
    delete dt;
    */
    printf("a\n");
    extractSupport(df);
}
