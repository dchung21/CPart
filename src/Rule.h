#ifndef RULE_H
#define RULE_H

#include <Rcpp.h>


/*
 * A rule partitions the dataset
 * @params:
 * column: column of data
 * value : value to check if column >= value
 *
 */

class Rule {
    private:
        unsigned int column;
        double value;
        float sse = std::numeric_limits<float>::infinity();

    public:
        Rule(unsigned int column, float value) {
            this->column = column;
            this->value = value;
        }

        // default constructor for dummy rules
        Rule() {
            this->column = 0;
        }

        bool match(const float val) {
            return val < this->value;
        }

        std::string getRule() {
            return std::to_string(this->column) + " < " + std::to_string(this->value);
        }

        float getSSE() {
            return this->sse;
        }

        void setSSE(const float sse) {
            this->sse = sse; 
        }

        unsigned int getColumn() {
            return this->column;
        }

        double getValue() {
            return this->value;
        }
};

#endif
