#include "../helpers.h"
#include "../common.h"
#include <string>
#include <numeric>

using namespace std;

class Calories {
    int total = 0;

public:
    Calories(istream& o) {
        string token;
        getline(o, token);

        while (token.size() != 0 && !o.eof()) {
            total += stoi(token);
            getline(o, token);
        }
    }

    int getTotal() const {
        return total;
    }

    friend ostream& operator<<(ostream& os, const Calories& cal) {
        os << "total: "<< cal.total << endl;
        return os;
    }
};

int main() {
    FileParser fp("2022/1-data");
    
    auto cals = fp.parseRest<Calories>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        auto maxCals = max_element(cals.begin(), cals.end(), [](const auto& a, const auto& b) {
            return a.getTotal() < b.getTotal();
        });

        out = [=](auto& o) {
            o << maxCals->getTotal() << endl;
        };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        sort(cals.begin(), cals.end(), [](const auto& a, const auto& b) {
            return a.getTotal() > b.getTotal();
        });

        const auto total = reduce(cals.begin(), cals.begin() + 3, 0, [](const auto& a, const auto& b) -> int {
            return a + b.getTotal();
        });

        out = [=](auto& o) {
            

            o << total << endl;
        };
    });

    dph.RunAll(cout);
}