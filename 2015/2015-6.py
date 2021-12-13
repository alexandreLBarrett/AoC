import re

data = list(open("2015-6-data").readlines())

class Light:
    def __init__(self):
        self.status = 0

    def toggle(self):
        self.status += 2

    def turnOn(self):
        self.status += 1

    def turnOff(self):
        if self.status > 0:
            self.status -= 1

    def brightness(self):
        return self.status

    def __str__(self) -> str:
        return str(1 if self.status == True else 0)
    def __unicode__(self):
        return self.__str__()
    def __repr__(self):
        return self.__str__()


def resetLights():
    lightsSize = 1000
    return [[Light() for _ in range(lightsSize)] for _ in range(lightsSize)]

def apply(lights, effect, start, end):
    dx = end[0] - start[0]
    dy = end[1] - start[1]
    
    for x in range(start[0], start[0] + dx + 1):
        for y in range(start[1], start[1] + dy + 1):
            if effect == "turn on":
                lights[x][y].turnOn()
            elif effect == "turn off":
                lights[x][y].turnOff()
            elif effect == "toggle":
                lights[x][y].toggle()


pattern = re.compile(r"(?P<action>turn off|toggle|turn on) (?P<startX>[0-9]{1,3}),(?P<startY>[0-9]{1,3}) through (?P<endX>[0-9]{1,3}),(?P<endY>[0-9]{1,3})")
def parse(line):    
    matches = pattern.findall(line)[0]
    return matches[0], (int(matches[1]),int(matches[2])), (int(matches[3]),int(matches[4]))

tests = [
    "toggle 0,0 through 1,1",
    "toggle 0,0 through 999,999",
]

lights = resetLights()
for line in data:
    action, start, end = parse(line)
    apply(lights, action, start, end)

count = 0
for lightRow in lights:
    for light in lightRow:
            count += light.brightness()

print(count)
