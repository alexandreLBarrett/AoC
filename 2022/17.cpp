#include "../helpers.h"
#include "../common.h"

using namespace std;

// puzzle: https://adventofcode.com/2022/day/17

struct Shape {
    Point<> position;
    vector<Point<>> pts;
    Shape() {}
    Shape(vector<Point<>> ps) : pts(ps) {}

    bool intersect(const Shape& shape) {
        for (Point<> pt1 : shape.pts) {
            pt1 += shape.position;
            for (Point<> pt2 : pts) {
                pt2 += position;

                if (pt1 == pt2) {
                    return true;
                }
            }
        }
        return false;
    }
};

struct Piece {
    Shape shape;   
    virtual unique_ptr<Piece> clone() const = 0;
    virtual void setLeftX(uint64_t x) = 0;
    virtual void setBottomY(uint64_t y) = 0;
    virtual int64_t getYMin() = 0;
    virtual int64_t getYMax() = 0;
    virtual int64_t getXMin() = 0;
    virtual int64_t getXMax() = 0;
};

struct LinePiece : public Piece {
    /*
        #0##
    */
    LinePiece() {
        shape = Shape({
            { -1, 0 },
            {  0, 0 },
            {  1, 0 },
            {  2, 0 },
        });
    }

    unique_ptr<Piece> clone() const {
        return make_unique<LinePiece>();
    }

    void setLeftX(uint64_t x) override {
        shape.position.x = x + 1;
    }

    void setBottomY(uint64_t y) override {
        shape.position.y = y;
    }

    int64_t getYMin() {
        return shape.position.y + shape.pts[1].y;
    }

    int64_t getYMax() {
        return shape.position.y + shape.pts[1].y;
    }

    int64_t getXMin() {
        return shape.position.x + shape.pts[0].x;
    }

    int64_t getXMax() {
        return shape.position.x + shape.pts[3].x;
    }
};

struct PlusPiece : public Piece {
    /*
        .#.
        #0#
        .#.
    */

    PlusPiece() {
        shape = Shape({
            {  0, 0 },
            { -1, 0 },
            {  1, 0 },
            {  0,-1 },
            {  0, 1 },
        });
    }

    unique_ptr<Piece> clone() const {
        return make_unique<PlusPiece>();
    }

    void setLeftX(uint64_t x) override {
        shape.position.x = x + 1;
    }

    void setBottomY(uint64_t y) override {
        shape.position.y = y + 1;
    }

    int64_t getYMin() {
        return shape.position.y + shape.pts[3].y;
    }

    int64_t getYMax() {
        return shape.position.y + shape.pts[4].y;
    }

    int64_t getXMin() {
        return shape.position.x + shape.pts[1].x;
    }

    int64_t getXMax() {
        return shape.position.x + shape.pts[2].x;
    }
};

struct LPiece : public Piece {
    /*
        ..#
        ..#
        ##0
    */

    LPiece() {
        shape = Shape({
            { -2, 0 },
            { -1, 0 },
            {  0, 0 },
            {  0, 1 },
            {  0, 2 },
        });
    }

    unique_ptr<Piece> clone() const {
        return make_unique<LPiece>();
    }

    void setLeftX(uint64_t x) override {
        shape.position.x = x + 2;
    }

    void setBottomY(uint64_t y) override {
        shape.position.y = y;
    }

    int64_t getYMin() {
        return shape.position.y + shape.pts[2].y;
    }

    int64_t getYMax() {
        return shape.position.y + shape.pts[4].y;
    }

    int64_t getXMin() {
        return shape.position.x + shape.pts[0].x;
    }

    int64_t getXMax() {
        return shape.position.x + shape.pts[2].x;
    }
};

struct WallPiece : public Piece {
    /*
        #
        0
        #
        #
    */

    WallPiece() {
        shape = Shape({
            {  0, 1 },
            {  0, 0 },
            {  0,-1 },
            {  0,-2 },
        });
    }

    unique_ptr<Piece> clone() const {
        return make_unique<WallPiece>();
    }

    void setLeftX(uint64_t x) override {
        shape.position.x = x;
    }

    void setBottomY(uint64_t y) override {
        shape.position.y = y + 2;
    }

    int64_t getYMin() {
        return shape.position.y + shape.pts[3].y;
    }

    int64_t getYMax() {
        return shape.position.y + shape.pts[0].y;
    }

    int64_t getXMin() {
        return shape.position.x + shape.pts[1].x;
    }

    int64_t getXMax() {
        return shape.position.x + shape.pts[1].x;
    }
};

struct SquarePiece : public Piece {
    /*
        ##
        0#
    */

    SquarePiece() {
        shape = Shape({
            {  1, 1 },
            {  0, 1 },
            {  1, 0 },
            {  0, 0 },
        });
    }

    unique_ptr<Piece> clone() const {
        return make_unique<SquarePiece>();
    }

    void setLeftX(uint64_t x) override {
        shape.position.x = x;
    }

    void setBottomY(uint64_t y) override {
        shape.position.y = y;
    }

    int64_t getYMin() {
        return shape.position.y + shape.pts[3].y;
    }

    int64_t getYMax() {
        return shape.position.y + shape.pts[0].y;
    }

    int64_t getXMin() {
        return shape.position.x + shape.pts[3].x;
    }

    int64_t getXMax() {
        return shape.position.x + shape.pts[0].x;
    }
};

struct Board {
    vector<bool> directions;
    uint64_t rockCounter = 0;
    uint64_t directionCounter = 0;
    uint64_t height = 0;
    uint64_t width = 0;
    vector<unique_ptr<Piece>> droppedPieces;

    const inline static unique_ptr<const Piece> nextPieces[5] = { 
        make_unique<const LinePiece>(),
        make_unique<const PlusPiece>(),
        make_unique<const LPiece>(),
        make_unique<const WallPiece>(),
        make_unique<const SquarePiece>(),
    };

    Board(vector<bool> directions, uint32_t width) : directions(directions), width(width) {}

    bool canMoveLateral(const unique_ptr<Piece>& s) {
        if (s->getXMin() < 0 || s->getXMax() >= width)
            return false;

        for (const auto& p : droppedPieces)
            if (p->shape.intersect(s->shape))
                return false;
        
        return true;
    }

    bool canMoveVertical(const unique_ptr<Piece>& s) {
        if (s->getYMin() < 0)
            return false;

        for (const auto& p : droppedPieces) {
            if (p->shape.intersect(s->shape))
                return false;
        }
        return true;
    }

    void simulateFor(uint64_t turns) {
        uint64_t directionCounter = 0;

        for (int i = 0; i < turns; ++i) {
            unique_ptr<Piece> currentPiece = nextPieces[i % 5]->clone();
            currentPiece->setLeftX(2);
            currentPiece->setBottomY(height + 3);

            // while is not settled
            bool settled = false;
            while (!settled) {
                int Xoffset = directions[directionCounter] ? 1 : -1;

                // Move left/right
                currentPiece->shape.position.x += Xoffset;
                if (!canMoveLateral(currentPiece)) {
                    currentPiece->shape.position.x -= Xoffset;
                }
            
                // Move down
                currentPiece->shape.position.y--;
                if (!canMoveVertical(currentPiece)) {
                    currentPiece->shape.position.y++;
                    settled = true;
                    height = max(height, (uint64_t)currentPiece->getYMax() + 1);
                    droppedPieces.push_back(move(currentPiece));
                }
                directionCounter = (directionCounter + 1) % width;
            }
        }
    }
};

int main() {
    FileParser fp("2022/17-data-test");

    // Parse file
    vector<bool> directions = fp.parseRest<bool>([](istream& is) {
        char c;
        is >> c;
        return c == '>';
    });

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        Board board(directions, 7);
        board.simulateFor(2022);
        out = [=](auto& o) {};
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {

        out = [=](auto& o) {};
    });

    dph.RunAll(cout);
}