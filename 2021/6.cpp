#include "../helpers.h"
#include "../common.h"

using namespace std;

class Fish {
public:
    Fish(ifstream& stream) {
        string value = "";
        getline(stream, value, ',');
        if (value == "") {
            getline(stream, value);
        }

        repTime = atoi(value.c_str());
    }

    Fish() = default;

    friend ostream& operator<<(ostream& o, const Fish& obj) {
        return o << to_string(obj.repTime);
    }

    int getRepTime() const {
        return repTime;
    }

    bool elapse() {
        if (repTime == 0) {
            repTime = 6;
            return true;
        }

        repTime -= 1;
        return false;
    }
private:
    int repTime = 8;
};

class FishSchool {
public:
    FishSchool(const vector<Fish>& fishes) {
        for (const auto& fish : fishes)
            fishCounts[fish.getRepTime()] += 1;
    }

    void addFish(long long fishNum) {
        fishCounts[8] = fishNum;
        fishCounts[6] += fishNum;
    }

    long long rotateFish() {
        long long newFish = fishCounts[0];
        for (int i = 0; i < 9; i++) {
            fishCounts[i] = fishCounts[i + 1];
        }
        return newFish;
    }

    long long getCount() {
        long long total = 0;
        for (long long i : fishCounts)
            total += i;

        return total;
    }
public:
    long long fishCounts[9] = {0};
};

int main() {
    FileParser fp("2021/6-data");
    auto fishes = fp.parseRest<Fish>();

    DayPartHandler<ostream> dph;

    long long result;
    auto printResult = [&](ostream& o) {
        o << result;
    };

    dph.AddPart([&result, printResult, fishes](auto& printRes) mutable {
        result = 0;

        for (int i = 0; i < 80; i++) {
            vector<Fish> newBorns;
            for (auto& fish : fishes) {
                if (fish.elapse()) {
                    newBorns.push_back(Fish());
                }
            }

            for (const auto& newBorn : newBorns)
                fishes.push_back(newBorn);
        }

        result = fishes.size();

        printRes = printResult;
    });

    
    dph.AddPart([&result, printResult, fishes](auto& printRes) mutable {
        result = 0;

        FishSchool fs(fishes);

        for (int i = 0; i < 256; i++) {
            long long newFishes = fs.rotateFish();
            fs.addFish(newFishes);
        }

        result = fs.getCount();

        printRes = printResult;
    });

    dph.RunAll(cout);
}