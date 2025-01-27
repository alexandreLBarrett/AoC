#include "../helpers.h"
#include "../common.h"

using namespace std;

// puzzle: https://adventofcode.com/2024/day/1

int main() {
    FileParser fp("2024/1-data");

    // Parse file

    auto pairs = fp.parseRest<std::pair<int, int>>();
    
    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {

        std::vector<int> firstColumn(pairs.size());
        std::vector<int> secondColumn(pairs.size());

        for (auto&& [i, p] : std::views::enumerate(pairs)) {
            firstColumn[i] = p.first;
            secondColumn[i] = p.second;
        }

        std::ranges::sort(firstColumn);
        std::ranges::sort(secondColumn);

        auto diffs = std::views::zip(firstColumn, secondColumn) 
            | std::views::transform([](const auto& val) -> int {
                return std::abs(std::get<0>(val) - std::get<1>(val));
            });
        int total = std::accumulate(diffs.begin(), diffs.end(), 0);

        out = [=](auto& o) { o << total << std::endl; };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {

        std::vector<int> firstColumn(pairs.size());
        std::vector<int> secondColumn(pairs.size());

        for (auto&& [i, p] : std::views::enumerate(pairs)) {
            firstColumn[i] = p.first;
            secondColumn[i] = p.second;
        }

        std::map<int, int> countTable;

        for (auto& val : secondColumn)
            countTable[val]++;

        auto temp = firstColumn | std::views::transform([&](int val) {
            return val * countTable[val];
        });

        int total = std::accumulate(temp.begin(), temp.end(), 0);

        out = [=](auto& o) { o << total << endl; };
    });

    dph.RunAll(cout);
}