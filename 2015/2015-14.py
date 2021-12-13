import re

data = list(open("2015-14-data").readlines())

test_data = """Comet can fly 14 km/s for 10 seconds, but then must rest for 127 seconds.
Dancer can fly 16 km/s for 11 seconds, but then must rest for 162 seconds.""".split("\n")

class Deer:
    def __init__(self, speed, period, rest):
        self.speed = speed
        self.period = period
        self.rest = rest
        self.distance = 0
        self.score = 0


parser = re.compile(r"(\w+) can fly ([0-9]+) km/s for ([0-9]+) seconds, but then must rest for ([0-9]+) seconds.")
def parse(data):
    retVal = dict()
    for line in data:
        parsed = parser.findall(line)[0]
        retVal[parsed[0]] = Deer(int(parsed[1]), int(parsed[2]), int(parsed[3]))
    return retVal

deers = parse(data)
time = 2503

def getDistance(deerName, time):
    deer = deers[deerName]
    fullIter = deer.period + deer.rest

    nbIter = int(time / fullIter)
    distance = deer.speed * (nbIter * deer.period)

    remainder = time - (nbIter * fullIter)

    distance += deer.speed * min(remainder, deer.period)
    deer.distance = distance

for i in range(1, time + 1):
    maxDistance = 0
    fastestDeer = ""
    for deerName in deers.keys():
        getDistance(deerName, i)

    for deerName in deers.keys():
        dist = deers[deerName].distance
        if dist > maxDistance:
            fastestDeer = deerName
            maxDistance = dist
    
    deers[fastestDeer].score += 1

maxVal = 0
for d in deers.keys():
    maxVal = max(deers[d].score, maxVal)

print(maxVal)