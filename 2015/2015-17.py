import re

data = [int(x) for x in list(open("2015-17-data").readlines())]
test_data = [20, 15, 10, 5, 5]
total_aim = 25

containers = dict()

for cont in test_data:
    if cont not in containers:
        containers[cont] = 0    
    containers[cont] += 1

combinations = dict()
for val in containers.keys():
    combinations[(val)] = val

for val in combinations.keys():
    for contKeys in containers.keys():
        if sum(val) + contKeys <= total_aim:
            combinations[val + (contKeys,)]

print(combinations)
