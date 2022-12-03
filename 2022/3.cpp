#include "../helpers.h"
#include "../common.h"

#include <unordered_set>
#include <numeric>
#include <array>

using namespace std;

// puzzle: https://adventofcode.com/2022/day/3

class Rucksack {
    string rightPocket;
    string leftPocket;
public: 
    static uint32_t getCost(char c) {
        // 'A' = 00100001
        // 'a' = 00110001
        if ((c & 0b100000) == 0) {
            return c - 'A' + 27;
        } else {
            return c - 'a' + 1;
        }
    }

    char getMismatchedItem() const {
        unordered_set<char> present;
        for (char c : leftPocket) {
            present.insert(c);
        }

        for (char c : rightPocket) {
            if (present.contains(c)) {
                return c;
            }
        }

        return -1;
    }

    string getFull() {
        return leftPocket + rightPocket;
    }

    Rucksack(istream& stream) {
        string s;
        getline(stream, s);

        uint32_t halfSize = s.size() / 2;

        leftPocket = s.substr(0, halfSize);
        rightPocket = s.substr(halfSize);
    }
};

class ElfGroup {
    string elfs[3];
public:
    ElfGroup(array<Rucksack, 3> rucks) {
        for (int i = 0; i < 3; i++)
            elfs[i] = rucks[i].getFull();
    };

    char getCommonType() const {
        unordered_set<char> present;
        for (char c : elfs[0]) {
            present.insert(c);
        }

        unordered_set<char> present2;
        for (char c : elfs[1]) {
            if (present.contains(c)) {
                present2.insert(c);
            }
        }

        for (char c : elfs[2]) {
            if (present2.contains(c)) {
                return c;
            }
        }

        return -1;
    }
};

int main() {
    FileParser fp("2022/3-data");

    auto rucksacks = fp.parseRest<Rucksack>();

    vector<ElfGroup> groups;
    for (int i = 0; i < rucksacks.size(); i += 3) {
        groups.emplace_back(ElfGroup{{ rucksacks[i], rucksacks[i + 1], rucksacks[i + 2] }});
    }

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        auto total = accumulate(rucksacks.begin(), rucksacks.end(), 0, [](const int acc, const auto& ruck) {
            return acc + Rucksack::getCost(ruck.getMismatchedItem());
        });

        out = [=](auto& o) {
            o << total << endl;
        };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        auto total = accumulate(groups.begin(), groups.end(), 0, [](const int acc, const auto& grp) {
            return acc + Rucksack::getCost(grp.getCommonType());
        });

        out = [=](auto& o) {
            o << total << endl;
        };
    });

    dph.RunAll(cout);
}