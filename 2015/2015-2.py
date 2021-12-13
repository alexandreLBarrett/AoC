
f = open("2015-2-data")

totalWrapping = 0
totalRibbon = 0

for line in f:
    dims = line.split('x')
    l = int(dims[0])
    w = int(dims[1])
    h = int(dims[2])

    front = l*w 
    side = w*h
    top = h*l

    boxWrapping = 2*front + 2*side + 2*top
    extra = min(front, side, top)
    
    ribbon = 0
    if extra == front:
        ribbon = 2*l + 2*w
    elif extra == side:
        ribbon = 2*w + 2*h
    elif extra == top:
        ribbon = 2*h + 2*l
    
    bow = w * h * l
    totalRibbon += bow + ribbon
    totalWrapping += boxWrapping + extra

print(totalWrapping, totalRibbon)