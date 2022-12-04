#include "../helpers.h"
#include "../common.h"

#include <numeric>

using namespace std;

// puzzle: https://adventofcode.com/2022/day/4

class AssignmentPair {
    mutable pair<int32_t, int32_t> ass[2];

    pair<int32_t, int32_t> getMinMax(const string& str) {
        auto delim = str.find('-');
        return {
            stoi(str.substr(0, delim)),
            stoi(str.substr(delim + 1))
        };
    }
public:

    bool doesOverlapAll() const {
        sort(ass, ass + 2, [](const auto a, const auto b) {
            return a.second - a.first > b.second - b.first;
        });

        const auto diffA = ass[0].first - ass[1].first;
        const auto diffB = ass[0].second - ass[1].second;

        return diffA <= 0 && diffB >= 0;
    }

    bool doesOverlapAny() const {
        sort(ass, ass + 2, [](const auto a, const auto b) {
            return a.first > b.first;
        });

        return ass[0].first <= ass[1].second || ass[1].first >= ass[0].second;
    }

    AssignmentPair(istream& s) {
        string str;

        getline(s, str, ',');
        ass[0] = getMinMax(str);

        getline(s, str);
        ass[1] = getMinMax(str);
    }
};

int main() {
    FileParser fp("2022/4-data");

    // Parse file
    auto asses = fp.parseRest<AssignmentPair>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        const auto total = accumulate(asses.begin(), asses.end(), 0, [](auto total, const AssignmentPair& item) {
            if (item.doesOverlapAll()) {
                total++;
            }
            return total;
        });

        out = [=](auto& o) {
            o << total << endl;
        };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        const auto total = accumulate(asses.begin(), asses.end(), 0, [](auto total, const AssignmentPair& item) {
            if (item.doesOverlapAny()) {
                total++;
            }
            return total;
        });
        out = [=](auto& o) {
            o << total << endl;
        };
    });

    dph.RunAll(cout);
}