#include "../helpers.h"
#include "../common.h"

#include <map>

using namespace std;

class InstructionSet {
    unordered_map<string, char> set;
public:
    InstructionSet(ifstream& stream) {
        string lhs;
        string rhs;
        while(!stream.eof()) {
            getline(stream, lhs, ' ');
            stream.ignore(3);
            getline(stream, rhs);
            set[lhs] = rhs[0];
        }
    }

    bool hasKey(const string& key) const {
        return set.contains(key);
    }

    char operator[](const string& key) const {
        return set.at(key);
    }
};

using Cache = map<string, map<char, uint64_t>>;

class PolymerTemplate {
    string polyTemplate;
    vector<Cache> caches;

    map<char, uint64_t> compileCounts(map<char, uint64_t> counts1, const map<char, uint64_t>& counts2) {
        for (const auto& count : counts2) {
            if (counts1.contains(count.first)) {
                counts1[count.first] += count.second;
            } else {
                counts1[count.first] = count.second;
            }
        }

        return counts1;
    }
public:
    PolymerTemplate(ifstream& stream) {
        getline(stream, polyTemplate);
        stream.ignore(1);
    }

    map<char, uint64_t> countDepth(const InstructionSet& is, const string& val, const int depth, const int maxDepth) {
        if (depth == maxDepth) {
            return {};
        }

        Cache& cache = caches[depth];
        if (cache.contains(val)) {
            return cache.at(val);
        }

        map<char, uint64_t> counts;

        if (is.hasKey(val)) {
            auto addition = is[val];
            counts = compileCounts(
                countDepth(is, { val[0], addition }, depth + 1, maxDepth),
                countDepth(is, { addition, val[1] }, depth + 1, maxDepth)
            );

            counts[addition]++;
        }

        cache[val] = counts;
        return counts;
    }

    map<char, uint64_t> depthCount(const InstructionSet& instructions, const int maxDepth) {
        caches.clear();
        for (int i = 0; i < maxDepth; i++) {
            caches.push_back(Cache());
        }

        map<char, uint64_t> counts;

        for (int i = 0; i < polyTemplate.size() - 1; i++) {
            counts[polyTemplate[i]]++;

            counts = compileCounts(
                countDepth(instructions, { polyTemplate[i], polyTemplate[i+1] }, 0, maxDepth),
                counts
            );
        }

        counts[polyTemplate[polyTemplate.size() - 1]]++;

        return counts;
    }

    uint64_t getExtremeDiffs(const map<char, uint64_t>&& counts) const {
        uint64_t min = UINT64_MAX;
        uint64_t max = 0;
        for (const auto kv : counts) {
            if (kv.second > max) max = kv.second;
            if (kv.second < min) min = kv.second;
        }

        return max - min;
    }
};

int main() {
    FileParser fp("2021/14-data");

    // Parse file
    auto polyTemplate = fp.parseOne<PolymerTemplate>();
    auto instructions = fp.parseOne<InstructionSet>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        uint64_t size = polyTemplate.getExtremeDiffs(polyTemplate.depthCount(instructions, 10));

        out = [=](auto& o) { o << size << endl; };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        uint64_t size = polyTemplate.getExtremeDiffs(polyTemplate.depthCount(instructions, 40));

        out = [=](auto& o) { o << size << endl; };
    });

    dph.RunAll(cout);
}