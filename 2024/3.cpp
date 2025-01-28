#include "../helpers.h"
#include "../common.h"

using namespace std;

// puzzle: https://adventofcode.com/2024/day/3

int main() {
    FileParser fp("2024/3-data");

    // Parse file
    std::string content = fp.getFileContent();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {

        std::regex reg {"mul\\(([0-9]+),([0-9]+)\\)"};

        auto begin = std::sregex_iterator{content.begin(), content.end(), reg};
        auto end = std::sregex_iterator{};

        int total = 0;
        for (;begin != end; ++begin) {
            std::smatch match = *begin;

            int a = std::stoi(match[1]);
            int b = std::stoi(match[2]);

            total += (a * b);
        }

        out = [=](auto& o) { o << total << endl; };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {

        std::regex mulReg {"mul\\(([0-9]+),([0-9]+)\\)"};

        auto beginI = 0;
        auto endI = content.find("don't()");

        int total = 0;

        while (beginI != std::string::npos) {
            auto view = std::string_view{ content.begin() + beginI, content.begin() + endI };
            auto begin = std::regex_iterator<std::string_view::iterator>{view.begin(), view.end(), mulReg};
            auto end = std::regex_iterator<std::string_view::iterator>{};

            for (;begin != end; ++begin) {
                std::match_results<std::string_view::iterator> match = *begin;

                int a = std::stoi(match[1]);
                int b = std::stoi(match[2]);

                total += (a * b);
            }

            beginI = content.find("do()", endI);
            endI = std::min(content.find("don't()", beginI), content.size());
        };

        out = [=](auto& o) { o << total << endl; };
    });

    dph.RunAll(cout);
}