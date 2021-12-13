import re

data = list(open("2015-13-data").readlines())

test_data = """Alice would gain 54 happiness units by sitting next to Bob.
Alice would lose 79 happiness units by sitting next to Carol.
Alice would lose 2 happiness units by sitting next to David.
Bob would gain 83 happiness units by sitting next to Alice.
Bob would lose 7 happiness units by sitting next to Carol.
Bob would lose 63 happiness units by sitting next to David.
Carol would lose 62 happiness units by sitting next to Alice.
Carol would gain 60 happiness units by sitting next to Bob.
Carol would gain 55 happiness units by sitting next to David.
David would gain 46 happiness units by sitting next to Alice.
David would lose 7 happiness units by sitting next to Bob.
David would gain 41 happiness units by sitting next to Carol.""".split('\n')

likes = dict()
likes["Alex"] = dict()
seatings = dict()

parser = re.compile(r"(\w+) would (\w+) ([0-9]+) happiness units by sitting next to (\w+).")
for line in data:
    values = parser.findall(line)[0]
    who = values[0]
    like = values[3]
    if who not in likes.keys():
        likes[who] = dict()
    if like not in likes.keys():
        likes[like] = dict()

    likes[who][like] = int(values[2]) * -1 if values[1] == 'lose' else int(values[2])
    likes[who]["Alex"] = 0
    likes["Alex"][who] = 0

for first in likes.keys():
    for second in likes[first].keys():
        seatings[(first, second)] = likes[first][second] + likes[second][first]
        seatings[(second, first)] = likes[first][second] + likes[second][first]

def fournish(paths):
    newPaths = paths.copy()
    for path in paths.keys():
        reference = path[len(path) - 1]
        for nextStep in likes[reference]:
            if nextStep in path:
                continue
            newPaths[path + (nextStep,)] = paths[path] + likes[reference][nextStep] + likes[nextStep][reference]
            newPaths[(nextStep,) + path[::-1]] = paths[path] + likes[reference][nextStep] + likes[nextStep][reference]
    return newPaths

prev = dict()
while prev != seatings:
    prev = seatings
    seatings = fournish(seatings)

fullSeatings = {key:seatings[key] for key in seatings.keys() if len(key) == len(likes.keys())}

for seating in fullSeatings:
    who = seating[0]
    like = seating[len(seating) - 1]
    total = likes[who][like] + likes[like][who]
    fullSeatings[seating] += total

print(max([val for val in fullSeatings.values()]))