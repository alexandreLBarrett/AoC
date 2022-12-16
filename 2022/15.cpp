#include "../helpers.h"
#include "../common.h"

using namespace std;

// puzzle: https://adventofcode.com/2022/day/15

uint64_t manhattanDistance(const Point<>& p1, const Point<>& p2) {
    return abs(p1.y - p2.y) + abs(p1.x - p2.x);
}

struct Pairing {
    Point<> sensor;
    Point<> beacon;
    int distance;

    Pairing() = default;
    Pairing(istream& is) {
        string s;
        getline(is, s, '=');
        is >> sensor.x;
        getline(is, s, '=');
        is >> sensor.y;
        getline(is, s, '=');
        is >> beacon.x;
        getline(is, s, '=');
        is >> beacon.y;
        distance = manhattanDistance(beacon, sensor);
    }

    int getMinX() const {
        return min(beacon.x, sensor.x - distance);
    }

    int getMaxX() const {
        return max(beacon.x, sensor.x + distance);
    }
};

class CaveSystem {
    vector<Pairing> pairings;
    int minX = 0, maxX = 0;
public:
    CaveSystem(const vector<Pairing>& pairings) : pairings(pairings) {
        for (auto& pair : pairings) {
            minX = min(pair.getMinX(), minX);
            maxX = max(pair.getMaxX(), maxX);
        }
    }

    bool isReachedByBeacon(const Point<>& pt, bool checkIfBeacon = false) {
        for (const auto& pair : pairings) {
            if (checkIfBeacon && pt == pair.beacon)
                return false;

            if (manhattanDistance(pt, pair.sensor) <= pair.distance) {
                return true;
            }
        }
        return false;
    }

    int countPossibleSpotsInLine(int line) {
        int count = 0;
        for (int x = minX; x < maxX; ++x) {
            auto pt = Point<>(x, line);

            if (isReachedByBeacon(pt, true)) {
                count++;
            }
        }
        return count;
    }

    vector<Point<>> getIntersections(const Pairing& pair1, const Pairing& pair2) {
        vector<Point<>> intersects;

        //  2
        // 1x3
        //  4
        Point<> p1[4] = {};
        p1[0] = p1[1] = p1[2] = p1[3] = pair1.sensor;
        p1[0].x -= pair1.distance;
        p1[1].y -= pair1.distance;
        p1[2].x += pair1.distance;
        p1[3].y += pair1.distance;

        Point<> p2[4] = {};
        p2[0] = p2[1] = p2[2] = p2[3] = pair2.sensor;
        p2[0].x -= pair2.distance;
        p2[1].y -= pair2.distance;
        p2[2].x += pair2.distance;
        p2[3].y += pair2.distance;

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                Point<>& A = p1[i];
                Point<>& B = p1[(i+1) % 4];
                Point<>& C = p2[j];
                Point<>& D = p2[(j+1) % 4];
                auto x1 = A.x, x2 = B.x, x3 = C.x, x4 = D.x;
                auto y1 = A.y, y2 = B.y, y3 = C.y, y4 = D.y;

                auto x1x2 = x1 - x2, y1y2 = y1 - y2,
                     x1x3 = x1 - x3, y1y3 = y1 - y3,
                     x3x4 = x3 - x4, y3y4 = y3 - y4;
                
                auto d =  x1x2 * y3y4 - y1y2 * x3x4;

                // parallel or coincident
                if (d == 0) continue;
                
                auto t = (x1x3 * y3y4 - y1y3 * x3x4) / (float)d,
                        u = -(x1x2 * y1y3 - y1y2 * x1x3) / (float)d;
                
                auto ist = t >= 0 && t <= 1,
                        isu = u >= 0 && u <= 1;
                
                if (isu && ist) {
                    auto x = x1 + t * (x2 - x1);
                    auto y = y1 + t * (y2 - y1);

                    Point<> inter1(ceil(x), ceil(y));
                    Point<> inter2(ceil(x), floor(y));
                    Point<> inter3(floor(x), ceil(y));
                    Point<> inter4(floor(x), floor(y));

                    cout << inter1 << inter2 << inter3 << inter4 << endl;
                }
            }
        }

        return intersects;
    }

    int findBeaconWithBounds(int min, int max) {
        for (int i = 0; i < pairings.size(); ++i) {
            for (int j = i + 1; j < pairings.size(); ++j) {
                
            }   
        }

        int count = 0;
        for (int y = min; y <= max; ++y) {
            for (int x = min; x <= max; ++x) {
                auto pt = Point<>(x, y);

                // if (!isReachedByBeacon(pt)) {
                //     cout << pt << endl;
                // }
            }
        }
        return count;
    }
};

int main() {
    FileParser fp("2022/15-data-test");

    // Parse file
    auto pairings = fp.parseRest<Pairing>();

    CaveSystem system(pairings);

    Pairing p1;
    p1.sensor = Point<>(0, 0);
    p1.distance = 10;
    Pairing p2;
    p2.sensor = Point<>(5, 0);
    p2.distance = 10;
    system.getIntersections(p1, p2);

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        CaveSystem system(pairings);
        auto total = system.countPossibleSpotsInLine(2000000);
        out = [=](auto& o) {
            o << total << endl;
        };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        CaveSystem system(pairings);
        //auto total = system.findBeaconWithBounds(0, 4000000);
        out = [=](auto& o) {};
    });

    dph.RunAll(cout);
}