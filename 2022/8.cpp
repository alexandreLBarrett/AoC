#include "../helpers.h"
#include "../common.h"

using namespace std;

// puzzle: https://adventofcode.com/2022/day/8
class Forest {
    vector<vector<int>> trees;
public:
    Forest(istream& is) {
        string s;
        while(getline(is, s)) {
            auto row = vector<int>();

            for (char c : s)
                row.push_back(c - '0');
            
            trees.push_back(row);
        }
    }

    int countVisibleTrees() const {
        vector<int> viewLeft;
        vector<int> viewRight;
        vector<int> viewTop;    
        vector<int> viewBottom;
        vector<vector<bool>> treesVisible;

        for (int i = 0; i < trees.size(); ++i) {
            vector<bool> vis(trees[i].size());
            vis[0] = true;
            vis[vis.size() - 1] = true;
            treesVisible.push_back(vis);
            viewLeft.push_back(trees[i].front());
            viewRight.push_back(trees[i].back());
        }

        treesVisible[0] = vector<bool>(trees[0].size(), true);
        treesVisible[treesVisible.size() - 1] = vector<bool>(trees[0].size(), true);
        viewTop = trees.front();
        viewBottom = trees.back();

        int height = trees.size();
        for (int y = 1; y < height - 1; ++y) {
            int width = trees[y].size();
            for (int x = 1; x < width - 1; ++x) {
                int currentLeft = trees[y][x];
                if (currentLeft > viewLeft[y]) {
                    treesVisible[y][x] = true;
                    viewLeft[y] = currentLeft;
                }

                int currentRight = trees[y][width - x - 1];
                if (currentRight > viewRight[y]) {
                    treesVisible[y][width - x - 1] = true;
                    viewRight[y] = currentRight;
                }

                int currentTop = trees[y][x];
                if (currentTop > viewTop[x]) {
                    treesVisible[y][x] = true;
                    viewTop[x] = currentTop;
                }

                int currentBottom = trees[height - y - 1][x];
                if (currentBottom > viewBottom[x]) {
                    treesVisible[height - y - 1][x] = true;
                    viewBottom[x] = currentBottom;
                }
            }
        }

        auto total = 0;
        for (int y = 0; y < height; ++y)
            for (int x = 0; x < trees[y].size(); ++x)
                if (treesVisible[y][x])
                    total++;

        return total;
    }

    int calculateBestSpotCost() const {
        int best = 0;

        int height = trees.size();
        for (int y = 1; y < height - 1; ++y) {
            int width = trees[y].size();
            for (int x = 1; x < width - 1; ++x) {
                int currentVal = trees[y][x];

                int totalLeft = 0;
                while ((x - totalLeft) > 0) {
                    totalLeft++; 
                    if (trees[y][x - totalLeft] >= currentVal) {
                        break;
                    }
                }

                int totalRight = 0;
                while((x + totalRight) < width - 1) {
                    totalRight++;
                    if (trees[y][x + totalRight] >= currentVal) {
                        break;
                    }
                }

                int totalUp = 0;
                while ((y - totalUp) > 0) {
                    totalUp++;
                    if (trees[y - totalUp][x] >= currentVal) {
                        break;
                    }
                }

                int totalDown = 0;
                while ((y + totalDown) < height - 1) {
                    totalDown++;
                    if (trees[y + totalDown][x] >= currentVal) {
                        break;
                    }
                }

                int total = totalLeft * totalRight * totalUp * totalDown;

                best = max(best, total);
            }
        }

        return best;
    }
};

int main() {
    FileParser fp("2022/8-data");

    // Parse file
    auto forest = fp.parseOne<Forest>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        auto total = forest.countVisibleTrees();
        out = [=](auto& o) {
            o << total << endl;
        };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        auto total = forest.calculateBestSpotCost();
        out = [=](auto& o) {
            o << total << endl;
        };
    });

    dph.RunAll(cout);
}