#include "../helpers.h"
#include "../common.h"

#include <map>

const char* DATA_FILENAME = "2021/4-data";

using namespace std;

class BingoCaller {
public:
    BingoCaller(std::ifstream& ss) {
        string line;
        getline(ss, line);

        std::stringstream lineStream(line);
        string token;
        
        while(getline(lineStream, token, ',')) {
            calls.push_back(atoi(token.c_str()));
        }

        getline(ss, line);
    }

    vector<int> getCalls() const {
        return calls;
    }
private:
    vector<int> calls;
};

class BingoCard {
public:
    bool isDone = false;

    BingoCard() = default; 
    BingoCard(std::ifstream& ss) {
        for (auto& row : card) {
            for (auto& value : row) {
                value = false;
            }
        }

        string token;
        
        for (int i = 0; i < 5; i++) {
            getline(ss, token);
            std::stringstream ss2(token);

            for (int j = 0; j < 5; j++) {
                int val;
                ss2 >> val;
                positions[val] = make_pair(i, j);
            }
        }
        // removes empty line
        getline(ss, token);
    }

    void readNumber(int n) {
        if (!positions.count(n))
            return;

        auto[x, y] = positions[n];
        card[x][y] = true;

        isDone = checkWinState();
    }

    int calculateScore(int lastCalled) {
        int totalScore = 0;
        for (auto& elem : positions) {
            if (!card[elem.second.first][elem.second.second]) {
                totalScore += elem.first;
            }
        }

        return totalScore * lastCalled;
    }
private:
    bool card[5][5];
    map<int, pair<int, int>> positions;

    bool checkWinState() {
        for (int i = 0; i < 5; i++) {
            bool rowGood = true;
            bool columnGood = true;
            for (int j = 0; j < 5; j++) {
                rowGood &= card[i][j];
                columnGood &= card[j][i];
            }
            if (rowGood || columnGood) return true;
        }

        return false;
    }
};

int processCards(const BingoCaller& caller, vector<BingoCard>& cards) {
    for (const auto& call : caller.getCalls()) {
        vector<BingoCard*> releventCards;
        for (auto& card : cards) {
            if (!card.isDone)
                releventCards.push_back(&card);
        }

        for (auto& card : releventCards) {
            card->readNumber(call);
        }

        if (releventCards.size() == 1 && releventCards[0]->isDone) {
            return releventCards[0]->calculateScore(call);
        }
    }
    return -1;
}

int main() {
    FileParser fp = FileParser(DATA_FILENAME);

    BingoCaller caller = fp.parseOne<BingoCaller>();
    vector<BingoCard> cards = fp.parseRest<BingoCard>();

    int val;
    Timer<ostream>::print(cout, Timer<ostream>::time([&]{
        val = processCards(caller, cards);
    }));

    cout << val << endl;
}