#include "../helpers.h"
#include "../common.h"
#include <vector>
#include <ranges>

using namespace std;

// puzzle: https://adventofcode.com/2023/day/5
struct Seeds {
    std::vector<int64_t> seeds;

    friend istream& operator>>(istream& is, Seeds& seeds) {
        is.ignore("seeds: "sv.length());
        string line;
        getline(is, line);
        stringstream lineStream{line};

        int64_t val;
        while (lineStream >> val) {
            seeds.seeds.push_back(val);
        }

        // skip empty line
        getline(is, line);
        return is;
    }
};

struct ConversionTable {
    struct Conversion {
        int64_t dest, source, count;

        friend istream& operator>>(istream& is, Conversion& conv) { 
            is >> conv.dest >> conv.source >> conv.count;
            return is;
        }
    };

    std::vector<Conversion> conversions;

    friend istream& operator>>(istream& is, ConversionTable& conversionTable) { 
        string line;
        getline(is, line);
        do {
            getline(is, line);
            if (line.empty() || is.eof())
                return is;

            stringstream lineStream{line};

            Conversion c;
            lineStream >> c;
            conversionTable.conversions.push_back(c);
        } while (is);

        return is;
    }
};

int main() {
    FileParser fp("2023/5-data");

    // Parse file

    auto seeds = fp.parseOne<Seeds>();
    auto conversions = fp.parseRest<ConversionTable>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        int64_t minVal = std::numeric_limits<int64_t>::max();
        for (auto& seed : seeds.seeds) {
            int64_t currentVal = seed;
            for (const auto& conversionTable : conversions) {
                for (const auto& conversion : conversionTable.conversions) {
                    int64_t dist = currentVal - conversion.source;
                    if (dist > 0 && dist < conversion.count) {
                        currentVal = conversion.dest + dist;
                        break;
                    }
                }
            }

            minVal = std::min(minVal, currentVal);
        }
        out = [=](auto& o) { o << minVal << endl; };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        int64_t minVal = std::numeric_limits<int64_t>::max();
        for (int i = 0; i < seeds.seeds.size(); i += 2) {
            for (int j = 0; j < seeds.seeds[i + 1]; ++j) {
                int64_t currentVal = seeds.seeds[i] + j;
                for (const auto& conversionTable : conversions) {
                    for (const auto& conversion : conversionTable.conversions) {
                        int64_t dist = currentVal - conversion.source;
                        if (dist >= 0 && dist < conversion.count) {
                            currentVal = conversion.dest + dist;
                            break;
                        }
                    }
                }
                minVal = std::min(minVal, currentVal);
            }
        }

        out = [=](auto& o) { o << minVal << endl; };
    });

    dph.RunAll(cout);
}