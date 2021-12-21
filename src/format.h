#ifndef FORMAT_H
#define FORMAT_H

#include <Rcpp.h>
#include <vector>
#include <string>
#include "Node.h"
#include "Rule.h"

class Node;
class Rule;

struct Interval {
    public:
        double lowerBound;
        double upperBound;
        unsigned int predictor;

        Interval(double lowerBound, double upperBound, unsigned int predictor) {
            this->lowerBound = lowerBound;
            this->upperBound = upperBound;
            this->predictor = predictor;
        }
        
        bool operator < (const Interval& interval) const {
            return lowerBound < interval.lowerBound;
        }
};

std::vector<Interval*> createDefault(const unsigned int& n, Rcpp::NumericMatrix& support) {
    std::vector<Interval*> output (n);
    for (unsigned int i = 0; i < n; i++) {
        output[i] = new Interval(support(i, 0), support(i, 1), i); 
    }

    return output;
}

// merge two intervals in place
// i1 contains the resulting interval
void merge(Interval* i1, Interval* i2) {
    i1->lowerBound = std::min(i1->lowerBound, i2->lowerBound);
    i1->upperBound = std::max(i1->upperBound, i2->upperBound);
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
        double maxElement = -std::numeric_limits<double>::infinity();

        for (unsigned int j = 0; j < m; j++) {
            minElement = std::min(minElement, col[j]);
            maxElement = std::max(maxElement, col[j]);
        }
        support(i, 0) = minElement;
        support(i, 1) = maxElement;
    }

    return support;
}


Rcpp::NumericMatrix extractPartition(const Rcpp::DataFrame& df, const Rcpp::NumericMatrix& support, Node* root, const unsigned int& numLeaves) {
    std::vector<Interval*> mergeIntervals; 
    Rcpp::NumericMatrix partitions(numLeaves, df.ncol() * 2);

    Rcpp::NumericMatrix support = extractSupport(df);
    dfs(root, numLeaves, n, mergeIntervals, partitions, support);

    return partitions;
}

void dfs(Node* root, unsigned int& k, unsigned int& n, std::vector<Interval*>& mergeIntervals, Rcpp::NumericMatrix& partitions, Rcpp::NumericMatrix support) {
    // we've reached a leaf record the results 
    if (!root->trueEdge && !root->falseEdge) {
        // if an interval has the same predictor have to merge

        // first create a default interval for every predictor with the support
        std::vector<Interval*> correctIntervals = createDefault(n, support);
        
        // then merge the intervals that we have from traversing the tree
        for (const auto interval : mergeIntervals) {
            Interval* targetInterval = correctIntervals[interval->predictor];
            merge(targetInterval, interval); 
        }

        // then place the result in the matrix
        for (unsigned int j = 0; j < 2*n; j++) {
            partitions(k, j) = correctIntervals[j]->lower;
            partitions(k, j+1) = correctIntervals[j+1]->upper;
        }
        k++;
    }

    // extract rule
    Rule* rule = root->getRule();
    unsigned int predictor = rule->getColumn();
    double upper = support(predictor, 0);
    double lower = support(predictor, 1);

    // dfs routine
    Interval* interval = new Interval(predictor, root->getVal(), upper);
    mergeIntervals.push_back(interval);
    dfs(root->trueEdge, k, n, mergeIntervals, partitions, support);
    delete mergeIntervals.back();
    mergeIntervals.pop_back();
    interval = new Interval(predictor, lower, root->getVal());
    mergeIntervals.push_back(interval);
    dfs(root->falseEdge, k, n, mergeIntervals, partitions, support);
    delete mergeIntervals.back();
    mergeIntervals.pop_back();
}

#endif
