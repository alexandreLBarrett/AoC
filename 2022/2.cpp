#include "../helpers.h"
#include "../common.h"

#include <numeric>

using namespace std;

class Turn {
    uint8_t opponent;
    uint8_t player;

    // For moves: ROCK = 0, PAPER = 1, SICSSORS = 2
    // For results: LOSS = 0, TIE = 1, WIN = 2
    const uint8_t results[3][3] {
        { 1, 0, 2 },
        { 2, 1, 0 },
        { 0, 2, 1 }
    };
public:
    int getPointsGuessed() const {
        return 1 + player + (uint8_t)results[player][opponent] * 3;
    }

    int getPointsConfirmed() const {
        int i = 0; 
        while (results[opponent][i] != (2 - player)) i++;
        return 1 + i + player * 3;
    }

    Turn(istream& o) {
        string s;

        o >> s;
        opponent = s[0] - 'A';

        o >> s;
        player = s[0] - 'X';
    }
};

int main() {
    FileParser fp("2022/2-data");

    // Parse file
    auto turns = fp.parseRest<Turn>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        
        auto total = accumulate(turns.begin(), turns.end(), 0, [](const int acc, const Turn& turn) {
            return acc + turn.getPointsGuessed();
        });

        out = [=](auto& o) {
            o << total;
        };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        auto total = accumulate(turns.begin(), turns.end(), 0, [](const int acc, const Turn& turn) {
            return acc + turn.getPointsConfirmed();
        });

        out = [=](auto& o) {
            o << total;
        };
    });

    dph.RunAll(cout);
}