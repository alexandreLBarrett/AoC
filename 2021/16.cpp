#include "../helpers.h"
#include "../common.h"

#include <bitset>
#include <span>

using namespace std;

const uint64_t BITSET_SIZE = 10000;

struct Packet {
    int versionSum = 0;

    bitset<BITSET_SIZE> set;

    uint64_t value;
    vector<Packet> subPackets;
    
    Packet() = default;

    Packet(bitset<BITSET_SIZE> set)
        : set(set)
    {
    }

    uint8_t getVersion(const int baseOffset) {
        uint8_t version = 0;
        version |= set[baseOffset] << 2;
        version |= set[baseOffset + 1] << 1;
        version |= set[baseOffset + 2] << 0;
        return version;
    }

    int parseWithLength(const int baseOffset,  uint16_t len) {
        uint32_t restLen = len;
        while (restLen != 0) {
            restLen -= parse(baseOffset + len - restLen);
        }
        return len;
    }

    int parseN(const int baseOffset, uint16_t n) {
        int totalRead = 0;
        for (int i = 0; i < n; i++) {
            totalRead = parse(baseOffset + totalRead) + totalRead;
        }
        return totalRead;
    }

    uint8_t getType(const int baseOffset) {
        uint8_t type = 0;
        type |= set[baseOffset + 3] << 2;
        type |= set[baseOffset + 4] << 1;
        type |= set[baseOffset + 5] << 0;
        return type;
    }

    int readAsOperator(const int baseOffset) {
        int typeIdOffset = baseOffset + 6;
        if (set[typeIdOffset] == 0) {
            uint16_t len = 0;
            for (int i = 0; i < 15; i++) {
                len |= set[typeIdOffset + i + 1] << 14 - i;
            }
            return parseWithLength(baseOffset + 22, len) + 22;
        } else {
            uint16_t len = 0;
            for (int i = 0; i < 11; i++) {
                len |= set[typeIdOffset + i + 1] << 10 - i;
            }
            return parseN(baseOffset + 18,  len) + 18;
        }
    }

    int readAsNumber(const int baseOffset) {
        Packet p;

        bool done = false;
        int subIndex = 0;
        const uint8_t valueOffset = baseOffset + 6;
        uint64_t value = 0;
        while (!done) {
            int subIndexOff = subIndex*5;
            done = set[valueOffset + subIndexOff] == 0;
            value |= set[valueOffset + subIndexOff + 1] << 3;
            value |= set[valueOffset + subIndexOff + 2] << 2;
            value |= set[valueOffset + subIndexOff + 3] << 1;
            value |= set[valueOffset + subIndexOff + 4] << 0;
            if (!done)
                value = value << 4;
            subIndex++;
        }

        p.value = value;
        subPackets.push_back(p);
        return 6 + subIndex * 5;
    }

    int parse(const int baseOffset = 0) {
        cout << "ver: " << (int)getVersion(baseOffset) << " type: " << (int)getType(baseOffset) << endl; 
        versionSum += getVersion(baseOffset);

        if (getType(baseOffset) == 4) {
            return readAsNumber(baseOffset);
        } else {
            return readAsOperator(baseOffset);
        }
    }
};

class BITESDecoder {
    Packet rootPacket;

    uint8_t fromHex(char c) {
        if (c >= 'A' && c <= 'F')  return c - 'A' + 10;
        if (c >= '0' && c <= '9')  return c - '0';
        return -1;
    }

public:
    BITESDecoder(ifstream& stream) {
        string hexValue;

        stream >> hexValue;

        bitset<BITSET_SIZE> set;
        for (uint64_t i = 0; i < hexValue.size(); i++) {
            uint8_t val = fromHex(hexValue[i]);
            set[i*4] = val & 0x08;
            set[i*4 + 1] = val & 0x04;
            set[i*4 + 2] = val & 0x02;
            set[i*4 + 3] = val & 0x01;
        }

        rootPacket = Packet(set);
    }

    int decode() {
        // auto a = rootPacket.set.to_string();
        // reverse(begin(a), end(a));
        // cout << a << endl;
        rootPacket.parse();
        return rootPacket.versionSum;
    }
};


int main() {
    FileParser fp("2021/16-data-test");

    // Parse file
    auto bites = fp.parseRest<BITESDecoder>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        for (auto b : bites) {
            cout << b.decode() << endl;
        }
        
        out = [=](auto& o) { };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {

        out = [=](auto& o) {};
    });

    dph.RunAll(cout);
}