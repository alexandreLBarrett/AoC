data = [int(x) for x in list(open("2021/1-data").readlines())]

test_data = [
    199,
    200,
    208,
    210,
    200,
    207,
    240,
    269,
    260,
    263,
]

"""
199  A      
200  A B    
208  A B C  
210  D B C
200  D E C
207  D E F 
240  G E F
269  G H F
260  G H 
263    H
"""

is_test = False
def main_1(data):
    totalUps = 0
    totalDowns = 0
    prev = data[0]
    for entry in data[1:]:
        if entry > prev:
            totalUps += 1
        else:
            totalDowns += 1

        prev = entry

    return totalUps, totalDowns

def main_2(data):
    windowsNum = 3

    windows = []
    iter = -1
    for idx, entry in enumerate(data):
        newIter = idx // 3

        if iter != newIter:
            scaledIter = newIter * windowsNum
            if scaledIter + windowsNum <= len(data):
                windows.append(0)
            if scaledIter + windowsNum + 1 <= len(data):
                windows.append(0)
            if scaledIter + windowsNum + 2 <= len(data):
                windows.append(0)
            iter = newIter
            
        offset = iter * 3 + idx % 3
        if offset < len(windows):
            windows[offset] += entry
        if idx > 0 and offset - 1 < len(windows):
            windows[offset - 1] += entry
        if idx > 1 and offset - 2 < len(windows):
            windows[offset - 2] += entry

    return windows

print(main_1(main_2(test_data if is_test else data)))