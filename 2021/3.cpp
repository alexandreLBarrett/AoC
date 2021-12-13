#include "../helpers.h"
#include "../common.h"

#include <bitset>

using namespace std;

const uint32_t INPUT_WIDTH = 5;
const char* DATA_FILENAME = "2021/3-data-test";

using Bitset = bitset<INPUT_WIDTH>;

Bitset getMostViewed(const vector<Bitset>& bits) {
    int counters[INPUT_WIDTH] = { 0 };

    for_each(begin(bits), end(bits), [&](auto& bitset) {
        for (int i = 0; i < INPUT_WIDTH; i++) {
            counters[i] += bitset[i] ? 1 : -1;
        }
    });

    Bitset gamma = 0;
    for (int i = 0; i < INPUT_WIDTH; i++) {
        gamma |= ((counters[i] >= 0) << i);
    }

    return gamma;
}

int part1(const vector<Bitset>& bits) {
    int counters[INPUT_WIDTH] = { 0 };

    auto gamma = getMostViewed(bits);
    
    auto epsilon = ~gamma;
    return epsilon.to_ulong() * gamma.to_ulong();
}

template<class T>
Bitset filterBits(const vector<Bitset>& bits, T pred) {
    auto oxygenBits = bits;

    uint8_t idx = INPUT_WIDTH - 1;
    while (oxygenBits.size() != 1) {
        auto mostViewed = getMostViewed(oxygenBits);

        vector<Bitset> newOxBits;
        for (auto bits : oxygenBits) {
            if (pred(mostViewed[idx], bits[idx])) {
                newOxBits.push_back(bits);
            }
        }
        oxygenBits = newOxBits;
        idx--;
    }

    return oxygenBits.at(0);
}

int part2 (const vector<Bitset>& bits) {
    return 
        filterBits(bits, [](auto a, auto b){return a == b;}).to_ulong() *
        filterBits(bits, [](auto a, auto b){return a != b;}).to_ulong();
}

int main() {
    FileParser fp(DATA_FILENAME);
    
    auto bits = fp.parseRest<Bitset>([](ifstream& stream){
        Bitset b;
        stream >> b;
        return b;
    });

    int ret1, ret2;

    Timer<ostream>::print(cout, Timer<ostream>::time([&]{
        ret1 = part1(bits);
    }));
    cout << "Part 1: " << ret1 << endl;

    Timer<ostream>::print(cout, Timer<ostream>::time([&]{
        ret2 = part2(bits);
    }));
    cout << "Part 2: " << ret2 << endl;
}