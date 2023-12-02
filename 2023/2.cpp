#include "../helpers.h"
#include "../common.h"

#include <numeric>
#include <ranges>

using namespace std;

// puzzle: https://adventofcode.com/2023/day/2

struct Pull {
    int red{}, green{}, blue{};

    friend istream& operator>>(istream& is, Pull& pull) {
        char delim{};
        std::string pullStr;
        std::getline(is, pullStr, ';');
        std::stringstream pullStream{pullStr};

        while(pullStream && !pullStream.eof()) {
            int count;
            pullStream >> count;

            string color;
            pullStream >> std::ws;
            getline(pullStream, color, ',');

            if (color == "red")
                pull.red += count;
            if (color == "green")
                pull.green += count;
            if (color == "blue")
                pull.blue += count;
        };

        return is;
    }

    constexpr bool operator>(const Pull& other) const {
        return red > other.red || green > other.green || blue > other.blue;
    }
};

struct Game {
    int id;
    std::vector<Pull> pulls;

    friend istream& operator>>(istream& is, Game& game) {
        std::string data;
        std::getline(is, data);
        std::stringstream dataStream{data};

        string trash;
        dataStream >> trash; // remove Game
        dataStream >> game.id;
        dataStream >> trash; // remove :
        
        while(dataStream && !dataStream.eof()) {
            Pull p;
            dataStream >> p;
            game.pulls.push_back(p);
        }

        return is;
    }
};

int main() {
    FileParser fp("2023/2-data");

    // Parse file
    auto games = fp.parseRest<Game>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        constexpr Pull limit{ 12, 13, 14 };

        // ranges implementation
        const int sum = std::ranges::fold_right(games | std::views::transform([&](const Game& game) { 
            return game.id * !std::ranges::any_of(game.pulls, [&](const Pull& other) {
                return other > limit;
            }); 
        }), 0, std::plus<int>{});

        // Iterative implmementation
        // int sum = 0;
        // for (const Game& game : games) {
        //     bool valid = true;

        //     for (auto i = 0; i < game.pulls.size() && valid; ++i) {
        //         valid = !(game.pulls[i] > limit);
        //     }

        //     sum += valid * game.id;
        // }

        out = [=](auto& o) { o << sum << endl; };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {

        // STL implementation
        // const int sum = std::accumulate(games.begin(), games.end(), 0, [](int acc, const auto& game) {
        //     const Pull p = std::accumulate(game.pulls.begin(), game.pulls.end(), Pull{}, [](auto min, const auto& pull) {
        //         min.red = std::max(min.red, pull.red);
        //         min.green = std::max(min.green, pull.green);
        //         min.blue = std::max(min.blue, pull.blue);
        //         return min;
        //     });
        //     return acc + (p.red * p.green * p.blue);
        // });

        // Iterative implementation
        int sum = 0;
        for (const Game& game : games) {
            Pull min{};
            for (const Pull& pull : game.pulls) {
                min.red = std::max(min.red, pull.red);
                min.green = std::max(min.green, pull.green);
                min.blue = std::max(min.blue, pull.blue);
            }
            sum += min.red * min.green * min.blue;
        }

        out = [=](auto& o) { o << sum << endl; };
    });

    dph.RunAll(cout);
}