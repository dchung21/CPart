#include <Rcpp.h>
#include <DecisionTree.h>
#include <format.h>
#include "Rule.h"

class Node;

using namespace Rcpp;

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

// [[Rcpp::export]]
Rcpp::NumericMatrix test(const DataFrame& df, const DataFrame& X) {

    /*
    DecisionTree* dt = new DecisionTree(df, "Y");
    dt->printTree();
    const unsigned int numLeaves = dt->getNumLeaves();
    Node* root = dt->getRoot();
    */
    Rule* arr[7];
    arr[0] = new Rule(4, 0.4944);
    arr[1] = new Rule(4, -1.578);
    arr[2] = new Rule(3, -1.232);
    arr[3] = new Rule(2, -1.077);
    arr[4] = new Rule(1, 1.3);
    arr[5] = new Rule(2, 0.964);
    arr[6] = new Rule(2, 0.7261);

    Node* nodeArr[15];
    nodeArr[0] = new Node(arr[0]);
    nodeArr[1] = new Node(arr[1]);
    nodeArr[2] = new Node(arr[2]);
    nodeArr[3] = new Node(arr[3]);
    nodeArr[4] = new Node(arr[4]);
    nodeArr[5] = new Node(arr[5]);
    nodeArr[6] = new Node(arr[6]);
    nodeArr[7] = new Node(12.32);
    nodeArr[8] = new Node(-9.203);
    nodeArr[9] = new Node(-7.51);
    nodeArr[10] = new Node(-0.6628);
    nodeArr[11] = new Node(2.988);
    nodeArr[12] = new Node(4.461);
    nodeArr[13] = new Node(4.135);
    nodeArr[14] = new Node(13.33);

    nodeArr[0]->trueEdge = nodeArr[1];
    nodeArr[0]->falseEdge = nodeArr[6];
    nodeArr[1]->trueEdge = nodeArr[7];
    nodeArr[1]->falseEdge = nodeArr[2];
    nodeArr[2]->trueEdge = nodeArr[8];
    nodeArr[2]->falseEdge = nodeArr[3];
    nodeArr[3]->trueEdge = nodeArr[9];
    nodeArr[3]->falseEdge = nodeArr[4];
    nodeArr[4]->trueEdge = nodeArr[5];
    nodeArr[4]->falseEdge = nodeArr[12];
    nodeArr[5]->trueEdge = nodeArr[10];
    nodeArr[5]->falseEdge = nodeArr[11];
    nodeArr[6]->trueEdge = nodeArr[13];
    nodeArr[6]->falseEdge = nodeArr[14];


    dfs(nodeArr[0], "");
    //Rcpp::NumericMatrix res = extractPartition(X, root, numLeaves);
    //delete dt;
    Rcpp::NumericMatrix res = extractPartition(X, nodeArr[0], 8);

    for (unsigned int i = 0; i < 15; i++) {
        delete nodeArr[i];
    }


    printf("b\n");
    return res;
}
