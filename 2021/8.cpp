#include "../helpers.h"
#include "../common.h"

#include <numeric>
#include <map>
#include <cstdarg>

using namespace std;

class SegmentEntry {
    enum class VALUES{
        _0 = 0, _1,_2,_3,_4,_5,_6,_7,_8,_9, INVALID
    } value;
    string strValue;

public:
    SegmentEntry() : value(VALUES::INVALID) {};
    SegmentEntry(const string& codedNumber) {
        strValue = codedNumber;
        int size = codedNumber.size();

        switch(size) {
            case 2: value = VALUES::_1; break;
            case 4: value = VALUES::_4; break;
            case 3: value = VALUES::_7; break;
            case 7: value = VALUES::_8; break;
            default: value = VALUES::INVALID; break;
        };
    }

    bool isInvalid() {
        return value == VALUES::INVALID;
    }

    int getValue() {
        return (int)value;
    }

    void setValue(int _value) {
        value = (VALUES)_value;
    }

    string getStringValue() {
        return strValue;
    }
};

class SegmentLine {
public:
    const static int INPUT_SIZE = 10;
    const static int OUTPUT_SIZE = 4;
private:
    struct Config {
        int s_1 = 0;
        int inc_1 = 0;
        int s_4 = 0;
        int inc_4 = 0;
        int s_8 = 0;
        int inc_8 = 0;
    };

    inline static int idx = 0;
    int idx_ = 0;

    vector<SegmentEntry> unknownsIns;
    vector<SegmentEntry> unknownsOuts;

    SegmentEntry decodedInput[INPUT_SIZE];
    SegmentEntry decodedOutput[OUTPUT_SIZE];

    char configuration[7];
    map<char, int> configurationMap;
    map<string, int> remainingConfigs;

    void fillConfig(const string& str, int startIdx, int inc) {
        for (auto chr : str) {
            if (!configurationMap.count(chr)) {
                configuration[startIdx] = chr;
                configurationMap[chr] = startIdx;
                startIdx += inc;
            }
        }
    }
    void loadConfigs(Config conf) {
        fillConfig(decodedInput[1].getStringValue(), conf.s_1, conf.inc_1);
        fillConfig(decodedInput[7].getStringValue(), 0, 0);
        fillConfig(decodedInput[4].getStringValue(), conf.s_4, conf.inc_4);
        fillConfig(decodedInput[8].getStringValue(), conf.s_8, conf.inc_8);
    }

    void addNumbers() {
        addNumber(0, {0, 1, 2, 4, 5, 6});
        addNumber(2, {0, 2, 3, 4, 6});
        addNumber(3, {0, 2, 3, 5, 6});
        addNumber(5, {0, 1, 3, 5, 6});
        addNumber(6, {0, 1, 3, 4, 5, 6});
        addNumber(9, {0, 1, 2, 3, 5, 6});
    }

    bool checkConfigsInput() {
        for (auto unkownIn : unknownsIns) {
            string val = unkownIn.getStringValue();
            if (!remainingConfigs.count(val)) {
                return false;
            }

            int entryValue = remainingConfigs[val];
            decodedInput[entryValue] = unkownIn;
            decodedInput[entryValue].setValue(entryValue);
        }

        return true;
    }

    bool checkConfigsOutput() {
        int idx = 0;
        for (auto unkownOut : unknownsOuts) {
            string token = unkownOut.getStringValue();
            int value = getValue(token);
            if (value == -1)
                return false;

            decodedOutput[idx] = SegmentEntry(token);
            decodedOutput[idx].setValue(value);
            idx++;
        }

        return true;
    }

    void reset() {
        for (auto& conf : configuration) {
            conf = 0;
        }

        configurationMap.clear();
        remainingConfigs.clear();

        decodedInput[0] = SegmentEntry();
        decodedInput[2] = SegmentEntry();
        decodedInput[3] = SegmentEntry();
        decodedInput[5] = SegmentEntry();
        decodedInput[6] = SegmentEntry();
        decodedInput[9] = SegmentEntry();
    }

    void addNumber(int value, std::initializer_list<int> indexes) {
        string final = "";
        for (auto index : indexes) {
            final += configuration[index];
        }
        sort(begin(final), end(final));
        remainingConfigs[final] = value;
    }

    int getValue(const string& val) {
        for (auto decoded : decodedInput) {
            if (decoded.getStringValue() == val) {
                return decoded.getValue();
            }
        }

        return -1;
    }
public:
    SegmentLine(ifstream& line) {
        idx_ = idx++;

        string token = "";
        for (int i = 0; i < INPUT_SIZE; i++) {
            line >> token;

            sort(begin(token), end(token));

            auto s = SegmentEntry(token);
            if (s.isInvalid()) {
                unknownsIns.push_back(s);
            } else {
                decodedInput[s.getValue()] = s.getStringValue();
            }
        }

        line >> token;

        for (int i = 0; i < OUTPUT_SIZE; i++) {
            line >> token;
            sort(begin(token), end(token));
            unknownsOuts.push_back(SegmentEntry(token));
        }

        static vector<Config> configs = {
            Config{ 2, 3, 1, 2, 4, 2 },
            Config{ 2, 3, 1, 2, 6, -2 },
            Config{ 2, 3, 3, -2, 6, -2 },
            Config{ 2, 3, 3, -2, 4, 2 },
            Config{ 5, -3, 1, 2, 4, 2 },
            Config{ 5, -3, 1, 2, 6, -2 },
            Config{ 5, -3, 3, -2, 6, -2 },
            Config{ 5, -3, 3, -2, 4, 2 },
        };

        for (const auto& config : configs) {
            loadConfigs(config);
            addNumbers();

            if (!checkConfigsInput()) {
                reset();
                continue;
            }

            if (!checkConfigsOutput()) {
                reset();
                continue;
            }

            break;
        }
    }

    int getValue() {
        int val = 0;
        for (int i = 0; i < OUTPUT_SIZE; i++) {
            val += pow(10, OUTPUT_SIZE - i - 1) * decodedOutput[i].getValue();
        }
        return val;
    }
};

int main() {
    FileParser fp("2021/8-data");

    auto segments = fp.parseRest<SegmentLine>();

    DayPartHandler dph;
    dph.AddPart([&](auto& out){
        int outputTotal = 0;

        for (auto seg : segments) {
            outputTotal += seg.getValue();
        }

        out = [=](auto& o) { o << outputTotal; };
    });
    dph.RunAll(cout);
}