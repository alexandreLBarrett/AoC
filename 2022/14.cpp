#include "../helpers.h"
#include "../common.h"

#include <set>

using namespace std;

// puzzle: https://adventofcode.com/2022/day/14

struct WallSegment {
    vector<Point<>> points;

    Point<> parsePoint(istream& is) {
        string coord;
        Point<> p;
        getline(is, coord, ',');
        p.x = stoi(coord);
        is >> p.y;
        return p;
    }

    WallSegment() = default;
    WallSegment(istream& is) {
        string line;
        getline(is, line);

        istringstream str(line);

        points.push_back(parsePoint(str));
        while (!str.eof()) {
            string s;
            str >> s;
            points.push_back(parsePoint(str));
        }
    }

    bool intersect(const Point<>& p) const {
        for (int i = 0; i < points.size() - 1; ++i) {
            const Point<>& lower = points[i];
            const Point<>& upper = points[i + 1];

            if (p == lower || p == upper) 
                return true;

            if (upper.x == lower.x) {
                auto yDiff = upper.y - lower.y;
                auto pyDiff = upper.y - p.y;
                if (p.x == lower.x && (yDiff < 0 == pyDiff < 0) && abs(pyDiff) <= abs(yDiff)) {
                    return true;
                }
            } else if (upper.y == lower.y) {
                auto xDiff = upper.x - lower.x;
                auto pxDiff = upper.x - p.x;
                if (p.y == lower.y && (xDiff < 0 == pxDiff < 0) && abs(pxDiff) <= abs(xDiff)) {
                    return true;
                }
            }
        }

        return false;
    }
};

class RockWall {
    vector<WallSegment> segments;
    set<Point<>> sands;
    const Point<> startPoint{500,0};
    Point<> topLeft = startPoint;
    Point<> bottomRight = startPoint;
public:
    RockWall(const vector<WallSegment>& vec) : segments(vec) {
        for (const auto& seg : vec) {
            for (const auto& pt : seg.points) {
                topLeft.x = min(topLeft.x, pt.x);
                topLeft.y = min(topLeft.y, pt.y);

                bottomRight.x = max(bottomRight.x, pt.x);
                bottomRight.y = max(bottomRight.y, pt.y);
            }
        }
    }

    bool isOccupied(const Point<>& p) const {
        if (sands.contains(p))
            return true;

        for (const auto& seg : segments) {
            if (seg.intersect(p))
                return true;
        }
        return false;
    }

    bool inSim(const Point<>& p) {
        return p.x >= topLeft.x && p.y >= topLeft.y && p.x <= bottomRight.x && p.y <= bottomRight.y;
    }

    bool moveIfCan(Point<>& p) {
        int offsets[3] = {0, -1, 1};
        for (int i = 0; i < 3; ++i)  {
            Point<> next = Point<>{p.x + offsets[i], p.y+1};
            if (!isOccupied(next)) {
                p = next;
                return true;
            }
        }
        return false;
    }

    void draw(ostream& os) const {
        auto maxX = max_element(sands.begin(), sands.end(), [](const Point<>& p1, const Point<>& p2) { return p1.x < p2.x; });
        auto minX = min_element(sands.begin(), sands.end(), [](const Point<>& p1, const Point<>& p2) { return p1.x < p2.x; });

        for (int y = topLeft.y; y <= bottomRight.y + 2; ++y) {
            for (int x = minX->x - 2; x <= maxX->x + 2; ++x) {
                Point<> p{x, y};
                if (p == startPoint) {
                    os << "+";
                }
                else {
                    if (sands.contains(p)) {
                        os << 'o';
                    } else if (isOccupied(p)) {
                        os << '#';
                    } else {
                        os << '.';
                    }
                }
            }
            os << endl;
        }
    }

    uint64_t simulate() {
        Point<> p = startPoint;
        uint64_t count = 0;
        while (true) {
            p = startPoint;
            while (moveIfCan(p)) {
                if (!inSim(p)) {
                    return count;
                }
            }
            count++;
            sands.insert(p);
        }
        return -1;
    }

    uint64_t simulateToFill() {
        auto seg = WallSegment();
        seg.points.push_back(Point<>{-INT32_MAX, bottomRight.y + 2});
        seg.points.push_back(Point<>{INT32_MAX, bottomRight.y + 2});
        segments.push_back(seg);

        Point<> p = startPoint;
        uint64_t count = 0;
        while (true) {
            p = startPoint;
            while (moveIfCan(p));

            count++;
            if (p == startPoint)
                return count;
            sands.insert(p);
        }
        return -1;
    }
};

int main() {
    FileParser fp("2022/14-data");

    // Parse file
    auto wallSegments = fp.parseRest<WallSegment>();
    auto b = wallSegments[0].intersect(Point<>{497, 6});

    auto seg = WallSegment();
    seg.points.push_back(Point<>{-INT32_MAX, 0});
    seg.points.push_back(Point<>{INT32_MAX, 0});
    auto inter = seg.intersect(Point<>{0, 0});

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        RockWall wall(wallSegments);
        auto count = wall.simulate();

        out = [=](auto& o) {
            wall.draw(o);
            o << count << endl;
        };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        RockWall wall(wallSegments);
        auto count = wall.simulateToFill();
        out = [=](auto& o) {
            wall.draw(o);
            o << count << endl;
        };
    });

    dph.RunAll(cout);
}