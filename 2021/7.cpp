#include "../helpers.h"
#include "../common.h"

using namespace std;

class CrabSub {
public:
    CrabSub(ifstream& stream) {
        string s;
        getline(stream, s, ',');

        if (s == "")
            getline(stream, s);

        position = atoi(s.c_str());
    }

    int getPosition() const {
        return position;
    }
private:
    int position;
};


pair<int, int> getMinMax(const vector<CrabSub> subs) {
    int min = INT_MAX;
    int max = 0;
    for (const auto& sub : subs) {
        const int subPos = sub.getPosition();
        
        if (subPos < min)
            min = subPos;

        if (subPos > max)
            max = subPos;
    }

    return make_pair(min, max);
}

constexpr int calculateCost(const vector<CrabSub>& subs, int cost) {
    int calcCost = 0;

    for (const auto& sub : subs) {
        calcCost += abs(sub.getPosition() - cost);
    }

    return calcCost;
}

constexpr int calculateCost2(const vector<CrabSub>& subs, const int cost) {
    int calcCost = 0;

    for (const auto& sub : subs) {
        const int localCost = abs(sub.getPosition() - cost);

        calcCost += abs((localCost * (1 + localCost)) / 2);
    }

    return calcCost;
}

int main() {
    FileParser fp("2021/7-data");
    auto subs = fp.parseRest<CrabSub>();

    DayPartHandler dph;

    int minCost = INT_MAX;
    int minSteps = INT_MAX;
    
    dph.AddPart([=](auto& callback) mutable {
        auto[min, max] = getMinMax(subs);
        
        for (int steps = min; steps < max; steps++) {
            int cost = calculateCost(subs, steps);

            if (cost < minCost) {
                minCost = cost;
                minSteps = steps;
            }
        }

        callback = [&](ostream& o) {
            o << minSteps << " with " << minCost << " cost";
        };
    });
    
    dph.AddPart([=](auto& callback) mutable {
        auto[min, max] = getMinMax(subs);
        
        for (int steps = min; steps < max; steps++) {
            int cost = calculateCost2(subs, steps);

            if (cost < minCost) {
                minCost = cost;
                minSteps = steps;
            }
        }

        callback = [&](ostream& o) {
            o << minSteps << " with " << minCost << " cost";
        };
    });

    dph.RunAll(cout);
}