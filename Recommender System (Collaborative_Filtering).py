import numpy as np
import math

user_shows_string = open("C:/Users/Pratik/workspace/Big_Data/q1-dataset/user-shows.txt")

ratings_for_each_user_string = user_shows_string.read().split('\n')

#user_shows_temp = list[range(0,len(ratings_for_each_user_string))]
#print ("User shows length : " + str(len(ratings_for_each_user_string)))

user_shows = [[]] * len(ratings_for_each_user_string)

for i in range(0,len(ratings_for_each_user_string)):
    user_shows[i] = ratings_for_each_user_string[i].split(' ')

for i in range(0,len(ratings_for_each_user_string)):
    for j in range(0, len(user_shows[i]) - 1):
        user_shows[i][j] = int(user_shows[i][j])
    del user_shows[i][len(user_shows[i]) - 1]
del user_shows[len(ratings_for_each_user_string)-1]



M = np.mat(user_shows)
p_list = [[0]*M.shape[0]]*M.shape[0]
P = np.mat(p_list)
q_list = [[0]*M.shape[1]]*M.shape[1]
Q = np.mat(q_list)

for i in range(0,M.shape[0]):
    temp_sum = 0
    for j in range(0 , M.shape[1]):
        temp_sum += M.item(i,j)
    P[i,i] = temp_sum
        
for i in range(0,M.shape[1]):
    temp_sum = 0
    for j in range(0 , M.shape[0]):
        temp_sum += M.item(j,i)
        #print temp_sum
    Q[i,i] = temp_sum

#Q_star = 1/np.sqrt(Q)

Q_star = Q

for i in range(0, Q_star.shape[0]):
    for j in range(0 , Q_star.shape[0]):
        if i == j and Q_star.item(i,i) != 0:
            Q_star[i,i] = 1/ math.sqrt(Q_star.item(i,i))

SI = Q_star * M.T * M * Q_star

#SI = np.multiply(M, np.multiply(Q_star, np.multiply(M.T, np.multiply(M ,Q_star))))
'''
print (M.shape)
print (Q_star.shape)
print (M.T.shape)
print (M.shape) 
print (Q_star.shape)
'''
none_found = True
for i in range(0, SI.shape[0]):
    for j in range(0 , SI.shape[1]):
        if SI.item(i,j) != 0:
            print (SI.item(i,j))
            none_found = False

if none_found:
    print ("NO NON ZERO ELEMENT FOUND")