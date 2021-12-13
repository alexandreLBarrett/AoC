from hashlib import md5

keyInput = "iwrupvqb"

idx = 0
good = False
result = ""
while not good:
    idx += 1

    good = True
    result = md5((keyInput + str(idx)).encode()).hexdigest()
    if idx % 5000 == 0:
        print(idx)

    for i in range(0,6):
        if result[i] != '0':
            good = False

    #good = idx == 609046
    

print(idx, result)
