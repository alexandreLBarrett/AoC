
data = list(open("2015-3-data").read())

coords = [0,0]
robotCoords = [0,0]

houses = { str(coords): 2 }

isRobot = False
strCoords = ""

for direction in data:
    xOff = 0
    yOff = 0

    if direction == "^":   yOff += 1
    elif direction == ">": xOff += 1
    elif direction == "<": xOff -= 1
    elif direction == "v": yOff -= 1

    if isRobot:
        robotCoords[0] += xOff
        robotCoords[1] += yOff
        strCoords = str(robotCoords)
    else:
        coords[0] += xOff
        coords[1] += yOff
        strCoords = str(coords)

    if strCoords in houses.keys():
        houses[strCoords] += 1
    else:
        houses[strCoords] = 1

    isRobot = not isRobot

print(len(houses.keys()))

