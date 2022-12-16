#include "../helpers.h"
#include "../common.h"

#include <numeric>

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
        for (const auto& pair : pairings) {
            minX = min(pair.getMinX(), minX);
            maxX = max(pair.getMaxX(), maxX);
        }
    }

    bool isReachedByBeacon(const vector<Pairing>& pairings, const Point<>& pt, const bool checkIfBeacon = false) {
        for (const auto& pair : pairings) {
            if (checkIfBeacon && pt == pair.beacon)
                return false;

            if (manhattanDistance(pt, pair.sensor) <= pair.distance) {
                return true;
            }
        }
        return false;
    }

    int countPossibleSpotsInLine(const int line) {
        vector<Pairing> pairs;

        for (const auto& pair : pairings) {
            if (abs(pair.sensor.y - line) <= pair.distance) {
                pairs.push_back(pair);
            }
        }

        int count = 0;
        for (int x = minX; x <= maxX; ++x) {
            if (isReachedByBeacon(pairs, {x, line}, true)) {
                count++;
            }
        }
        return count;
    }

    int64_t findBeaconWithBounds(int64_t min, int64_t max) {
        int count = 0;
        for (int64_t y = min; y <= max; ++y) {
            vector<Pairing> pairs;
            //cout << "ligne " << y << " => ";

            vector<Point<>> segments;
            for (const auto& pair : pairings) {
                const auto dist = pair.distance - abs(pair.sensor.y - y);
                if (dist >= 0) {
                    segments.emplace_back(pair.sensor.x - dist, pair.sensor.x + dist);
                }
            }

            sort(segments.begin(), segments.end(), [](const Point<>& p1, const Point<>& p2) {
                return p1.x < p2.x;
            });

            vector<Point<>> concatSegments = { segments[0] };
            for (int i = 1; i < segments.size(); ++i) {
                auto& seg = segments[i];

                bool found = false;
                for (int j = 0; j < concatSegments.size(); ++j) {
                    auto& compSeg = concatSegments[j];

                    if (seg.x <= compSeg.y + 1) {
                        compSeg.y = std::max(seg.y, compSeg.y);
                        found = true;
                    }
                }

                if (!found)
                    concatSegments.push_back(seg);
            }

            for (int i = 0; i < concatSegments.size() - 1; ++i) {
                auto& seg1 = concatSegments[i];
                auto& seg2 = concatSegments[i+1];

                for (int64_t x = seg1.y + 1; x < seg2.x; ++x) {
                    cout << x * 4000000 + y << endl;
                }
            }
        }
        return count;
    }
};

int main() {
    FileParser fp("2022/15-data");

    // Parse file
    auto pairings = fp.parseRest<Pairing>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        CaveSystem system(pairings);
        auto total = system.countPossibleSpotsInLine(10 /*2000000*/);
        out = [=](auto& o) {
            o << total << endl;
        };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        CaveSystem system(pairings);
        auto total = system.findBeaconWithBounds(0, 4000000);
        out = [=](auto& o) {
            o << total << endl;
        };
    });

    dph.RunAll(cout);
}