import re

data = list(open("2015-5-data").readlines())

rules = [
#   re.compile(r"(.*[aeiou].*){3}"), # at least 3 vowels
    re.compile(r".*(?P<double>..).*(?P=double).*"),
    re.compile(r".*(?P<double>.).(?P=double).*"),
#   re.compile(r"(?!.*ab.*|.*cd.*|.*pq.*|.*xy.*)"),
]

tests = [
    "jchzalrnumimnmhp",
    "ugknbfddgicrmopn",
    "haegwjzuvuyypxyu",
    "dvszwmarrgswjxmb",
    "aaa",
    ###
    "qjhvhtzxzqqjkmpb",
    "xxyxx",
    "uurcxstgmygtbstg",
    "ieodomkazucvgmuy"
]

count = 0
for line in data:
    line = line.strip()
    matched = True
    for i in rules:
        matched = matched and i.match(line)

    if matched != None:
        count += 1

    print(line, " +" if matched != None else " -")
    
print(count)