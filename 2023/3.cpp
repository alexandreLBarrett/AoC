#include "../helpers.h"
#include "../common.h"
#include <set>
#include <unordered_set>
#include <map>
#include <numeric>
using namespace std;

using coord = std::pair<int, int>;

// puzzle: https://adventofcode.com/2023/day/3
struct Schematic {
    std::vector<int> partNumbers;
    std::map<coord, int> partNumberPositions;
    std::set<coord> symbols;
    std::set<coord> gears;

    friend istream& operator>>(istream& is, Schematic& Schematic) {
        std::string line;
        int j{};
        while (is && !is.eof()) {
            std::getline(is, line);

            std::string currentPartNum{};
            for (int i = 0; i < line.size(); ++i) {
                char c = line[i];
                if (isdigit(c)) {
                    currentPartNum += c;
                } else {
                    if (!currentPartNum.empty()) {
                        Schematic.partNumbers.push_back(stoi(currentPartNum));
                        int index = Schematic.partNumbers.size() - 1;

                        for (int k = currentPartNum.size(); k > 0; --k) {
                            Schematic.partNumberPositions[std::pair{j, i - k}] = index;
                        }

                        currentPartNum = "";
                    }

                    if (c != '.') {
                        std::pair pos{j, i};
                        Schematic.symbols.insert(pos);
                        if (c == '*')
                            Schematic.gears.insert(pos);
                    }
                }
            }

            ++j;
        }
        return is;
    }
};

int main() {
    FileParser fp("2023/3-data");

    // Parse file

    auto schematic = fp.parseOne<Schematic>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        std::unordered_set<int> idsToKeep;
        for (const auto& pos : schematic.symbols) {
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    std::pair neighbor{pos.first + i, pos.second + j};
                    if (schematic.partNumberPositions.contains(neighbor)) {
                        idsToKeep.insert(schematic.partNumberPositions[neighbor]);
                    }
                }
            }
        }

        int sum = std::accumulate(idsToKeep.begin(), idsToKeep.end(), 0, [&](auto acc, auto id) {
            return acc + schematic.partNumbers[id];
        });

        out = [=](auto& o) { o << sum << endl;};
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {

        std::vector<int> gearRatios;
        for (const auto& pos : schematic.gears) {
            std::unordered_set<int> surroundedParts;
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    std::pair neighbor{pos.first + i, pos.second + j};
                    if (schematic.partNumberPositions.contains(neighbor)) {
                        surroundedParts.insert(schematic.partNumberPositions[neighbor]);
                    }
                }
            }

            if (surroundedParts.size() == 2) {
                auto it = surroundedParts.begin();
                gearRatios.push_back(schematic.partNumbers[*it] * schematic.partNumbers[*next(it)]);
            }
        }

        int sum = std::accumulate(gearRatios.begin(), gearRatios.end(), 0);

        out = [=](auto& o) { o << sum << endl; };
    });

    dph.RunAll(cout);
}