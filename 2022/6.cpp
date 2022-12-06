#include "../helpers.h"
#include "../common.h"

#include <span>

using namespace std;

// puzzle: https://adventofcode.com/2022/day/6

template<class T, size_t N> [[nodiscard]]
constexpr auto slide(span<T,N> s, size_t offset, size_t width) {
    return s.subspan(offset, offset + width <= s.size() ? width : 0U);
}

class ElfStream {
    string s;
public:
    ElfStream(istream& is) {
        getline(is, s);
    }

    bool foundEnd(const span<const char>& window, uint8_t packetSize) const {
        for (int i = 0; i < packetSize; ++i) {
            for (int j = i + 1; j < packetSize; ++j) {
                if (window[i] == window[j])
                    return false;
            }
        }

        return true;
    }

    uint32_t parseSignal(uint8_t packetSize) const {
        if (s.size() < packetSize)
            return 0;

        for (int i = 0; i < s.size() - packetSize; ++i) {
            auto window = slide(span{s}, i, packetSize);
            if (foundEnd(window, packetSize)) {
                return i + packetSize;
            }
        }
        return 0;
    }
};

int main() {
    FileParser fp("2022/6-data");

    // Parse file
    auto streams = fp.parseRest<ElfStream>();

    DayPartHandler<ostream> dph;

    dph.AddPart([=](auto& out) mutable {
        vector<uint32_t> sizes;
        for (const auto& s : streams) {
            sizes.push_back(s.parseSignal(4));
        }

        out = [=](auto& o) {
            for (auto s : sizes)
                cout << s << endl;
        };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        vector<uint32_t> sizes;
        for (const auto& s : streams) {
            sizes.push_back(s.parseSignal(14));
        }

        out = [=](auto& o) {
            for (auto s : sizes)
                cout << s << endl;
        };
    });

    dph.RunAll(cout);
}