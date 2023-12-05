#include "../helpers.h"
#include "../common.h"
#include <set>
#include <map>
#include <cmath>
#include <numeric>

using namespace std;

// puzzle: https://adventofcode.com/2023/day/4
struct Card {
    std::vector<int> winning;
    std::set<int> has;

    friend istream& operator>>(istream& is, Card& card)
    {
        string line;
        getline(is, line, ':');
        getline(is, line, '|');
        stringstream winningSteam{line};
        getline(is, line);
        stringstream hasStream{line};

        string val;
        while (winningSteam >> val) 
            card.winning.push_back(stoi(val));

        while (hasStream >> val) 
            card.has.insert(stoi(val));

        return is;
    }
};

int main() {
    FileParser fp("2023/4-data");

    // Parse file
    auto cards = fp.parseRest<Card>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        int sum = 0;
        for (const auto& card : cards) {
            int totalIntersect{};
            for (auto& winningNum : card.winning)
                if (card.has.contains(winningNum))
                    totalIntersect++;

            if (totalIntersect)
                sum += pow(2, totalIntersect - 1);
        }
        out = [=](auto& o) { o << sum << endl; };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        std::vector<int> scratchCardsTotal;
        scratchCardsTotal.resize(cards.size());

        for (int i = 0; i < cards.size(); ++i) {
            scratchCardsTotal[i] += 1;
            auto& card = cards[i];

            int totalIntersect{};
            for (auto& winningNum : card.winning)
                if (card.has.contains(winningNum))
                    totalIntersect++;

            for (int j = 0; j < totalIntersect; ++j) {
                scratchCardsTotal[1 + i + j] += scratchCardsTotal[i];
            }
        }

        int sum = std::accumulate(scratchCardsTotal.begin(), scratchCardsTotal.end(), 0);
        out = [=](auto& o) { o << sum << endl; };
    });

    dph.RunAll(cout);
}