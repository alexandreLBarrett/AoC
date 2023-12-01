#include "../helpers.h"
#include "../common.h"

using namespace std;

// puzzle: https://adventofcode.com/2023/day/1

int main() {
    FileParser fp("2023/1-data");

    struct 
    // Parse file
    std::vector<std::string> lines = fp.parseRest<std::string>([](std::ifstream& ss) {
        std::string s;
        ss >> s;
        return s;
    });

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        int sum = 0;
        for (const auto& line : lines) {
            auto lowerVal = std::find_if(line.begin(), line.end(), [](char c) {
                return std::isdigit(c);
            });

            auto latterVal = std::find_if(line.rbegin(), line.rend(), [](char c) {
                return std::isdigit(c);
            });
            char a[]{*lowerVal, *latterVal, '\0'};
            sum += atoi(a);
        }

        out = [=](auto& o) { o << sum << endl; };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        int sum = 0;
        std::vector<std::string> digits{ 
            "1", "2", "3", "4", "5", "6", "7", "8", "9",
            "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
        };

        for (const auto& line : lines) {
            pair<int, int> foundFirst{std::numeric_limits<int>::max(), 0};
            pair<int, int> foundLast{0,0}; 
            for (int i = 0; i < digits.size(); ++i) {
                if (auto pos = line.find(digits[i]); pos != string::npos && pos <= foundFirst.first) {
                    foundFirst = { pos, i % 9 + 1 };
                }
                if (auto pos = line.rfind(digits[i]); pos != string::npos && pos >= foundLast.first) {
                    foundLast = { pos, i % 9 + 1 };
                }
            }

            sum += foundFirst.second * 10 + foundLast.second;
        }

        out = [=](auto& o) { o << sum << endl; };
    });

    dph.RunAll(cout);
}