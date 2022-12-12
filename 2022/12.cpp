#include "../helpers.h"
#include "../common.h"

using namespace std;

// puzzle: https://adventofcode.com/2022/day/12

class Terrain {
    vector<vector<uint8_t>> heightmap;
    Point<> S;
    Point<> E;

    vector<Point<>> traverse(Point<> from, Point<> to, vector<vector<uint8_t>> marked) {
        uint8_t fromHeight = heightmap[from.y][from.x];
        uint8_t toHeight = heightmap[to.y][to.x];

        int8_t heightDiff = toHeight - fromHeight;
    }
public:
    Terrain(istream& is) {
        string s;
        int i = 0;
        while (getline(is, s)) {
            heightmap.emplace_back();
            for (int j = 0; j < s.length(); ++j) {
                char c = s[j];

                uint8_t height = 0;
                switch(c) {
                    case 'S': 
                        S = Point<>(j, i);
                        break;
                    case 'E': 
                        E = Point<>(j, i);
                        height = 'z' - 'a';
                        break;
                    default:
                        height = c - 'a';
                }

                heightmap[i].push_back(height);
            }
            ++i;
        }
    }

    void traverse() {

    }
};

int main() {
    FileParser fp("2022/12-data-test");

    // Parse file
    auto terrain = fp.parseOne<Terrain>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        terrain.traverse();
        out = [=](auto& o) {};
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {

        out = [=](auto& o) {};
    });

    dph.RunAll(cout);
}