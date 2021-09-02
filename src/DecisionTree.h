#include <Rcpp.h>
#include <vector>
#include <limits>
#include <queue>
#include "Leaf.h"
#include "Rule.h"
#include "DecisionNode.h"
#include "util.h"

class Node; 

class DecisionTree {
    private:
        Node* root = nullptr;
        Rcpp::StringVector colNames;
        std::string target;
        //unsigned int rowThreshold;
        //float sumThreshold;
        float threshold;
        Rcpp::NumericVector targetCol;

        Node* buildTree(const Rcpp::DataFrame& df, const std::vector<int> rows) {
            // if we have reached a row threshold we're done building thre tree
            if (rows.size() <= threshold)
                return new Leaf(this->targetCol, rows);

            // else we have to find the best split first
            Rule* r = nullptr;
            Rule* rule = new Rule();
            for (const auto& col : colNames) {
                std::string colName = Rcpp::as<std::string>(col);
                Rcpp::NumericVector dfcol = df[colName];
                r = buildTreeHelper(dfcol, rows, colName);
                std::cout << r->getRule() << " " << r->getSSE() << std::endl;
                if (r->getSSE() < rule->getSSE())
                    rule = r;

                else 
                    delete r;
            }

            // at this stage we have found the rule that minimizes sse
            // now we partition the rows
            std::vector<int> trueRows, falseRows;

            Rcpp::NumericVector dfcol = df[rule->getColumn()];
            std::cout << rule->getRule() << std::endl;
            for (const auto& i : rows) {
                if (rule->match(dfcol[i])) 
                    trueRows.push_back(i);
                else {
                    falseRows.push_back(i);
                }
            }

            std::cout << rule->getSSE() << std::endl;
            DecisionNode* node = new DecisionNode(rule);
            node->trueEdge = buildTree(df, trueRows);
            node->falseEdge = buildTree(df, falseRows); 

            return node;
        }

        // find optimal rule by minimizing sum of squared errors
        Rule* buildTreeHelper(const Rcpp::NumericVector& vec, const std::vector<int>& rows, const std::string& col) {
            Rule* r;
            Rule* rule = new Rule();
            Rcpp::NumericVector sortedVec = clone(vec);
            std::sort(sortedVec.begin(), sortedVec.end());
            unsigned int nRows = rows.size();
            float minSSE = std::numeric_limits<float>::infinity();

            for (int i = 0; i < nRows-1; i++) {
                // take average of two data points
                float avg = (sortedVec[rows[i]] + sortedVec[rows[i+1]]) / 2;
                r = new Rule(col, avg);
                float trueMean = 0;
                float falseMean = 0;

                std::vector<int> trueRows, falseRows;

                // partition 
                for (const int& j : rows) {
                    float val = vec[j];
                    if (r->match(val)) {
                        trueRows.push_back(j);
                        trueMean += this->targetCol[j];
                    }

                    else {
                        falseRows.push_back(j);
                        falseMean += this->targetCol[j];
                    }
                }

                // only care about nonzero partitions
                if (trueRows.size() == 0 || falseRows.size() == 0) {
                    delete r;
                    continue;
                }

                trueMean /= trueRows.size();
                falseMean /= falseRows.size();

                float sumSE = sse(this->targetCol, trueRows, falseRows, trueMean, falseMean);
                r->setSSE(sumSE);
                if (sumSE < minSSE) {
                    minSSE = sumSE;    
                    delete rule;
                    rule = r;
                }
                
                else {
                    delete r;
                }
            }

            return rule;
        }

        void dfs(Node* ptr, std::string spacing = "") {
            std::cout << spacing;
            ptr->printNode();

            if (ptr->trueEdge) {
                std::cout << spacing << "---> true";
                dfs(ptr->trueEdge, spacing + " ");
            }
            if (ptr->falseEdge) {
                std::cout << spacing << "---> false";
                dfs(ptr->falseEdge, spacing + " ");
            }
        }

    public:
        DecisionTree(const Rcpp::DataFrame& df, std::string target, int threshold = 20) {
            this->target = target;

            const int n = df.nrows();
            std::vector<int> rows (n);
            for (int i = 0; i < n; i++)
                rows[i] = i;

            // we should tweak the threshold of this....
            this->threshold = threshold;

            Rcpp::CharacterVector tCols = df.names();

            // remove the target var from colnames
            for (const auto& col : tCols) {
                if (col != target)
                    this->colNames.push_back(col);
            }

            this->targetCol = df[this->target];
            
            root = buildTree(df, rows);
        }

        // bfs to delete all the nodes and free memory
        ~DecisionTree() {
            Node* ptr = root;

            std::queue<Node*> q;
            if (root)
                q.push(ptr);

            while (!q.empty()) {
                Node* front = q.front();
                q.pop();
                if (front->trueEdge)
                    q.push(front->trueEdge);
                if (front->falseEdge)
                    q.push(front->falseEdge);

                delete front;
            }
        }
        
        // dfs to output tree
        void printTree() {
            dfs(root);
        } 
};
