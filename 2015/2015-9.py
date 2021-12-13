import re

data = list(open("2015-9-data").readlines())

lineParser = re.compile(r"(\w+) to (\w+) = ([0-9]+)")

paths = dict()
dists = dict()

tests = [
    "London to Dublin = 464",
    "London to Belfast = 518",
    "Dublin to Belfast = 141"
]

for line in data:
    matches = lineParser.findall(line)[0]
    cityFrom = matches[0]
    cityTo = matches[1]
    distance = int(matches[2])

    if cityFrom not in dists.keys():
        dists[cityFrom] = dict()
    if cityTo not in dists.keys():
        dists[cityTo] = dict()

    dists[cityFrom][cityTo] = distance
    dists[cityTo][cityFrom] = distance

    paths[(cityFrom, cityTo)] = distance
    paths[(cityTo, cityFrom)] = distance



def fournish(paths):
    newPaths = paths.copy()
    for path in paths.keys():
        reference = path[len(path) - 1]
        for nextStep in dists[reference]:
            if nextStep in path:
                continue
            newPaths[path + (nextStep,)] = paths[path] + dists[reference][nextStep]
            newPaths[(nextStep,) + path[::-1]] = paths[path] + dists[reference][nextStep]
    return newPaths

prev = dict()
while prev != paths:
    prev = paths
    paths = fournish(paths)

print(min([paths[key] for key in paths.keys() if len(key) == len(dists.keys())]))
print(max([paths[key] for key in paths.keys() if len(key) == len(dists.keys())]))
