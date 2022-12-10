#include "../helpers.h"
#include "../common.h"

using namespace std;

// puzzle: https://adventofcode.com/2022/day/10

struct CPU;

struct Instruction {
    virtual void execute(CPU& cpu) const = 0;
    virtual Instruction* clone() const = 0;
};

struct InstructionList {
    vector<Instruction*> instructions;

    InstructionList(istream&);
    InstructionList(const InstructionList&);
    ~InstructionList();
};

struct CPU {
    uint32_t tickNum = 0;
    int64_t X = 1;
    uint64_t accValue = 0;
    string CRT = "";

    void saveState() {
        if (tickNum == 20 || (20 + tickNum) % 40 == 0) {
            accValue += tickNum * X;
        }
    }

    void writeCRT() {
        auto tick = tickNum % 40;
        if (tick >= X - 1 && tick <= X + 1) {
            CRT += "#";
        } else {
            CRT += ".";
        }

        if ((tickNum + 1) % 40 == 0) {
            CRT += '\n';
        }
    }

    void tick() {
        writeCRT();
        tickNum++;
        saveState();
    }

    void tick(int newX) {
        tick();
        X = newX;
    }

    void executeProgram(const InstructionList& list) {
        for_each(list.instructions.begin(), list.instructions.end(), [this](const Instruction* instr) {
            instr->execute(*this);
        });
    }
};

struct NOOP_Instruction : public Instruction {
    void execute(CPU& cpu) const override {
        cpu.tick();
    }

    Instruction* clone() const {
        return new NOOP_Instruction();
    }
};

struct ADDX_Instruction : public Instruction {
    int param;

    ADDX_Instruction(int param) : param(param) {}

    void execute(CPU& cpu) const override {
        cpu.tick();
        cpu.tick(cpu.X + param);
    }

    Instruction* clone() const {
        return new ADDX_Instruction(param);
    }
};

InstructionList::InstructionList(const InstructionList& other) {
    instructions.reserve(other.instructions.size());
    for_each(other.instructions.begin(), other.instructions.end(), [this](const Instruction* inst) {
        instructions.push_back(inst->clone());
    });
}

InstructionList::InstructionList(istream& is) {
    string type;

    while (!is.eof()) {
        is >> type;

        if (type == "noop") {
            instructions.push_back(new NOOP_Instruction());
        } else {
            int param;
            is >> param;
            instructions.push_back(new ADDX_Instruction(param));
        }
    }
}

InstructionList::~InstructionList() {
    for_each(instructions.begin(), instructions.end(), [](const Instruction* instr) {
        delete instr;
    });
}


int main() {
    FileParser fp("2022/10-data");

    // Parse file

    auto il = fp.parseOne<InstructionList>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        CPU cpu;
        cpu.executeProgram(il);

        out = [=](auto& o) {
            cout << cpu.accValue << endl;
        };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        CPU cpu;
        cpu.executeProgram(il);

        out = [=](auto& o) {
            o << cpu.CRT << endl;
        };
    });

    dph.RunAll(cout);
}