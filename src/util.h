#include <Rcpp.h>
#include <vector>
#include <algorithm>

// sum of squared errors
float sse (const Rcpp::NumericVector v, const std::vector<int>& trueRows, const std::vector<int>& falseRows, 
                    const float& trueMean, const float& falseMean) {

    float ss = 0;

    for (const auto& i : trueRows)
        ss += std::pow(v[i] - trueMean, 2);

    for (const auto& i : falseRows)
       ss += std::pow(v[i] - falseMean, 2); 

    return ss;
}


