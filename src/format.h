#ifndef FORMAT_H
#define FORMAT_H

#include <RcppArmadillo.h>
#include <vector>
#include "Node.h"
#include "Rule.h"
#include "Interval.h"

// [[Rcpp::depends(RcppArmadillo)]]
std::vector<Interval*> createDefault(const unsigned int& n, Rcpp::NumericMatrix& support) {
    std::vector<Interval*> output (n);
    for (unsigned int i = 0; i < n; i++) {
        output[i] = new Interval(support(i, 0), support(i, 1), i); 
    }

    return output;
}

// extract support from generated data
Rcpp::NumericMatrix extractSupport(const Rcpp::DataFrame& df) {
    const unsigned int n = df.ncol();
    const unsigned int m = df.nrow();

    // N x 2 matrix, columns: min, max
    Rcpp::NumericMatrix support(n, 2);
    
    for (unsigned int i = 0; i < n; i++) {
        Rcpp::NumericVector col = df[i];
        support(i, 0) = col[0];
        support(i, 1) = col[0];

        for (unsigned int j = 0; j < m; j++) {
            support(i, 0) = std::min(support(i, 0), col[j]);
            support(i, 1) = std::max(support(i, 1), col[j]);
        }
    }

    return support;
}

void dfs(Node* root, unsigned int& k, const unsigned int& n, std::vector<Interval*>& mergeIntervals, Rcpp::NumericMatrix& partitions, Rcpp::NumericMatrix support) {
    // we've reached a leaf record the results 
    if (!root->trueEdge && !root->falseEdge) {
        // if an interval has the same predictor have to merge
        // first create a default interval for every predictor with the support
        std::vector<Interval*> correctIntervals = createDefault(n, support);
        
        // then merge the intervals that we have from traversing the tree
        for (const auto interval : mergeIntervals) {
            Interval* targetInterval = correctIntervals[interval->predictor];
            targetInterval->merge(interval);
        }

        for (unsigned int j = 0, i = 0; j < 2*n && i < n; j += 2, i++) {
            partitions(k, j) = correctIntervals[i]->lowerBound;
            partitions(k, j+1) = correctIntervals[i]->upperBound;
        }
         
        k++;
        return;
    }

    Rule* rule = root->getRule();
    unsigned int predictor = rule->getColumn();
    double lower = support(predictor, 0);
    double upper = support(predictor, 1);

    double ruleValue = root->getRule()->getValue();

    // true edge then false edge
    Interval* interval = new Interval(lower, ruleValue, predictor);
    mergeIntervals.push_back(interval);
    dfs(root->trueEdge, k, n, mergeIntervals, partitions, support);
    delete mergeIntervals.back();
    mergeIntervals.pop_back();

    interval = new Interval(ruleValue, upper, predictor);
    mergeIntervals.push_back(interval);
    dfs(root->falseEdge, k, n, mergeIntervals, partitions, support);
    delete mergeIntervals.back();
    mergeIntervals.pop_back();
}

Rcpp::NumericMatrix extractPartition(const Rcpp::DataFrame& df, Node* root, const unsigned int& numLeaves) {
    std::vector<Interval*> mergeIntervals; 
    const unsigned int n = df.ncol();
    unsigned int k = numLeaves;
    Rcpp::NumericMatrix partitions(numLeaves, 2*n);
    Rcpp::NumericMatrix support = extractSupport(df);

    unsigned int i = 0;
    dfs(root, i, n, mergeIntervals, partitions, support);
    return partitions;
}
#endif