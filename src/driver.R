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

X = cbind(X, y)
X = data.frame(X)
names(X)[6] = "Y"
X


X = X %>% filter(X5 >= 0.494426)
model <- tree(Y ~ ., data = X)
plot(model)
text(model)
test(X, "Y", 10)
