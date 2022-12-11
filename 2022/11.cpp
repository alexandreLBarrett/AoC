#include "../helpers.h"
#include "../common.h"

#include <regex>
#include <functional>
#include <queue>
#include <numeric>

#include "../double_integer.hpp"

using namespace std;

// puzzle: https://adventofcode.com/2022/day/11

using uint128_t = double_integer<uint64_t, uint64_t>;
using uint256_t = double_integer<uint128_t, uint128_t>;
using WorryLevel = double_integer<uint256_t, uint256_t>;

struct Throw {
    WorryLevel worryValue;
    int targetMonkey;

    Throw() {}
};
class Monkey {
    queue<WorryLevel> items;
    function<void(WorryLevel&)> operation;
    int divisibleBy;
    int monkeyIfTrue;
    int monkeyIfFalse;
public:
    Monkey(istream& is) {
        string s, statement;
        while (getline(is, s) && s != "") {
            statement += s;
        }

        regex rgx("Monkey [0-9]:  Starting items: ((?:[0-9]+,? ?)+)  Operation: new = old ([*-+/]) (\\w+)  Test: divisible by ([0-9]+)    If true: throw to monkey ([0-9]+)    If false: throw to monkey ([0-9]+)");
        smatch matches;

        if (regex_search(statement, matches, rgx)) {
            string rest = matches[1].str();
            const string sep = ", ";
            int pos = -sep.size();
            do {
                pos += sep.size();
                rest = rest.substr(pos);
                pos = rest.find(sep);
                string num = rest.substr(0, pos);
                items.push(stoi(num));
            } while (pos != string::npos);

            char op = matches[2].str()[0];
            string value = matches[3].str();
            operation = [this, op, value](WorryLevel& item) {
                WorryLevel opValue = value == "old" ? item : WorryLevel(stoi(value));
                switch(op) {
                    case '*': item *= opValue; break;
                    case '-': item -= opValue; break;
                    case '+': item += opValue; break;
                    case '/': item /= opValue; break;
                }
            };

            divisibleBy = stoi(matches[4].str());
            monkeyIfTrue = stoi(matches[5].str());
            monkeyIfFalse = stoi(matches[6].str());
        }
    }

    bool isEmptyHanded() const {
        return items.size() == 0;
    }

    void catchItem(WorryLevel item) {
        items.push(item);
    }

    Throw inspect(bool worryDepr) {
        WorryLevel worry = items.front();
        items.pop();

        operation(worry);
        if (worryDepr)
            worry /= 3;

        Throw t{};
        t.worryValue = worry;
        worry %= divisibleBy;

        t.targetMonkey =  worry == WorryLevel(0) ? monkeyIfTrue : monkeyIfFalse;

        return t;
    }
};

class MonkeyCircle {
    vector<Monkey> monkeys;
    vector<uint64_t> monkeyTurns;
public:
    MonkeyCircle(istream& is) {
        while (is) {
            monkeys.emplace_back(is);
            monkeyTurns.push_back(0);
        }
    }

    void inspectMonkeys(int rounds, bool worryDepr) {
        for (int j = 0; j < rounds; ++j) {
            for (int i = 0; i < monkeys.size(); ++i) {
                Monkey& monkey = monkeys[i];
                while (!monkey.isEmptyHanded()) {
                    auto throwTo = monkey.inspect(worryDepr);
                    monkeyTurns[i]++;
                    monkeys[throwTo.targetMonkey].catchItem(throwTo.worryValue);
                }
            }
        }
    }

    WorryLevel getWorryMult() {
        sort(monkeyTurns.begin(), monkeyTurns.end(), greater{});

        return monkeyTurns[0] * monkeyTurns[1];
    }
};

template<class T>
ostream& print(ostream& o, double_integer<T, T> i) {
    return o << print(o, i.hi) << print(o, i.lo);
}

ostream& print(ostream& o, uint64_t i) {
    return o << i;
}

int main() {
    FileParser fp("2022/11-data-test");

    // Parse file
    auto mc = fp.parseOne<MonkeyCircle>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        mc.inspectMonkeys(20, true);
        WorryLevel mult = mc.getWorryMult();
        out = [=](auto& o) {
            //o << print(o, mult) << endl;
        };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        mc.inspectMonkeys(10000, false);
        WorryLevel mult = mc.getWorryMult();
        out = [=](auto& o) {
            //o << print(o, mult) << endl;
        };
    });

    dph.RunAll(cout);
}