#include "../helpers.h"
#include "../common.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <map>
#include <set>
#include <queue>

using namespace std;

class Cavern {
    struct Tile : public Point {
        char value = 0;
        double heuristic = 0;
        
        Tile() = default;

        Tile(char value)
            : value(value)
        {
        }

        Tile(const Tile& tile) {
            value = tile.value;
            heuristic = tile.heuristic;
            x = tile.x;
            y = tile.y;
        }

        void calculateHeuristic(Point endPoint) {
            heuristic = sqrt(pow(x - endPoint.x, 2) + pow(y - endPoint.y, 2));
        }

        bool operator==(const Tile& other) const {
            return x == other.x && y == other.y;
        }
    };

    int side = 0;
    vector<Tile> grid;

    bool check(int index, int offset) {
        return index + offset >= 0 
            && index + offset < grid.size() 
            && (
                index % side + offset >= 0
                && index % side + offset < side
            );
    }

    vector<Tile*> getSurroundings(int i) {
        vector<Tile*> ret;

        if (check(i - side, 0)) ret.push_back(&grid[i - side]);
        if (check(i, -1))       ret.push_back(&grid[i - 1]);
        if (check(i, 1))        ret.push_back(&grid[i + 1]);
        if (check(i + side, 0)) ret.push_back(&grid[i + side]);

        return ret;
    }
public:

    Cavern(ifstream& stream) {
        for (std::istreambuf_iterator<char> it(stream), end; it != end; ++it) {
            if (*it == '\n') continue;
            grid.push_back({ *it - '0' });   
        }

        side = sqrt(grid.size());
        for (int i = 0; i < grid.size(); i++) {
            grid[i].fromIndex(i, side);
            grid[i].calculateHeuristic({ side - 1, side - 1 });
        }
    }

    void expandBy(int factor) {
        vector<Tile> newGrid;
        const int newWidth = factor * side;
        newGrid.resize(pow(newWidth, 2));

        for (int y = 0; y < factor; y++) {
            for (int x = 0; x < factor; x++) {
                for (int i = 0; i < grid.size(); i++) {
                    int gridX = i % side;
                    int gridY = i / side;

                    int newX = x * side + gridX;
                    int newY = (y * side * newWidth) + gridY * newWidth;

                    newGrid[newY + newX] = grid[i];
                    newGrid[newY + newX].fromIndex(newY + newX, newWidth);

                    int newValue = (newGrid[newY + newX].value + x + y) - 9;
                    if (newValue < 1)
                        newGrid[newY + newX].value = newValue + 9;
                    else
                        newGrid[newY + newX].value = newValue;
                    
                }
            }      
        }
        side = newWidth;
        grid = newGrid;
    }

    vector<Tile> search() {
        map<Tile*, vector<Tile*>> tiles;

        for (int i = 0; i < grid.size(); i++) {
            tiles[&grid[i]] = getSurroundings(i);
        }
            
        Tile* start = &grid[0];
        Tile* end = &grid[grid.size() - 1];

        // Set of pairs of Tile and best cost to get there
        map<Tile*, int> visited;
        visited[start] = 0;

        // Tile of potential next nodes with cost
        map<Tile*, int> nexts;
        nexts[start] = start->value;

        while (nexts.size() > 0) {
            int minVal = INT_MAX;
            Tile* next;
            for (auto kv : nexts) {
                if (kv.second < minVal) {
                    next = kv.first;
                    minVal = kv.second;
                }
            }

            nexts.erase(next);
            visited[next] = minVal;
            
            for (auto tile : tiles[next]) {
                if (!visited.contains(tile)) {
                    if (nexts.contains(tile)) {
                        nexts[tile] = min(minVal + tile->value, nexts[tile]);
                    } else {
                        nexts[tile] = minVal + tile->value;
                    }
                }
            }
        }

        Tile* current = end;
        vector<Tile> path;
        while (current != start) {
            int lookFor = visited[current] - current->value;

            auto sibligns = tiles[current];
            for (auto sib : sibligns) {
                if (visited[sib] == lookFor) {
                    path.push_back(*current);
                    current = sib;
                    break;
                }
            }
        }

        path.push_back(*start);

        reverse(path.begin(), path.end());

        return path;
    }

    uint64_t getPathCost(const vector<Tile>& path) const {
        uint64_t total = 0;
        for (const auto& p : path) {
            total += p.value;
        }
        total -= path[0].value;
        return total;
    }

    void drawPath(ostream& out, const vector<Tile>& path) const {
        for (int i = 0; i < grid.size(); i++) {
            if (find(begin(path), end(path), grid[i]) != end(path)) {
                out << "\033[32m" << (int)grid[i].value << "\033[0m";
            } else {
                out << (int)grid[i].value;
            }

            if ((i + 1) % side == 0) 
                out << endl;
        }
        out << endl << "Total cost: " << getPathCost(path) << endl;
    }
};

int main() {
    // enables terminal color escape sequences
    #ifdef _WIN32
    SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    #endif

    FileParser fp("2021/15-data");

    // Parse file
    auto cavern = fp.parseOne<Cavern>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        auto e = cavern.search();
        out = [=](auto& o) { cavern.drawPath(o, e); };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        cavern.expandBy(5);
        auto e = cavern.search();
        out = [=](auto& o) { cavern.drawPath(o, e); };
    });

    dph.RunAll(cout);
}
