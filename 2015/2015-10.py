inputValue = "1321131112"

tests = [
    "1",        # becomes 11
    "11",       # becomes 21
    "21",       # becomes 1211
    "1211",     # becomes 111221
    "111221"    # becomes 312211
]

def runTests():
    for test in tests:
        print(run(test))

def run(value):
    suiteCount = 0
    n = ""
    out = ""
    for idx, number in enumerate(value):
        n = number
        suiteCount += 1
        if idx + 1 < len(value) and value[idx + 1] != number:
            out += str(suiteCount)
            out += number
            suiteCount = 0

    out += str(suiteCount)
    out += n
    return out


for i in range(50):
    inputValue = run(inputValue)
print(len(inputValue))

