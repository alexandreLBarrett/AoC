#include "../helpers.h"
#include "../common.h"

#include <queue>
#include <algorithm>
#include <map>
#include <set>

using namespace std;

// puzzle: https://adventofcode.com/2022/day/12

class Terrain {
    using TerrainPoint = Point<uint8_t>;

    vector<vector<TerrainPoint>> heightmap;
    TerrainPoint* S;
    TerrainPoint* E;
    vector<TerrainPoint*> lowest;
    uint32_t width = 0;
    uint32_t height = 0;

public:
    Terrain& operator=(const Terrain& other) {
        heightmap = other.heightmap;
        S = &heightmap[other.S->y][other.S->x];
        E = &heightmap[other.E->y][other.E->x];
        width = other.width;
        height = other.height;

        for (int y = 0; y < height; ++y)
            lowest.push_back(&heightmap[y][0]);
        return *this;
    }

    Terrain(const Terrain& other) {
        *this = other;
    }

    Terrain(istream& is) {
        string s;
        int i = 0;
        TerrainPoint ps;
        TerrainPoint pe;

        while (getline(is, s)) {
            heightmap.emplace_back();
            for (int j = 0; j < s.length(); ++j) {
                char c = s[j];
                
                switch(c) {
                    case 'S': 
                        heightmap[i].push_back(TerrainPoint(j, i, 0));
                        ps = heightmap[i][j];
                        break;
                    case 'E': 
                        heightmap[i].push_back(TerrainPoint(j, i, 25));
                        pe = heightmap[i][j];
                        break;
                    default:
                        heightmap[i].push_back(TerrainPoint(j, i, c - 'a'));
                }
            }
            ++i;
        }

        S = &heightmap[ps.y][ps.x];
        E = &heightmap[pe.y][pe.x];

        height = i;
        width = heightmap[i - 1].size();

        for (int y = 0; y < height; ++y)
            lowest.push_back(&heightmap[y][0]);
    }

    void addPoint(const set<const TerrainPoint*>& visited, vector<const TerrainPoint*>& nexts, const TerrainPoint* current, const TerrainPoint* next) const {
        if (visited.contains(next)) return;
        if (next->value > current->value + 1) return;

        nexts.push_back(next);
    }

    int traverse() {
        return traverse(S);
    }

    int traverse(TerrainPoint* startPt) const {
        set<const TerrainPoint*> visited;
        map<const TerrainPoint*, const TerrainPoint*> parents;
        queue<const TerrainPoint*> next;
        next.push(startPt);
        visited.insert(startPt);

        while (!next.empty()) {
            const TerrainPoint& p = *next.front();
            next.pop();

            vector<const TerrainPoint*> nextDirs;
            if (p.x - 1 >= 0)        addPoint(visited, nextDirs, &p, &heightmap[p.y][p.x - 1]);
            if (p.x + 1 < width)     addPoint(visited, nextDirs, &p, &heightmap[p.y][p.x + 1]);
            if (p.y - 1 >= 0)        addPoint(visited, nextDirs, &p, &heightmap[p.y - 1][p.x]);
            if (p.y + 1 < height)    addPoint(visited, nextDirs, &p, &heightmap[p.y + 1][p.x]);

            for (auto* dir : nextDirs) {
                parents[dir] = &p;
                visited.insert(dir);
                next.push(dir);

                if (dir == E) {
                    while (next.size() > 0) next.pop();
                }
            }
        }

        const TerrainPoint* current = E;
        int count = 0;
        while (current != startPt) {
            count++;
            if (!parents.contains(current))
                return 0;

            current = parents[current];
        }

        return count;
    }

    int bestPath() {
        int m = INT_MAX;
        for (TerrainPoint* low : lowest) {
            m = min(traverse(low), m);
        }
        return m;
    }
};

int main() {
    FileParser fp("2022/12-data");

    // Parse file
    auto terrain = fp.parseOne<Terrain>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        auto pathLen = terrain.traverse();
        out = [=](auto& o) {
            o << pathLen << endl;
        };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        auto bestPath = terrain.bestPath();
        out = [=](auto& o) {
            o << bestPath << endl;
        };
    });

    dph.RunAll(cout);
}