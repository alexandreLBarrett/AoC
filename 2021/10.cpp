#include "../helpers.h"
#include "../common.h"

#include <map>
#include <set>
#include <stack>

using namespace std;


typedef unsigned long long int size_tt;

class InstructionLine {
    string line;

    static map<char, int> costMap;
    static map<char, size_t> ptsMap;
    static map<char, char> charPairs;

    static const set<char> opens;

    stack<char> counts;
public:
    InstructionLine(ifstream& stream) {
        getline(stream, line);
    }

    char getWrongChar() {
        for (auto c : line) {
            if (opens.count(c) > 0 ) {
                counts.push(c);
            } else {
                char top = counts.top();
                if (charPairs[c] != top)
                    return c; 
                counts.pop();
            }
        }

        return 0;
    }

    static int getCost(char c) {
        if (costMap.count(c) == 0) {
            return 0;
        }
        return costMap[c];
    }

    size_t getFillInCost() {
        string corruptedLine = "";

        size_t cost = 0;
        while (counts.size() > 0) {
            char c = counts.top();
            
            cost *= 5;
            cost += ptsMap[c];

            corruptedLine += charPairs[c];
            counts.pop();
        };

        return cost;
    }
};

map<char, int> InstructionLine::costMap{
    {')', 3},
    {']', 57},
    {'}', 1197},
    {'>', 25137},
};

map<char, size_t> InstructionLine::ptsMap{
    {'(', 1},
    {'[', 2},
    {'{', 3},
    {'<', 4},
};

map<char, char> InstructionLine::charPairs{
    {')', '('},
    {'}', '{'},
    {']', '['},
    {'>', '<'},

    {'(', ')'},
    {'{', '}'},
    {'[', ']'},
    {'<', '>'},
};

const set<char> InstructionLine::opens{
    '(', '{', '[', '<'
};

int main() {
    FileParser fp("2021/10-data");
    auto lines = fp.parseRest<InstructionLine>();

    DayPartHandler<ostream> dph;

    dph.AddPart([=](auto& o) mutable {
        int total = 0;

        for (auto& line : lines) {
            total += InstructionLine::getCost(line.getWrongChar());
        }

        o = [=](auto& out){ out << total; };
    });

    dph.AddPart([=](auto& o) mutable {
        vector<InstructionLine> cleanLines;

        for (auto& line : lines) {
            if (line.getWrongChar() == 0) {
                cleanLines.push_back(line);
            }
        }

        vector<size_t> costs;
        for (auto& line : cleanLines) {
            costs.push_back(line.getFillInCost());
        }

        sort(begin(costs), end(costs));

        o = [=](auto& out) {
            out << costs[costs.size() / 2]; 
        };
    });

    dph.RunAll(cout);
}
