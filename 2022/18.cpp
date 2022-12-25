#include "../helpers.h"
#include "../common.h"

#include <tuple>
#include <queue>

using namespace std;

// puzzle: https://adventofcode.com/2022/day/18

struct Drop {
    uint32_t x,y,z;
    Drop(istream& is) {
        char c;
        is >> x;
        is >> c;
        is >> y;
        is >> c;
        is >> z;
    }
};

template<int Size>
uint32_t countTotalSurfaceArea(const vector<Drop>& drops) {
    bool root[Size][Size][Size];
    uint32_t count = 0;

    for (int i = 0; i < Size; ++i) {
        for (int j = 0; j < Size; ++j) {
            for (int k = 0; k < Size; ++k) {
                root[i][j][k] = false;
            }
        }
    }
    
    for (const auto& d : drops) {
        root[d.x][d.y][d.z] = true;
    }

    for (const auto& d : drops) {
        if (!root[d.x + 1][d.y][d.z]) ++count;
        if (!root[d.x - 1][d.y][d.z]) ++count;
        if (!root[d.x][d.y + 1][d.z]) ++count;
        if (!root[d.x][d.y - 1][d.z]) ++count;
        if (!root[d.x][d.y][d.z + 1]) ++count;
        if (!root[d.x][d.y][d.z - 1]) ++count;
    }

    return count;
}

template<int Size>
uint32_t countSurfaceArea(const vector<Drop>& drops) {
    bool root[Size][Size][Size];
    bool visited[Size][Size][Size];

    for (int i = 0; i < Size; ++i) {
        for (int j = 0; j < Size; ++j) {
            for (int k = 0; k < Size; ++k) {
                root[i][j][k] = false;
                visited[i][j][k] = false;
            }
        }
    }
    
    
    for (const auto& d : drops) {
        root[d.x][d.y][d.z] = true;
    }

    using Pos = tuple<uint32_t, uint32_t, uint32_t>;
    queue<Pos> queue;
    queue.push(make_tuple(0,0,0));
    visited[0][0][0] = true;

    while (!queue.empty()) {
        auto [x,y,z] = queue.front();
        queue.pop();

        Pos neighbors[] = {
            make_tuple(x+1, y, z),
            make_tuple(x-1, y, z),
            make_tuple(x, y+1, z),
            make_tuple(x, y-1, z),
            make_tuple(x, y, z+1),
            make_tuple(x, y, z-1),
        };

        for (int i = 0; i < 6; ++i) {
            auto pos = neighbors[i];
            auto [x2,y2,z2] = pos;

            if (x2 < 0 || y2 < 0 || z2 < 0 || x2 >= Size || y2 >= Size || z2 >= Size)
                continue;

            if (!root[x2][y2][z2] && !visited[x2][y2][z2]) {
                visited[x2][y2][z2] = true;
                queue.push(pos);
            }
        }
    }

    for (const auto& pos : drops) {
        visited[pos.x][pos.y][pos.z] = true;
    }

    vector<Pos> poss;
    for (int i = 0; i < Size; ++i) {
        for (int j = 0; j < Size; ++j) {
            for (int k = 0; k < Size; ++k) {
                if (!visited[i][j][k]) {
                    poss.push_back(make_tuple(i,j,k));
                }
            }
        }
    }

    uint32_t count = 0;
    for (const auto& d : poss) {
        auto [x, y, z] = d;

        if (root[x + 1][y][z]) ++count;
        if (root[x - 1][y][z]) ++count;
        if (root[x][y + 1][z]) ++count;
        if (root[x][y - 1][z]) ++count;
        if (root[x][y][z + 1]) ++count;
        if (root[x][y][z - 1]) ++count;
    }

    // Make a diff of water and the rest
    // remove known obisidian cells
    // then same algo as 1 but with air bubbles

    return count;
}

int main() {
    FileParser fp("2022/18-data");

    // Parse file
    auto drops = fp.parseRest<Drop>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        auto val = countTotalSurfaceArea<21>(drops);
        out = [=](auto& o) {
            o << val << endl;
        };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        auto surf = countTotalSurfaceArea<21>(drops);
        auto val = countSurfaceArea<21>(drops);

        out = [=](auto& o) {
            o << surf - val << endl;
        };
    });

    dph.RunAll(cout);
}