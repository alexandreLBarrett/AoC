#include "../helpers.h"
#include "../common.h"

using namespace std;

// puzzle: https://adventofcode.com/2024/day/4

struct Vec2 {
    int x, y;
    
    Vec2& operator+=(const Vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
};

Vec2 operator+(const Vec2& a, const Vec2& b) {
    return { a.x + b.x, a.y + b.y };
}

Vec2 operator-(const Vec2& a, const Vec2& b) {
    return { a.x - b.x, a.y - b.y };
}

Vec2 operator*(const Vec2& a, const Vec2& b) {
    return { a.x * b.x, a.y * b.y };
}

Vec2 operator*(const Vec2& a, int b) {
    return { a.x * b, a.y * b };
}


int main() {
    FileParser fp("2024/4-data");

    // Parse file

    auto board = fp.parseRest<std::string>();

    DayPartHandler<ostream> dph;

    static const char* XMAS = "XMAS";

    static auto isValidPos = [](const Vec2& pos, const Vec2& size) {
        return pos.x >= 0 && pos.y >= 0 && pos.x < size.x && pos.y < size.y;
    };

    static auto checkOffset = [](const std::vector<std::string>& board, const Vec2& startPos, const Vec2& dirOffset, const Vec2& size) -> bool
    {
        Vec2 current = startPos + dirOffset;

        for (int i = 1; i < 4; ++i) {
            if (!isValidPos(current, size) || (isValidPos(current, size) && board[current.y][current.x] != XMAS[i]))
                return false;

            current += dirOffset;
        }

        return true;
    };

    static auto checkDirection = [](const std::vector<std::string>& board, const Vec2& startPos, const Vec2& gridSize) -> int {
        return 
            checkOffset(board, startPos, Vec2{-1, -1}, gridSize) +
            checkOffset(board, startPos, Vec2{0, -1}, gridSize) +
            checkOffset(board, startPos, Vec2{1, -1}, gridSize) +
            checkOffset(board, startPos, Vec2{1, 0}, gridSize) +
            checkOffset(board, startPos, Vec2{1, 1}, gridSize) +
            checkOffset(board, startPos, Vec2{0, 1}, gridSize) +
            checkOffset(board, startPos, Vec2{-1, 1}, gridSize) +
            checkOffset(board, startPos, Vec2{-1, 0}, gridSize);
    };

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        Vec2 size{ (int)board[0].size(), (int)board.size()};
        int total = 0;
        for (auto y = 0; y < size.y; ++y) {
            for (auto x = 0; x < size.x; ++x) {
                int prev = total;
                if (board[y][x] == 'X')
                    total += checkDirection(board, {x, y}, size);
            }
        }
        
        out = [=](auto& o) { o << total << endl; };
    });


    static auto checkM_S = [](const std::vector<std::string>& board, const Vec2& startPos, const Vec2& dirOffset, const Vec2& size) -> bool
    {
        auto c1 = startPos + dirOffset;
        auto c2 = startPos - dirOffset;

        if (!isValidPos(c1, size) || !isValidPos(c2, size))
            return false;

        char l1 = board[c1.y][c1.x];
        char l2 = board[c2.y][c2.x];

        return (l1 == 'M' && l2 == 'S') || (l1 == 'S' && l2 == 'M');
    };

    static auto checkMAS = [](const std::vector<std::string>& board, const Vec2& startPos, const Vec2& gridSize) -> int {
        return 
            checkM_S(board, startPos, Vec2{-1, -1}, gridSize) && 
            checkM_S(board, startPos, Vec2{1, -1}, gridSize);
    };

    // Part 2
    dph.AddPart([=](auto& out) mutable {

        Vec2 size{ (int)board[0].size(), (int)board.size()};
        int total = 0;
        for (auto y = 0; y < size.y; ++y) {
            for (auto x = 0; x < size.x; ++x) {
                int prev = total;
                if (board[y][x] == 'A')
                    total += checkMAS(board, {x, y}, size);
            }
        }

        out = [=](auto& o) { o << total << endl; };
    });

    dph.RunAll(cout);
}