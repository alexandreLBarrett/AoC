#include "../helpers.h"
#include "../common.h"

#include <stack>
#include <numeric>

using namespace std;

// puzzle: https://adventofcode.com/2022/day/5

struct Instruction {
    int count;
    int origin;
    int destination;

    Instruction(istream& is) {
        string garbage;
        is >> garbage; // move
        is >> count;
        is >> garbage; // from
        is >> origin;
        is >> garbage; // to
        is >> destination;
    }
};

class Dock {
    vector<stack<char>> dock;
public:
    Dock(istream& is) {
        vector<string> rows;

        string parse;
        while (getline(is, parse) && parse.length() != 0) {
            rows.push_back(parse);
        }

        auto count = rows.back().length() / 4 + 1;
        rows.pop_back();

        dock = vector<stack<char>>(count);

        for_each(rows.rbegin(), rows.rend(), [this, count](const auto& val) {
            const int baseOffset = 1;
            for (int i = 0; i < count; ++i) {
                char crateNum = val[baseOffset + (i * 4)];
                if (crateNum == ' ')
                    continue;

                dock[i].push(crateNum);
            }
        });
    }

    void runInstruction(const Instruction& instr) {
        for (int i = 0; i < instr.count; ++i) {
            auto val = dock[instr.origin - 1].top();
            dock[instr.origin - 1].pop();
            dock[instr.destination - 1].push(val);
        }
    }

    void runInstructionStable(const Instruction& instr) {
        vector<char> temp;
        for (int i = 0; i < instr.count; ++i) {
            temp.push_back(dock[instr.origin - 1].top());
            dock[instr.origin - 1].pop();
        }

        for_each(temp.rbegin(), temp.rend(), [&](char c) {
            dock[instr.destination - 1].push(c);
        });
    }

    string getTops() const {
        string s;
        for (const auto& stack : dock)
            s += stack.top();
        return s;
    }
};

int main() {
    FileParser fp("2022/5-data");

    // Parse file
    auto dock = fp.parseOne<Dock>();
    auto instrs = fp.parseRest<Instruction>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        for (const auto& instr : instrs)
            dock.runInstruction(instr);

        out = [=](auto& o) {
            o << dock.getTops() << endl;
        };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        for (const auto& instr : instrs)
            dock.runInstructionStable(instr);

        out = [=](auto& o) {
            o << dock.getTops() << endl;
        };
    });

    dph.RunAll(cout);
}