#include <Rcpp.h>
//#include <DecisionTree.h>
#include <format.h>

using namespace Rcpp;

// [[Rcpp::export]]
Rcpp::NumericMatrix test(const DataFrame& df) {
    /*
    DecisionTree* dt = new DecisionTree(df, col, threshold);
    dt->printTree();
    delete dt;
    */
    printf("a\n");
    return extractSupport(df);
}
