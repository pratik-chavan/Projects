import operator

############################## P A S S   1 ################################## 

f = open('browsing.txt' , 'r')
lines = f.readlines()
items_counts = {}
for line in lines:
    items = line.split(' ')
    for item in items:
        if item in items_counts:
            items_counts[item] += 1
        else:
            items_counts[item] = 1

frequent_items = {}
del items_counts['\n']
for item in items_counts:
    if items_counts[item] >= 100:
        frequent_items[item] = items_counts[item]
            
############################## P A S S   2 ################################## 
candidate_pairs = {}

for line in lines:
    items = line.split(' ')
    del items[len(items) - 1]
    for item_1 in items:
        if item_1 in frequent_items.keys():
            for item_2 in items:
                if item_2 in frequent_items.keys() and item_1 != item_2:
                    candidate_pair = item_1 + " " + item_2
                    if candidate_pair in candidate_pairs:
                        candidate_pairs[candidate_pair] += 1
                    else:
                        candidate_pairs[candidate_pair] = 1

frequent_pairs = {}

for candidate_pair in candidate_pairs:
    if candidate_pairs[candidate_pair] >= 100:
        frequent_pairs[candidate_pair] = candidate_pairs[candidate_pair]

frequent_pairs_list = []

confidence_frequent_pairs = {}

for frequent_pair in frequent_pairs:
    frequent_pair_list_entry = frequent_pair.split(" ")
    frequent_pairs_list.append(frequent_pair_list_entry)


for frequent_pair in frequent_pairs_list:
    item_1 = frequent_pair[0]
    item_2 = frequent_pair[1]
    count_1_confidence_1_2 = 0
    count_2_confidence_1_2 = 0
    count_1_confidence_2_1 = 0
    count_2_confidence_2_1 = 0
    confidence_1_2 = 0
    confidence_2_1 = 0
    
    for line in lines:
        items = line.split(' ')
        del items[len(items) - 1]
        
        if item_1 in line:
            count_1_confidence_1_2 += 1
            if item_2 in line:
                count_2_confidence_1_2 += 1
                
        if item_2 in line:
            count_2_confidence_2_1 += 1
            if item_1 in line:
                count_1_confidence_2_1 += 1
    
    confidence_1_2 = float(count_2_confidence_1_2) /  count_1_confidence_1_2         
    confidence_2_1 = float(count_1_confidence_2_1) /  count_2_confidence_2_1
    
    confidence_frequent_pairs[item_1 + "->" + item_2] =  confidence_1_2
    confidence_frequent_pairs[item_2 + "->" + item_1] =  confidence_2_1

sorted_x = sorted(confidence_frequent_pairs.items(), key=operator.itemgetter(1))
sorted_x.reverse()
for i in range(0,5):
    print ("Association Rule : " + sorted_x[i][0] + " Confidence : " + str(sorted_x[i][1]))
