import re

def increment(count, idx):
    increaseChar = chr(ord(count[idx]) + 1)
    carry = False
    if ord(increaseChar) == ord('z') + 1:
        carry = True
        increaseChar = 'a'

    countList = list(count)
    countList[idx] = increaseChar
    count = "".join(countList)

    if carry:
        return increment(count, idx - 1)
    else:
        return count


doubleRepeat = re.compile(r".*(?P<double1>.)(?P=double1).*(?P<double2>.)(?P=double2).*")
lioMatch = re.compile(r".*(?:[lio]).*")
def validate(code):
    # first validation
    # suite de 3
    validated = False
    for i, ch in enumerate(code):
        if i + 3 >= len(code):
            break
        if  ord(code[i]) == ord(ch) and \
            ord(code[i + 1]) == ord(ch) + 1 and \
            ord(code[i + 2]) == ord(ch) + 2:
            validated = True
            break

    if not validated:
        return False

    # second 
    # pas i,o,l
    if lioMatch.match(code) != None:
        return False

    # third
    # 2x repeat
    if doubleRepeat.match(code) == None:
        return False
    return True

current = "hepxxzaa"
while not validate(current):
    current = increment(current, len(current) - 1)

print(current)