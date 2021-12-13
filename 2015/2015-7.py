import re

data = list(open("2015-7-data").readlines())

values = dict()
def getValue(valueName):
    if type(valueName) == str and valueName.isnumeric():
        return int(valueName)

    value = values[valueName]
    if type(value) == Rule:
        value.apply()

    if type(value) == str and not value.isnumeric():
        print(value)
        return getValue(value)
    
    return int(values[valueName])

def setValue(key, value):
    value &= 0x0000FFFF
    values[key] = value

class Rule:
    def __init__(self, name, left, right, output):
        self.name = name
        self.left = left
        self.right = right
        self.output = output
    
    def apply(self):
        if self.name == "RSHIFT":
            leftValue = getValue(self.left)
            setValue(self.output, leftValue >> self.right)
        if self.name == "LSHIFT":
            leftValue = getValue(self.left)
            setValue(self.output, leftValue << self.right)
        if self.name == "NOT":
            rightValue = getValue(self.right)
            setValue(self.output, ~rightValue)
        if self.name == "AND":
            rightValue = getValue(self.right)
            leftValue = getValue(self.left)
            setValue(self.output, rightValue & leftValue)
        if self.name == "OR":
            rightValue = getValue(self.right)
            leftValue = getValue(self.left)
            setValue(self.output, rightValue | leftValue)



isStartNode = re.compile(r"([a-z]{1,2}|[0-9]+) -> ([a-z]+)")
isShiftRule = re.compile(r"([a-z]{1,2}|[0-9]+) (RSHIFT|LSHIFT) ([a-z]{1,2}|[0-9]+) -> ([a-z]{1,2})")
isNotRule = re.compile(r"(NOT) ([a-z]{1,2}) -> ([a-z]{1,2})")
isAndOrRule = re.compile(r"([a-z]{1,2}|[0-9]+) (AND|OR) ([a-z]{1,2}|[0-9]+) -> ([a-z]{1,2})")

test_rules = [
    "123 -> x",
    "456 -> y",
    "x AND y -> d",
    "x OR y -> e",
    "x LSHIFT 2 -> f",
    "y RSHIFT 2 -> g",
    "NOT x -> h",
    "NOT y -> i",
    "1 OR y -> z"
]

def calculateWires():
    global values
    values = dict()
    for line in data:
        if isStartNode.match(line):
            info = isStartNode.findall(line)[0]
            values[info[1]] = info[0]
        elif isShiftRule.match(line):
            info = isShiftRule.findall(line)[0]
            values[info[3]] = Rule(info[1], info[0], int(info[2]), info[3])
        elif isNotRule.match(line):
            info = isNotRule.findall(line)[0]
            values[info[2]] = Rule(info[0], None, info[1], info[2])
        elif isAndOrRule.match(line):
            info = isAndOrRule.findall(line)[0]
            values[info[3]] = Rule(info[1], info[0], info[2], info[3])

calculateWires()
for v in values.keys():
    values[v] = getValue(v)

newB = values["a"]

calculateWires()
values['b'] = newB
for v in values.keys():
    values[v] = getValue(v)

print(values['a'])

    