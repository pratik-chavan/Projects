import numpy as np
from scipy.optimize import minimize
from scipy.io import loadmat
from math import sqrt


def initializeWeights(n_in,n_out):
    """
    # initializeWeights return the random weights for Neural Network given the
    # number of node in the input layer and output layer

    # Input:
    # n_in: number of nodes of the input layer
    # n_out: number of nodes of the output layer
       
    # Output: 
    # W: matrix of random initial weights with size (n_out x (n_in + 1))"""
    
    epsilon = sqrt(6) / sqrt(n_in + n_out + 1);
    W = (np.random.rand(n_out, n_in + 1)*2* epsilon) - epsilon;
    return W
    
    
    
def sigmoid(z):
    
    """# Notice that z can be a scalar, a vector or a matrix
    # return the sigmoid of input z"""
    
    return  1/(1+np.exp(-z))
    
    
    

def preprocess():
    """ Input:
     Although this function doesn't have any input, you are required to load
     the MNIST data set from file 'mnist_all.mat'.

     Output:
     train_data: matrix of training set. Each row of train_data contains 
       feature vector of a image
     train_label: vector of label corresponding to each image in the training
       set
     validation_data: matrix of training set. Each row of validation_data 
       contains feature vector of a image
     validation_label: vector of label corresponding to each image in the 
       training set
     test_data: matrix of training set. Each row of test_data contains 
       feature vector of a image
     test_label: vector of label corresponding to each image in the testing
       set

     Some suggestions for preprocessing step:
     - divide the original data set to training, validation and testing set
           with corresponding labels
     - convert original data set from integer to double by using double()
           function
     - normalize the data to [0, 1]
     - feature selection"""
    
    
    #Pick a reasonable size for validation data
    mat = loadmat('E:/Sem 2/ML/PA -1/basecode/basecode/mnist_all.mat')
    
    train_data = np.array([])
    train_label = np.array([])
    validation_data = np.array([])
    validation_label = np.array([])
    test_data = np.array([])
    test_label = np.array([])
    A1 = []
    A2 = []
    A3 = []
    A4 = []
    A5 = []
    A6 = [] 
    B1 = []
    B2 = []
    B3 = []  
    for i in range(0,10):
        A = mat.get("train"+str(i))  #extracting every training set
        B = mat.get("test"+str(i))
        a = range(A.shape[0])
        b = range(B.shape[0])
        B1[len(B1):] = B   
                    
        aperm = np.random.permutation(a)    #using random data to separate validation and training data
        A1[len(A1):] = (A[aperm[0:1000],:]) #appending list with validation data    
        A2[len(A2):] = (A[aperm[1000:],:])  #appending list with training data
        A5 = (A[aperm[0:1000],:10])
        A6 = (A[aperm[1000:],:10])
        B2 = B[:,:10]
        B2[0:] = 0
        A5[0:] = 0
        A6[0:] = 0
        A5[:,[i]] = 1
        A6[:,[i]] = 1
        A3[len(A3):] = A5
        A4[len(A4):] = A6
        B2[:,[i]] = 1
        B3[len(B3):] = B2
        #print A5        
        #print A6
            
    
    validation_data = validation_data.astype(np.float32)
    validation_data = np.array(A1)/255.0
    train_data = train_data.astype(np.float32)
    train_data = np.array(A2)/255
    validation_label = np.array(A3)
    train_label = np.array(A4)
    test_data = test_data.astype(np.float32)
    test_data = np.array(B1)/255
    test_label = np.array(B3)
    
    
    bool_arr_train=np.all(train_data==train_data[0,:],axis=0)

    #bool_arr is a boolean array
    #If an element in bool_arr is true, then the corresponding column in train_data has all its elements equal
    indices_arr_train=[]
    
    for i in range(bool_arr_train.size):
        if bool_arr_train[i]==True:
            indices_arr_train.append(i)
            #indices_arr_validation.append(i)
            #indices_arr_test.append(i)

    #for i in range(bool_arr_train.size):
     #   if bool_arr_train[i]==True:
    
    #for i in range(bool_arr_train.size):
     #   if bool_arr_train[i]==True:
    
    
    #indices_arr contains the indices of the columns whose all elements are equal
    train_data=np.delete(train_data,indices_arr_train,1)
    #print (train_data.shape)
    validation_data=np.delete(validation_data,indices_arr_train,1)
    #print (test_data.shape)
    test_data=np.delete(test_data,indices_arr_train,1)
    #print (test_data.shape)
    
    #print "Train Label " , train_label
    #print "Test Label " , test_label
    #print "Validation Label " , validation_label
    return train_data, train_label, validation_data, validation_label, test_data, test_label
    
    
    

def nnObjFunction(params, *args):
    """% nnObjFunction computes the value of objective function (negative log 
    %   likelihood error function with regularization) given the parameters 
    %   of Neural Networks, thetraining data, their corresponding training 
    %   labels and lambda - regularization hyper-parameter.

    % Input:
    % params: vector of weights of 2 matrices w1 (weights of connections from
    %     input layer to hidden layer) and w2 (weights of connections from
    %     hidden layer to output layer) where all of the weights are contained
    %     in a single vector.
    % n_input: number of node in input layer (not include the bias node)
    % n_hidden: number of node in hidden layer (not include the bias node)
    % n_class: number of node in output layer (number of classes in
    %     classification problem
    % training_data: matrix of training data. Each row of this matrix
    %     represents the feature vector of a particular image
    % training_label: the vector of truth label of training images. Each entry
    %     in the vector represents the truth label of its corresponding image.
    % lambda: regularization hyper-parameter. This value is used for fixing the
    %     overfitting problem.
       
    % Output: 
    % obj_val: a scalar value representing value of error function
    % obj_grad: a SINGLE vector of gradient value of error function
    % NOTE: how to compute obj_grad
    % Use backpropagation algorithm to compute the gradient of error function
    % for each weights in weight matrices.

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % reshape 'params' vector into 2 matrices of weight w1 and w2
    % w1: matrix of weights of connections from input layer to hidden layers.
    %     w1(i, j) represents the weight of connection from unit j in input 
    %     layer to unit i in hidden layer.
    % w2: matrix of weights of connections from hidden layer to output layers.
    %     w2(i, j) represents the weight of connection from unit j in hidden 
    %     layer to unit i in output layer."""
    
    n_input, n_hidden, n_class, training_data, training_label, lambdaval = args
    
    
    w1 = params[0:n_hidden * (n_input + 1)].reshape( (n_hidden, (n_input + 1)))
    w2 = params[(n_hidden * (n_input + 1)):].reshape((n_class, (n_hidden + 1)))
    obj_val = 0  
    sig1 = []
    sig2 = []
    
    ar = np.ones(shape=(training_data.shape[0],1))
    training_data = np.hstack((training_data,ar))
    z = np.dot(training_data,w1.T)
    z = sigmoid(z)
    z1 = np.hstack((z, ar))
    o = np.dot(z1, w2.T)
    o = sigmoid(o)
    
    delta = o - training_label
    #J = sum(sum(training_label*np.log(o)+(1-training_label)*np.log(1-o)))/(-training_data.shape[0])   #eq 5
    J = (sum(sum(training_label*np.log(o) + ((1-training_label) * np.log(1-o)))))/(-training_data.shape[0])
    #print J
    #dlzj = np.dot(delta[0][:,np.newaxis],z1[0][:,np.newaxis].T)
    #for i in range(1,training_data.shape[0]):
        #dlzj += np.dot(delta[i][:,np.newaxis],z1[i][:,np.newaxis].T) #sum of equation 9 which is used in equation 16
    dlzj = np.dot(delta.T , z1)
    dw2=np.dot(delta,w2)
    dw2z=z1*(1-z1)
    dlzp = np.dot(np.transpose(dw2z*dw2),training_data)   #equation 12
    lamda = lambdaval
    Jcap = J + (lamda/(2*training_data.shape[0]))*((sum(sum(w1*w1)))+(sum(sum(w2*w2))))   #equation 15
    obj_val = Jcap
    #Make sure you reshape the gradient matrices to a 1D array. for instance if your gradient matrices are grad_w1 and grad_w2
    #you would use code similar to the one below to create a flat array
    #obj_grad = np.concatenate((grad_w1.flatten(), grad_w2.flatten()),0)
    obj_grad = np.array([])
    Jcap1 = (sum(dlzj)+lamda*w2)/training_data.shape[0]
    dlzp = np.delete(dlzp,(-1),axis=0)
    Jcap2 = (sum(dlzp)+lamda*w1)/training_data.shape[0]
    obj_grad = np.concatenate((Jcap1.flatten(),Jcap2.flatten()),0)
    print obj_val
    return (obj_val,obj_grad)



def nnPredict(w1,w2,data):
    
    """% nnPredict predicts the label of data given the parameter w1, w2 of Neural
    % Network.

    % Input:
    % w1: matrix of weights of connections from input layer to hidden layers.
    %     w1(i, j) represents the weight of connection from unit i in input 
    %     layer to unit j in hidden layer.
    % w2: matrix of weights of connections from hidden layer to output layers.
    %     w2(i, j) represents the weight of connection from unit i in input 
    %     layer to unit j in hidden layer.
    % data: matrix of data. Each row of this matrix represents the feature 
    %       vector of a particular image
       
    % Output: 
    % label: a column vector of predicted labels""" 
    #w1 = params[0:n_hidden * (n_input + 1)].reshape( (n_hidden, (n_input + 1)))
    #w2 = params[(n_hidden * (n_input + 1)):].reshape((n_class, (n_hidden + 1)))

    
    ar = np.ones(shape=(data.shape[0],1))
    data = np.hstack((data,ar))
    z = np.dot(data,w1.T)
    z = sigmoid(z)
    z1 = np.hstack((z, ar))
    o = np.dot(z1, w2.T)
    o = sigmoid(o)
        
    labels = np.array([])
    labels = np.argmax(o,axis =1)
    #print " o " , o.shape , " z " , z.shape, " data " , data.shape, " w1 ", w1.shape, " w2 " , w2.shape
    #print labels
    return labels
    #Your code here
    
    



"""**************Neural Network Script Starts here********************************"""
np.seterr(over='ignore')
train_data, train_label, validation_data,validation_label, test_data, test_label = preprocess();


#  Train Neural Network

# set the number of nodes in input unit (not including bias unit)
n_input = train_data.shape[1]; 

# set the number of nodes in hidden unit (not including bias unit)
n_hidden =400;
				   
# set the number of nodes in output unit
n_class = 10;				   

# initialize the weights into some random matrices
initial_w1 = initializeWeights(n_input, n_hidden);
initial_w2 = initializeWeights(n_hidden, n_class);

# unroll 2 weight matrices into single column vector
initialWeights = np.concatenate((initial_w1.flatten(), initial_w2.flatten()),0)

# set the regularization hyper-parameter
lambdaval = 0.001;

'''
n_input = 5
n_hidden = 3
n_class = 2
training_data = np.array([np.linspace(0,1,num=5),np.linspace(1,0,num=5)])
training_label = np.array([0,1])
lambdaval = 0
params = np.linspace(-5,5, num=26)
'''
args = (n_input, n_hidden, n_class, train_data, train_label, lambdaval)

#Train Neural Network using fmin_cg or minimize from scipy,optimize module. Check documentation for a working example

opts = {'maxiter' : 50}    # Preferred value.

nn_params = minimize(nnObjFunction, initialWeights, jac=True, args=args,method='CG', options=opts)

#In Case you want to use fmin_cg, you may have to split the nnObjectFunction to two functions nnObjFunctionVal
#and nnObjGradient. Check documentation for this function before you proceed.
#nn_params, cost = fmin_cg(nnObjFunctionVal, initialWeights, nnObjGradient,args = args, maxiter = 50)


#Reshape nnParams from 1D vector into w1 and w2 matrices
w1 = nn_params.x[0:n_hidden * (n_input + 1)].reshape( (n_hidden, (n_input + 1)))
w2 = nn_params.x[(n_hidden * (n_input + 1)):].reshape((n_class, (n_hidden + 1)))


#Test the computed parameters
predicted_label = nnPredict(w1,w2,train_data)
print predicted_label
#find the accuracy on Training Dataset
train_label = np.argmax(train_label,axis =1)
print train_label
print('\n Training set Accuracy:' + str(100*np.mean((predicted_label == train_label).astype(float))) + '%')
predicted_label = nnPredict(w1,w2,validation_data)
print predicted_label
#find the accuracy on Validation Dataset
validation_label = np.argmax(validation_label,axis =1)
print validation_label
print('\n Validation set Accuracy:' + str(100*np.mean((predicted_label == validation_label).astype(float))) + '%')


predicted_label = nnPredict(w1,w2,test_data)
print predicted_label
#find the accuracy on Validation Dataset
test_label = np.argmax(test_label,axis =1)
print test_label
print('\n Test set Accuracy:' + str(100*np.mean((predicted_label == test_label).astype(float))) + '%')
