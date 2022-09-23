#include <stdio.h>

// type representing the values a predictor x_i can take on
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

        void merge(Interval* interval) {
            this->lowerBound = std::max(this->lowerBound, interval->lowerBound);
            this->upperBound = std::min(this->upperBound, interval->upperBound);
        }

        void print() {
            std::cout << "[ " << this->lowerBound << ", " << this->upperBound << "]" << std::endl;
        }
};