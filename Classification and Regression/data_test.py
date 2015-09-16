import numpy as np
from scipy.optimize import minimize
from scipy.io import loadmat
from math import sqrt
import scipy.io
import matplotlib.pyplot as plt
import pickle

X,y,Xtest,ytest = pickle.load(open('E:/Sem 2/ML/PA2/sample.pickle','rb')) 


#def ldaLearn(X,y):
    # Inputs
    # X - a N x d matrix with each row corresponding to a training example
    # y - a N x 1 column vector indicating the labels for each training example
    #
    # Outputs
    # means - A d x k matrix containing learnt means for each of the k classes
    # covmat - A single d x d learnt covariance matrix 
    
    # IMPLEMENT THIS METHOD

k = np.amax(y);
means = np.zeros((X.shape[1],k));    
counter = np.zeros((k)); 
k = int(k);
print(k);   
for i in range(0,y.shape[0]):
    for j in range(0, X.shape[1]):
        for m in range(1, k+1):
            if (y[i] == m):
                means[j][m-1] += X[i][j];
                

for i in range(0,y.shape[0]):
    for m in range(1, k+1):
        if (y[i] == m):
            counter[m-1] = counter[m-1] + 1;
            
for i in range(0, X.shape[1]):    
    for j in range(0, k):    
        means[i][j] = means[i][j] / float(counter[j]);
    
print("Means : ", means);    

mcd = np.array([np.mean(means[0]), np.mean(means[1])]);
print("Mean Corrected Error : " , mcd);

x1 = X - mcd;
print(x1);

print(means)
'''
 #   return means,covmat
ctemp = np.array([]);
for i in range(1, k+1):
    for j in range(0, X.shape[0]):
        


'''
print(X.shape);
print(y.shape);
print(Xtest.shape);
print(ytest.shape);
print(ytest);
