import re

data = list(open("2015-8-data").readlines())

backslashesPattern = re.compile(r"\\\\")
quotePattern = re.compile(r"\\\"")
hexPattern = re.compile(r"\\x[0-9A-Fa-f]{2}")
string = "erse\\x9et\\x67mzhlmhy\\x67yftoti"

def processSlashes(line):
    m = backslashesPattern.findall(line)
    for match in m:
        line = line.replace(match, '_')
    return line

def processQuotes(line):
    m = quotePattern.findall(line)
    for match in m:
        line = line.replace(match, '_')
    return line

def processHex(line):
    m = hexPattern.findall(line)
    for match in m:
        line = line.replace(match, "_")
    return line

def process(line):
    line = processSlashes(line)
    line = processQuotes(line)
    line = processHex(line)
    return line

test_strings = [
    r'"\xFF"',
    r'"aaa\"aaa"',
    r'"abc"',
    r'""',
]

def pt1():
    total = 0
    for line in data:
        line = line.strip()
        lineCharSize = len(line)

        after = process(line[1:-1])
        lineByteSize = len(after)
        total += lineCharSize - lineByteSize
    
    return total

def pt2():
    total = 0
    for line in data:
        line = line.strip()
        after = line.replace('\\', r"\\")
        after = after.replace('\"', r"\"")
        after = '"' + after + '"'
        before = line
        print(before, after)
        total += len(after) - len(before)
    print(total)


pt2()