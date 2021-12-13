import re

data = list(open("2015-16-data").readlines())

inputVal = { 
    'children': 3,
    'cats': 7,
    'samoyeds': 2,
    'pomeranians': 3,
    'akitas': 0,
    'vizslas': 0,
    'goldfish': 5,
    'trees': 3,
    'cars': 2,
    'perfumes': 1,
}

# Sue ([0-9]+): 

parser = re.compile(r"Sue ([0-9]+): (.*)")
def parse(data):
    sues = dict()

    for line in data:

        matches = parser.findall(line)[0]
        attrs = matches[1].split(', ')
        sueId = int(matches[0])
        sues[sueId] = dict()

        for attr in attrs:
            keyval =  attr.split(': ')
            sues[sueId][keyval[0]] = int(keyval[1])

    return sues

def getScore(sue):
    score = 0
    for key in sue.keys():
        if key in inputVal.keys():
            if (key == 'cats' or key == 'trees'):
                if inputVal[key] < sue[key]:
                    score += 1
                continue

            if (key == 'pomeranians' or key == 'goldfish'):
                if inputVal[key] > sue[key]:
                    score += 1
                continue

            if inputVal[key] == sue[key]:
                score += 1

    return score


sues = parse(data)

scores = dict()

maxScore = 0
bestSue = None
for sue in sues.keys():
    score = getScore(sues[sue])
    if score > maxScore:
        maxScore = score
        bestSue = sue

print(maxScore, bestSue)