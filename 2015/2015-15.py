import re
from datetime import datetime

data = list(open("2015-15-data").readlines())

test_data = """Butterscotch: capacity -1, durability -2, flavor 6, texture 3, calories 8
Cinnamon: capacity 2, durability 3, flavor -2, texture -1, calories 3""".split("\n")

class Ingredient:
    def __init__(self, name, capacity, durability, flavor, texture, calories):
        self.name = name
        self.capacity = capacity
        self.durability = durability
        self.flavor = flavor
        self.texture = texture
        self.calories = calories

    def getTotal(self):
        return self.capacity * self.durability * self.flavor * self.texture

    def __str__(self):
        return f"{self.name}: cap = {self.capacity} dura = {self.durability} flav = {self.flavor} text = {self.texture} cals = {self.calories}"

    def __repr__(self):
        return self.__str__()

parser = re.compile(r"(\w+): capacity (-?[0-9]+), durability (-?[0-9]+), flavor (-?[0-9]+), texture (-?[0-9]+), calories (-?[0-9]+)")
def parse(data):
    ingredients = []

    for line in data:
        matches = parser.findall(line)[0]
        ingredients.append(Ingredient(matches[0], int(matches[1]), int(matches[2]), int(matches[3]), int(matches[4]), int(matches[5])))
    
    return ingredients

ingredients = parse(data)

ratios = dict()
def calculateProportions(*args):
    ingr = Ingredient("", 0,0,0,0,0)
    for i, val in enumerate(args):
        ingr.capacity = ingr.capacity + ingredients[i].capacity * val
        ingr.durability = ingr.durability + ingredients[i].durability * val
        ingr.flavor = ingr.flavor + ingredients[i].flavor * val
        ingr.texture = ingr.texture + ingredients[i].texture * val
        ingr.calories = ingr.calories + ingredients[i].calories * val
    
    ingr.capacity = max(ingr.capacity, 0)
    ingr.durability = max(ingr.durability, 0)
    ingr.flavor = max(ingr.flavor, 0)
    ingr.texture = max(ingr.texture, 0)
    ingr.calories = max(ingr.calories, 0)

    ratios[str(args)] = ingr

now = datetime.now()
for a in range(101):
    if a == 100:
        calculateProportions(a, 0, 0, 0)
        continue

    for b in range(101):
        if a + b == 100:
            calculateProportions(a, b, 0, 0)
            continue

        for c in range(101):
            if a + b + c == 100:
                calculateProportions(a, b, c, 0)
                continue

            for d in range(101):
                if a + b + c + d == 100:
                    calculateProportions(a, b, c, d)

current_time = datetime.now() - now
print("Current Time =", current_time)

print(ratios)
print(max([x.getTotal() for x in ratios.values()]))

zeroes = [x for x in ratios.values() if x.getTotal() == 0]
print(len(zeroes), len(zeroes) / len(ratios.values()) * 100)