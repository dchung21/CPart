#ifndef FORMAT_H
#define FORMAT_H

#include <Rcpp.h>
#include <DecisionTree.h>
#include <vector>

struct Interval {
    public:
        double lowerBound;
        double upperBound;
        string predictor;

        Interval(double lowerBound, double upperBound, double predictor) {
            this.lowerBound = lowerBound;
            this.upperBound = upperBound;
            this.predictor = predictor;
        }
        
        bool operator < (const Interval& interval) const {
            return lowerBound < interval.lowerBound;
        }
};

std::vector<Interval*> createDefault(const unsigned int& n, Rcpp::NumericMatrix& support) {
    std::vector<Interval*> output (n);
    for (unsigned int i = 0; i < n; i++) {
        output[i] = new Interval(support(i, 1), support(i, 2), i); 
    }

    return output;
}

// extract support from generated data
// Rcpp::NumericMatrix extractSupport(const Rcpp::DataFrame& df) {
// assume nonzero df length
Rcpp::NumericMatrix extractSupport(const Rcpp::DataFrame& df) {
    const unsigned int n = df.ncol();
    const unsigned int m = df.nrow();

    // N x 2 matrix, columns: min, max
    Rcpp::NumericMatrix support(n, 2);
    
    for (unsigned int i = 0; i < n; i++) {
        Rcpp::NumericVector col = df[i];
        // change for element
        double minElement = std::numeric_limits<double>::infinity();
        double maxElement = std::numeric_limits<double>::infinity();

        for (unsigned int j = 0; j < m; j++) {
            minElement = std::min(minElement, col[j]);
            maxElement = std::max(maxElement, col[j]);
        }
        support(i, 1) = minElement;
        support(i, 2) = maxElement;
    }

    return support;
}

Rcpp::NumericMatrix extractPartition(const Rcpp::DataFrame& df, const Rcpp::NumericMatrix& support, Node* root, const unsigned int& numLeaves) {
    std::vector<Interval*> mergeIntervals; 
    Rcpp::NumericMatrix partitions(numLeaves, df.ncol());

    /* should figure out upper limit for depth
    partitionRules.reserve(numLeaves * sizeof(Rule));
    */

}

void dfs(Node* root, unsigned int& k, std::vector<Interval*>& mergeIntervals) {
    // we're done do something
    if (!root->left && !root->right) {
        k++;
    }

    // extract rule
    Rule* rule = root->getRule();
    std::string predictor = rule->getColumn();
    Interval* interval = new interval(predictor, rule->getValue, upper);
    mergeIntervals.push_back(interval);
    dfs(root->left, k);
    delete mergeIntervals.back();
    mergeIntervals.pop_back();
    dfs(root->right, k);
    delete mergeIntervals.back();
    mergeIntervals.pop_back();
    }

#endif
