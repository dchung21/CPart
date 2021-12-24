#ifndef FORMAT_H
#define FORMAT_H

#include <Rcpp.h>
#include <vector>
#include <string>
#include "Node.h"
#include "Rule.h"


/*
 * type representing the values a predictor x_i can take on
 * 
 *
 */
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

        void print() {
            std::cout << "[ " << this->lowerBound << ", " << this->upperBound << "]" << std::endl;
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
    i1->lowerBound = std::max(i1->lowerBound, i2->lowerBound);
    i1->upperBound = std::min(i1->upperBound, i2->upperBound);
}


// extract support from generated data
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




void dfs(Node* root, unsigned int& k, const unsigned int& n, std::vector<Interval*>& mergeIntervals, Rcpp::NumericMatrix& partitions, Rcpp::NumericMatrix support) {
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
        for (unsigned int j = 0, i = 0; j < 2*n && i < n; j += 2, i++) {
            partitions(k, j) = correctIntervals[i]->lowerBound;
            partitions(k, j+1) = correctIntervals[i]->upperBound;
        }
         
        k++;
        return;
    }

    // extract rule
    Rule* rule = root->getRule();
    unsigned int predictor = rule->getColumn();
    double lower = support(predictor, 0);
    double upper = support(predictor, 1);

    // dfs routine
    double ruleValue = root->getRule()->getValue();

 
    // true edge
    Interval* interval = new Interval(lower, ruleValue, predictor);
    mergeIntervals.push_back(interval);
    dfs(root->trueEdge, k, n, mergeIntervals, partitions, support);
    delete mergeIntervals.back();
    mergeIntervals.pop_back();

    // false edge
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
