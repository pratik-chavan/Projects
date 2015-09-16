from __future__ import division
import numpy as np
from scipy.optimize import minimize
from scipy.io import loadmat
from math import sqrt
import scipy.io
import matplotlib.pyplot as plt
import pickle
import math

X,y,Xtest,ytest = pickle.load(open('E:/Sem 2/ML/PA2/sample.pickle','rb'))
A = np.column_stack((X,y))
grp = np.unique(A[:,2])
B = np.array([0.0,0.0,0.0,0.0,0.0])
C = np.array([0.0,0.0,0.0,0.0,0.0])
A = A[A[:,2].argsort()]
uniq, inv = np.unique(y, return_inverse=True)
n = np.bincount(inv)
for idx, val in enumerate(A[:,2]):
    for idx1, val1 in enumerate(grp):
        if (val == val1):
            B[idx1]+=A[idx,0]
            C[idx1]+=A[idx,1]
means = np.hstack((B/n, C/n))
print means
gmean = X.mean(axis=0)
A[:,0] = A[:,0] - gmean[0];
A[:,1] = A[:,1] - gmean[1];
#print A
#cov = np.cov(A[:,:2], rowvar = 0)
qdacov = []
p=0

for val in n:
    ar = np.matrix(A[p:val+p,:2])
    ar1 = np.cov(ar, rowvar=0)
    qdacov.append(ar1)
    p=val
    print ar1




k = np.amax(y);
k = int(k);
maxval = 0;
pred = 0;
m = means;
m1 = np.ndarray([]);
m1 = np.reshape(m,(2,5)).T;
print(m1);
prediction = np.zeros((100,1));

   
temp = 0;      
                 
for i in range(0, ytest.shape[0]):
    for j in range(1, k+1):
        tmp = np.mat(m1[j-1,:]);
        #print(tmp.shape , cinv.shape, np.mat(Xtest[i]).T.shape, tmp.shape ,cinv.shape, tmp.T.shape);
        #m1[j] = np.reshape(2,1);
        #temp1 = np.dot( np.dot(tmp,np.linalg.inv(qdacov[j-1])) ,np.mat(Xtest[i]).T ); 
        #temp2 = (0.5)* np.dot(np.dot(tmp,np.linalg.inv(qdacov[j-1])),tmp.T); 
        '''
        temp1 = tmp* np.linalg.inv(qdacov[j-1]) *np.mat(Xtest[i]).T;
        temp2 = (0.5)*tmp* np.linalg.inv(qdacov[j-1]) *tmp.T;
        
        
        #print(temp1, temp2);
        
        temp = temp1 - temp2;
        
        '''
        temp1 = (1/ (2*3.14168)**(Xtest.shape[1]/2.0) * math.sqrt((np.linalg.det(qdacov[j-1]))))
        temp2 = math.exp((-0.5)*(Xtest[i]-tmp) * qdacov[j-1]* (Xtest[i]-tmp).T);
        temp = temp1*temp2
        
        if temp > maxval:
            maxval = temp;
            pred = j; 
    maxval = 0;               
    prediction[i] = pred;
        
match = 0.0;
for i in range(0, prediction.shape[0]):
    #print prediction[i] , ytest[i]
    if ytest[i] == prediction[i]:
        match = match + 1;
acc = match / ytest.shape[0];
print "QDA Accuracy : " , acc*100
