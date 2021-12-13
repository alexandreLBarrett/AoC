#include "../helpers.h"
#include "../common.h"

#include <numeric>

using namespace std;

const size_t GRID_SIZE = 10;

class OctopusGrid {
    vector<size_t> grid;

    int getGridIndex(int32_t x, int32_t y) {
        if (x < 0 || x >= GRID_SIZE || y < 0 || y >= GRID_SIZE)
            return -1;
        
        return y * GRID_SIZE + x;
    }
    
    static int flashCount;
public:
    OctopusGrid(ifstream& stream) {
        for (std::istreambuf_iterator<char> it(stream), end; it != end; ++it) {
            if (*it == '\n') continue;
            grid.push_back(*it - '0');   
        }
    }

    void impactEnvironant(int32_t x, int32_t y) {
        int index = getGridIndex(x, y);
        
        if (index == -1) return;
        if (grid[index] == 10) return;

        grid[index] += 1;

        if (grid[index] == 10) {
            for (auto offX : {0, -1, 1}) {
                for (auto offY : {0, -1, 1}) {
                    if (offX == 0 && offY == 0) continue;

                    impactEnvironant(x + offX, y + offY);
                }
            }
        }
    }

    void reset(int32_t x, int32_t y) {
        int index = getGridIndex(x, y);
        
        if (index == -1) return;
        if (grid[index] != 10) return;

        grid[index] = 0;
        flashCount++;
    }

    bool isAllFlash() {
        return accumulate(begin(grid), end(grid), 0) == 0;
    }

    void dump() {
        for (int i = 0; i < grid.size(); i++) {
            if (i % GRID_SIZE == 0)
                cout << endl;

            cout << grid[i];            
        }

        cout << endl;
    }

    int getFlashCount() const { return flashCount; }
};

int OctopusGrid::flashCount = 0;

int main() {
    FileParser fp("2021/11-data");

    auto octopusGrid = fp.parseOne<OctopusGrid>();

    DayPartHandler<ostream> dph;

    dph.AddPart([=](auto& out) mutable {
        for (int step = 0; step < 100; step++) {
            for (int32_t i = 0; i < GRID_SIZE; i++) {
                for (int32_t j = 0; j < GRID_SIZE; j++) {
                    octopusGrid.impactEnvironant(j, i);
                }   
            }

            for (int32_t i = 0; i < GRID_SIZE; i++) {
                for (int32_t j = 0; j < GRID_SIZE; j++) {
                    octopusGrid.reset(j, i);
                }
            }
        }

        out = [=](auto& o) { o << octopusGrid.getFlashCount() << endl; };
    });

    dph.AddPart([=](auto& out) mutable {
        int step;
        for (step = 0; !octopusGrid.isAllFlash(); step++) {
            for (int32_t i = 0; i < GRID_SIZE; i++) {
                for (int32_t j = 0; j < GRID_SIZE; j++) {
                    octopusGrid.impactEnvironant(j, i);
                }
            }

            for (int32_t i = 0; i < GRID_SIZE; i++) {
                for (int32_t j = 0; j < GRID_SIZE; j++) {
                    octopusGrid.reset(j, i);
                }
            }
        }

        out = [=](auto& o) { o << step << endl; };
    });

    dph.RunAll(cout);
}
