#include "../helpers.h"
#include "../common.h"

using namespace std;

// puzzle: https://adventofcode.com/2024/day/2

int main() {
    FileParser fp("2024/2-data");

    // Parse file
    auto entries = fp.parseRest<std::vector<int>>();

    DayPartHandler<ostream> dph;

    static auto validateOrder = [](bool isDescending, int a, int b) -> bool {
        return isDescending ? a > b : b > a;
    };

    static auto validateDiff = [](int a, int b) -> bool {
        int diff = std::abs(a - b);
        return diff >= 1 && diff <= 3;
    };

    static auto validateRow = [](const std::vector<int>& data) -> bool {
        bool valid = true;
        bool descending = data[0] > data[1];
        for (int i = 0; i < data.size() - 1 && valid; ++i) {
            valid &= validateOrder(descending, data[i], data[i + 1]);
            valid &= validateDiff(data[i], data[i + 1]);
        }

        return valid;
    };

    static auto validateRowRelaxed = [](std::vector<int> data) -> bool {
           
        for (int hiddenI = 0; hiddenI < data.size(); hiddenI++) {
            auto getTrueId = [&](int i) {
                return i >= hiddenI ? i + 1 : i;
            };     

            bool valid = true;
            bool descending = data[getTrueId(0)] > data[getTrueId(1)];

            for (int i = 0; i < data.size() - 2 && valid; ++i) {
                valid &= validateOrder(descending, data[getTrueId(i)], data[getTrueId(i + 1)]);
                valid &= validateDiff(data[getTrueId(i)], data[getTrueId(i + 1)]);
            }

            if (valid)
                return true;
        }

        return false;
    };

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        int totalValid = 0;

        for (auto& data : entries) {
            if (validateRow(data))
                totalValid++;
        }

        out = [=](auto& o) { o << totalValid << endl; };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        int totalValid = 0;

        for (auto& data : entries) {
            if (validateRow(data) || validateRowRelaxed(data))
                totalValid++;
        }

        out = [=](auto& o) { o << totalValid << endl; };
    });

    dph.RunAll(cout);
}