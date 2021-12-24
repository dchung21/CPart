library(Rcpp)
library(rpart)
library(rpart.plot)
library(tree)
library(dplyr)

sourceCpp("~/Desktop/CPart/src/driver.cpp")

set.seed(5)
d = 5
n = 100
X = mvtnorm::rmvnorm(100, rep(0, d), diag(1, d)) # covariates
beta = 1:d
eps = rnorm(n, 0, 3)
y = X %*% beta + eps # response

tree_model = rpart(y ~ X)

plot(tree_model)
text(tree_model)

og_X = X
X = cbind(X, y)
X = data.frame(X)
names(X)[6] = "Y"
test(X, X)

