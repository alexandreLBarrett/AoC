import math

def line_intersection(line1, line2):
    x1, x2, x3, x4 = line1[0][0], line1[1][0], line2[0][0], line2[1][0]
    y1, y2, y3, y4 = line1[0][1], line1[1][1], line2[0][1], line2[1][1]

    dx1 = x2 - x1
    dx2 = x4 - x3
    dy1 = y2 - y1
    dy2 = y4 - y3
    dx3 = x1 - x3
    dy3 = y1 - y3

    det = dx1 * dy2 - dx2 * dy1
    det1 = dx1 * dy3 - dx3 * dy1
    det2 = dx2 * dy3 - dx3 * dy2

    if det == 0.0:  # lines are parallel
        if det1 != 0.0 or det2 != 0.0:  # lines are not co-linear
            return None  # so no solution

        if dx1:
            if x1 < x3 < x2 or x1 > x3 > x2:
                return math.inf  # infinitely many solutions
        else:
            if y1 < y3 < y2 or y1 > y3 > y2:
                return math.inf  # infinitely many solutions

        if line1[0] == line2[0] or line1[1] == line2[0]:
            return line2[0]
        elif line1[0] == line2[1] or line1[1] == line2[1]:
            return line2[1]

        return None  # no intersection

    s = det1 / det
    t = det2 / det

    if 0.0 < s < 1.0 and 0.0 < t < 1.0:
        return x1 + t * dx1, y1 + t * dy1

print("one intersection")
print(line_intersection(((0.0,0.0), (6.0,6.0)),((0.0,9.0), (9.0,0.0))))
print(line_intersection(((-2, -2), (2, 2)), ((2, -2), (-2, 2))))
print(line_intersection(((0.5, 0.5), (1.5, 0.5)), ((1.0, 0.0), (1.0, 2.0))))
print(line_intersection(((0, -1), (0, 0)), ((0, 0), (0, 1))))
print(line_intersection(((-1, 0), (0, 0)), ((0, 0), (1, 0))))

print()
print("no intersection")
print(line_intersection(((-1, -1), (0, 0)), ((2, -4), (2, 4))))
print(line_intersection(((0.0,0.0), (0.0,9.0)),((9.0,0.0), (9.0,99.0))))
print(line_intersection(((0, 0), (1, 1)), ((1, 0), (2, 1))))
print(line_intersection(((-1, 1), (0, 1)), ((0, 0), (1, 0))))
print(line_intersection(((1, -1), (1, 0)), ((0, 0), (0, -1))))

print()
print("infinite intersection")
print(line_intersection(((-1, -1), (1, 1)), ((0, 0), (2, 2))))
print(line_intersection(((-1, 0), (1, 0)), ((0, 0), (2, 0))))
print(line_intersection(((0, -1), (0, 1)), ((0, 0), (0, 2))))
print(line_intersection(((-1, 0), (0, 0)), ((0, 0), (-1, 0))))
print(line_intersection(((1, 0), (0, 0)), ((0, 0), (1, 0))))