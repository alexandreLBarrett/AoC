#include "../helpers.h"
#include "../common.h"

#include <bitset>
#include <span>

using namespace std;

const uint64_t BITSET_SIZE = 10000;

struct Packet {
    int versionSum = 0;

    bitset<BITSET_SIZE> set;

    uint64_t value = 0;
    uint64_t size = 0;
    uint64_t version = 0;
    uint64_t type = 0;
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

    vector<Packet> parseWithLength(const int baseOffset,  uint16_t len) {
        vector<Packet> packets;
        uint32_t restLen = len;
        while (restLen != 0) {
            auto pack = parse(baseOffset + len - restLen);
            restLen -= pack.size;
            packets.push_back(pack);
        }
        return packets;
    }

    vector<Packet> parseN(const int baseOffset, uint16_t n) {
        vector<Packet> packets;

        int totalRead = 0;
        for (int i = 0; i < n; i++) {
            auto p = parse(baseOffset + totalRead);
            totalRead += p.size;
            packets.push_back(p);
        }

        return packets;
    }

    uint8_t getType(const int baseOffset) {
        uint8_t type = 0;
        type |= set[baseOffset + 3] << 2;
        type |= set[baseOffset + 4] << 1;
        type |= set[baseOffset + 5] << 0;
        return type;
    }

    vector<Packet> readAsOperator(const int baseOffset) {
        int typeIdOffset = baseOffset + 6;
        if (set[typeIdOffset] == 0) {
            uint16_t len = 0;
            for (int i = 0; i < 15; i++) {
                len |= set[typeIdOffset + i + 1] << 14 - i;
            }
            size = 22;
            return parseWithLength(baseOffset + 22, len);
        } else {
            uint16_t len = 0;
            for (int i = 0; i < 11; i++) {
                len |= set[typeIdOffset + i + 1] << 10 - i;
            }
            size = 18;
            return parseN(baseOffset + 18,  len);
        }
    }

    void readAsNumber(const int baseOffset) {
        bool done = false;
        int subIndex = 0;
        const uint64_t valueOffset = baseOffset + 6;
        value = 0;
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
        size = 6 + subIndex * 5;
    }

    Packet parse(const int baseOffset = 0) {
        Packet p(set);
        //cout << "ver: " << (int)getVersion(baseOffset) << " type: " << (int)getType(baseOffset) << endl; 
        p.version = getVersion(baseOffset);
        p.type = getType(baseOffset);

        if (getType(baseOffset) == 4) {
            p.readAsNumber(baseOffset);
        } else {
            p.subPackets = p.readAsOperator(baseOffset);
            for (const auto& pack : p.subPackets) {
                p.size += pack.size;
            }
        }
        return p;
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

    uint64_t calcluatePacket(const Packet& pack) {
        switch (pack.type) {
            case 0: {
                uint64_t total = 0;
                for (const auto& p : pack.subPackets) {
                    total += calcluatePacket(p);
                }
                return total;
            }
            case 1: {
                uint64_t total = 1;
                for (const auto& p : pack.subPackets) {
                    total *= calcluatePacket(p);
                }
                return total;
            }
            case 2: {
                uint64_t min = UINT64_MAX;
                for (const auto& p : pack.subPackets) {
                    uint64_t value = calcluatePacket(p);
                    if (value < min)
                        min = value;
                }
                return min;
            }
            case 3: {
                uint64_t max = 0;
                for (const auto& p : pack.subPackets) {
                    uint64_t value = calcluatePacket(p);
                    if (value > max)
                        max = value;
                }
                return max;
            }
            case 4:
                return pack.value;
            case 5: {
                return calcluatePacket(pack.subPackets[0]) > calcluatePacket(pack.subPackets[1]) ? 1 : 0;
            }
            case 6: {
                return calcluatePacket(pack.subPackets[0]) < calcluatePacket(pack.subPackets[1]) ? 1 : 0;
            }
            case 7: {
                return calcluatePacket(pack.subPackets[0]) == calcluatePacket(pack.subPackets[1]) ? 1 : 0;
            }
        }
        return -1;
    }

    Packet decode() {
        return rootPacket.parse();
    }

    int getVersionSum(const Packet& pack) {
        int sumTotal = pack.version;
        for (const Packet& sub : pack.subPackets) {
            sumTotal += getVersionSum(sub);
        }
        return sumTotal;
    }
};


int main() {
    FileParser fp("2021/16-data");

    // Parse file
    auto bites = fp.parseRest<BITESDecoder>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        auto packet = bites[0].decode();
        int value = bites[0].getVersionSum(packet);
    
        out = [=](auto& o) { o << value << endl; };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        auto packet = bites[0].decode();
        uint64_t value = bites[0].calcluatePacket(packet);

        out = [=](auto& o) { o << value << endl; };
    });

    dph.RunAll(cout);
}