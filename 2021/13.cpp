#include "../helpers.h"
#include "../common.h"

#include <unordered_set>

using namespace std;

struct FoldInstruction {
    char axis;
    int index;

    FoldInstruction(ifstream& stream) {
        constexpr int skipLen = length("fold along ");
        stream.ignore(skipLen);

        string token;
        getline(stream, token, '=');
        axis = token[0];
        getline(stream, token);
        index = atoi(token.c_str());
    }
};

struct PaperPoint {
    int x, y;

    friend bool operator==(const PaperPoint& lhs, const PaperPoint& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
};

typedef PaperPoint Size;

using PaperSheet = unordered_set<PaperPoint, decltype([](const PaperPoint& pt) -> size_t { return pt.y * 1000 + pt.x; })>;

class Paper {
    Size size{ 0, 0 };
    PaperSheet paperPoints;
public:
    Paper(ifstream& stream) {
        string token;
        for (;;) {
            PaperPoint pt;

            auto restore = stream.tellg();
            getline(stream, token);
            if (token == "") break;
            stream.seekg(restore, ios_base::beg);

            getline(stream, token, ',');

            pt.x = atoi(token.c_str());

            getline(stream, token);

            pt.y = atoi(token.c_str());

            if (pt.x > size.x)
                size.x = pt.x;

            if (pt.y > size.y)
                size.y = pt.y;

            paperPoints.insert(pt);
        }
    }

    void fold(const FoldInstruction& fold) {
        vector<PaperPoint> affectedPts;

        for (const auto& point : paperPoints) {
            if (
                fold.axis == 'y' && point.y > fold.index
                || fold.axis == 'x' && point.x > fold.index
            ) {
                affectedPts.push_back(point);
            }
        }

        for (const auto& affectedPt : affectedPts) {
            paperPoints.erase(affectedPt);
            paperPoints.insert({
                fold.axis == 'y' ? affectedPt.x : fold.index - (affectedPt.x - fold.index),
                fold.axis == 'y' ? fold.index - (affectedPt.y - fold.index) : affectedPt.y,
            });
        }

        if (fold.axis == 'y')
            size.y = fold.index;
        else if (fold.axis == 'x')
            size.x = fold.index;
    }

    size_t countDots() const {
        return paperPoints.size();
    }

    void draw(wostream& o) const {
        for (int y = 0; y < size.y; y++) {
            for (int x = 0; x < size.x; x++) {
                o << (paperPoints.contains({ x, y }) ? (wchar_t)0x2588 : (wchar_t)0x0020);
            }   
            o << endl;
        }
         
        o << "dots: " << countDots() << endl;
    }
};

int main() {
    FileParser fp("2021/13-data");

    auto paper = fp.parseOne<Paper>();
    auto instructions = fp.parseRest<FoldInstruction>();

    DayPartHandler<wostream> dph;

    dph.AddPart([=](auto& out) mutable {
        paper.fold(instructions[0]);

        out = [=](auto& o) { o << paper.countDots() << endl; };
    });

    dph.AddPart([=](auto& out) mutable {
        for (const auto& instruction : instructions) {
            paper.fold(instruction);
        }

        out = [=](auto& o) { paper.draw(o); };
    });

    dph.RunAll(wcout);
}
