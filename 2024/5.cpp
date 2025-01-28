#include "../helpers.h"
#include "../common.h"

using namespace std;

// puzzle: https://adventofcode.com/2024/day/5

int main() {
    FileParser fp("2024/5-data");

    struct Rule {
        int page, pageToRight;
    };

    struct Manual {
        std::vector<int> pages;
    };

    auto rules = fp.parseRestUntilFail<Rule>([](std::ifstream& is) -> std::optional<Rule> {
        std::string s;
        std::getline(is, s);

        if (s.empty())
            return std::nullopt;

        auto sep = s.find('|');
        return Rule{
            std::stoi(s.substr(0, sep)),
            std::stoi(s.substr(sep + 1)),
        };
    });

    auto manuals = fp.parseRest<Manual>([](std::ifstream& is) {
        std::string s;
        std::getline(is, s);

        s = std::regex_replace(s, std::regex{","}, " ");

        std::stringstream ss{std::move(s)};

        Manual m;
        ss >> m.pages;
        return m;
    });

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {

        std::map<int, std::vector<int>> greaterList;
        for (const auto& [page, rightPage] : rules) 
        {
            greaterList[page].push_back(rightPage);
        }

        int total = 0;
        for (Manual& m : manuals)
        {
            auto prev = m.pages;
            std::sort(m.pages.begin(), m.pages.end(), [&](int a, int b) -> bool {
                return std::ranges::find(greaterList[a], b) != greaterList[a].end();
            });

            if (prev == m.pages)
                total += m.pages[m.pages.size() / 2];
        }

        out = [=](auto& o) { o << total << endl; };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {

        std::map<int, std::vector<int>> greaterList;
        for (const auto& [page, rightPage] : rules) 
        {
            greaterList[page].push_back(rightPage);
        }

        int total = 0;
        for (Manual& m : manuals)
        {
            auto prev = m.pages;
            std::sort(m.pages.begin(), m.pages.end(), [&](int a, int b) -> bool {
                return std::ranges::find(greaterList[a], b) != greaterList[a].end();
            });

            if (prev != m.pages)
                total += m.pages[m.pages.size() / 2];
        }

        out = [=](auto& o) { o << total << endl; };
    });

    dph.RunAll(cout);
}