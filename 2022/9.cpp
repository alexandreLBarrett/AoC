#include "../helpers.h"
#include "../common.h"

#include <unordered_set>

using namespace std;

// puzzle: https://adventofcode.com/2022/day/9

struct Instruction {
    enum class Direction { UP,DOWN,LEFT,RIGHT };

    Direction direction;
    int count;

    Instruction(istream& is) {
        string dir;
        is >> dir;
        switch (dir[0]) {
            case 'R': direction = Direction::RIGHT; break;
            case 'U': direction = Direction::UP; break;
            case 'L': direction = Direction::LEFT; break;
            case 'D': direction = Direction::DOWN; break;
        }
        is >> count;
    }
};

template<int N>
class Path {
    struct Position {
        int x = 0, y = 0;

        Position& operator+=(const Position& other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        friend Position operator+(const Position& a, const Position& b) {
            return Position{a.x + b.x, a.y + b.y};
        }

        friend Position operator-(const Position& a, const Position& b) {
            return Position{a.x - b.x, a.y - b.y};
        }

        friend bool operator==(const Position& a, const Position& b) {
            return a.x == b.x && a.y == b.y;
        }
    };

    struct PositionHash {
        auto operator()(const Position& p) const -> size_t {
            return hash<int>{}(p.x) ^ hash<int>{}(p.y);
        }
    };

    unordered_set<Position, PositionHash> path;
    Position rope[N + 1];
public:
    void followTail(int i) {
        Position direction = rope[i - 1] - rope[i];
        if (abs(direction.x) > 1 || abs(direction.y) > 1) {
            if (direction.x < 0) direction.x = max(direction.x, -1);
            else                 direction.x = min(direction.x, 1);

            if (direction.y < 0) direction.y = max(direction.y, -1);
            else                 direction.y = min(direction.y, 1);

            rope[i] += direction; 
        }
    }

    void applyPath(const Instruction& instr) {
        for (int i = 0; i < instr.count; ++i) {
            switch (instr.direction) {
                case Instruction::Direction::RIGHT: rope[0].x++; break;
                case Instruction::Direction::UP:    rope[0].y++; break;
                case Instruction::Direction::LEFT:  rope[0].x--; break;
                case Instruction::Direction::DOWN:  rope[0].y--; break;
            }

            for (int j = 1; j <= N; ++j) {
                followTail(j);
            }
            
            path.insert(rope[N]);
        }
    }

    int getTravelDistance() {
        return path.size();
    }
};

int main() {
    FileParser fp("2022/9-data");

    // Parse file
    auto instructions = fp.parseRest<Instruction>();
    

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        Path<1> path;
        for (const auto& instr : instructions) {
            path.applyPath(instr);
        }
        auto total = path.getTravelDistance();

        out = [=](auto& o) {
            o << total << endl;
        };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        Path<9> path;
        for (const auto& instr : instructions) {
            path.applyPath(instr);
        }
        auto total = path.getTravelDistance();

        out = [=](auto& o) {
            o << total << endl;
        };
    });

    dph.RunAll(cout);
}