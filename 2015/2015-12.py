import json, re

data = open("2015-12-data").read()

jsonData = json.loads(data)

total = 0
def parseVal(nodeVal):
    global total
    if type(nodeVal) == int:
        total += nodeVal
        return
    if type(nodeVal) == str:
        return
    if type(nodeVal) == list:
        for elem in nodeVal:
            parseVal(elem)
        return

    if "red" in nodeVal.values():
        return

    for key in nodeVal.keys():
        val = nodeVal[key]
        parseVal(val)

parseVal(jsonData)
print(total)