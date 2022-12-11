#include "../helpers.h"
#include "../common.h"

#include <regex>
#include <functional>

using namespace std;

// puzzle: https://adventofcode.com/2022/day/11

class Monkey {
    int id;
    vector<int> items;
    function<void(int&)> operation;
    int divisibleBy;
    int monkeyIfTrue;
    int monkeyIfFalse;
public:
    Monkey(istream& is) {
        string s, statement;
        getline(is, s);

        while (s != "") {
            statement += s;
            getline(is, s);
        }

        regex rgx("Monkey ([0-9]):  Starting items: ((?:[0-9]+,? ?)+)  Operation: new = old ([*-+/]) ([0-9]+)  Test: divisible by ([0-9]+)    If true: throw to monkey ([0-9]+)    If false: throw to monkey ([0-9]+)");
        smatch matches;

        if (regex_search(statement, matches, rgx)) {
            id = stoi(matches[1].str());

            string itemCSV = matches[2].str();

            char op = matches[3].str()[0];
            int value = stoi(matches[4].str());
            operation = [this, op, value](int& item) {
                switch(op) {
                    case '*': item *= value; break;
                    case '-': item -= value; break;
                    case '+': item += value; break;
                    case '/': item /= value; break;
                }
            };

            divisibleBy = stoi(matches[5].str());
            monkeyIfTrue = stoi(matches[6].str());
            monkeyIfFalse = stoi(matches[7].str());
        }
    }
};

class MonkeyCircle {
    vector<Monkey> monkeys;
public:
    MonkeyCircle(istream& is) {
        while (is) {
            monkeys.emplace_back(is);
        }
    }
};

int main() {
    FileParser fp("2022/11-data-test");

    // Parse file
    auto mc = fp.parseOne<MonkeyCircle>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {

        out = [=](auto& o) {};
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {

        out = [=](auto& o) {};
    });

    dph.RunAll(cout);
}