#include "../helpers.h"
#include "../common.h"

#include <map>

using namespace std;

// puzzle: https://adventofcode.com/2023/day/7

struct Hand { 
    std::string cards{};
    uint64_t bid{};

    friend istream& operator>>(istream& is, Hand& hand) {
        is >> hand.cards;
        is >> hand.bid;
        return is;
    }

    int getType() const {
        std::map<char, int> counts;
        
        for (int i = 0; i < 5; ++i) {
            counts[cards[i]]++;
        }

        if (counts.size() == 1) { // Five of a kind
            return 0;
        } else if (counts.size() == 2) { // Four of a kind or fullhouse
            for (auto& [key, count] : counts) {
                if (count == 4) {
                    return 1;
                }
            }

            return 2;
        } else if (counts.size() == 3) { // Three of a kind or two pair
            for (auto& [key, count] : counts) {
                if (count == 3) {
                    return 3;
                }
            }

            return 4;
        } else if (counts.size() == 4) { // One pair
            return 5;
        } else if (counts.size() == 5) { // High card
            return 6;
        }

        return -1;
    }

    static inline std::map<char, int> cardValues{
        {'A', 12},
        {'K', 11},
        {'Q', 10},
        {'J', 9},
        {'T', 8},
        {'9', 7},
        {'8', 6},
        {'7', 5},
        {'6', 4},
        {'5', 3},
        {'4', 2},
        {'3', 1},
        {'2', 0},
    };

    bool isGreater(const string& cards0, const string& cards1) const {
        for (int i = 0; i < 5; ++i) {
            if (cardValues.at(cards0[i]) < cardValues.at(cards1[i])) {
                return false;
            }
        }
        return true;
    }

    bool operator<(const Hand& other) const { 
        if (getType() == other.getType()) {
            return isGreater(cards, other.cards);
        }

        return getType() < other.getType();
    }
};

int main() {
    FileParser fp("2023/7-data");

    // Parse file
    auto hands = fp.parseRest<Hand>();

    Hand h1{"65666", 0};
    Hand h2{"86888", 0};
    
    bool t = h1 < h2;

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        std::sort(hands.begin(), hands.end());

        auto i = is_sorted_until(hands.begin(), hands.end());


        uint64_t sum{};
        auto size = hands.size();
        for (int i = 0; i < size; ++i) {
            int rank = size - i;

            auto& hand = hands[i];
            sum += hand.bid * rank;
        }

        out = [=](auto& o) { o << sum << endl; };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {

        out = [=](auto& o) {};
    });

    dph.RunAll(cout);
}