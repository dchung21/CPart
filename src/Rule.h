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
        std::string column;
        double value;
        float sse = std::numeric_limits<float>::infinity();

    public:
        Rule(std::string column, float value) {
            this->column = column;
            this->value = value;
        }

        // default constructor for dummy rules
        Rule() {
            this->column = "";
        }

        bool match(const float val) {
            return val < this->value;
        }

        std::string getRule() {
            return this->column + " < " + std::to_string(this->value);
        }

        float getSSE() {
            return this->sse;
        }

        void setSSE(const float sse) {
            this->sse = sse; 
        }

        std::string getColumn() {
            return this->column;
        }

        std::double getValue() {
            return this->value;
        }
};

#endif
